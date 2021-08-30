---
layout: post
title: Migrating from Subversion to Mercurial
categories: blog
---

jemalloc has settled into [Firefox](http://developer.mozilla.org/) pretty nicely
at this point, so after having mostly worked on [Lyken](http://lyken.net/) for a
few weeks while waiting for the dust to settle, I'm planning to start working on
adding the necessary functionality to allow the [Tamarin JavaScript
engine](http://www.mozilla.org/projects/tamarin/) to integrate without requiring
a separately managed heap for garbage collection.  One of the first things I ran
into was that the Tamarin source code is available as a
[Mercurial](http://www.selenic.com/mercurial/) repository, so it seemed like a
good time to become familiar with yet another version control system (VCS).

Over the past ten years, there has been a proliferation of VCS's, especially
those supporting distributed development models
([Arch](http://www.gnuarch.org/gnuarchwiki/), [darcs](http://darcs.net/),
[BitKeeper](http://www.bitkeeper.com/), [git](http://git.or.cz/),
[svk](http://svk.bestpractical.com/), Mercurial,
[Bazaar](http://bazaar-vcs.org/), etc.), but for some reason I've found it
difficult to get excited about them.  The biggest barrier for me has been
perceived complexity, but that is perhaps attributable in part to lack of
exposure.  Well, I've been exposed to Mercurial now, and I _really_ like it so
far.

I've primarily been using [Subversion](http://subversion.tigris.org/) for the
past several years, and much to my surprise, Mercurial felt completely natural
almost right away.  In fact, it was immediately easier to deal with branching
and merging than it has ever been  for me with any other VCS.  I have
historically avoided branched development when at all possible, because it has
been hard to make sure that the VCS was doing what I intended.

While [Stuart](http://blog.pavlov.net/) and I were getting jemalloc working in
Firefox, we were tossing patches back and forth constantly.  I spent a total of
~2 days just dealing with patch merges, and changes were dropped on the floor on
multiple occasions.  It occurs to me now that I could have avoided the majority
of this work if we had been using something like Mercurial.  We wouldn't have
lost changes, we wouldn't have had mystery failures due to subtle patch
conflicts, and so on.

Mercurial is so cool that I spent almost two full days trying to migrate my
Subversion repositories.  In particular, I was initially trying to convert the
Lyken repository, which consisted of 1023 revisions and perhaps 1000 files, with
a couple of vendor code imports and one temporary branch (all pretty
straightforward as repositories go).  I tried all of the following:
- [hgsvn](http://cheeseshop.python.org/pypi/hgsvn) silently failed to commit 233
  files, which made the resulting repository almost completely useless.  I poked
  around in the code a bit and determined that fixing the problem myself would
  be a major undertaking.
- [yahg2svn](http://hg.rosdahl.net/yasvn2hg) could only handle 'trunk',
  'branches', and 'tags' at the top level, and I had 'vendor' as well.  I hacked
  on the code a bit and probably could have gotten it to work eventually, but I
  moved on in pursuit of easier solutions.
- [hg
  convert](http://www.selenic.com/mercurial/wiki/index.cgi/ConvertExtension),
  which is an extension that comes with Mercurial, failed to do more than throw
  exceptions due to pickling failures.
- [Tailor](http://progetti.arstecnica.it/tailor) mostly worked, though it was
  completely broken as installed on my Ubuntu/amd64 7.10 system, so I had to
  install it manually.  It got confused by a handful of revisions, but it merely
  left them as unmerged branches, and the fallout was minimal.

I never did find a complete example of how to use Tailor to convert a Subversion
repository to Mercurial format, so here's a bit more detail, in the hope that it
will be of use to someone.

The command line I used was:

```sh
tailor -D -v -F "" --configfile lyken.tailor
```

The hard part though was coming up with the configuration file.  Of course, the
[manual](http://progetti.arstecnica.it/tailor/browser/README.rst) might have
helped, had I found it before writing this blog post.

```
[DEFAULT]
verbose = True

[lyken]
target = hg:target
start-revision = INITIAL
root-directory = /home/jasone/tmp
state-file = tailor.state
source = svn:source
subdir = hg_lyken

[hg:target]

[svn:source]
module = /
repository = file:///home/jasone/tmp/svn_lyken
```

You can peruse the [resulting repository](http://lyken.net/cgi-bin/hg_lyken) to
see what sorts of warts I had to clean up after the conversion.  I have
successfully converted several other repositories using the same method.  The
[Onyx](http://www.canonware.com/onyx/) repository is giving Tailor a real
workout though, since it consists of 3475 revisions and (this is the killer) due
to how [cvs2svn](http://cvs2svn.tigris.org/) did things back when I switched to
Subversion from [CVS](http://www.nongnu.org/cvs/), there are 180 extant
branches, 47 extant tags, and [_gasp_] 89087 extant files in the latest
revision.  It will probably take most of a day for Tailor to complete the
conversion, and I can see in the log output that there are going to be a lot of
problems in all the spontaneous branches cvs2svn generated.

### Comments

###### At [March 12, 2008 9:10 AM](#comment1){:name='comment'1}, [past](http://www.blogger.com/profile/05077555675379961731) said...

_jemalloc has settled into Firefox pretty nicely at this point, so after having
mostly worked on Lyken for a few weeks while waiting for the dust to settle, I'm
planning to start working on adding the necessary functionality to allow the
Tamarin JavaScript engine to integrate without requiring a separately managed
heap for garbage collection._

Could we also hope that you might take a stab at porting Tamarin to FreeBSD? It would be [really](http://docs.freebsd.org/cgi/mid.cgi?46AA5977.9080206) appreciated!

---

###### At [March 12, 2008 11:31 AM](#comment2){:name='comment2'}, [Logan](http://agoln.net) said...

I am also a recent switcher from Subversion to Mercurial, and agree that there
are a lot of advantages to drcs.  I never quite got it working on Windows when I
first tried using it, but have loved using it on Linux.
