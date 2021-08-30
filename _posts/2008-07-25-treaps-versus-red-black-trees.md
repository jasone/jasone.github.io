---
layout: post
title: Treaps versus red-black trees
categories: blog
---

In a previous blog entry, I discussed the [difficulties associated with
implementing left-leaning red-black
trees](http://www.canonware.com/%7Ettt/2008/04/left-leaning-red-black-trees-are-hard.html).
A couple of readers commented that [treaps](http://en.wikipedia.org/wiki/Treap)
might be superior to [red-black
trees](http://en.wikipedia.org/wiki/Red-black_tree), and as part of some
[recent](http://www.canonware.com/%7Ettt/2008/07/overzealous-use-of-my-red-black-tree.html)
jemalloc optimization work, I had occasion to implement treaps in order to
measure tree operation overhead.

Most of this article discusses performance, but let me first mention
implementation difficulty.  It took me about 90 hours to
design/implement/test/benchmark left-leaning red-black trees, and less than 10
hours for treaps.  Search/insert/delete for red-black trees is O(lg n), versus
O(n) for treaps.  However, the average case for treaps is (lg n), and the
chances of worst case behavior are vanishingly small, thanks to
(pseudo-)randomness.  Thus, real-world performance differences are only
incremental.  To be fair, I made red-black trees harder by avoiding recursion.
Regardless however, treaps are _way_ easier to implement than red-black trees.

As for benchmarking, I wrote functionally identical benchmark programs for three
red-black tree implementations and two treap implementations.  The tree
implementations are:
- [**rb\_new**](http://www.canonware.com/download/rb/rb_new/): Left-leaning
  red-black trees.
- [**rb\_old**](http://www.canonware.com/download/rb/rb_old/): Standard
  red-black trees.
- [**RB**](http://www.canonware.com/download/rb/tree/): Standard red-black
  trees, as implemented by the \*BSD sys/tree.h.
- [**trp\_hash**](http://www.canonware.com/download/trp/trp_hash/): Treaps, with
  priorities computed via pointer hashing.
- [**trp\_prng**](http://www.canonware.com/download/trp/trp_prng/): Treaps, with
  priorities computed via pseudo-random number generation (PRNG).

The benchmark programs iteratively generate permutations of NNODES nodes, for
NSETS node sets.  For each node set, the programs iteratively build and tear
down a tree using the first [1..NNODES] nodes in the set.  Each insert/remove
operation is accompanied by NSEARCH rounds of searching for every object in the
tree, and NITER rounds of iterating over every object in the tree.  Don't worry
too much about the details; in short the benchmark programs can be configured to
predominantly benchmark insertion/deletion, searching, and/or iteration.

The following table summarizes benchmark results as measured on a 2.2 GHz amd64
Ubuntu 8.10 system.  The benchmarks were all compiled with "gcc -O3", and the
times are user+system time (fastest of three runs):

| NNODES, NSETS, NSEARCH, NITER (focus)                  | rb\_new | rb\_old | RB    | trp\_hash | trp\_prng |
| ------------------------ | -------:| -------:| -----:| ---------:| ---------:|
| 1500,25,0,0 (ins/del)    |    7.60 |    3.99 |  4.25 |     17.57 |      7.58 |
| 125,25,10,0 (search)     |   17.74 |   18.61 | 16.60 |     17.84 |     17.77 |
| 250,25,0,100 (iteration) |   18.45 |   21.06 | 19.19 |     20.45 |     20.40 |

Insertion/deletion is fastest for the red-black tree implementations that do
lazy fixup (rb\_old and RB).  rb\_new uses a single-pass algorithm, which
requires more work.  trp\_prng is about the same speed as rb\_new, but trp\_hash
is way slower, due to the repeated hash computations that are required to avoid
explicitly storing node priorities.

Search performance is similar for all implementations, which indicates that
there are no major disparities in tree balance.

Iteration performance is similar for all implementations, even though they use
substantially different algorithms.  If tree size were much larger, rb\_old and
RB would suffer, since they use an O(n lg n) algorithm, whereas rb\_new and
trp\_\* use O(n) algorithms.  rb\_new uses a rather complicated iterative
algorithm, but trp\_\* use recursion and callback functions due to the weak
upper bound on tree depth.

Sadly, there is no decisive winner, though any of the five tree implementations
is perfectly adequate for the vast majority of applications.  The winners
according to various criteria are:
- **Space**: rb\_new and trp\_hash.
- **Speed** (insertion/deletion): rb\_old and RB.
- **Ease of implementation**: trp\_prng

### Comments

###### At [September 27, 2008 7:35 PM](#comment1){:name='comment1'}, [Calvin](http://www.blogger.com/profile/17709318189440902192) said...

Great post... I was in search for a best data structure for my data  grouping
engine :) i think RB would be the best than AVL, AA, Skip list and all others..

Thanks.

---

###### At [July 5, 2009 1:44 PM](#comment2){:name='comment2'}, Anonymous said...

Jason,

You pulled a comment about a week ago from one of our devs that looked at your
non-recursive llrb tree implementation.

There were some issues w/ it leaning far left on ordered dataset insertion.  If
you'd like to take a look at our test cases, please just provide some contact
info.

We would have contacted you directly before posting, but you don't list any
email or contact info.

Always looking for good work in the data structures space & we have a common
interest in optimizing for C vs. C++.

Thanx!

-Tomas  
http://fuel7.com/

---

###### At [July 5, 2009 2:15 PM](#comment3){:name='comment3'}, Jason said...

Tomas,

The comment you referred to provided no details on what might be wrong, and
looked suspiciously like blog comment spam, so I removed it.

I'm interested in further details, and if there is in fact a problem, I
definitely want to fix it.  Are you aware that left-leaning red-black trees have
a looser bound on tree height than standard red-black trees?  Standard red-black
trees have a maximum height that is (2*lg n), where n is the number of tree
nodes, whereas the maximum height for left-leaning red-black trees is (3*lg n).
If I recall correctly, ordered left-to-right insertion will exercise the worst
case.

You can find links to test and benchmark programs on my [red-black tree web page](http://www.canonware.com/rb/), as well as my email address.

Thanks,  
Jason
