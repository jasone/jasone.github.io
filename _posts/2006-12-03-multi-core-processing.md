---
layout: post
title:  Impacts of multi-core processing on programming language design
categories: blog
---

Within the next couple of years, all modern desktop computers will have multiple
CPUs, thanks to multi-core packaging.  This doesn't matter very much in the
context of programming languages though until the number of CPUs crosses a
certain threshold, say, 4-8 CPUs.  There are a few reasons for this, including:
- 2 CPUs aren't a very compelling motivation for the significant extra
  development effort required for multi-threaded programming.
- There are numerous hackish programming solutions that work reasonably  well
  when targeting sub-4-CPU systems, which reduces the pressure to develop
  general solutions.
- General solutions have some inherent overhead that substantially eats into the
  performance gains.  Depending on the programming model, general solutions need
  on the order of 4-8 CPUs before the overhead is an acceptable tradeoff for the
  improved scalability.

I have been waiting for multi-threaded programming to provide truly scalable
performance gains for general computing ever since I started out developing
software on IBM's OS/2 operating system in the early 1990s.  The crossover point
has been "a few years" away for 15 years now, but now it's so close I can smell
it.

Researchers have long since developed the raw technology necessary to see us
through this transition, but current tools are woefully lacking.  Let me briefly
describe the shortcomings of some of the tools we currently have available.
- **C/C++ pthreads:** C/C++ are dangerous, difficult languages when it comes to
  writing reliable software, leaving alone multi-threading.  The single-image
  programming model that pthreads provides adds insult to injury.  Even experts
  have a very difficult time writing reliable multi-threaded C/C++ programs.  On
  top of this, development aids such as debuggers are of limited use here,
  because of the [Heisenbug
  Principle](http://www.catb.org/%7Eesr/jargon/html/H/heisenbug.html).
- **Java, C#:** These languages improve on the C/C++ situation by providing
  language-level threading support, and the runtime environments improve
  analysis and debugging prospects.  For the next few years, this is probably
  the best we're going to do, but the single-image programming model is rather
  difficult to deal with, even under the best of circumstances.  I think there
  will always be a place for languages like these, but that as the number of
  CPUs in computers increases, these will be increasingly seen as low-level
  languages.
- **Erlang:** Erlang relies entirely on message passing for communication among
  threads (let's ignore for now that Erlang's terminology differs).  Threads do
  not explicitly share memory.  There are two problems with this:
  - Erlang actually runs all threads inside a single process that uses only one
    CPU.  This is an implementation detail, but in practice it limits
    flexibility, and the workarounds are less than ideal.
  - The overhead of passing all data as messages between threads is very
    expensive, depending on the application.  This is a general concern, but at
    some threshold number of CPUs, I expect it to become an acceptable cost of
    developing highly scalable multi-threaded software.
- **Perl, Python, Ruby, etc.:** These languages vary in their approaches to
  multi-threading, but I think it fair to say that none of them provide
  scalable, useful multi-threaded development support.  I find this noteworthy
  because this class of languages is of increasing importance both for scripting
  and for larger-scale systems programming.  These languages will have to adapt
  if they are to maintain their value as systems programming languages.

I alluded above to a division between two approaches to multi-threaded
programming: 1) single-image and 2) message-passing.  Right now, (1) is of
primary importance (and the time to worry about it really is _now_).  (2) is
creeping up on us quickly though; I predict that it will really start mattering
when we start dealing with anything more than about 8 CPUs.  Why?  Because in my
experience, the only reliable approach to writing software that scales beyond 8
CPUs is to rely mostly on message passing.

Okay, here's where I pull out my crystal ball.  I predict that five years from
now, the languages that provide the highest productivity with regard to
multi-threaded programming will make message-passing easy (i.e. it will be the
primary mode of multi-threaded development), and shared-image-based threading
possible.  Right now, none of the available languages I'm aware of provide this
focus.  What really concerns me though is that of the primary "scripting"
languages, none are even _close_ to providing the necessary programming
infrastructure, let alone the appropriate focus on methodology.  This is where
my attentions are currently focused, and I expect many of my future ramblings
will relate to the topic.

---

### Comments

###### At [December 6, 2006 2:55 AM](#comment1){:name='comment1'}, Per Gustafsson said...

Two comments on Erlang multithreading

The latest release Erlang release can run threads on several different processes
and you can start the emulator in hybrid mode which basically means that each
thread has a private heap and also a message heap which is shared by all threads
this makes it possible to pass messages by just copying a reference to the
message.

---

###### At [December 20, 2006 4:12 AM](#comment2){:name='comment2'}, [Lori](http://www.blogger.com/profile/16627903151623020628) said...

On the last day of Christmas, my brother brought to me:

12 woefully lacking tools  
11 layers of programming infrastructure  
10 C++ pthreads  
9 hackish programming solutions  
8 CPUs  
7 approaches to multi-threading  
6 single-image programming models  
Multi-threaded programming  
4 future ramblings  
3 low-level languages  
2 years to the crossover point  
And a crystal ball looking into the future of computer language

---

###### At [January 1, 2008 6:10 PM](#comment3){:name='comment3'}, Ade said...

Interesting...since purchasing a dual core processor a couple of days ago, now
that I've gotten a little bored of overclocking it I have been thinking about
what options I have should I choose to try and concurrently program it.

I'm finding that my options are somewhat limited, with Java sounding like
perhaps the best bet.  In the past year I have been spending a lot of time with
Ruby, and enjoying the language - apparently, its threading capabilities are
lacking, but are you aware of the jRuby project, which is pure Java
implementation of Ruby?

<http://jruby.codehaus.org/>

In this context viewing Java as a "low level" language makes sense.

---

###### At [January 1, 2008 6:20 PM](#comment4){:name='comment4'}, Ade said...

An interesting tidbit about threading in Ruby + Java (jRuby):

<http://blade.nagaokaut.ac.jp/cgi-bin/scat.rb/ruby/ruby-talk/182382>
