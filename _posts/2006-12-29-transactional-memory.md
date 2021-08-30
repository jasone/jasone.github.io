---
layout: post
title: "Transactional memory: Panacea or confounder?"
categories: blog
---

There is a very nice review article on the subject of transactional memory (TM)
as applied to programming languages in [ACM Queue, Vol. 4 No.
10](http://www.acmqueue.org/modules.php?name=Content&pa=showpage&amp;pid=444).
The article does a reasonable job of describing the basics of transaction
processing, and describes how these techniques could be used to directly expose
transaction semantics as a built-in programming language feature.

Transaction processing has been actively researched by the databases community
since well before my time, probably for 30+ years.  This is important, because
it means that the really new ideas embodied by TM are limited to approximately
the following:
- Transactions can be exposed as a general purpose language construct (not just
  in database languages).
- Hardware can be modified to reduce TM overhead.

These ideas are interesting enough, and certainly warrant research.  However,
there are some basic compromises that transactions require, and nothing about TM
changes that.

There is a fundamental difference of approach when writing code that uses
synchronization primitives (mutexes, condition variables, etc.) for
deadlock/livelock-free algorithms, versus transaction processing.  For the
former, we specifically write code that will always make forward progress, while
trying to limit the synchronization overhead.  With transactions, we are working
at a much higher level, and leave it to the transaction manager to detect when
conflicts arise, then (hopefully) make progress even when transactions conflict.

So, I think TM is both panacea and confounder:
- **Panacea:** Programmers can work at a higher level that doesn't require
  thinking as hard about parallel execution.
- **Confounder:** When working with transactions instead of low level
  synchronization, there is a lack of detailed information that prevents
  achieving maximum performance for many applications.

TM may be the right tool for some programming jobs, but it is almost certainly
not the right tool for all programming jobs.
