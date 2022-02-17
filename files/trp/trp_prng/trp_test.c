#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "trp.h"

#define NNODES 32
#define NSETS 200

//#define VERBOSE
//#define TREE_PRINT
//#define FORWARD_PRINT
//#define REVERSE_PRINT

typedef struct node_s node_t;

struct node_s {
#define NODE_MAGIC 0x9823af7e
    uint32_t magic;
    trp_node(node_t) link;
    long key;
};

static int
nodeCmp(node_t *aA, node_t *aB) {
    int rVal;

    assert(aA->magic == NODE_MAGIC);
    assert(aB->magic == NODE_MAGIC);

    rVal = (aA->key > aB->key) - (aA->key < aB->key);
    if (rVal == 0) {
	// Duplicates are not allowed in the tree, so force an arbitrary
	// ordering for non-identical items with equal keys.
	rVal = (((uintptr_t) aA) > ((uintptr_t) aB))
	  - (((uintptr_t) aA) < ((uintptr_t) aB));
    }
    return rVal;
}

typedef trp(node_t) tree_t;
trp_gen(static, tree_, tree_t, node_t, link, nodeCmp, 1297, 1301);

static unsigned
treeRecurse(node_t *aNode) {
    unsigned rVal = 0;
    node_t *leftNode = trp_left_get(node_t, link, aNode);
    node_t *rightNode = trp_right_get(node_t, link, aNode);

    if (aNode == NULL) {
#ifdef TREE_PRINT
	fprintf(stderr, ".");
#endif
	return rVal;
    }
    /* Self. */
    assert(aNode->magic == NODE_MAGIC);
#ifdef TREE_PRINT
    fprintf(stderr, "%ld", aNode->key);
#endif

    /* Left subtree. */
    if (leftNode != NULL) {
#ifdef TREE_PRINT
	fprintf(stderr, "[");
#endif
	rVal += treeRecurse(leftNode);
#ifdef TREE_PRINT
	fprintf(stderr, "]");
#endif
    }
#ifdef TREE_PRINT
    else {
	fprintf(stderr, ".");
    }
#endif

    /* Right subtree. */
    if (rightNode != NULL) {
#ifdef TREE_PRINT
	fprintf(stderr, "<");
#endif
	rVal += treeRecurse(rightNode);
#ifdef TREE_PRINT
	fprintf(stderr, ">");
#endif
    }
#ifdef TREE_PRINT
    else {
	fprintf(stderr, ".");
    }
#endif

    return rVal;
}

static node_t *
treeIterateCb(tree_t *aTree, node_t *aNode, void *data) {
    unsigned *i = (unsigned *)data;
    node_t *sNode;

    assert(aNode->magic == NODE_MAGIC);
#ifdef FORWARD_PRINT
    if (*i != 0) {
	fprintf(stderr, "-->");
    }
    fprintf(stderr, "%ld", aNode->key);
#endif

    /* Test trp_search(). */
    sNode = tree_search(aTree, aNode);
    assert(sNode == aNode);

    /* Test trp_nsearch(). */
    sNode = tree_nsearch(aTree, aNode);
    assert(sNode == aNode);

    /* Test trp_psearch(). */
    sNode = tree_psearch(aTree, aNode);
    assert(sNode == aNode);

    (*i)++;

    return NULL;
}

static unsigned
treeIterate(tree_t *aTree) {
    unsigned i;

    i = 0;
    tree_iter(aTree, NULL, treeIterateCb, (void *)&i);

    return i;
}

static node_t *
treeReverseIterateCb(tree_t *aTree, node_t *aNode, void *data) {
    unsigned *i = (unsigned *)data;
    node_t *sNode;

    assert(aNode->magic == NODE_MAGIC);
#ifdef FORWARD_PRINT
    if (*i != 0) {
	fprintf(stderr, "<--");
    }
    fprintf(stderr, "%ld", aNode->key);
#endif

    /* Test trp_search(). */
    sNode = tree_search(aTree, aNode);
    assert(sNode == aNode);

    /* Test trp_nsearch(). */
    sNode = tree_nsearch(aTree, aNode);
    assert(sNode == aNode);

    /* Test trp_psearch(). */
    sNode = tree_psearch(aTree, aNode);
    assert(sNode == aNode);

    (*i)++;

    return NULL;
}

static unsigned
treeIterateReverse(tree_t *aTree) {
    unsigned i;

    i = 0;
    tree_reverse_iter(aTree, NULL, treeReverseIterateCb, (void *)&i);

    return i;
}

static void
nodeRemove(tree_t *aTree, node_t *aNode, unsigned aNNodes) {
    node_t *sNode;

#ifdef VERBOSE
    fprintf(stderr, "trp_remove(%3ld)", aNode->key);
#endif
    tree_remove(aTree, aNode);

    /* Test trp_nsearch(). */
    sNode = tree_nsearch(aTree, aNode);
    assert(sNode == NULL || sNode->key >= aNode->key);

    /* Test trp_psearch(). */
    sNode = tree_psearch(aTree, aNode);
    assert(sNode == NULL || sNode->key <= aNode->key);

    aNode->magic = 0;

#ifdef TREE_PRINT
    fprintf(stderr, "\n\t   tree: ");
#endif
#ifdef FORWARD_PRINT
    fprintf(stderr, "\n\tforward: ");
#endif
    assert(aNNodes - 1 == treeIterate(aTree));
#ifdef REVERSE_PRINT
    fprintf(stderr, "\n\treverse: ");
#endif
    assert(aNNodes - 1 == treeIterateReverse(aTree));
#ifdef VERBOSE
    fprintf(stderr, "\n");
#endif
}

static node_t *
removeIterateCb(tree_t *aTree, node_t *aNode, void *data) {
    unsigned *nNodes = (unsigned *)data;
    node_t *ret = tree_next(aTree, aNode);

    nodeRemove(aTree, aNode, *nNodes);

    return ret;
}

static node_t *
removeReverseIterateCb(tree_t *aTree, node_t *aNode, void *data) {
    unsigned *nNodes = (unsigned *)data;
    node_t *ret = tree_prev(aTree, aNode);

    nodeRemove(aTree, aNode, *nNodes);

    return ret;
}

int
main(void) {
    tree_t tree;
    long set[NNODES];
    node_t nodes[NNODES], key, *sNode, *nodeA;
    unsigned i, j, k;

    fprintf(stderr, "Test begin\n");

    /* Initialize tree. */
    tree_new(&tree, 42);

    /*
     * Empty tree.
     */
    fprintf(stderr, "Empty tree:\n");

    /* trp_first(). */
    nodeA = tree_first(&tree);
    if (nodeA == NULL) {
	fprintf(stderr, "trp_first() --> nil\n");
    } else {
	fprintf(stderr, "trp_first() --> %ld\n", nodeA->key);
    }

    /* trp_last(). */
    nodeA = tree_last(&tree);
    if (nodeA == NULL) {
	fprintf(stderr, "trp_last() --> nil\n");
    } else {
	fprintf(stderr, "trp_last() --> %ld\n", nodeA->key);
    }

    /* trp_search(). */
    key.key = 0;
    key.magic = NODE_MAGIC;
    nodeA = tree_search(&tree, &key);
    if (nodeA == NULL) {
	fprintf(stderr, "trp_search(0) --> nil\n");
    } else {
	fprintf(stderr, "trp_search(0) --> %ld\n", nodeA->key);
    }

    /* trp_nsearch(). */
    key.key = 0;
    key.magic = NODE_MAGIC;
    nodeA = tree_nsearch(&tree, &key);
    if (nodeA == NULL) {
	fprintf(stderr, "trp_nsearch(0) --> nil\n");
    } else {
	fprintf(stderr, "trp_nsearch(0) --> %ld\n", nodeA->key);
    }

    /* trp_psearch(). */
    key.key = 0;
    key.magic = NODE_MAGIC;
    nodeA = tree_psearch(&tree, &key);
    if (nodeA == NULL) {
	fprintf(stderr, "trp_psearch(0) --> nil\n");
    } else {
	fprintf(stderr, "trp_psearch(0) --> %ld\n", nodeA->key);
    }

    /* trp_insert(). */
    srandom(42);
    for (i = 0; i < NSETS; i++) {
	for (j = 0; j < NNODES; j++) {
	    set[j] = (long) (((double) NNODES)
	      * ((double) random() / ((double)RAND_MAX)));
	}

	for (j = 1; j <= NNODES; j++) {
#ifdef VERBOSE
	    fprintf(stderr, "Tree %u, %u node%s\n", i, j, j != 1 ? "s" : "");
#endif

	    /* Initialize tree and nodes. */
	    tree_new(&tree, 42);
	    for (k = 0; k < j; k++) {
		nodes[k].magic = NODE_MAGIC;
		nodes[k].key = set[k];
	    }

	    /* Insert nodes. */
	    for (k = 0; k < j; k++) {
#ifdef VERBOSE
		fprintf(stderr, "trp_insert(%3ld)", nodes[k].key);
#endif
		tree_insert(&tree, &nodes[k]);

#ifdef TREE_PRINT
		fprintf(stderr, "\n\t   tree: ");
#endif
#ifdef FORWARD_PRINT
		fprintf(stderr, "\n\tforward: ");
#endif
		assert(k + 1 == treeIterate(&tree));
#ifdef REVERSE_PRINT
		fprintf(stderr, "\n\treverse: ");
#endif
		assert(k + 1 == treeIterateReverse(&tree));
#ifdef VERBOSE
		fprintf(stderr, "\n");
#endif

		sNode = tree_first(&tree);
		assert(sNode != NULL);

		sNode = tree_last(&tree);
		assert(sNode != NULL);

		sNode = tree_next(&tree, &nodes[k]);
		sNode = tree_prev(&tree, &nodes[k]);
	    }

	    /* Remove nodes. */
	    switch (i % 4) {
		case 0: {
		    for (k = 0; k < j; k++) {
			nodeRemove(&tree, &nodes[k], j - k);
		    }
		    break;
		} case 1: {
		    for (k = j; k > 0; k--) {
			nodeRemove(&tree, &nodes[k-1], k);
		    }
		    break;
		} case 2: {
		    node_t *start;
		    unsigned nNodes = j;

		    start = NULL;
		    do {
			start = tree_iter(&tree, start, removeIterateCb,
			  (void *)&nNodes);
			nNodes--;
		    } while (start != NULL);
		    assert(nNodes == 0);
		    break;
		} case 3: {
		    node_t *start;
		    unsigned nNodes = j;

		    start = NULL;
		    do {
			start = tree_reverse_iter(&tree, start,
			  removeReverseIterateCb, (void *)&nNodes);
			nNodes--;
		    } while (start != NULL);
		    assert(nNodes == 0);
		    break;
		} default: {
		    assert(false);
		}
	    }
	}
    }

    fprintf(stderr, "Test end\n");
    return 0;
}
