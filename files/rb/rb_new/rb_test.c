#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "rb.h"

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
    rb_node(node_t) link;
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

typedef rb_tree(node_t) tree_t;
rb_wrap(static, tree_, tree_t, node_t, link, nodeCmp);

static unsigned
treeRecurse(node_t *aNode, unsigned aBlackHeight, unsigned aBlackDepth,
  node_t *aNil) {
    unsigned rVal = 0;
    node_t *leftNode = rbp_left_get(node_t, link, aNode);
    node_t *rightNode = rbp_right_get(node_t, link, aNode);

    if (rbp_red_get(node_t, link, aNode) == false) {
	aBlackDepth++;
    }

    // At most two consecutive red children are allowed.
    if (rbp_red_get(node_t, link, aNode)) {
	node_t *tNode = rbp_left_get(node_t, link, leftNode);
	assert(rbp_red_get(node_t, link, leftNode) == false
	  || rbp_red_get(node_t, link, tNode) == false);
    }

    // Right children can never be red.
    assert(rbp_red_get(node_t, link, rightNode) == false);

    if (aNode == aNil) {
#ifdef TREE_PRINT
	fprintf(stderr, ".");
#endif
	return rVal;
    }
    /* Self. */
    assert(aNode->magic == NODE_MAGIC);
#ifdef TREE_PRINT
    fprintf(stderr, "%ld%c", aNode->key,
      rbp_red_get(node_t, link, aNode) ? 'r' : 'b');
#endif

    /* Left subtree. */
    if (leftNode != aNil) {
#ifdef TREE_PRINT
	fprintf(stderr, "[");
#endif
	rVal += treeRecurse(leftNode, aBlackHeight, aBlackDepth, aNil);
#ifdef TREE_PRINT
	fprintf(stderr, "]");
#endif
    }
#ifdef TREE_PRINT
    else {
	rVal += (aBlackDepth != aBlackHeight);
	fprintf(stderr, ".");
    }
#endif

    /* Right subtree. */
    if (rightNode != aNil) {
#ifdef TREE_PRINT
	fprintf(stderr, "<");
#endif
	rVal += treeRecurse(rightNode, aBlackHeight, aBlackDepth, aNil);
#ifdef TREE_PRINT
	fprintf(stderr, ">");
#endif
    }
#ifdef TREE_PRINT
    else {
	rVal += (aBlackDepth != aBlackHeight);
	fprintf(stderr, ".");
    }
#endif

    return rVal;
}

static unsigned
treeIterate(tree_t *aTree) {
    unsigned i;
    node_t *node, *sNode;

    i = 0;
    rb_foreach_begin(node_t, link, aTree, node) {
	assert(node->magic == NODE_MAGIC);
#ifdef FORWARD_PRINT
	if (i != 0) {
	    fprintf(stderr, "-->");
	}
	fprintf(stderr, "%ld", node->key);
#endif

	/* Test rb_search(). */
	sNode = tree_search(aTree, node);
	assert(sNode == node);

	/* Test rb_nsearch(). */
	sNode = tree_nsearch(aTree, node);
	assert(sNode == node);

	/* Test rb_psearch(). */
	sNode = tree_psearch(aTree, node);
	assert(sNode == node);

	i++;
    } rb_foreach_end(node_t, link, aTree, node)

    return i;
}

static unsigned
treeIterateReverse(tree_t *aTree) {
    unsigned i;
    node_t *node, *sNode;

    i = 0;
    rb_foreach_reverse_begin(node_t, link, aTree, node) {
	assert(node->magic == NODE_MAGIC);
#ifdef FORWARD_PRINT
	if (i != 0) {
	    fprintf(stderr, "<--");
	}
	fprintf(stderr, "%ld", node->key);
#endif

	/* Test rb_search(). */
	sNode = tree_search(aTree, node);
	assert(sNode == node);

	/* Test rb_nsearch(). */
	sNode = tree_nsearch(aTree, node);
	assert(sNode == node);

	/* Test rb_psearch(). */
	sNode = tree_psearch(aTree, node);
	assert(sNode == node);

	i++;
    } rb_foreach_reverse_end(node_t, link, aTree, node)

    return i;
}

static void
nodeRemove(tree_t *aTree, node_t *aNode, unsigned aNNodes) {
    node_t *sNode;
    unsigned blackHeight, imbalances;

#ifdef VERBOSE
    fprintf(stderr, "rb_remove(%3ld)", aNode->key);
#endif
    tree_remove(aTree, aNode);

    /* Test rb_nsearch(). */
    sNode = tree_nsearch(aTree, aNode);
    assert(sNode == NULL || sNode->key >= aNode->key);

    /* Test rb_psearch(). */
    sNode = tree_psearch(aTree, aNode);
    assert(sNode == NULL || sNode->key <= aNode->key);

    aNode->magic = 0;

#ifdef TREE_PRINT
    fprintf(stderr, "\n\t   tree: ");
#endif
    rbp_black_height(node_t, link, aTree, blackHeight);
    imbalances = treeRecurse(aTree->rbt_root, blackHeight, 0,
      &(aTree->rbt_nil));
    if (imbalances != 0) {
	fprintf(stderr, "\nTree imbalance\n");
	abort();
    }
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

int
main(void) {
    tree_t tree;
    long set[NNODES];
    node_t nodes[NNODES], key, *sNode, *nodeA;
    unsigned i, j, k, blackHeight, imbalances;

    fprintf(stderr, "Test begin\n");

    /* Initialize tree. */
    tree_new(&tree);

    /*
     * Empty tree.
     */
    fprintf(stderr, "Empty tree:\n");

    /* rb_first(). */
    nodeA = tree_first(&tree);
    if (nodeA == NULL) {
	fprintf(stderr, "rb_first() --> nil\n");
    } else {
	fprintf(stderr, "rb_first() --> %ld\n", nodeA->key);
    }

    /* rb_last(). */
    nodeA = tree_last(&tree);
    if (nodeA == NULL) {
	fprintf(stderr, "rb_last() --> nil\n");
    } else {
	fprintf(stderr, "rb_last() --> %ld\n", nodeA->key);
    }

    /* rb_search(). */
    key.key = 0;
    key.magic = NODE_MAGIC;
    nodeA = tree_search(&tree, &key);
    if (nodeA == NULL) {
	fprintf(stderr, "rb_search(0) --> nil\n");
    } else {
	fprintf(stderr, "rb_search(0) --> %ld\n", nodeA->key);
    }

    /* rb_nsearch(). */
    key.key = 0;
    key.magic = NODE_MAGIC;
    nodeA = tree_nsearch(&tree, &key);
    if (nodeA == NULL) {
	fprintf(stderr, "rb_nsearch(0) --> nil\n");
    } else {
	fprintf(stderr, "rb_nsearch(0) --> %ld\n", nodeA->key);
    }

    /* rb_psearch(). */
    key.key = 0;
    key.magic = NODE_MAGIC;
    nodeA = tree_psearch(&tree, &key);
    if (nodeA == NULL) {
	fprintf(stderr, "rb_psearch(0) --> nil\n");
    } else {
	fprintf(stderr, "rb_psearch(0) --> %ld\n", nodeA->key);
    }

    /* rb_insert(). */
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
	    tree_new(&tree);
	    tree.rbt_nil.magic = 0;
	    for (k = 0; k < j; k++) {
		nodes[k].magic = NODE_MAGIC;
		nodes[k].key = set[k];
	    }

	    /* Insert nodes. */
	    for (k = 0; k < j; k++) {
#ifdef VERBOSE
		fprintf(stderr, "rb_insert(%3ld)", nodes[k].key);
#endif
		tree_insert(&tree, &nodes[k]);

#ifdef TREE_PRINT
		fprintf(stderr, "\n\t   tree: ");
#endif
		rbp_black_height(node_t, link, &tree, blackHeight);
		imbalances = treeRecurse(tree.rbt_root, blackHeight, 0,
		  &(tree.rbt_nil));
		if (imbalances != 0) {
		    fprintf(stderr, "\nTree imbalance\n");
		    abort();
		}
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
		    node_t *node;
		    unsigned nNodes = j;
		    rb_foreach_begin(node_t, link, &tree, node) {
			node_t *tNode;
			tNode = tree_next(&tree, node);
			nodeRemove(&tree, node, nNodes);
			rb_foreach_next(node_t, link, nodeCmp, &tree, tNode);
			nNodes--;
		    } rb_foreach_end(node_t, link, &tree, node)
		    break;
		} case 3: {
		    node_t *node;
		    unsigned nNodes = j;
		    rb_foreach_reverse_begin(node_t, link, &tree, node) {
			node_t *tNode;
			tNode = tree_prev(&tree, node);
			nodeRemove(&tree, node, nNodes);
			rb_foreach_reverse_prev(node_t, link, nodeCmp, &tree,
			  tNode);
			nNodes--;
		    } rb_foreach_reverse_end(node_t, link, &tree, node)
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
