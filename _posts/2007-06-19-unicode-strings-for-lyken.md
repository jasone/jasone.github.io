---
layout: post
title: Unicode strings for Lyken
categories: blog
---

[Lyken](http://lyken.net/), a programming language I am currently developing,
uses [Unicode](http://unicode.org/) for all strings.  Lyken is just one of many
languages that has to overcome a set of design challenges associated with
Unicode, though at least Lyken itself has no legacy support requirements.
However, since Lyken's runtime library is written in C, I still have to devise a
way to provide pure Unicode string support in Lyken, without making runtime
library development overly cumbersome.

A couple of years ago I decided to use a simplistic internal representation for
strings in Lyken.  The idea was to maintain an ASCII representation of each
string that was purely ASCII, but to also maintain a UCS-4 representation of
every string (lazily created for pure ASCII strings).  This had a critical
problem though:  C library interfaces use `(char *)` strings, thus making it
impossible to use non-ASCII strings for many purposes.  This problem made it
clear that I needed to somehow support UTF-8 in Lyken's runtime library.

One possible approach would be to internally store each string both as UTF-8 and
UCS-4, but that is a tremendous waste of memory both for ASCII and non-ASCII
strings.  Instead, I have decided to just store strings as UTF-8, but that has
performance issues for indexed access.

In order to mitigate the indexed access performance issue for UTF-8, I store a
lazily initialized table that records the location of every n<sup>th</sup>
character (n=32 for now).  Immutable strings make lazy table initialization safe
for multi-threaded programs, with no need for synchronization.  The table is
only needed for non-ASCII strings and is known to be present just past the end
of the string itself iff the string's byte/character lengths differ.

I have searched for information on better approaches to solving the indexed
access problem for UTF-8 strings, but have found nothing.  If you know of
anything better, please let me know.

### Comments

###### At [June 21, 2007 3:35 AM](#comment1){:name='comment1'}, [self](http://www.blogger.com/profile/05731049271980572297) said...

Will [this](http://swtch.com/plan9port/unix/) help any?

---

###### At [June 21, 2007 8:09 AM](#comment2){:name='comment2'}, Jason said...

I don't see anything in the Plan 9 libraries that does quite what I'm looking
for.  Plan 9 does serve as a good example for ubiquitous UTF-8 support though.
I found this [paper](http://plan9.bell-labs.com/sys/doc/utf.pdf) to be a really
good read.
