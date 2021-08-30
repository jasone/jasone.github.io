---
layout: post
title: Left-leaning red-black trees are hard to implement
categories: blog
---

Back in 2002, I needed balanced trees for a project I was working on, so I used
the description and pseudo-code in [Introduction to
Algorithms](http://mitpress.mit.edu/catalog/item/default.asp?ttype=2&amp;tid=8570)
to implement red-black trees.  I vaguely recall spending perhaps two days on
implementation and testing.  That
[implementation](http://www.canonware.com/%7Ettt/rb_2002.h) uses C preprocessor
macros in order to make it possible to link data structures into one or more
red-black trees without requiring container objects.

About the same time, Niels Provos added a similar implementation to
[OpenBSD](http://www.openbsd.org/), which was
[imported](http://www.freebsd.org/cgi/cvsweb.cgi/src/sys/sys/tree.h) into
[FreeBSD](http://www.freebsd.org/), so when I imported
[jemalloc](http://www.freebsd.org/cgi/cvsweb.cgi/src/lib/libc/stdlib/malloc.c)
into FreeBSD, I switched from my own red-black tree implementation to the
standard one.  Unfortunately, both implementations use nodes that include four
pieces of information: parent, left child, right child, and color (red or
black).  That typically adds up to 16 or 32 bytes on 32- and 64-bit systems,
respectively.  A few months ago I fixed some scalability issues [Stuart
Parmenter](http://blog.pavlov.net/) found in jemalloc by replacing linear
searches with tree searches, but that meant adding more tree links.  These trees
now take up ~2% of all mapped memory, so I have been contemplating ways to
reduce the overhead.

A couple of weeks ago, I came across some
[slides](http://www.cs.princeton.edu/%7Ers/talks/LLRB/RedBlack.pdf) for a talk
that [Robert Sedgewick](http://www.cs.princeton.edu/%7Ers/) recently gave on
left-leaning red-black trees.  His slides pointedly disparage the use of parent
pointers, and they also make left-leaning red-black trees look simple to
implement.  Left-leaning red-black trees maintain a logical 1:1 correspondence
with 2-3-4 B-trees, which is a huge help in understanding seemingly complex tree
transformations.

Last Monday, I started implementing left-leaning red-black trees, expecting to
spend perhaps 15 hours on the project.  I'm here more than 60 hours of work
later to tell you that left-leaning red-black trees are _hard_ to implement, and
contrary to Sedgewick's claims, their implementation appears to require
approximately the same amount of code and complexity as standard red-black
trees.   Part of the catch is that although standard red-black trees have
additional cases to deal with due to 3-nodes that can lean left or right,
left-leaning red-black trees have a universal asymmetry between the left and
right versions of the algorithms.

If memory overhead weren't my primary concern for this project, I would have
dropped red-black trees in favor of treaps.  Unfortunately, treaps require
either recursive implementation or parent pointers, and they also require an
extra "priority" field, whereas red-black trees can be implemented without
recursion or parent pointers, and it is possible to stuff the red-black bit in
the least significant bit of one of the left/right pointers.

For the curious or those in need of such a beast, here is my [left-leaning
red-black tree
implementation](http://www.freebsd.org/cgi/cvsweb.cgi/src/lib/libc/stdlib/rb.h).
<strike>One point of interest is that my benchmarks show it to be ~25% slower
than my standard red-black tree implementation.  The red-black bit twiddling
overhead only accounts for about 1/5 of the slowdown.  I attribute the other 4/5
to the overhead of transforming the tree on the down pass, rather than lazily
fixing up tree structure violations afterward.</strike>

**[26 April 2008]** I did some further experimentation to understand the
performance disparity between implementations.  The benchmarks mentioned above
were flawed, in that they always searched for the most recently inserted item.
Since top-down insertion/deletion is more disruptive than lazy fixup, the
searches significantly favored the old implementation.  I fixed the benchmarks
to compute the times for random searches, random insertions/deletions, and
in-order tree traversal.

The old rb.h and sys/tree.h perform essentially the same for all operations.
The new rb.h takes almost twice as long for insertion/deletion, is the same
speed for searches, and is slightly faster for iteration.  Red/black bit
twiddling overhead accounts for ~6% of insertion/deletion time, and &lt;3% of
search time.

I am actually quite pleased with these benchmark results, because they show that
for random inputs, left-leaning red-black trees do not noticeably suffer from
the fact that tree height is O(3h) rather than O(2h), where h is the height of
an equivalent fully balanced tree.

### Comments

###### At [April 22, 2008 6:42 AM](#comment1){:name='comment1'}, [cmm](http://cmm.kakpryg.net/) said...

make sure to download the slides again, Sedgewick has updated the whole thing
rather radically a couple of days ago.

among other changes, the trees became more symmetric, and also there is a link
to a complete and supposedly-working Java implementation.

that's what we get for tracking research in progress. :)

---

###### At [April 22, 2008 7:50 AM](#comment2){:name='comment2'}, Anonymous said...

Since pointers tend to be nicely aligned these days (if you get them from calloc
they are supposed to be), have you thought of using the low bits to indicate
red/black?  You only need one.

I haven't waded through your code to see if you are already doing that.

--lm

---

###### At [April 22, 2008 9:28 AM](#comment3){:name='comment3'}, shtef said...

Why not using AVL trees?

---

###### At [April 22, 2008 9:35 AM](#comment4){:name='comment4'}, Jason said...

cmm, I just looked through Sedgewick's new slides, and the algorithms look to me
like an overall improvement.  There is unfortunately one disadvantage to
balancing 4-nodes (rather than leaving them left-leaning): for a non-recursive
implementation, it is necessary to keep track of yet another level of the tree
as context, so that the parent of the newly balanced 4-node can be updated.

Incidentally, there is a hint in the new version of the slides that when
Sedgewick says recursion can be removed, he is suggesting using a stack for
context.  I actually removed recursion by tracking a fixed amount of additional
context.  I suspect that he isn't considering the implications of such an
implementation approach on his algorithms.

---

###### At [April 22, 2008 9:35 AM](#comment5){:name='comment5'}, Anonymous said...

Your C code is very advanced.

I wonder, how long will it take me to become as skillful as you are!

---

###### At [April 22, 2008 9:46 AM](#comment6){:name='comment6'}, [Tom](http://www.blogger.com/profile/07009479916609344352) said...

So, um, looking at that original source code... you have HUNDREDS of lines of
macros... that doesn't seem like a very good idea.

How do you expect to debug this code?  Do you have any idea if it's correct or
not?  And it's going to bloat your object code, too, not that that is a huge
consideration.

It's 2008 - you should be using templates for this stuff.  It's hard to find a C
compiler these days that *won't* run as a C++ compiler as well and use
templates.

---

###### At [April 22, 2008 9:54 AM](#comment7){:name='comment7'}, [Tom](http://www.blogger.com/profile/07009479916609344352) said...

To the anonymous commenter, I hope you do not use this as an example for yourself!

It's a nightmare for maintenance.

None of the code is commented.

Since it's all macros, there's no typechecking, so if you put inconsistent
types, you'll get a very obscure compiler error, or worse, no error at all.

The macro nature of it means that all your "functions" are surrounded by
ultra-obscure "do { ... } while (0)" blocks (I know what they mean, but this
should at least be commented!)

There's an obvious class structure here, there's no reason NOT to use a class
(it is not for example more efficient to use a struct), but instead everything's
done as macros.

For God's sake, it's 2008!  Use classes and templates already!  If your main
code cannot do it, write it as an external module in C++ with the extern
linkage.  You won't need the C++ libraries if you do it  right, it's be more
efficient, shorter code, and far easier to maintain.

---

###### At [April 22, 2008 10:58 AM](#comment8){:name='comment8}, Søren Sandmann said...

It is actually possible to get rid of the priority field in treaps by computing
a priority from the address of the node itself.

Basically, just use some hash of the pointer to the node itself as the priority.

This is how I did it in GSequence in the glib library.

---

###### At [April 22, 2008 11:47 AM](#comment9){:name='comment9'}, [shaurz](http://www.blogger.com/profile/03588038254545671774) said...

do{}while(0) obscure? It's a common C idiom! And there are reasons not to use
C++.

---

###### At [April 22, 2008 12:09 PM](#comment10){:name='comment10'}, [ohxten](http://www.16software.com) said...

I recommend first purchasing a red-black tree seedling. Plant it in a nice open
area, far away from the road or other trees. This will allow it to grow as large
as possible.

As it grows, place a stake on the left side of the tree. Of course, it all
depends on which way you look at the tree which side is left, but you can choose
whichever side you prefer.

Tie some string to the trunk of the tree, pull tightly so the red-black tree is
leaning left (again, all depending on which side you look at it from), and tie
it to the stake.

Cry happy tears as your left-leaning red-black tree grows. Your implementation
is successful.

Water it well.

---

###### At [April 22, 2008 1:43 PM](#comment11){:name='comment11'}, [Gwenhwyfaer](http://www.blogger.com/profile/03775254923855147509) said...

So Tom, let me get this right - your response to seeing a library that states
from the outset that it's written in C is "don't write it in C", and your main
supporting argument for that is "C is obsolete, and all C compilers compile C++
too" (which isn't even true)...?

Well, obviously I have no vested interest in preventing you from making a fool
of yourself in public, but... Actually, no. Knock yourself out.

---

###### At [April 22, 2008 6:58 PM](#comment12){:name='comment12'}, Anonymous said...

Tom,

While its true that most people who make C compilers also make C++ compilers,
its not relevant.  This code would be more elegant in C++, but it would also be
more elegant in SML.  The author is interested in a C implementation, to use in
C programs.  Something you can't really do in C++.  C and C++ are different
languages.  They have a similar lineage, but there are in fact rather few real C
programs which would compile with a C++ compiler, and visa versa.

---

###### At [April 23, 2008 4:58 AM](#comment13){:name='comment13'}, Anonymous said...

Jesus Christ, use an AVL tree!

---

###### At [April 23, 2008 7:35 AM](#comment14){:name='comment14'}, [mtrimpe](http://www.blogger.com/profile/04220702365957612769) said...

I think he didn't use an AVL tree because he was interested in exploring the
promising ideas explained in the recently surfaced research-paper on
left-leaning red-black-trees; which claimed that they were easier to implement
and understand.

---

###### At [April 23, 2008 8:32 AM](#comment15){:name='comment15'}, Jason said...

Regarding AVL trees, I did not use them because they require recursive
implementation.

Regarding pointer hashing to generate treap priority fields, this is something I
considered, but since randomness is critical to treap performance, I would have
used something like [Bob Jenkins's
lookup3](http://burtleburtle.net/bob/hash/doobs.html).  This is something I
still might try out, but treaps still have two things going against it for use
in malloc: recursive implementation, and concerns about performance variance.

Regarding the heavy use of macros, this is necessary in order to use red-black
trees as a generic data structure that can be mixed in with other data
structures.  As for debugging and maintenance, I have extensive tests that give
me some measure of confidence in the code's correctness.  The macros make using
an interactive debugger less useful, but they do not make the code undebuggable.

Regarding the use of C++, that's a non-starter; malloc goes in libc!

---

###### At [May 19, 2008 7:48 AM](#comment16){:name='comment16'}, Bill Cox said...

Hi, Jason.

Thanks for this very helpful discussion!  I'm currently enhancing the datadraw
database generator (datadraw.sf.net) to support ordered\_lists, and so far I've
been working on an LLRB tree version.  I want to take your experience into
account, and do the best implementation I can.  Just to be clear, in summary,
you found:
- Traditional red/black trees are faster than LLRBs for insert and delete by a
  wide margin
- AVL trees require recursion
- Treaps require recursion.

I'm going to challenge you on treap recursion, though I haven't tried it yet.  I
always implement heaps with floatUp and floatDown functions using while loops
rather than recursion.  Why are treaps different?

Bill Cox, bill@billrocks.org

---

###### At [May 19, 2008 8:01 AM](#comment17){:name='comment17'}, Jason said...

Bill,

The reason LLRBs are slower than the other implementations I compared to is that
the down-pass-only approach does more node splitting/merging than does lazy
fixup.  I expect that most of the performance difference could be removed by
doing lazy fixup for LLRBs.

Regarding treap recursion, you _must_ do a down-pass, then an up-pass, so you
need to have parent pointers, a stack that tracks the current path through the
tree, or use recursion.  I suppose you could search down the tree in order to
find the parent of the current node during the up-pass phase, but that's much
less efficient than the alternatives.

---

###### At [July 5, 2008 4:27 PM](#comment18){:name='comment18'}, [Michael](http://www.blogger.com/profile/17951691329821568211) said...

You may be interested in the top-down red-black tree implementation at
[eternallyconfuzzled](http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx)

In my benchmarks, I found the top-down to be twice as fast as the bottom-up
approach in insertion and deletion.

I have also made a threaded version of the top-down tree.

---

###### At [September 23, 2008 9:01 AM](#comment19){:name='comment19'}, Anonymous said...

I've been reading about and non-recursive AVL implementation which is
interesting.

[Non recursive AVL](http://neil.brown.name/blog/AVL)

---

###### At [September 26, 2008 3:48 AM](#comment20){:name='comment20'}, Alexander said...

It's interesting that LLRB trees try to limit the ways to represent 3- and
2-nodes (they must lean on the left), whether traditional RB trees allow more
flexibility, but still require a rigid representation of 4-nodes. This requires
some fixups on the parent nodes when you go down the tree.

If we allowed arbitrary representations of 4-nodes (any group of 3 nodes
connected by red links), then we would need to do no fixup on the parents when
going down, but just fixups on the children.

This sounds like the laziest possible approach and it really looks like the
lazier the better...

---

###### At [January 25, 2009 2:50 PM](#comment21){:name='comment21'}, [Default](http://www.blogger.com/profile/11769065948592548272) said...

Hi, just a question why did you use macros instead of function pointers to
create a generic RBT?

---

###### At [January 25, 2009 2:57 PM](#comment22){:name='comment22'}, Jason said...

If the implementation were to use function pointers, it would prevent the
compiler from inlining code, hard-coding structure field offsets, etc.  Think of
the cpp-based implementation as C's best (least bad?) approximation of generics
(e.g. C++ templates).

---

###### At [March 16, 2009 12:05 PM](#comment23){:name='comment23'}, [Quinn Taylor](http://www.blogger.com/profile/13813057891310867947) said...

AVL trees don't necessarily require recursion; I recently implemented one with
iterative insert and remove that uses a malloc()ed array as a stack. My code is
implemented in Objective-C and as such is specifically designed to accommodate
objects, but the principles could easily be adapted. (Note that all my BST
variants use a sentinel node at all leaf nodes, which does a bit to boost
performance any time you search for a node (including insert and delete).

http://cocoaheads.byu.edu/code/CHDataStructures/  
http://dysart.cs.byu.edu/chsvn/CHDataStructures/

The file of interest probably include:

http://dysart.cs.byu.edu/chsvn/CHDataStructures/source/CHAVLTree.m  
http://dysart.cs.byu.edu/chsvn/CHDataStructures/source/CHAbstractBinarySearchTree.m  
http://dysart.cs.byu.edu/chsvn/CHDataStructures/source/CHAbstractBinarySearchTree\_Internal.h  
(If these last few links are ever broken, check the first link to see if the SVN
repository has moved.)

I've been looking for ways to speed up my remove for red-black trees; the
top-down algorithm (at least as I've implemented it) actually slows down
exponentially rather than linearly with the number of items. This probably isn't
a concern for small trees, but as a library author I'd much rather have it work
fast in all cases.  :-)  Any leads would be much appreciated.

---

###### At [January 28, 2010 8:36 AM](#comment24){:name='comment24'}, [陈明](http://www.blogger.com/profile/07042472465788908587) said...

Why do you refuse to use recursion in your implementation? Recursion can
simplify your codes, and make them more readable. Also I doubt how much
recursion will be slower than iteration.
