---
layout: post
title: Overzealous use of my red-black tree hammer
categories: blog
---

When [Firefox](http://www.mozilla.com/firefox/) 3 was released, jemalloc was
left disabled for the [OS X](http://www.apple.com/macosx/) version, essentially
because OS X's malloc implementation did as good a job as jemalloc (in terms of
both speed and memory usage), and we didn't think it worth introducing potential
regressions due to changed memory layout.  Recently I have been working on a
[memory reserve](http://wiki.mozilla.org/Mozilla_2/Memory/OOM_API) system that
allows Firefox to simplify its error handling with regard to out-of-memory
errors.  Since the memory reserve is necessarily deeply integrated with the
allocator, we need to use jemalloc on all platforms in order to take advantage
of this new facility.  This prompted me to take a closer look at jemalloc
performance on OS X.  In summary:
1. Replacing the system malloc on OS X is Really Hard(TM).
2. jemalloc was doing _way_ too much [red-black tree](http://en.wikipedia.org/wiki/Red-black_tree) manipulation.

On [ELF](http://en.wikipedia.org/wiki/Executable_and_Linkable_Format)-based
systems (pretty much all modern Unix and Unix-like systems except OS X), it is
possible to cleanly replace the system malloc, either by directly implementing
the appropriate functions (malloc, realloc, free, etc.), or by using the
LD\_PRELOAD environment variable to preload a dynamic library that contains a
malloc implementation.  For Windows, replacing malloc is much harder; it is
necessary to [create a custom
CRT](http://benjamin.smedbergs.us/blog/2008-01-10/patching-the-windows-crt/).
On the bright side, at least it is _possible_ to create a custom CRT, since
source code is included with [MS Visual
Studio](http://msdn.microsoft.com/en-us/vstudio/default.aspx).

OS X uses the [Mach-O](http://en.wikipedia.org/wiki/Mach-O) format, and in order
to completely replace the system malloc, it would be necessary to compile a
custom libSystem. As far as I know, that has not been possible outside the
confines of [Apple](http://apple.com/) since version 10.3 (2+ years ago).  Even
if it were possible, there would be all sorts of undesirable aspects to shipping
a custom libSystem with Firefox; libSystem is a huge library, and binary
compatibility issues would be a constant problem.  So, the only remaining viable
option is to subvert the [malloc zone
machinery](http://developer.apple.com/documentation/Performance/Conceptual/ManagingMemory/Articles/MemoryAlloc.html#//apple_ref/doc/uid/20001881-100720).
There is no supported method for changing the default zone, and furthermore,
[CoreFoundation](http://developer.apple.com/corefoundation/) directly accesses
the default zone.  Enough about that though; suffice it to say that I did find
ways to subvert the malloc zone machinery.

Once Firefox was successfully using jemalloc for all memory allocation, I
started doing performance tests.  Memory usage differences were minor, but
jemalloc was consistently slower than OS X's allocator.  It took a lot of
profiling for me to finally accept the hard truth: jemalloc was spending way too
much time manipulating red-black trees.  My first experimental solution was to
replace red-black trees with [treaps](http://en.wikipedia.org/wiki/Treap).
However, this made little overall difference.  So, the problem was _too many_
tree operations, not _slow_ tree operations.

After a bit of code review, it became clear that when I fixed a page allocation
bottleneck earlier this year, I was overzealous with the application of
red-black trees.  It is possible to use constant-time algorithms based on linear
page map data structures for splitting/coalescing sequential runs of pages, but
I had re-coded these operations entirely using red-black trees.  So, I enhanced
the page map data structures to support splitting/coalescing, and jemalloc
became markedly faster.  For example, Firefox sped up by as much as ~10% on
[JavaScript](http://en.wikipedia.org/wiki/JavaScript)-heavy benchmarks.  (As a
side benefit, memory usage went down by 1-2%).

In essence, my initial failure was to disregard the difference between a O(1)
algorithm and a O(lg n) algorithm.  Intuitively, I think of logarithmic-time
algorithms as fast, but constant factors and large n can conspire to make
logarthmic time not nearly good enough.

### Comments

###### At [July 27, 2008 6:49 AM](#comment1){:name='comment1'}, Anonymous said...

Have you taken a look at Vladimir Marangozov's PyMalloc used in the CPython
interpreter?

It's a optimization for many small objects with similar sizes, a typical memory
usage pattern in interpreters. It falls back to the system malloc for anything
larger than a few hundred bytes.

---

###### At [July 27, 2008 9:40 AM](#comment2){:name='comment2'}, [Turly](http://www.blogger.com/profile/01313311916172146471) said...

You could use mach\_override () to override malloc on OS X.
