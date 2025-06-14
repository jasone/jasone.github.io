---
layout: post
title: jemalloc Postmortem
categories: blog
---

The [jemalloc](https://jemalloc.net/) memory allocator was first conceived in early 2004, and has
been in public use for about 20 years now. Thanks to the nature of open source software licensing,
jemalloc will remain publicly available indefinitely. But active upstream development has come to an
end. This post briefly describes jemalloc's development phases, each with some success/failure
highlights, followed by some retrospective commentary.

## Phase 0: Lyken

In 2004 I began work on the Lyken programming language in the context of scientific computing. Lyken
was an eventual dead end, but its manual memory allocator was functionally complete by May 2005.
(The garbage collector which was to leverage its features was never completed.) In September 2005 I
started integrating the allocator into FreeBSD, and in March 2006 I removed the allocator from
Lyken, in favor of thin wrappers around system allocator functionality.

Why remove the memory allocator from Lyken after so much effort went into it? Well, once the
allocator was integrated into FreeBSD, it became apparent that the only feature missing from the
system allocator was a mechanism for tracking allocation volume in order to trigger per thread
garbage collection. And that could be implemented via thin wrappers using thread-specific data and
[dlsym(3)](https://www.man7.org/linux/man-pages/man3/dlsym.3.html). Interestingly, many years later
jemalloc even added the statistics gathering that Lyken would have needed.

## Phase 1: FreeBSD

Back in 2005 the transition to multi-processor computers was ongoing. FreeBSD had Poul-Henning
Kamp's excellent phkmalloc memory allocator, but that allocator had no provisions for parallel
thread execution. Lyken's allocator seemed like an obvious scalability improvement, and with
encouragement from friends and colleagues I integrated what quickly became known as jemalloc. Ah,
but not so fast! Shortly after integration it became apparent that jemalloc had terrible
fragmentation issues under some loads, notably those induced by [KDE](https://kde.org/)
applications. Just when I thought I was mostly done, this real-world failure called jemalloc's
viability into question.

In brief, the fragmentation issue arose from using a unified extent allocation approach (i.e. no
size class segregation). I had taken basic inspiration from Doug Lea's
[dlmalloc](https://gee.cs.oswego.edu/pub/misc/malloc.c), but without the intertwined battle-tested
heuristics that avoided many of the worst fragmentation issues. Much frantic research and
experimentation ensued. By the time jemalloc was part of a FreeBSD release, its layout algorithms
had completely changed to use size-segregated regions, as described in the [2006 BSDCan jemalloc
paper](https://people.freebsd.org/~jasone/jemalloc/bsdcan2006/jemalloc.pdf).

## Phase 1.5: Firefox

In November 2007, [Mozilla Firefox](https://www.mozilla.org/en-US/firefox/new/) 3 was nearing
release, and high fragmentation was an unresolved issue, especially on Microsoft Windows. Thus began
a year of working with Mozilla on memory allocation. Porting jemalloc to Linux was trivial, but
Windows was another matter. The canonical jemalloc sources were in the FreeBSD libc library, so we
essentially forked jemalloc and added portability code, upstreaming anything that was relevant to
FreeBSD. The entire implementation was still in one file, which reduced the friction of fork
maintenance, but the implementation complexity definitely surpassed what is reasonable in a single
file sometime during this phase of development.

Years later, Mozilla developers made significant contributions to the upstream jemalloc in an effort
to move away from their fork. Unfortunately, Mozilla benchmarks consistently showed that the forked
version outperformed the upstream version. I don't know if this was due to overfitting to a local
optimum or an actual indication of performance regression, but it remains one of my biggest jemalloc
disappointments.

## Phase 2: Facebook

When I started work at Facebook in 2009, I was surprised to discover that the biggest impediment to
ubiquitous jemalloc use in Facebook infrastructure was instrumentation. Critical internal services
were in the awkward situation of depending on jemalloc to keep memory fragmentation under control,
but engineers needed to debug memory leaks with [tcmalloc](https://github.com/google/tcmalloc) and
the `pprof` heap profiling tool that is part of
[gperftools](https://github.com/gperftools/gperftools). `pprof`-compatible heap profiling
functionality headlined the jemalloc 1.0.0 release.

jemalloc development migrated to [GitHub](https://github.com/jemalloc/) and continued sporadically
for the next few years as issues and opportunities arose. Other developers started contributing
significant functionality. Version 3.0.0 introduced extensive testing infrastructure, as well as
[Valgrind](https://valgrind.org/) support. The 4.x release series introduced decay-based purging and
[JSON](https://en.wikipedia.org/wiki/JSON)-formatted telemetry. The 5.x series transitioned from
"chunks" to "extents" to pave the way for better interaction with 2 MiB huge pages.

Somewhat more controversially, I removed Valgrind support in 5.0.0 because it was a significant
maintenance complication (numerous tendrils in subtle places), and it was unused inside Facebook;
other tools like `pprof` and [MemorySanitizer](https://clang.llvm.org/docs/MemorySanitizer.html)
dominated. I had received very little feedback about Valgrind support, and extrapolated that it was
not being used. In retrospect, that seems not to have been the case. In particular, the [Rust
language](https://www.rust-lang.org/) directly incorporated jemalloc into compiled programs, and I
think there was some overlap between Rust developers and Valgrind developers. People were angry.
jemalloc was probably booted from Rust binaries sooner than the natural course of development might
have otherwise dictated.

Facebook's internal telemetry is a wonder to behold, and it is a massive boon to have performance
data from myriad services informing memory allocator development. I don't think it's an accident
that two of the fastest memory allocators of the past decade (tcmalloc and jemalloc) benefit from
such data. Even "simple" things like fast-path optimizations are much easier to get right when there
are aggregated [Linux perf](https://en.wikipedia.org/wiki/Perf_%28Linux%29) data on hand. Harder
things like fragmentation avoidance are still hard, but if thousands of distinct workflows behave
well with no outlier regressions, then a change is probably safe. jemalloc has benefited immensely
from being integral to the Facebook infrastructure in terms of performance, resilience, and
consistent behavior. Additionally, jemalloc's own integrated statistics reporting capabilities arose
directly in response to this ubiquitous telemetry environment, and this turned out to generally
benefit both jemalloc development and non-Facebook application tuning/debugging far in excess of the
implementation effort required.

During my last year at Facebook I was encouraged to build a small jemalloc team so that we could
tackle some big tasks that would have been otherwise daunting. On top of major performance
improvements, we got things like continuous integration testing and comprehensive telemetry. When I
left Facebook in 2017, the jemalloc team carried on doing excellent development and maintenance work
for several years, almost entirely without my involvement, under the leadership of my esteemed
colleague, Qi Wang, and as evidenced by the commit history, with the excellent contributions of many
others.

## Phase 3: Meta

The nature of jemalloc development noticeably shifted around the time that Facebook rebranded itself
as Meta. Facebook infrastructure engineering reduced investment in core technology, instead
emphasizing *return* on investment. This is apparent in the jemalloc commit history. In particular,
the seeds for principled huge page allocation (HPA) were sown way back in 2016! HPA work continued
apace for several years, slowed, then stagnated as tweaks piled on top of each other without the
requisite refactoring that keeps a codebase healthy. This feature trajectory recently cratered. The
heartbreak for me is somewhat blunted since I have not been closely involved for years, but as a
result of recent changes within Meta we no longer have anyone shepherding long-term jemalloc
development with an eye toward general utility.

I don't want to dwell on drama, but it is perhaps worth mentioning that we reached a sad end for
jemalloc in the hands of Facebook/Meta even though most of the people involved were acting in good
faith. Corporate cultures shift in compliance with both external and internal pressures. And people
find themselves in impossible situations where the main choices are 1) make poor decisions under
extreme pressure, 2) comply under extreme pressure, or 3) get routed around. As individuals we
sometimes have enough influence to slow organizational degradation, maybe even contribute to
isolated renaissances, but none of us can prevent the inevitable.

I remain very grateful to my former colleagues for all their excellent work on jemalloc, and
Facebook/Meta in general for investing so much, for so long.

## Phase 4: Stasis

What now? As far as I am concerned, "upstream" jemalloc development has concluded. Meta's needs
stopped aligning well with those of external uses some time ago, and they are better off doing their
own thing. Were I to reengage, the first step would be at least hundreds of hours of refactoring to
pay off accrued technical debt. And I'm not sufficiently excited by what would come after to pay
such a high upfront cost. Perhaps others will create viable forks, whether from the `dev` branch or
from the 5.3.0 release (already three years old!).

In above sections I mentioned several phase-specific failures, but there were some generic failures
that surprised me despite a career focused on open source development.

- As mentioned, removing Valgrind caused some bad sentiment. But the root of the problem is lack of
  awareness about external uses and needs. I probably would have worked with others to preserve
  Valgrind support if I'd known that it mattered to anyone. As another example, I was completely
  unaware of jemalloc's use as the
  [Android](https://en.wikipedia.org/wiki/Android_(operating_system)) memory allocator for perhaps
  two years. And years later, unaware of its replacement until after the fact.
- Even though jemalloc development remained completely out in the open (not siloed inside Facebook),
  the project never grew to retain primary contributors from other organizations. The Mozilla effort
  by Mike Hommey to move Firefox to the upstream jemalloc was a near miss. Efforts by others to
  transition to a [CMake](https://cmake.org/)-based build system stalled multiple times, and never
  crossed the finish line. I knew from hard experience with
  [Darwin](https://en.wikipedia.org/wiki/Darwin_(operating_system)) that internally siloed open
  source projects cannot thrive ([HHVM](https://hhvm.com/) was a repeat lesson), but jemalloc needed
  more than open development to thrive as an independent project.

jemalloc was an odd diversion for me, since I have been a strong proponent of garbage collection
over manual memory management for over 25 years. Personally I'm happy to be working again on
garbage-collected systems, but jemalloc was a tremendously fulfilling project. Thank you to everyone
who made this project so worthwhile, collaborators, supporters, and users alike.
