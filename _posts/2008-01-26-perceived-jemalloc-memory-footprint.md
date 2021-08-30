---
layout: post
title: Perceived jemalloc memory footprint
categories: blog
---

For the past couple of months I have been working with the
[Mozilla](http://www.mozilla.com/en-US/about/) folks to integrate jemalloc into
[Firefox](http://www.mozilla.org/projects/firefox/).  This past week,
[Stuart](http://blog.pavlov.net/) has been doing lots of performance testing to
make sure jemalloc is actually an improvement, and he ran into an interesting
problem on Windows: jemalloc appears to use more memory than the default
allocator, because Windows' task manager reports mapped memory rather than
actual working set.  As near as we could tell, jemalloc was actually reducing
the working set a bit, but the perception from looking at the task manager
statistics was that jemalloc was a huge pessimization.  This is because jemalloc
manages memory in chunks, and leaves each chunk mapped until it is completely
empty.  Unfortunately, even though there is a way to tell Windows that unused
pages can be discarded if memory becomes tight, appearances make it seem as if
jemalloc is hogging memory.  Well, appearances do matter, so I have been working
frantically the past few days to come up with a solution.  The upshot is that I
may have ended up with a solution to related problems for jemalloc in
[FreeBSD](http://www.freebsd.org/), its native setting.

In FreeBSD, there is an optional runtime flag that tells malloc to call
[madvise(3)](http://www.freebsd.org/cgi/man.cgi?query=madvise&amp;apropos=0&amp;sektion=0&amp;manpath=FreeBSD+7.0-RELEASE&amp;format=html)
for pages that are still mapped, but for which the data are no longer needed.
This would be great, but madvise() is quite expensive to call, which leaves us
with little choice but to disable those calls by default.  What that means is
that when memory becomes tight and the kernel needs to free up some RAM, it has
to swap out the junk in those pages, just as if the junk were critical data.
The repercussions are system-wide, since pretty much every application has those
madvise() calls disabled.

The solution is pretty straightforward: rather than calling madvise() as soon as
pages of memory can be discarded, simply make a note that those pages are dirty.
Then, if the amount of dirty discardable memory exceeds some threshold, march
down through memory and call madvise() until the amount of dirty memory has been
brought under control.  This tends to vastly reduce the number of madvise()
calls, but without ever leaving very much dirty memory laying around.

I still need to do a bunch of performance analysis before integrating this
change into FreeBSD, but my expectation is that as an indirect result of trying
to make jemalloc look good on Windows, FreeBSD is going to benefit.

### Comments

###### At [January 31, 2008 9:56 AM](#comment1){:name='comment1'}, [Tommy](http://www.blogger.com/profile/01623482863723373451) said...

Hey Jason, this is Tom Reilly from Adobe.  Nice chatting yesterday.

Window's VirtualFree w/ MEM\_DECOMMIT was designed for this use case.  Its a
little stronger than madvise w/ MADV\_FREE though b/c you know the page is no
longer backed.

On some OS's you can call mmap again on a region with PROT\_NONE and it will
remap the memory as decommitted.  This is what we use for "trimming".  Although
that technique exposed a bug in OS X so we don't use it there ... I haven't
tried madvise yet though, that may be the ticket for OS X.

---

###### At [February 3, 2008 9:45 AM](#comment2){:name='comment2'}, Jason said...

tommy,

We did end up using VirtualFree(..., MEM\_DECOMMIT) on Windows, with the desired
results.  The difficulty was adding code to keep track of whether pages are
decommitted so that they can be re-committed upon reuse.

On OS X I tried several things, including madvise() and msync(...,
MS\_KILLPAGES).  The only thing that worked for reducing resident set size was
to mmap(... MAP\_FIXED ...) on top of an existing mapping to dissociate dirty
pages from the process.  Does this sound like something that might hit the same
bug you ran into with using PROT\_NONE?  I haven't noticed any problems.

---

###### At [February 4, 2008 8:30 AM](#comment3){:name='comment3'}, [Tommy](http://www.blogger.com/profile/01623482863723373451) said...

mmap w/ PROT\_NONE and MAP\_FIXED was our original solution and that works on
Linux/Solaris.   On OS X it leads to a virtual memory leak and eventual crash.
Its like they create a new mapping and don't erase the old one or something.  We
told Apple about the problem.

The fix we're thinking for OS X is vm\_deallocate followed by vm\_map.  It
introduces a race condition that must be handled (another thread mmap's the
memory after its decallocated) but it appears to work.
