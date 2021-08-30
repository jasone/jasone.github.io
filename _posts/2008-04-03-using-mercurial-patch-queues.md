---
layout: post
title: Using Mercurial patch queues for daily development
categories: blog
---

I recently watched a
[video](http://video.google.com/videoplay?docid=-7724296011317502612)
([slides](http://www.selenic.com/mercurial/wiki/index.cgi/Presentations?action=AttachFile&amp;do=get&amp;target=google.pdf))
of Bryan O'Sullivan speaking about Mercurial.  The presentation was mainly a
(great) introduction to Mercurial, but I was surprised to learn that [Mercurial
patch queues](http://www.selenic.com/mercurial/wiki/index.cgi/MqExtension) could
be useful even when using a repository that I have full commit access to.  In a
nutshell, Bryan described how he uses patch queues to checkpoint his work
without cluttering the permanent revision history.  Checkpointing is mainly
useful to me when I am about to try a risky programming solution on top of
reasonable code that only partially implements a feature.  Historically, I have
archived my entire sandbox at such critical points, but patch queues are a much
cleaner solution; they make it possible to separate work into distinct patches
and checkpoint regularly without performing heavyweight archiving operations.
Note that reverting to an earlier state is much easier with patch queues, which
makes failed experiments much less costly.  This all sounds great, but it took
me several hours and a lot of mistakes to actually figure out how to use patch
queues in this fashion, so I'm recording the solution here with the hope that it
will be useful to others.

The first step is to enable the mq extension (see [Configuration
directions](http://www.selenic.com/mercurial/wiki/index.cgi/MqExtension)),
though it is enabled by default on my Ubuntu 7.10 systems, and in fact following
the standard configuration directions blindly causes some strange warnings.

Following is a terse example of how to perform every operation that I find
useful when using patch queues for daily development:

```sh
~> hg init pizza # Create repository.
~> cd pizza
~/pizza> echo "pepperoni" > ingredients
~/pizza> echo "black olives" >> ingredients
~/pizza> echo "hand-tossed" > crust
~/pizza> hg add ingredients crust
~/pizza> hg commit -m "Initial pizza recipe."
~/pizza> hg qinit # Initialize unversioned patch queue repository.
~/pizza> hg qnew more-ingredients.patch # Create new working patch.
~/pizza> echo "mushrooms" >> ingredients
~/pizza> hg qrefresh # Checkpoint before creating a new patch.
~/pizza> hg qnew specify-sauce.patch
~/pizza> hg qseries # Look at the patch queue.
more-ingredients.patch
specify-sauce.patch
~/pizza> hg qapplied # See which patches are applied.
more-ingredients.patch
specify-sauce.patch
~/pizza> echo "tomato" > sauce
~/pizza> hg add sauce
~/pizza> hg qrefresh
~/pizza> hg qpop # Pop patch, in order to work on more-ingredients.patch again.
Now at: more-ingredients.patch
~/pizza> hg qapplied
more-ingredients.patch
~/pizza> hg qunapplied
specify-sauce.patch
~/pizza> echo "green peppers" >> ingredients
~/pizza> hg diff
diff -r 4f3f2d833e6f ingredients
--- a/ingredients       Thu Apr 03 15:38:01 2008 -0700
+++ b/ingredients       Thu Apr 03 15:40:55 2008 -0700
@@ -1,3 +1,4 @@ pepperoni
pepperoni
black olives
mushrooms
+green peppers
~/pizza> hg qrefresh
~/pizza> hg qpush # Go back to specify-sauce.patch.
applying specify-sauce.patch
Now at: specify-sauce.patch
~/pizza> echo "marinara" > sauce
~/pizza> hg qdiff
diff -r aadd3ecd3c8e sauce
--- /dev/null   Thu Jan 01 00:00:00 1970 +0000
+++ b/sauce     Thu Apr 03 15:43:00 2008 -0700
@@ -0,0 +1,1 @@
+marinara
~/pizza> hg qrefresh
~/pizza> hg qpop
Now at: more-ingredients.patch
~/pizza> hg qrefresh -e # Edit commit message.
~/pizza> hg qpush
applying specify-sauce.patch
Now at: specify-sauce.patch
~/pizza> hg qrefresh -e
~/pizza> hg log
changeset:   2:6714598e1ccc
tag:         qtip
tag:         tip
tag:         specify-sauce.patch
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:44:32 2008 -0700
summary:     Specify which sauce to use.

changeset:   1:cc9c1fdf1038
tag:         qbase
tag:         more-ingredients.patch
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:44:01 2008 -0700
summary:     Specify more ingredients.

changeset:   0:d3ee82132d36
tag:         qparent
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:34:29 2008 -0700
summary:     Initial pizza recipe.

~/pizza> hg qseries
more-ingredients.patch
specify-sauce.patch
~/pizza> hg qdelete -r more-ingredients.patch # Commit patch.
~/pizza> hg qdelete -r specify-sauce.patch
~/pizza> hg log
changeset:   2:6714598e1ccc
tag:         tip
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:44:32 2008 -0700
summary:     Specify which sauce to use.

changeset:   1:cc9c1fdf1038
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:44:01 2008 -0700
summary:     Specify more ingredients.

changeset:   0:d3ee82132d36
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:34:29 2008 -0700
summary:     Initial pizza recipe.

~/pizza> hg qnew modify-crust.patch
~/pizza> echo "deep dish" > crust
~/pizza> hg qrefresh
~/pizza> hg qnew experiment.patch
~/pizza> echo "pesto" > sauce
~/pizza> hg qrefresh
~/pizza> hg log
changeset:   4:173178d3d17d
tag:         qtip
tag:         tip
tag:         experiment.patch
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 16:16:10 2008 -0700
summary:     [mq]: experiment.patch

changeset:   3:4961f95336c5
tag:         modify-crust.patch
tag:         qbase
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 16:14:55 2008 -0700
summary:     [mq]: modify-crust.patch

changeset:   2:6714598e1ccc
tag:         qparent
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:44:32 2008 -0700
summary:     Specify which sauce to use.

changeset:   1:cc9c1fdf1038
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:44:01 2008 -0700
summary:     Specify more ingredients.

changeset:   0:d3ee82132d36
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:34:29 2008 -0700
summary:     Initial pizza recipe.

~/pizza> hg qpop
Now at: modify-crust.patch
~/pizza> hg qdelete experiment.patch # Discard horrible experiment.
~/pizza> hg log
changeset:   3:4961f95336c5
tag:         qtip
tag:         tip
tag:         modify-crust.patch
tag:         qbase
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 16:14:55 2008 -0700
summary:     [mq]: modify-crust.patch

changeset:   2:6714598e1ccc
tag:         qparent
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:44:32 2008 -0700
summary:     Specify which sauce to use.

changeset:   1:cc9c1fdf1038
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:44:01 2008 -0700
summary:     Specify more ingredients.

changeset:   0:d3ee82132d36
user:        Jason Evans <jasone@canonware.com>
date:        Thu Apr 03 15:34:29 2008 -0700
summary:     Initial pizza recipe.

~/pizza> cat sauce
marinara
```

The trickiest parts of the above are committing/deleting with the qdelete
command, and editing the commit message with qrefresh.  I omitted the many ways
of messing up the order of operations, so tread lightly and experiment with a
toy repository before you use this mode of operation for real.

### Comments

###### At [April 3, 2008 5:04 PM](#comment1){:name='comment1'}, [Evan](http://www.blogger.com/profile/13583968552805274415) said...

I can't help but note that this is what git's local branches are for, so that
development of temporary things is the same as "real" development.  (I know
Mercurial has other nice aspects to it.)

---

###### At [April 3, 2008 9:28 PM](#comment2){:name='comment2'}, [Zendwy](http://www.blogger.com/profile/13776953464954296283) said...

Mmmm, cat sauce, my favorite.

---

###### At [June 16, 2008 3:00 PM](#comment3){:name='comment3'}, [James Moore](http://www.blogger.com/profile/07317484043295391400) said...

Evan - think of this as more like 'git stash'.  Mercurial does local branches
slightly differently than git, but the functionality ends up feeling similar.

---

###### At [July 26, 2008 8:21 AM](#comment4){:name='comment4'}, Anonymous said...

I find it more straightforward to perform experiments in a clone.

However, I do like MQ for then taking those experimental changesets and folding
them into a single change to commit back to the original repository.

---

###### At [July 26, 2008 9:56 PM](#comment5){:name='comment5'}, [bvk-chaitanya](http://www.blogger.com/profile/00530152269564307039) said...

Nice post, it was simple and straight.

Thanks :-)
