---
layout: post
title: Firefox fragmentation?
categories: blog
---

As [Firefox](http://www.mozilla.org/projects/firefox/) 3 nears release, some of
its developers are taking a close look at memory fragmentation issues. There is
good information over at
[pavlov.net](http://blog.pavlov.net/2007/11/10/memory-fragmentation/) that I
won't repeat here. One recurring theme though is that memory usage in version 2
was reported by some users to be problematic, and fragmentation is a suspected
culprit.  This has motivated an investigation of memory fragmentation before
version 3 is released.

As the author of
[jemalloc](http://people.freebsd.org/%7Ejasone/jemalloc/bsdcan2006/jemalloc.pdf),
I have a deep (read: obsessive) interest in memory fragmentation issues, so I
spent some time brushing off my [malloc plotting
tools](http://people.freebsd.org/%7Ejasone/jemalloc/progs/) today. Here is a
plot from a run of firefox 2.0.0.9 running on FreeBSD-current. In order to
generate the allocation trace, I launched firefox, then went through several
cycles of opening lots of tabs/windows and then closing most of them.

![]({{ "/images/firefox/firefox_20071128b.png" | absolute_url }}){:height="100%" width="100%"}

Time starts at the left, and execution ends at the right.  Each vertical column
of pixels represents a snapshot of memory usage at a particular moment during
program execution (time is measured by allocation events).  Since there are
millions of allocation events, most snapshots are left out to make the plot size
manageable.  Similarly, there are many bytes of memory that must be represented
by each vertical column of pixels, so each pixel represents a bucket of 256kB.
Low addresses are at the bottom of the plot.

Note the peaks that are mostly green.  Those occur during peak memory usage
periods, and overall, the plot shows that fragmentation isn't bad.  Take this
with a grain of salt though, since the plot only represents perhaps 15 minutes
of heavy web browsing.

If you want to see much more detail (each bucket is 4kB -- one page), take a look at this [image](/images/firefox//firefox_20071128a.png).  It is big enough to cause most image viewers to choke, so beware.

### Comments

###### At [December 17, 2007 11:20 PM](#comment1){:name='comment1'}, kad77 said...

Hey Jason,

Enjoyed the post. I've been following pavlov's work on the memory issues for for
a few weeks, too.

I hope you two can continue working with jemalloc to Firefox's benefit and
hopefully inclusion of am improved allocator in FF3.

Many of us have waited a long time for various Mozilla memory issues to get some
greater attention... better late in the game than not t all.

Thanks for your hard work.
