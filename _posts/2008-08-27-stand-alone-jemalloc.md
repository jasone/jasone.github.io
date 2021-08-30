---
layout: post
title: Stand-alone jemalloc for Linux
categories: blog
---

I have received numerous requests for a version of jemalloc that is ported to
various operating systems.  My plan has long been to create a jemalloc
distribution that supports \*BSD, Linux, Solaris, OS X, and Windows, but there
are a lot of portability headaches to deal with for OS X and Windows, so the
project keeps being neglected.

Porting to Linux isn't much trouble though, so I whipped up a minimal
distribution this morning, and put it in [this
directory](http://www.canonware.com/download/jemalloc/).  Maybe someday I'll get
around to a proper widely portable distribution, but until then, I hope people
find the Linux port useful.

### Comments

###### At [August 28, 2008 12:37 PM](#comment1){:name='comment1'}, [paul](http://www.blogger.com/profile/01858629491688027277) said...

Sweet! Glad to here you are making a stand-alone version. Jemalloc seems to be a
nice allocator. Before this I ripped the code out of firefox and used it and
that has worked well. Looking forward to seeing this picked up by distros so it
easy to get and us.

Great work!

---

###### At [October 3, 2008 2:36 AM](#comment2){:name='comment2'}, [Pierre Habouzit](http://blog.madism.org/) said...

FWIW, a couple of remarks:

(1) under linux pthread\_mutex\_lock/unlock are available even if not linked
against the libpthread. glibc has stubs to ensure that, that do nothing when
libpthread isn't loaded:

nm -D /lib/libc-2.7.so|grep pthread\_mutex  
00000000000db010 T pthread\_mutex\_destroy  
00000000000db040 T pthread\_mutex\_init  
00000000000db070 T pthread\_mutex\_lock  
00000000000db0a0 T pthread\_mutex\_unlock

Sadly it's not true for pthread\_spin\_\*.

(2) your realloc case should really try to use mremap for the Huge allocations,
because memcopy-ing the whole area is slow.

(3) Afaict, with recent glibc's every linux architectures support TLS (at least
every architectures you test for in your implementation).

---

###### At [October 3, 2008 3:29 AM](#comment3){:name='comment3'}, [Pierre Habouzit](http://blog.madism.org/) said...

Oh and as of pthread\_atfork that is missing from the default libc stubs, you only need to write this:

\#include \<pthread.h\>

int pthread\_atfork() \_\_attribute\_\_((weak, alias(&quot;\_\_zero\_stub&quot;));

static int \_\_zero\_stub() { return 0; }

If you wish to build jemalloc.so with -Bsymbolic or similar then you have to do
that in a libjemalloc-pthread-stubs.so that jemalloc links against, so that when
the libpthread is present, it takes precedence, if you don't use -Bsymbolic, I
believe it'll just DWYM. In the worst case scenario the stubs library will be
required, too bad.

This way you can get rid of your \_\_threading define altogether.

(Note: I know the code is ugly, I took this from the libpthread-stubs xcb is
using, it is obvious how to write it in a way that wont make gcc -Wall -Wextra
complain about function prototypes, just it's longer and doesn't fit in a blog
post ;p)

---

###### At [January 6, 2009 3:54 AM](#comment4){:name='comment4'}, [pharaoh](http://www.blogger.com/profile/11372932619910979324) said...

Does openoffice work with jemalloc? I have LD\_PRELOADED the jemalloc lib,
openoffice starts but it crashes while creating a new file. Any idea?
