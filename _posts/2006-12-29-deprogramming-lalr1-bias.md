---
layout: post
title: Deprogramming the LALR(1) bias
categories: blog
---

Look around for LR(1) parser generators, and you will primarily find LALR(1)
parser generators.  There seems to be an unspoken assumption that LALR(1) is
somehow better than LR(1), but look at the following pertinent facts:
- In terms of grammars that can be handled by various parser generation
  techniques, SLR(1) &sub; LALR(1) &sub; LR(1).
- SLR(1) and LALR(1) tables are always the same size, but LR(1) tables are
  potentially larger.

The important thing to notice here is that LALR(1) is _not_ the most powerful of
the three parser generation techniques listed above.  LALR(1) may introduce
reduce/reduce conflicts that do not exist when using LR(1).

So then, why is there a bias toward LALR(1)?  I suspect that it has to do with
the well known and widely cited [dragon
book](http://en.wikipedia.org/wiki/Compilers:_Principles%2C_Techniques%2C_and_Tools),
which treats LALR(1) as the culmination of the parser generation algorithms it
presents.  There is one little detail that is not mentioned at all though: it is
possible to compress LR(1) tables to be the same as LALR(1) tables, with the
exception of avoiding table compression that would introduce LALR(1)'s
reduce/reduce conflicts.  Granted, the book is over 20 years old now, but I
would not be surprised if the [new
edition](http://en.wikipedia.org/wiki/Compilers:_Principles%2C_Techniques%2C_and_Tools_%282nd_Edition%29)
preserves this omission.

I recently finished implementing a parser generator (as the basis for a GLR
parser, as described by the
[Elkhound](http://www.cs.berkeley.edu/%7Esmcpeak/elkhound/) [technical
report](http://www.cs.berkeley.edu/%7Esmcpeak/elkhound/elkhound.ps).  I was
initially unable to wrap my head around the "efficient" method that the dragon
book provides for LALR(1) parser generation, so I took the incremental approach
of implementing SLR(1), converting that to LR(1), then finally converting that
to LALR(1) -- the "easy, inefficient" method according to the dragon book.
Well, when I got to the point of compressing LR(1) tables to LALR(1) tables, I
questioned the necessity of compressing states when reduce/reduce conflicts
would result.  As near as I could tell, there is no fundamental requirement to
do so, which means that compressed LR(1) tables that avoid such conflicts should
be the gold standard, rather than LALR(1).

This seemed too obvious to have been overlooked by the compiler community, so I
looked high and low for prior art.  The best I found was a [comp.compilers
post](http://groups-beta.google.com/group/comp.compilers/browse_thread/thread/c02b5fff15f1a2ca/e19dadb035501123?&amp;hl=en)
by Chris F. Clark on 1 July 2003, which says, in part:
> That brings us back to "full" or canonical LR(k), where k is >= 1.
> LR(k) parsing preserves the needed left context to disambiguate
> certain situations where LALR(k) finds the rules to conflict.  The
> canonical way of doing this is to not merge states with different
> lookaheads in the first place. That causes an explosion in the
> table size as many states are kept distinct simply because they
> have different lookaheads, when in reality the lookahead for those
> states will never be consulted.  A more modern method for solving
> the problem involves splitting the states only when a conflict is
> detected.  Then, if the grammar is LALR, no splitting will occur
> and one has the same size machine as the LR(0), but as conflicts
> arise the require left context to resolve, the tables slowly grow.

There is likely some discussion of this optimization somewhere in the primary
literature, but several hours of searching failed to turn it up.  The closest I
found is [S&ouml;nke Kannapinn's PhD
thesis](http://edocs.tu-berlin.de/diss/2001/kannapinn_soenke.pdf) (in German,
but the abstract is also in English), which comes to the same conclusion via a
very different route.

In summary, if you need to write a parser generator, make it LR(1) and use
careful table compression, rather than using the less general LALR(1) approach.

---

### Comments

###### At [January 8, 2007 11:46 AM](#comment1){:name='comment1'}, [georges-m-r.louis](http://www.blogger.com/profile/11899715126060778293) said...

Long ago (1973) there was a paper by David Pager (U. of Hawaii) on the "lane
tracing" algorithm to construct LR(k) parsers.

ABSTRACT. The paper presents, as far as the author is aware, the first practical general method for constructing LR(k) parsers. It has been used, without computational difficulty, to produce LR(1), LR(2) and LR(3) parsers for grammars of the size of ALGOL.

Search "david pager lane tracing" on Google and you might ba able to get a copy.

---

###### At [January 8, 2007 1:09 PM](#comment2){:name='comment2'} Jason said...

Excellent, thank you for the reference, georges-m-r.louis.  I finally broke down
and re-joined the ACM this morning in order to get access to various papers on
this subject.  Another apparently pertinent paper is _Efficient Full LR(1)
Parser Generation_, by David Spector.

I've been fighting LR(1) parser generation performance problems for a couple of
weeks now, so these papers are of keen interest at the moment.

---

###### At [January 9, 2007 6:44 AM](#comment3){:name='comment3'}, [Chris K](http://www.blogger.com/profile/18278436935541738068) said...

The [Menhir](http://cristal.inria.fr/~fpottier/menhir/) parser generator for
O'Caml is an LR(1) parser generator based on Pager's algorithm.  The reference
given in Menhir's manual is:

David Pager. A practical general method for constructing LR(k) parsers. Acta
Informatica, 7:249–268, 1977.

Although O'Caml comes with a LALR parser generator (ocamlyacc) Menhir is quickly
replacing it as the preferred tool of many developers.

---

###### At [January 10, 2007 5:15 AM](#comment4){:name='comment4'}, [Josef](http://www.blogger.com/profile/13272830598221833253) said...

Another way to look at it is as follows: How many reduce-reduce conflicts have
you encountered which was due to the fact that you used a tool which only
accepted LALR grammars instead of full LR?

On one level I totally agree with you, parser generators should accept LR
grammars. But on the other hand I have personally never been bitten by the LALR
restriction and so I really couldn't care less. I prefer to fight the battles
that actually make a difference. This one does not.

---

###### At [January 10, 2007 1:08 PM](#comment5){:name='comment5'}, [Adrian Thurston](http://www.blogger.com/profile/07166814808666322064) said...

Hi, I once tried this approach. I built an LR(1) parser generator. When I
applied it to a substantial grammar (ADA I think) I found it to be painfully
slow compared to its LALR(1) counterpart. All the extra states resulted in lots
of churning away. Snappy LALR(1) is much better when doing grammar development.

---

###### At [January 10, 2007 1:15 PM](#comment6){:name='comment6'}, [Adrian Thurston](http://www.blogger.com/profile/07166814808666322064) said...

Note: I haven't tried the optimization mentioned.

---

###### At [January 10, 2007 1:38 PM](#comment7){:name='comment7'}, Jason said...

[josef](http://www.blogger.com/profile/13272830598221833253), I agree that this
isn't a battle worth bleeding over, but it's surprising to me that there's a
battle to be fought at all. =)  I was surprised to discover that LALR is
actually a pessimization as compared to LR.  As for encountering LALR-induced
reduce-reduce conflicts, I encountered them repeatedly while using the
[lemon](http://www.hwaci.com/sw/lemon/) LALR parser generator to develop the
grammar for Lyken (the language project which is motivating my parser generation
efforts).  The critical issue with these conflicts was that I didn't have
sufficient understanding of the parser generation algorithms at the time to be
able to fully understand why something that looked completely legitimate to me
was causing conflicts.

[adrian](http://www.blogger.com/profile/07166814808666322064), I had the same
performance experience with my parser generator when I applied it to the Lyken
grammar.  Parser generation would have taken days, maybe even weeks, to
complete.  Fortunately, the 1977 paper by David Pager that [chris
k](http://www.blogger.com/profile/18278436935541738068) mentioned describes an
algorithm that completely solves the algorithmic complexity issue.  I was able
to read that paper and retrofit its algorithms into my parser generator in one
(admittedly very long) day.  The biggest challenge was that the paper uses very
different terminology from what I've encountered elsewhere.  For the Lyken
grammar, the Pager LR(1) algorithm currently takes approximately 4X as long as
the SLR(1) algorithm does.  This is for 105 tokens, 107 non-terminals, 300
productions, and 544 resulting states, which is quite large enough to
demonstrate that there are no significant algorithmic scaling problems.

---

###### At [March 18, 2007 5:35 PM](#comment8){:name='comment8'}, Tom said...

So do you think you have come up with a full solution for LR(1) parser generator
using Pager's algorithm? Something like yacc or bison that can join the open
source community? I'm really interested to know. I have not found another one
yet.

I know that around 1981 there was a LR parser generator implementing Pager's
algorithm in fortran. Then the Menhir parser generator for O'Caml as mentioned
by Chris. I'm looking around to see if a implementation in C/C++ exists.

---

###### At [March 18, 2007 5:52 PM](#comment9){:name='comment9'}, Jason said...

Tom, yes, I am confident that my parser generator implementation is in fact
LR(1), though its API is not the traditional YACC/bison one.  chris k's comment
mentions Menhir, which is also apparently LR(1), using the same basic approach.

I plan to release my Python-based parser generator as open source; the only
reason for the delay is that I wanted to shake out the bugs.  It has been pretty
solid for a while now, so now I just need to spend a bit of time publishing it.

---

###### At [March 22, 2007 12:51 PM](#comment10){:name='comment10'}, [T](http://www.blogger.com/profile/15200614294221441958) said...

In your post: "For the Lyken grammar, the Pager LR(1) algorithm currently takes
approximately 4X as long as the SLR(1) algorithm does. This is for 105 tokens,
107 non-terminals, 300 productions, and 544 resulting states, which is quite
large enough to demonstrate that there are no significant algorithmic scaling
problems." So approximately what's the time/memory cost for this?

Also for the original Knuth LR(1) implementation in Python, did you have a
time/memory cost estimation?

Hope to see your publication soon.

---

###### At [March 22, 2007 4:52 PM](#comment11){:name='comment11'}, Jason said...

[T](http://www.blogger.com/profile/15200614294221441958), I don't know the
algorithmic complexity of Pager's algorithm off the top of my head, but I can
give you some rough idea of performance for the Lyken grammar.

As an aside, the Lyken grammar has since grown to 104 tokens, 117 non-terminals,
and 525 productions.  This is because I found GLR parsing to have some
unacceptable exponential blowup possibilities (due to ambiguity between lvals
and expressions) that essentially required the removal of all ambiguity.  As a
result, the Lyken grammar is now completely unambiguous, at the expense of a
large number of extra productions.

On an Opteron 2.2GHz system, it takes 5:17 and approximately 220MB RAM to
generate the parser pickle.  It is worth noting that the number of productions
is not a strong predictor for parser generation time -- I have two sets of 32
interrelated productions (for procedure declarations and calls) that are to
blame for perhaps 3/4 of the total runtime.

Experiments with a C-based Python module that used the Knuth LR(1) algorithm
showed Python to be about 100X slower than C.  Extrapolating from those results,
I would expect a C-based Pager LR(1) parser generator to be able to process the
Lyken grammar in 5-10 seconds at the most.  This is on par with the runtimes for
generating a Lyken parser with [lemon](http://www.hwaci.com/sw/lemon/), which is
an LALR(1) parser generator.

As for the original Knuth LR(1) implementation, it would have exhausted memory
long before finishing (it would have required at least hundreds of gigabytes),
and it would have taken at least months to complete.  I let it run for nearly
three days, and it was _far_ from finishing, but using nearly 16GB of memory.

---

###### At [September 2, 2007 1:02 PM](#comment12){:name='comment12'}, Anonymous said...

I would be happy to compare my LR(1) parser generator construction experiences
with other people.  My product is called: LRGen 7.0 and it is generating parsers
in about the same time as my other LALR parser generator.

For the DB2 grammar, one of the largest in my possession, it takes about 1.5
seconds to generate the parser tables.  Most of the time is spent in compressing
the tables.

I compiled it with the Microsoft Visual Studio 2005 C/C++ compiler.

I used the Pager concept of merging the states during construction and it was
rather difficult, I think, but it runs very fast.  I used a few other tricks
also.

Note the DB2 grammar produces over 3,200 LALR type states (LR(0) construction
states).

Paul B Mann  
paulbmann.com

---

###### At [October 7, 2008 12:15 PM](#comment13){:name='comment13'}, [Paulo Barreto](http://www.larc.usp.br/~pbarreto) said...

I was shocked by Jason's claim that a full LR(1) parser generator would consume
hundreds of gigabytes of memory. Many years ago I implemented Pager's algorithm
(actually following Heilbrunner's description of the weak and strong
compatibility relations) in Modula 2 on a DOS(!) platform with all its
limitations, and it worked just fine for grammars as big as C++ and Eiffel.
Hence a few hundred *kilobytes* during execution (and just a few dozen kilobytes
if that much for the resulting parsing tables) would sound a more plausible
figure.

A reimplementation in Java I wrote some years later (and still works today)
takes 1.2 s to produce a 13 KB parser for Java on a 2.3 GHz Turion 64X2.

I wonder what kind of languge Lyken is; even if its grammar was designed on
purpose to blow the LR(1) construction, it should not take that much memory. I'd
certainly like to try my application on it.

---

###### At [October 7, 2008 12:37 PM](#comment14){:name='comment14'}, Jason said...

[Paulo](http://www.larc.usp.br/~pbarreto"), I made claim that a full LR(1)
parser generator would require at least hundreds of GiB _if the naive Knuth
algorithm were used_.  The Pager algorithm is what I used to solve this
intractability, and indeed my parser generator's memory requirements are no
longer a problem.

---

###### At [October 7, 2008 1:17 PM](#comment15){:name='comment15'}, [Paulo Barreto](http://www.larc.usp.br/~pbarreto) said...

Jason,

Well, it's easy to test Knuth's canonical construction in my application.

The Java grammar I'm using produces a parser with 557 states; building it
requires 358 weak compatibility tests (no strong compatibility test), and the
result coincides with the LALR(1) automaton. The parser generation time properly
is about 168 ms.

After disabling all compatibility tests (no LR(1) states are merged unless they are identical, which is precisely Knuth's original idea), my application produces a parser with 820 states the generation time becomes 272 ms. I didn't measure how much memory this took (I admit I'm not sure how to do it in Java), but it cannot have been too much.

But I must point out a typo in my previous comment. The generated parser (a Java
source file) takes about 130 KB, not 13 KB as I wrote (more precisely, 124 KB
with Pager's construction and 180 KB with the canonical automaton).

Still, I'd love to test my application with a Lyken grammar. Is there a grammar description online?

---

###### At [October 7, 2008 1:23 PM](#comment16){:name='comment16'}, Jason said...

[Paulo](http://www.larc.usp.br/~pbarreto), the most recent Lyken grammar is
specified
[here](http://lyken.net/cgi-bin/hg_lyken?file/96ecb053f840/lyken/bootstrap/build/Lyken.spec)..
If you want to play with a grammar closer to the one I was using a couple of
years ago (the original basis for this blog post), you should be able to find it
by browsing the Mercurial repository.

---

###### At [October 7, 2008 1:36 PM](#comment17){:name='comment17'}, [Paulo Barreto](http://www.larc.usp.br/~pbarreto) said...

Hmm, it's not in a BNF-like format; there's lots of instructions interspersed
among the productions (Python?). Well, I'll try to extract (manually, oh my)
something I could feed my program with; I just hope this doesn't disfigure the
language definition inadvertently.

I see you're replying in almost real time; maybe a better forum for this
discussion would be an IM system.

---

###### At [October 7, 2008 10:35 PM](#comment18){:name='comment18'}, [Paulo Barreto](http://www.larc.usp.br/~pbarreto) said...

Well Jason, I've got a partial verdict.

It took quite an effort to translate the spec you provided to BNF form and guess
what some symbols like "xxxxx" meant, but in the end I produced a grammar that
my application accepted.

In minimal (Pager) LR(1) mode it produces a mere 798 states after testing weak
compatibility 62294 times; parsing automaton generation takes just 1312 ms. In
canonical (Knuth) mode it produces 1883 states in precisely twice as long (2624
ms). In both cases the result was far from the extreme behavior you reported.

But my application also detected lots of shift-reduce errors. Although I can't
be sure I didn't introduce such problems in the hand translation from the spec,
I see the original spec file contains what seems to be explicit precedence
clauses a la yacc -- my application is not prepared to extend LR(1) grammars
with these tricks.

I couldn't find the original Lyken grammar from the indication you gave, but
I'll bet the problem with the generator you used was in the implementation
details, not in the canonical LR(1) construction (although, of course, the
minimal automaton is a better choice anyway).

(If you wish to see for yourself, I can send you both the formatted grammar I
used and my application).
