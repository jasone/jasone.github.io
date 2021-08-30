---
layout: post
title: Parsing.py parser generator is now available
categories: blog
---

The parser generator I implemented has been quite stable for over a month now.
It has the potential to be of use to others, so I am making it publicly
available.  Parsing.py is a stand-alone pure Python module.  This makes it easy
to maintain and use, but as a result it is substantially slower than C-based
parser generators and parsers.  That is the only negative thing I can think of
to say though.  In my obviously biased opinion, the Parsing module is extremely
cool.  If you need to implement a parser in Python, you should give it a serious
look.

Here is a quick summary of what the Parsing module is:
- True LR(1) parser generator.  Python slowness aside, the algorithms used are
  extremely scalable; I am currently using it for a grammar with well over 500
  productions.
- Both standard LR (aka CFSM) and GLR parser drivers.
- Tight Python integration.  Parser generator directives are specified via
  docstrings.  Rather than running a parser generator as a separate step, it is
  done on the fly, and the results are cached in a pickle.  For subsequent runs,
  as long as the pickle is still compatible with the parser specification, the
  pickle is used directly.
- Extensive error checking and logging.  You can get a very clear idea of what
  is going wrong, during both parser generation and parsing, by enabling logfile
  output.

The module is heavily documented via docstrings.  The easiest way to view the
documentation in a reasonable format is via the interactive python command line
(`import Parsing; help(Parsing)`).  It is worth mentioning here that you
need Python 2.5.  As far as I know, the `(... if ... else ...)` expression
syntax is the only reason for this dependency, so if you want to use Parsing.py
with an older Python interpreter, porting it should not cause you much trouble.

Okay, without further delay, here it is: <http://www.canonware.com/download/Parsing/Parsing.py>

### Comments

###### At [March 22, 2007 6:24 AM](#comment1){:name='comment1'}, Len said...

May I see a sample please?

---

###### At [March 22, 2007 10:16 AM](#comment2){:name='comment2'}, Anonymous said...

Yea, I agree. A small code sample would be very nice. It is easier to get
started with a mininmal example.

---

###### At [March 22, 2007 1:25 PM](#comment3){:name='comment3'}, Jason said...

Okay, I'll write a new entry later today with a simple example.

---

###### At [January 1, 2009 1:27 AM](#comment4){:name='comment4'}, [W@tErF@!rY](http://www.blogger.com/profile/05922122790826112814) said...

Hi. I am final year ug student implementing a LR(1) parser generator in
javascript. Your site has helped me a lot. i would like to ask a favor, if thats
possible. Can you give me pointers on how to test my parser generator? I'm
currently implementing the pager algorithm and not really sure how to test it.
Any help would be much appreciated. Thanks

---

###### At [January 1, 2009 11:03 AM](#comment5){:name='comment5'},  Jason said...

I suggest finding as many small test cases as possible, from compiler books,
newsgroups, etc.  Once you're confident all of those work correctly, find a
non-trivial parser (for something like the C language) and verify that the
generated parser can handle a wide variety of valid inputs.
