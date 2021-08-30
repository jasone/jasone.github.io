---
layout: post
title: Mr. Malloc gets schooled
categories: blog
---

I've been terribly busy for the past 8 months, frantically developing
[Crux](http://www.canonware.com/Crux/) and using it to conduct experiments so
that I can finish my thesis for a PhD in computational biology.  An ironic thing
happened this weekend, and this is the perfect forum for sharing it.  I spent
two days trying to figure out why Crux's memory usage was growing without bound
when analyzing large datasets.  I looked for memory leaks, inefficient caching,
garbage collection issues, _any_ explanation for the memory usage.  After much
pain and agony (including that inflicted on the system administrator who kindly
patched up the bleeding Beowulf cluster I left in my wake), I finally came to
the conclusion that the problem wasn't in Crux.  That led me to...
[glibc](http://www.gnu.org/software/libc/)'s
[ptmalloc](http://www.malloc.de/en/).  Launching Crux with
[jemalloc](http://www.freebsd.org/cgi/cvsweb.cgi/src/lib/libc/stdlib/malloc.c)
`LD\_PRELOAD`'ed made the problem go away!

It turns out that memory was fragmenting without bound.  Crux incrementally
reallocates (actually `free(...); posix_memalign(...)`) vectors of
double-precision floating point numbers.  For the dataset I'm currently
analyzing, these vectors are multiples of ~337KiB, where the multiplier is
anything from 1 to ~65 (20+MiB).  I wouldn't have expected this to cause malloc
any fragmentation problems, since the last I knew, ptmalloc simply used `mmap()`
for all allocations above 128KiB.  However, here's what it does now (taken
directly from the glibc source code):

> Update in 2006:
>
> The above was written in 2001. Since then the world has changed a lot.  Memory
> got bigger.  Applications got bigger. The virtual address space layout in 32
> bit linux changed.
>
> In the new situation, brk() and mmap space is shared and there are no
> artificial limits on brk size imposed by the kernel. What is more,
> applications have started using transient allocations larger than the 128Kb as
> was imagined in 2001.
>
> The price for mmap is also high now; each time glibc mmaps from the kernel,
> the kernel is forced to zero out the memory it gives to the application.
> Zeroing memory is expensive and eats a lot of cache and memory bandwidth. This
> has nothing to do with the efficiency of the virtual memory system, by doing
> mmap the kernel just has no choice but to zero.
>
> In 2001, the kernel had a maximum size for brk() which was about 800 megabytes
> on 32 bit x86, at that point brk() would hit the first mmaped shared libaries
> and couldn't expand anymore. With current 2.6 kernels, the VA space layout is
> different and brk() and mmap both can span the entire heap at will.
>
> Rather than using a static threshold for the brk/mmap tradeoff, we are now
> using a simple dynamic one. The goal is still to avoid fragmentation. The old
> goals we kept are
> 1) try to get the long lived large allocations to use mmap()
> 2) really large allocations should always use mmap() and we're adding now:
> 3) transient allocations should use brk() to avoid forcing the kernel having
>    to zero memory over and over again
>
> The implementation works with a sliding threshold, which is by default limited
> to go between 128Kb and 32Mb (64Mb for 64 bitmachines) **_[actually
> 512KiB/32MiB for 32/64-bit machines as configured in glibc]_** and starts out
> at 128Kb as per the 2001 default.
>
> This allows us to satisfy requirement 1) under the assumption that long lived
> allocations are made early in the process' lifespan, before it has started
> doing dynamic allocations of the same size (which will increase the
> threshold).
>
> The upperbound on the threshold satisfies requirement 2)
>
> The threshold goes up in value when the application frees memory that was
> allocated with the mmap allocator. The idea is that once the application
> starts freeing memory of a certain size, it's highly probable that this is a
> size the application uses for transient allocations. This estimator is there
> to satisfy the new third requirement.

So, ptmalloc got smart, and appears to have opened itself up to a serious
fragmentation problem due to some inadequacy in its data segment layout
policies.  Here's a simple fix that disables the sliding threshold:

```c
mallopt(M_MMAP_THRESHOLD, 128*1024);
```

How did I fail to consider this possibility for two whole days?  There are a few
contributing factors:
- I'd never actually seen ptmalloc fail spectacularly before.  I've received
  several emails over the past year from people using jemalloc to avoid ptmalloc
  fragmentation problems, but I didn't know what conditions actually triggered
  the problem(s).
- As the author of jemalloc, I'm keenly aware of how often people are wrong when
  they blame the memory allocator for their problems.
- My reasoning about how memory allocation works is tainted by intimate
  knowledge of how jemalloc works, and I failed to consider that Crux's memory
  allocation patterns could cause problems for other allocators.

So there you have it: I've been schooled, and not in a pretty way.

### Comments

###### At [May 18, 2009 6:53 AM](#comment1){:name='comment1'}, Anonymous said...

Why did you need special alignment?

---

###### At [May 18, 2009 10:14 AM](#comment2){:name='comment2'}, Jason said...

The special alignment requirement (64-byte alignment) is due to concurrently
processing the vectors with pthreads.  It's important to avoid modifying the
same L1 cache line in more than one thread.  In practice, this is not an issue
except with small vectors, since all modern allocators use page alignment for
those that exceed one page.

---

###### At [May 18, 2009 10:15 AM](#comment3){:name='comment3'}, [xose](http://www.blogger.com/profile/17358721743680414194) said...

latest glibc malloc release makes malloc to be more scalable.

**glibc 2.10 news**: <http://udrepper.livejournal.com/20948.html>  
code:  <http://sourceware.org/git/?p=glibc.git;a=tree;f=malloc>

\-thanks\-

regards,

---

###### At [May 19, 2009 8:49 PM](#comment4){:name='comment4'}, [poige](http://www.blogger.com/profile/14290248818990453110) said...

Hi, Jason! I wonder did you use http://www.canonware.com/download/jemalloc/ versions ( \#  jemalloc\_linux\_20080827a.tbz  
\# jemalloc\_linux\_20080828a.tbz ) or a newer one? :-)

---

###### At [May 20, 2009 5:00 AM](#comment5){:name='comment5'}, [poige](http://poige.livejournal.com/) said...

xose, "more scalable" doesn't mean to be "more effective" in terms of _memory
fragmentation_ (what's happened to be the matter of Jason's posting).

---

###### At [May 20, 2009 8:00 AM](#comment6){:name='comment6'}, Jason said...

poige, I used the same jemalloc sources as are available in
[jemalloc\_linux\_20080828a.tbz](http://www.canonware.com/download/jemalloc/jemalloc_linux_20080828a.tbz).

---

###### At [June 19, 2009 12:02 PM](#comment7){:name='comment7'}, [hyc](http://www.blogger.com/profile/15473250487285924085) said...

Too bad the new glibc malloc seems to be totally broken in heavily threaded
code.

<https://bugzilla.redhat.com/show_bug.cgi?id=504963>

I'd be happier if it was a bug in my code, because then I could actually fix it.
