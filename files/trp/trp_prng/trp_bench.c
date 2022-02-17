#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define NDEBUG
#include <assert.h>
#include "trp.h"

#define NNODES 150
#define NSETS 10
#define NSEARCH 10
#define NITER 0

//#define VERBOSE

typedef struct node_s node_t;

struct node_s {
#define NODE_MAGIC 0x9823af7e
    uint32_t magic;
    trp_node(node_t) link;
    long key;
};

static int
nodeCmpEq(node_t *aA, node_t *aB) {
    assert(aA->magic == NODE_MAGIC);
    assert(aB->magic == NODE_MAGIC);
    return (aA->key > aB->key) - (aA->key < aB->key);
}

static int
nodeCmpIdent(node_t *aA, node_t *aB) {
    int rVal = nodeCmpEq(aA, aB);
    if (rVal == 0) {
	// Duplicates are not allowed in the tree, so force an arbitrary
	// ordering for non-identical items with equal keys.
	rVal = (((uintptr_t) aA) > ((uintptr_t) aB))
	  - (((uintptr_t) aA) < ((uintptr_t) aB));
    }
    return rVal;
}

typedef trp(node_t) tree_t;
trp_gen(static, tree_, tree_t, node_t, link, nodeCmpIdent, 1297, 1301);

static node_t *
treeIterateCb(tree_t *aTree, node_t *aNode, void *data) {
    unsigned *i = (unsigned *)data;
    node_t *sNode, key;

    assert(aNode->magic == NODE_MAGIC);

    /* Test trp_search(). */
    key.key = aNode->key;
    key.magic = NODE_MAGIC;
    sNode = tree_search(aTree, &key);
    assert(sNode != NULL);
    assert(sNode->key == key.key);

    /* Test trp_nsearch(). */
    sNode = tree_nsearch(aTree, &key);
    assert(sNode != NULL);
    assert(sNode->key == key.key);

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

static unsigned
treeIterateReverse(tree_t *aTree) {
    unsigned i;

    i = 0;
    tree_reverse_iter(aTree, NULL, treeIterateCb, (void *)&i);

    return i;
}

int
main(void) {
    tree_t tree;
    long set[NNODES];
    node_t nodes[NNODES], key, *sNode;
    unsigned i, j, k, l, m, blackHeight, imbalances;

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
	    tree_new(&tree, i);
	    for (k = 0; k < j; k++) {
		nodes[k].magic = NODE_MAGIC;
		nodes[k].key = set[k];
	    }

	    /* Insert nodes. */
	    for (k = 0; k < j; k++) {
		tree_insert(&tree, &nodes[k]);

		for (l = 0; l < NSEARCH; l++) {
		    for (m = 0; m <= k; m++) {
			sNode = tree_first(&tree);
			sNode = tree_last(&tree);

			key.key = nodes[m].key;
			key.magic = NODE_MAGIC;
			sNode = tree_search(&tree, &key);
			sNode = tree_nsearch(&tree, &key);
		    }
		}
	    }

	    for (k = 0; k < NITER; k++) {
		treeIterate(&tree);
		treeIterateReverse(&tree);
	    }

	    /* Remove nodes. */
	    for (k = 0; k < j; k++) {
		for (l = 0; l < NSEARCH; l++) {
		    for (m = 0; m <= k; m++) {
			sNode = tree_first(&tree);
			sNode = tree_last(&tree);

			key.key = nodes[m].key;
			key.magic = NODE_MAGIC;
			sNode = tree_search(&tree, &key);
			sNode = tree_nsearch(&tree, &key);
		    }
		}

		tree_remove(&tree, &nodes[k]);

		nodes[k].magic = 0;
	    }
	}
    }

    return 0;
}
