#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define NDEBUG
#include <assert.h>
#include "rb.h"

#define NNODES 1500
#define NSETS 25
#define NSEARCH 0
#define NITER 0

//#define VERBOSE

typedef struct node_s node_t;

struct node_s {
#define NODE_MAGIC 0x9823af7e
    uint32_t magic;
    rb_node(node_t) link;
    long key;
};

typedef rb_tree(node_t) tree_t;

static inline int
nodeCmpEq(node_t *aA, node_t *aB) {
    assert(aA->magic == NODE_MAGIC);
    assert(aB->magic == NODE_MAGIC);
    return (aA->key > aB->key) - (aA->key < aB->key);
}

static inline int
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

static unsigned
treeIterate(tree_t *aTree) {
    unsigned i;
    node_t *node, *sNode, key;

    i = 0;
    rb_foreach_begin(node_t, link, aTree, node) {
	assert(node->magic == NODE_MAGIC);

	/* Test rb_search(). */
	key.key = node->key;
	key.magic = NODE_MAGIC;
	rb_search(node_t, link, nodeCmpIdent, aTree, &key, sNode);
	assert(sNode != NULL);
	assert(sNode->key == key.key);

	/* Test rb_nsearch(). */
	rb_nsearch(node_t, link, nodeCmpIdent, aTree, &key, sNode);
	assert(sNode != NULL);
	assert(sNode->key == key.key);

	i++;
    } rb_foreach_end(node_t, link, aTree, node)

    return i;
}

static unsigned
treeIterateReverse(tree_t *aTree) {
    unsigned i;
    node_t *node, *sNode, key;

    i = 0;
    rb_foreach_reverse_begin(node_t, link, aTree, node) {
	assert(node->magic == NODE_MAGIC);

	/* Test rb_search(). */
	key.key = node->key;
	key.magic = NODE_MAGIC;
	rb_search(node_t, link, nodeCmpIdent, aTree, &key, sNode);
	assert(sNode != NULL);
	assert(sNode->key == key.key);

	/* Test rb_nsearch(). */
	rb_nsearch(node_t, link, nodeCmpIdent, aTree, &key, sNode);
	assert(sNode != NULL);
	assert(sNode->key == key.key);

	i++;
    } rb_foreach_reverse_end(node_t, link, aTree, node)

    return i;
}

int
main(void) {
    tree_t tree;
    long set[NNODES];
    node_t nodes[NNODES], key, *sNode;
    unsigned i, j, k, l, m;

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
	    rb_new(node_t, link, &tree);
	    for (k = 0; k < j; k++) {
		nodes[k].magic = NODE_MAGIC;
		nodes[k].key = set[k];
	    }

	    /* Insert nodes. */
	    for (k = 0; k < j; k++) {
		rb_insert(node_t, link, nodeCmpIdent, &tree, &nodes[k]);

		for (l = 0; l < NSEARCH; l++) {
		    for (m = 0; m <= k; m++) {
			rb_first(node_t, link, &tree, sNode);
			rb_last(node_t, link, &tree, sNode);

			key.key = nodes[m].key;
			key.magic = NODE_MAGIC;
			rb_search(node_t, link, nodeCmpIdent, &tree, &key,
			  sNode);
			rb_nsearch(node_t, link, nodeCmpIdent, &tree, &key,
			  sNode);
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
			rb_first(node_t, link, &tree, sNode);
			rb_last(node_t, link, &tree, sNode);

			key.key = nodes[m].key;
			key.magic = NODE_MAGIC;
			rb_search(node_t, link, nodeCmpIdent, &tree, &key,
			  sNode);
			rb_nsearch(node_t, link, nodeCmpIdent, &tree, &key,
			  sNode);
		    }
		}

		rb_remove(node_t, link, nodeCmpIdent, &tree, &nodes[k]);

		nodes[k].magic = 0;
	    }
	}
    }

    return 0;
}
