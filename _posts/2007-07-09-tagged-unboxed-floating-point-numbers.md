---
layout: post
title: Tagged unboxed floating point numbers
categories: blog
---

Several modern programming language implementations employ a representation of
object reference slots that is self-describing, in order to facilitate run-time
type checks and automatic garbage collection.  By reserving one or more bits to
indicate the type of data stored within the slot, it is possible to
differentiate a pointer (also known as a reference to a "boxed" object) from,
say, an integer (also known as an "unboxed" integer).

Suppose that reference slots are 64-bits wide, and that 61 bits can be used to
store an unboxed integer.  Assuming signed integers, we can store an integer in
\[-2^60..2^60), but outside that range, we are forced to create a boxed integer
object and store a reference to that object.  This is in fact how
[Lyken](http://lyken.net/) implements integers (though it preserves 62 bits of
accuracy for integers).

Now, suppose that we want to support double-precision floating point numbers.
The fundamental approaches taken by every implementation I have found in the
literature are to either 1) box _all_ floating point numbers, or 2) to use a
combination of boxed floating point numbers and untagged floating point numbers.
As one might imagine, (1) can cause serious performance degradation for
numerically intensive programs, due to the need to create new boxed objects to
store the result of each floating point computation.  As for (2), there are
numerous papers that discuss various compilation strategies for finding
opportunities to use _untagged_ unboxed floating point numbers, but these
techniques appear to to be limited to particular problem domains, since they
mainly try to convert vectors of floating point numbers to be untagged and
unboxed.  Nowhere have I found any mention whatsoever of using _tagged_ unboxed
floating point numbers.

Let us consider the IEEE 754 floating point number format to see what challenges
there are to tagged unboxed double-precision floating point numbers.  (If you
are unfamiliar with the format, I suggest taking a look at the
[Wikipedia](http://en.wikipedia.org/wiki/IEEE_754) page for an overview.)  There
are three fields: 1) sign, 2) exponent, and 3) fraction.

<span style="color: rgb(51, 51, 255);">s</span><span style="color: rgb(255, 0, 0);">eeeeeee eeee</span><span style="color: rgb(51, 204, 0);">ffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff</span>

Suppose we were to steal 3 bits from the fraction field.  In order to avoid
losing precision, we would have to box all numbers that did not have a
particular bit pattern for those 3 stolen bits, thus allowing us to unbox
perhaps 12.5% of the time.  This is not compelling.

What if we were to instead steal bits from the exponent?  This is much more
useful, because it allows us to accurately store all values except those with
the most extreme exponent values.  Of course, there are programs that actually
need the full range of exponent values, but they are by no means the common
case.

There are some details that make such unboxing more work than for integers:
- The exponent must be re-biased.
- It is harder to remove the exponent bits, since they are internal.
- There are special values that require special handling (+-0.0, +-Inf, NaN).

Explaining the nuances in words is rather tedious, so actual code follows instead.

```c
typedef union {
  uint64_t u;
  int64_t i;
  double r;
} LktRealUnion;

void
LkRealNew(LktSlot *aReal, double aVal) {
    LktRealUnion val;
    val.r = aVal;

    // Check whether +-0.0.
    if (val.u & 0x7fffffffffffffffLLU) {
        LktRealUnion unboxed;

        // Re-bias the exponent by subtracting 896.  This makes the useful
        // exponent range for unboxed reals [-127..128].
        unboxed.u = val.u - 0x3800000000000000LLU;
        // Check that the most significant 3 exponent bits are 0.
        if (unboxed.u & 0x7000000000000000LLU) {
            if ((val.u & 0x7ff0000000000000LLU) == 0x7ff0000000000000LLU) {
                // Special value (Inf or NaN).
                uint64_t sign = (val.u & 0x8000000000000000LLU);
                unboxed.u <<= 3;
                unboxed.u &= 0x7fffffffffffffffLLU; // Clear sign bit.
                unboxed.u |= sign;
                unboxed.u |= 0x3; // Tag.
                aReal->u.b = unboxed.u;
            } else {
                // Overflow; box.

                // [...]
            }
        } else {
            uint64_t sign = (val.u & 0x8000000000000000LLU);
            unboxed.u <<= 3;
            // Sign bit is already cleared as a result of exponent re-biasing.
            unboxed.u |= sign;
            unboxed.u |= 0x3; // Tag.
            aReal->u.b = unboxed.u;
        }
    } else {
        // +-0.0.
        aReal->u.b = val.u | 0x3; // Tag.
    }
}

double
LkRealGet(LktSlot *aReal) {
    LktRealUnion val;

    LkmAssert(LkSlotTypeGet(aReal) == LkRealType());
    // Checked whether boxed.
    val.u = aReal->u.b;
    if ((val.u & 0x7) == 0x3) {
        // Check whether +-0.0.
        if (val.u & 0x7ffffffffffffff8LLU) {
            val.i >>= 3; // Sign-extended shift preserves the sign bit.
            val.u &= 0x8fffffffffffffffLLU; // Clear upper exponent bits.
            // Check whether a special value (Inf or NaN).
            if ((val.u & 0x0ff0000000000000LLU) != 0x0ff0000000000000LLU) {
                // Re-bias the exponent by adding 896.
                val.u += 0x3800000000000000LLU;
            } else {
                // Special value.  Set all exponent bits.
                val.u |= 0x7ff0000000000000LLU;
            }
        } else {
            // +-0.0.
            val.u &= 0x8000000000000000LLU;
        }
        return val.r;
    } else {
        // Boxed.
        LktReal *r = (LktReal *) aReal->u.p;
        return r->val;
    }
}
```

As you can see, unboxed floating point numbers do incur some overhead, but for
typical applications, they appear to me to be a big improvement over uniformly
boxed floating point numbers.

### Comments

###### At [February 3, 2008 12:47 AM](#comment1){:name='comment1'}, Anonymous said...

You should take a look at <http://article.gmane.org/gmane.comp.lang.lua.general/44823> for an even more clever approach.

---

###### At [February 3, 2008 9:37 AM](#comment2){:name='comment2'}, Jason said...

I recently encountered the [described
technique](http://article.gmane.org/gmane.comp.lang.lua.general/44823) in
Tamarin.  It is indeed clever, but it has the problem that it requires several
more bits than my method, thus reducing the range of other unboxed values.
Pointers on 64-bit systems are troublesome, though in practice modern systems
only use 40-48 bits for pointers, thus allowing for some bit stealing tricks.

Anyway, thank you for the reference.  I am certainly going to need to benchmark
both methods to decide which one to use.

---

###### At [February 20, 2008 3:44 PM](#comment3){:name='comment3'}, [Emanuele Ruffaldi](http://www.blogger.com/profile/15248692860738608266) said...

I've played with the NaN technique some time ago and written some example code.

In my case the 64bit value can be a double precision floating point value or a
tagged value, but unluckly I was not handling 64 bit pointers and 64 bit
integers.

<http://www.teslacore.it/wiki/index.php?title=FloatVariant>

---

###### At [May 14, 2008 6:13 PM](#comment4){:name='comment4'}, [edwsmith](http://www.blogger.com/profile/12160240582257831723) said...

We're starting to face the x64 demon in Tamarin tracing, which uses the NaN
trick on x86/ARM.  
<https://bugzilla.mozilla.org/show_bug.cgi?id=433793>

Any outcome from experimenting with both formats?

---

###### At [May 14, 2008 9:25 PM](#comment5){:name='comment5'}, Jason said...

No, I haven't conducted these experiments yet for Lyken, though I've given the
issue a bit of thought in the context of Tamarin.  I suspect that it will be
faster to use the method described in my blog post than it would be to collapse
pointers into the fraction bits of type double.

This hunch is based on the (unconfirmed) assumption that reference pointers are
substantially more common than unboxed floating point numbers.  Collapsing
pointers is faster than re-biasing floating point numbers and dealing with
boxing on overflow, but I doubt that even numerically intensive code has a large
proportion of floating point objects, as compared to reference pointers.
