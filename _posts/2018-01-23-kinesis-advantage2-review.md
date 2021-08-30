---
layout: post
title:  A Year and Twenty-One with Kinesis Ergonomic Keyboards
categories: blog
---

[![Kinesis Advantage2]({{ "/images/kinesis/advantage2_small.jpg" | absolute_url }}){:height="100%" width="100%"}]({{ "/images/kinesis/advantage2.jpg" | absolute_url }})
*Kinesis Advantage2*

For the past year I have used a pair of [Kinesis
Advantage2](https://www.kinesis-ergo.com/shop/advantage2/) keyboards with my
Linux workstation, which has far exceeded any previous typing experience since
learning to touch type in 1988.  I have used many keyboards over the past thirty
years, including the majority of Kinesis ergonomic keyboard models starting
in 1996.  Here you can read a short review of the Kinesis Advantage2, followed
by enough background context to weary all but the most die-hard keyboard
enthusiasts.  Let us begin!

## The Kinesis Advantage2 is nearly perfect

The Kinesis Advantage2 ergonomic keyboard is the latest in a long line of
Kinesis keyboards which use separated concave keywells to closely match the arcs
our fingertips naturally travel when curling and uncurling our fingers.  The
keywells fit my (average-sized male) hands very well, and I am able to rest my
palms on the padded palm rests and reach all but some of the function keys
(which I almost never use) without moving my palms.  I always type with my
palms at least lightly contacting the palm rests, which reduces shoulder strain
and provides additional reference points that aid in accuracy.  A keyboard tray
with tilt adjustment makes it possible to achieve optimal wrist angles without
any unwanted pinching or stretching, and the keywell separation removes any need
to twist the wrists.

For those who have never used a Kinesis ergonomic keyboard, one of the most
concerning questions is how difficult the adjustment is to the thumb key
clusters, and to a lesser degree the ortholinear keywell grids.  I have to
think back over twenty years to answer these questions, so some of the details
are fuzzy, but in short, the thumb key clusters did take several weeks to adjust
to, whereas the ortholinear keywell grids almost immediately felt superior.
Within the first few days of use I was able to type numbers with higher
confidence than ever before.  You are likely to recover your productivity within
a month, and to fall in love with the keyboard within three months.

For those who have used previous models, you probably want to know how reliable
the electronics are.  Later on I go into more detail on the problems with
previous models, but suffice it to say that in my experience the Advantage2's
electronics have been flawless.  I have not experienced a single typing glitch,
nor have I had to modify or reprogram/restore the layout since initial setup
over a year ago.  Admittedly I do not make use of the more advanced
programmability because my tools (vim, zsh, etc.) provide equivalent
functionality.  However, when I initially set up my pair of identical keyboards
with slightly customized layouts, I configured one manually, saved the
configuration file to my computer, then copied the configuration to the other
keyboard.  All of this functionality worked as advertised, and I expect much
more advanced use cases would also work without issue.

Now it is time for full disclosure regarding one of the contributing factors to
my keyboards' awesomeness: I replaced the Cherry MX brown key switches with
Gateron blue switches, which increases the tactile feedback, and makes the
built-in electronic clicker unnecessary.  Unfortunately, Kinesis does not sell
keyboards with blue switches installed, possibly due to worry both about the
market size and the potential for returns from people who consider the switches
too noisy for an office environment.  That said, Kinesis provides excellent
support to people who are willing to spend the extra time and money to replace
the factory-installed switches.  I
[am](https://medium.com/i0exception/kinesis-blue-e1df5f03e14)
[not](http://www.gilesorr.com/misc/keyboards/kinesis2keys.html)
[alone](http://tech.skryl.org/post/60613686455/the-kinesis-conversion) in
having assembled custom replacements with Kinesis-specific parts purchased
directly from Kinesis, and switches purchased elsewhere.  Should you replace
the key switches when you purchase a Kinesis Advantage2?  Not unless you are
already irrevocably enamoured with something other than Cherry MX brown or red
switches, as I was due to an experiment with [ErgoDox](https://ergodox-ez.com/)
keyboards in 2016.  Read more below about my experiences with other keyboards
if your interest is piqued.

In summary, the Kinesis Advantage2 is an extremely good product, bordering on
perfection for my own use case.  If you type a lot and suffer discomfort
while doing so, you should seriously evaluate this keyboard.  If you still need
convincing, read below about the details of my experience to see if any of your
lingering doubts are allayed.

## Typing pain, ergonomics, and adaptation

In 1996 I was in my last year of undergraduate computer science education, and I
started suffering pretty severely from "emacs pinky", as well as right shoulder
pain from reaching for the mouse.  The pinky pain was approaching disability
levels, so I started looking hard for solutions.  I vaguely remember
encountering some alternatives like the
[DataHand](https://en.wikipedia.org/wiki/DataHand) (prohibitively expensive) and
[Maltron fully ergonomic
keyboards](https://www.maltron.com/3d-dual-hand-keyboards.html) (hard to
purchase in the United States at the time).  There were also some interesting
user-implemented space bar hacks that allowed the left thumb to activate the
control and/or meta keys, both of which are heavily utilized when editing with
emacs.  I chose to try the Kinesis keyboard because it appeared to be a good
compromise between functionality and price.

Did the Kinesis keyboard help?  Oh yes, very much.  The emacs pinky pain went
away completely, and no new pains replaced it.  It took many months for complete
healing though.  Keep in mind that the left pinky is also heavily used to hold
down the shift key.  As for the shoulder pain, I didn't really expect the
keyboard to solve that problem, but it did help because the mouse can be placed
closer than with a standard keyboard, which means a less strained reach.  Over
the years I have increasingly adopted a mouse-frugal work style, and no longer
have any shoulder pain.

More details about typing ergonomics are in order here, because it takes some
conscious effort to train yourself to type in a way that doesn't cause undue
strain.  In particular, holding a modifier key and typing symbols simultaneously
-- "chording" -- with the same hand forces uncomfortable hand contortions.  I
was fortunate to have been trained to use both shift keys so that chords are
split between the hands, but similar discipline is important for the control
and alt (meta, command, option, whatever) modifiers in the thumb clusters.  I
learned the importance of this as my hands complained about repeated chording
while using emacs.  It works quite well to simultaneously press the control and
alt keys with one thumb, but it is important to split most other chords between
the hands.

If you looked carefully at the image of my keyboard, you may have noticed that
my shift keys are swapped with the keys one row down.  I first made this change
for the left hand over fifteen years ago due to discomfort from subtly twisting
my wrist to reach the left shift key.  I made the same change to the right hand
in 2016 mainly for consistency reasons; I never experienced discomfort in my
right wrist.

Unless you need to enter international characters, there is a redundant key to
the left of the arrow keys on the left hand.  As a vim user I find it convenient
to remap this as an escape key, rather than typing control-\[ (which is still a
far superior alternative to reaching for the itty bitty escape key at the top
left of the keyboard).

## Qwerty, Dvorak, Qwerty, Dvorak...

The first Kinesis keyboard I purchased in 1996 was a "Kinesis Professional
Qwerty/Dvorak" dual-legend model.  If I remember correctly, only the
professional model supported Dvorak at the time, due to flash/macro memory
requirements not met by the less expensive model.  I spent several afternoons
after college classes getting used to the thumb clusters, and got over the hump
without reverting to another keyboard.  However, my short-lived attempts at
switching to Dvorak were doomed because I was too busy to withstand the
productivity loss during retraining.  My first real chance at Dvorak came during
my last week as a Sun Microsystems employee in 1998.  All my work was done
outside various minor administrative tasks, which left me with three or four
entire workdays to intensively retrain.  My typing was still rather slow when I
started at my new job the following week, but as time went on, I regained my
typing speed and left behind some irksome flaws, especially symmetry problems
with mixing up 'E' versus 'I' and 'P' versus 'Q'.

I did not use Qwerty again at all until 2001, when I started work at Apple on
Mac OS X.  In contrast to typical terminal environments of Unix-like operating
systems, the common GUI hotkeys on Apple computers are configured for left hand
chording while operating the mouse with the right hand.  I tried the alternate
keyboard layout that uses Dvorak when typing text, and Qwerty when holding the
command key.  Reintroducing Qwerty to my typing was highly disruptive, and after
the first few days at Apple I decided to switch wholesale back to Qwerty.  This
retraining was disruptive, but my typing speed recovered much faster than during
the Dvorak retraining.  Interestingly, the symmetry problems I had with Qwerty
typing prior to using Dvorak did not return, so my Qwerty accuracy improved as a
side effect of the time using Dvorak.

Read further down about my experiences with [ErgoDox](https://ergodox-ez.com/)
keyboards to learn the unjustifiable reasons for my switch back to Dvorak at the
end of 2016.  Retraining to Dvorak the second time was nearly as much work as it
was the first time, though some bits of Dvorak muscle memory lingered from the
first time.  I enjoy using Dvorak rather than Qwerty, but aside from being
somewhat more comfortable to use (less impressive finger gymnastics), the
advantages are marginal.  I spent significant time with typing tutor programs in
2017, and came to the conclusion that my typing speed is primarily limited by
cognitive factors rather than physical factors.  My everyday typing speed tends
to measure in the 70-90 words per minute range, and with extensive training I
could perhaps sustain 110-120 words per minute.  These speeds are well below the
limits the Qwerty layout imposes.

Recall that the initial impetus for my exploration of ergonomic keyboards was
emacs pinky.  If you use emacs, you might wonder how well the Dvorak layout
interacts as compared to Qwerty.  In my experience, emacs key bindings work
equally well (or poorly, depending on your perspective) with both layouts.
emacs key bindings do not generally take into account physical key positions, so
for example the motion keys ('P', 'N', 'F', and 'B') are only slightly less
convenient with Dvorak due to 'P' and 'N' being on opposite hands, and 'F' and
'B' both being on the right forefinger.  I switched from emacs to vim in 2004
due to more streamlined programmability for supporting experimental programming
language syntax highlighting, so my first stint with Dvorak was as an emacs
user, and the current stint is as a vim user.  In my experience, vim is actually
slightly better with the Dvorak layout than with Qwerty, primarily because the
motion keys ('H', 'J', 'K', and 'L') are split across four fingers rather than
three.  Furthermore, the positions of those four keys feel pretty natural for
their respective editing motions.  The only common vim editing character that is
perhaps in a worse location is ':' (shifted ';'), but that is a small price to
pay for the general advantages afforded by moving ';' off the home row.

## Glitches and failure

I purchased or was issued via employers roughly a dozen Kinesis ergonomic
keyboards, from which I learned quite a bit about defects, flaws, and
maintenance.  One of the keyboards was part of a batch with defective key
switches that had some sort of residual lubricant contamination; Kinesis handled
this well from a support perspective.  Several other keyboards failed earlier
than necessary due to two contributing factors:
1. My cats spent many hours on my lap, shedding fur that was wicked into the key
   switches, apparently as a result of air displacement/replacement during key
   presses.  After several keyboard failures I figured this out and stopped
   letting cats ruin my keyboards.
2. All Kinesis ergonomic keyboards prior to the Advantage2 had poor
   [key
   debouncing](https://en.wikipedia.org/wiki/Keyboard_technology#Debouncing).
   This made the keyboards especially sensitive to deteriorating key switch
   contacts, whether due to wear, dust, or... cat fur.  In my experience,
   debouncing was especially poor for the 'A' key.
I gave several of the keyboards to family members, so I didn't churn through a
dozen keyboards all on my own.  Other mishaps also contributed, like being hit
by a car while carrying one home on my bicycle, and kittens chewing through a
cord.  In one case I wore out an Advantage keyboard in a corporate office
environment without a cat ever coming near it.  That keyboard survived about
four years of intense software development before key bounce became too severe
for continued use.  If I recall correctly, one of the first two keyboards I
purchased (the one not involved in an auto/bicycle collision) lasted for about
eight years, which is roughly equivalent to four years of full time use since I
was pretty evenly splitting my typing between two Kinesis keyboards.

Unfortunately, there was another design flaw in the early models that persisted
through all models prior to the Advantage2.  The left shift key would sometimes
get logically stuck "down".  This could be worked around by pressing and
releasing the left shift key again, but of course this was quite disruptive
because of the text that had JUST BEEN TYPED IN ALL CAPS.  Not all users
suffered from this glitch, but I experienced it on all models prior to the
Advantage2.  I suspect that triggering it required hitting a race condition
while rapidly switching back and forth between chording with the right versus
left shift keys.  This is a good strain-reducing habit that not everyone
practices.

When the USB model came along, a whole new level of stuck modifier glitchiness
became the norm.  These glitches involved combinations of stuck modifier keys
(control, alt/option, and command?), and they were confusing enough that I
resorted to resetting the keyboard by unplugging it and then plugging it back
in.  Such glitches were common enough that I learned to plug my keyboards into
ports within easy reach, and I commonly reset the keyboard multiple times per
day.  I'll be honest here: despite my love for the ergonomics of Kinesis
keyboards, my frustration with these glitches drove me to start exploring
alternatives, and the Advantage2 was barely in time to save me.

A few words on routine maintenance are perhaps worthwhile.  Unless you use your
keyboard in harsh environments (proximity to cats counts!), you can likely keep
an Advantage2 keyboard in good working order for ten-plus years of heavy use
without replacing key switches, as long as you clean it thoroughly once or twice
per year.  I do the following to keep my keyboards working well:
1. Remove all key caps with a key cap puller, scrub them with a toothbrush
   and Simple Green, and wipe them dry with a rag.
2. While the key caps are off the keyboard, use compressed air to blow out all
   accumulated dust and particulates.  If any hair, fur, lint, etc. remains in
   or around the key switches, remove it with fingers or tweezers.
3. Remove the palm rest pads, and wipe down the entirety of the keyboard with
   a rag dampened with Simple Green.
4. Apply new palm rest pads.
5. Re-install the key caps.

In the future, as individual key switches fail, I plan to also disassemble,
clean, refurbish, and if necessary replace them, now that I have an [MX switch
top removal
tool](https://mechanicalkeyboards.com/shop/index.php?l=product_detail&p=1363).

## Other keyboard experiences
[![One (clean) keyboard to rule them all]({{ "/images/kinesis/rule_them_all_small.jpg" | absolute_url }}){:height="100%" width="100%"}]({{ "/images/kinesis/rule_them_all.jpg" | absolute_url }})
*One (clean) keyboard to rule them all*

I have experimented with other keyboards for reasons including ergonomics,
(Dvorak) key mapping, and switch technology.  Following are brief summaries of
some enlightening experiences.

- **[Happy Hacking
  Keyboard](https://en.wikipedia.org/wiki/Happy_Hacking_Keyboard) Lite:** During
  the first of my two stints with the Dvorak keyboard layout, I sent a feature
  request email to PFU for Dvorak keyboard layout support.  They later contacted
  me to test their pending product release, and I bought a keyboard at a
  discount from them in order to test the keyboard and provide feedback.  The
  keyboard was very compact, and the lack of number key pad allowed the mouse to
  be quite close to the right hand's normal typing position, much like with the
  Kinesis.  It was also unusual in that it placed the control key where the caps
  lock key typically is.  These attributes made the keyboard a nice
  low-configuration option for use on Unix-like systems.  The rubber dome
  switches provided adequate tactile feedback.  Overall I liked the keyboard,
  and I used it for years as the console keyboard for various FreeBSD
  development machines.  The keyboard helped me recognize how nice it is to have
  the mouse close to the right hand, but it had typical ergonomic issues that
  kept it from being a serious contender for primary keyboard.
- **[Unicomp Model M](http://www.pckeyboard.com/page/category/UKBD):** IBM is
  famous (or infamous if you can't tolerate the noise) for its [buckling
  spring](https://en.wikipedia.org/wiki/Buckling_spring) keyboard switches, and
  there remain devotees who go to the trouble of keeping now-ancient Model M and
  Model F keyboards in service on modern development machines.  I missed out on
  owning an IBM keyboard, but in 2006 I purchased a Unicomp Model M, which is
  substantially similar.  I was primarily interested in the tactile feedback of
  the switches, since I knew in advance that the ergonomics would be otherwise
  unacceptable.  The tactile feedback was indeed excellent, better than anything
  else I had thus far experienced, but to my surprise, I found the keyboard
  quite fatiguing due to the high activation force required ([perhaps
  75g](https://techreport.com/review/24461/a-first-look-at-cherry-mx-green-key-switches)
  for the buckling spring switch, as compared to 50g for the blue switches I
  currently use).  My conclusion from this experiment was that if I were to
  build a custom keyboard, buckling spring switches would not be a realistic
  option, because completely custom key switch construction would be required to
  reduce the activation force.
- **[Maltron](https://www.maltron.com/3d-dual-hand-keyboards.html) (cheap
  facsimile):** Around 2006 I noticed that Maltron had apparently licensed a
  company in the Americas (in Brazil, perhaps?) to manufacture and sell Maltron
  ergonomic keyboards.  These were clearly marketed as "budget" ergonomic
  keyboards, but the quality of the keyboard I purchased was so poor that I used
  it only briefly and eventually discarded it.  I did benefit from the
  experience of physically interacting with the keyboard, even though the key
  switches were too unreliable to actually use for work and some of the keycaps
  bound up with each other.  My general takeaway was that ignoring the
  substandard manufacturing, the Maltron had roughly equivalent ergonomics to
  the Kinesis.  In theory, the extra keys on the Maltron are nice, but in
  practice, putting additional easily reachable keys on the Kinesis keyboard
  requires making the keyboard taller.  That is a questionable trade-off, since
  the distance between the lap top and the keyboard surface is ergonomically
  important.
- **[ErgoDox EZ](https://ergodox-ez.com/) (Gateron blue):** In late 2016 one of
  my brothers scored an epic [nerd snipe](https://xkcd.com/356/) by introducing
  me to the [ErgoDox EZ](https://ergodox-ez.com/) keyboard.  I was gearing up to
  [completely](http://humblehacker.com/blog/20100720/hacking-the-kinesis-contoured-keyboard/)
  [replace](http://humblehacker.com/blog/20100814/hacking-the-kinesis-contoured-keyboard-part-ii/#more-108)
  the electronics and key switches in my Kinesis Advantage keyboards, and was
  unaware of the recent Advantage2 release.  The ErgoDox EZ could be ordered
  with Gateron blue switches and "sculpted" (varying height/angle) blank
  keycaps, and it had a physical layout close enough to that of the Kinesis that
  I could probably make the transition with little retraining.  I spent a _lot_
  of time with the ErgoDox over roughly two months.  My first impression was
  that the Gateron blue switches felt amazing.  My next less favorable
  impression was that the lack of keywells meant I couldn't rest my palms and
  reach all the keys, and I had to adopt a floating hand typing style.  I
  started futzing with keyboard layout using the excellent [online
  configurator](http://configure.ergodox-ez.com/keyboard_layouts), and pushed
  the limits of what is possible without modifying the operating system's
  keymap, given how modifiers and scan codes are actually transmitted.  This is
  where the nerd snipe really came into its own: after experiencing such freedom
  of configuration, I couldn't help being drawn back to the Dvorak layout,
  despite a fifteen year hiatus.  Perhaps the most tantalizing major layout
  variation I tried was inspired by the [Programmer Dvorak
  layout](https://www.kaufmann.no/roland/dvorak/), the goal being to put all
  symbols on the primary layer to ease typing while programming.  I put the
  numbers on a separate number keypad layer, much like what the Kinesis keypad
  layer provides, but I kept running into limitations that prevented reliable
  behavior.  The end result of all these experiments was that I switched to
  Dvorak and moved both shift keys up one row.  I never found a satisfactory
  alternate for the shift keys that avoided the pinkies.  There was one
  bottom-row key on each hand that I could easily reach with my thumbs, but when
  I tried to use these as shift keys I discovered that hitting shift immediately
  after a space overloaded the right thumb.  As an aside, palm keys or a foot
  pedal would perhaps work, but when I tried a foot pedal with my first Kinesis
  keyboard, I found it too restraining to always have my foot in contact with
  the pedal.  My main lingering desire when switching back to the Kinesis
  keyboards was to move the tab key to a stronger finger, as was possible with
  the extra inner key columns on the ErgoDox.

  After all this, I discovered the Kinesis Advantage2, and never looked back.
  My brother now has my ErgoDox EZ keyboards, in addition to his original
  keyboard with Gateron brown switches and non-sculpted keycaps.  He strongly
  prefers the sculpted keycaps, and he prefers the blue switches to the brown
  ones, but not enough to inflict the extra noise on his officemates.  He really
  likes the true split keyboard, because it allows him to place his mouse
  between the two halves.

## Possible Advantage2 improvements

Pre-Advantage2 models of the Kinesis ergonomic keyboards were maddeningly close
to awesome, but their flaws were frustrating enough that over the past two
decades I've spent a few thousand dollars and many weeks evaluating, designing,
and prototyping replacement options.  The Advantage2 has conclusively put an end
to these frustrations, especially with blue switches installed.  The Advantage2
is close enough to perfect that my only ongoing thought on the matter is to hope
that Kinesis continues to manufacture an equivalent product for several decades
to come.  That said, there are a few things that could make the keyboards
incrementally better for me.
- I would like to be able to purchase keyboards with blue switches installed,
  rather than going to considerable extra effort and expense to assemble
  replacement keywells and thumb clusters from parts.  That said, given how well
  Kinesis supports motivated customers, the main issue is that I think blue
  switches are superior for a significant proportion of customers, but only a
  small minority will be willing to go to the amount of trouble currently
  required to experience such typing nirvana.
- As mentioned earlier, with blue switches, the electronic clicker is
  unnecessary, and I have it turned off.  However, the top row (function keys)
  is a different kind of switch, and it would be nice to be able to enable the
  clicker just for the top row.  For me this is of marginal value though, since
  I literally go months at a time without using the top row at all.
- The Advantage keyboards had a builtin USB hub, which was a convenient place to
  plug in the receiver for a Logitech Performance MX wireless mouse (distance
  between mouse and receiver is important).  Similarly, I have at times attached
  a GlidePoint touchpad to the space between the keywells and plugged it in to
  the USB hub, mainly as a workaround for suboptimal desk setups that made
  reaching for the mouse difficult.  However I am reluctant to ask for such a
  feature from Kinesis, given that the impending transition to USB-C or whatever
  comes next is ultimately more important to the product line, and a builtin hub
  makes such transitions harder, both electronically and physically.

[![A Kinesis Advantage2 resting in its natural habitat]({{ "/images/kinesis/natural_habitat_small.jpg" | absolute_url }}){:height="100%" width="100%"}]({{ "/images/kinesis/natural_habitat.jpg" | absolute_url }})
*A Kinesis Advantage2 resting in its natural habitat*
