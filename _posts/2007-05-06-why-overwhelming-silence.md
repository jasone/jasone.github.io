---
layout: post
title: Why the overwhelming silence?
categories: blog
---

I uploaded an updated version of the [Parsing
module](/files/Parsing/Parsing.py).  The changes are
minor, which is a good indicator of the code's maturity when you consider that I
continue to use it heavily to create new parsers.  This is a really solid parser
generator, yet the public reception has been overwhelming silence.

I can guess why this might be, but given the generally sad state of Python-based
parser generator software, I expected that at least _someone_ would find Parsing
useful.  I would really appreciate hearing from people who evaluate the
software, why you decide not to use it, so that I can potentially do a better
job of meeting the programming community's needs.

### Comments

###### At [June 24, 2007 12:56 PM](#comment1){:name='comment1'}, [self](http://www.blogger.com/profile/05731049271980572297) said...

Where did you post about it?

---

###### At [June 24, 2007 3:01 PM](#comment2){:name='comment2'}, Jason said...

I posted in comp.lang.python.announce and comp.compilers.  freshmeat.net would
be a good idea too, but those newsgroups appear to be widely read enough that
the word should have gotten out, and they are more specific to a Python parser
generator than freshmeat is.

---

###### At [September 26, 2007 2:01 AM](#comment3){:name='comment3'}, [Zendwy](http://www.blogger.com/profile/13776953464954296283) said...

I already have a parser generator in my head.

---

###### At [November 28, 2007 6:05 PM](#comment4){:name='commen4'}, [Kragen Javier SItaker](http://pobox.com/~kragen/) said...

Probably nobody's heard of it.  Except for me, and I haven't written any parsers
in Python lately, and I had forgotten you had written it.

On Freshmeat, Parsing.py is categorized in Software Development::Compilers (425
projects), Software Development::Code Generation (567 projects), and Software
Development::Libraries::Python Modules (432 projects).  Nobody is going to find
it that way.  That is not your fault, but it is true.

Comp.lang.python isn't organized so that someone can say, "Hey, I want to write
a parser in Python; I wonder what parser generators people have written over the
last few years that might be useful?"  So only people who happened to be about
to start writing a parser when you posted your announcement are likely to have
noticed it, or those with an intense interest in parser generators.

<http://del.icio.us/url/ada074f08bcb1c29b6657510925c8777> says that 10 people on
del have bookmarked the Parsing home page, all of them in August, probably in
response to your comp.lang.python post then, in response to a request.  It's one
of only 35 entries in <http://del.icio.us/tag/parser%2Bgenerator%2Bpython> but
maybe that wouldn't be the first place someone would look for it.

In "You and Your Research", Hamming recommends spending as much time promoting
your research as it takes to do it in the first place, so that other people can
benefit from it.  Here are some ideas about how you could promote it:
- Find other people who are writing things that need parsers in Python and see
  if they might be interested in using Parsing.py.
- Give it a more Googleable name, and one that isn't so similar to pyParsing.
- Find other people's parsers and write new versions of them in Parsing.py.
  Point out the differences, positive or negative.  Especially if they're
  talking about them in blogs, comp.lang.python, or web forums.
- Figure out who maintains http://python.fyxm.net/topics/parsing.html and
  suggest that they add Parsing.py to the list.
- Apparently Martin von Loewis wrote a paper about Python parser generators;
  maybe you should email him to see if he's interested in Parsing.py:
  <http://python.fyxm.net/sigs/parser-sig/towards-standard.html>

---

###### At [March 12, 2008 8:33 AM](#comment5){:name='comment5'}, Anonymous said...

I suggest writing an enthusiatic post describing your parser's benefits, and
then posting it to a Python-programmer-heavy site like programming.reddit.com.

---

###### At [March 14, 2008 9:07 AM](#comment6){:name='comment6'}, [union](http://www.blogger.com/profile/09666913974313452020) said...

Hi, thanks for the module, its just what I was looking for.

I only found it by chance (I followed the link from Pavlov's site to your
jemalloc article, after that I decided to read some other posts, and thats how I
found about it.)

Probably it would be good idea to at least submit it to wiki on pythons main
site, and do submit to reddedit.

Thank you  
Luka

PS: I am using it for configuration language, that has outgrown it's current
regex implementation.
