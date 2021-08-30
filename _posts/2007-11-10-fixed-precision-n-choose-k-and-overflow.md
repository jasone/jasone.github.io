---
layout: post
title: Fixed-precision (n choose k) and overflow
categories: blog
---

I recently found myself needing to compute (n choose k) with 64-bit integers.
Recall that (n choose k) is equal to n!/\[k!(n-k)!].  Mathematically, this is
not a difficult computation, but when considered in the context of integer
overflow, the problem becomes much harder.

To illustrate the problem, consider the computation of (9 choose 4) using 8-bit
signed integers.  We can start off by doing some straightforward cancellation,
which leaves us with \[9\*8\*7\*6]/\[4\*3\*2].  Where do we go from here though?
If we multiply all of the terms in the numerator first, we get an intermediate
result of \[3024]/\[4\*3\*2], which clearly does not fit in the \[-128..127]
range.  The method that we are taught on paper is to cancel factors until none
are left in the denominator, then multiply the remaining factors in the
numerator to get the answer.  We can write a program that effectively does the
same thing, but do we really have to create vectors of terms and duplicate the
hand method?

I searched high and low for information about how best to implement (n choose k)
with fixed precision integers, without success.  While considering the mechanics
of coding the hand method, I realized that computing greatest common divisors
(GCDs) would be a critical component.  I then began to wonder if there might be
an iterative algorithm that does not require manipulating vectors of integers.
Here is what I came up with.  (n choose k) is \[n\*(n-1)\*...\*(n-k+1)] /
\[k\*(k-1)\*...\*1].  Let us call the vectors of terms in the numerator and
denominator \[C] and \[D], respectively, so (n choose k) is \[C]/\[D].

1. If (k > n/2), set k <-- n-k.  This does not change the result, but it
   reduces the computational overhead for later steps.
2. Initialize accumulators A and B for the numerator and denominator of the
   result to 1, so that A/B is 1/1.  Note that upon completion, B will always be
   1, thus leaving the result in A, but during computation, B may be greater
   than 1.
3. While possible without overflowing A (and while \[C] is non-empty),
   repeatedly merge the first term of \[C] (call it c) into A and remove the
   term from \[C].  This is achieved via the following steps:
   1. Divide g <-- GCD(c, B).
   2. B <-- B/g and c <-- c/g.  This removes common factors.
   3. A <-- A\*c.
4. While possible without overflowing B (and while \[D] is non-empty),
   repeatedly merge the first term of \[D] into B and remove the term from \[D].
   This is achieved using the same algorithm as for step 3.
5. If no progress was made in steps 3 or 4, fail due to unavoidable overflow.
6. If \[C] or \[D] is non-empty, go back to step 3.

Here is a [reference implementation in C](http://www.canonware.com/%7Ettt/nk.c).

Since implementing the algorithm, I have been troubled by a seemingly simple
question: does this algorithm ever fail even though the final result can be
expressed without overflow?  My intuition is that the algorithm always succeeds,
but a proof has thus far eluded me.  I have exhaustively tested the algorithm
for 32-bit integers, and the algorithm never fails.  Unfortunately, I really
need to move on to other work, since the algorithm certainly works well enough
for my needs.

### Comments

###### At [January 14, 2008 6:16 PM](#comment1){:name='comment1'}, [Steve](http://www.blogger.com/profile/01559733050070077480) said...

Here's another way guaranteed not to overflow:

```perl
# return true if n is prime
sub isprime($) {
    my ($n) = @_;
    return 0 if ($n < 2);
    return 1 if ($n == 2);
    return 0 if (($n & 1) == 0);
    for (my $i = 3; $i * $i <= $n; $i += 2) {
        return 0 if (($n % $i) == 0);
    }
    return 1;
}

# return multiplicity of prime factor p in n!
sub multiplicity($$) {
    my ($p, $n) = @_;
    my $m = 0;
    while ($n > 0) {
        $n = int($n / $p);
        $m += $n;
    }
    return $m;
}

# compute n choose k
sub goodchoose($$) {
    my ($n, $k) = @_;
    my $c = 1;
    for my $p (2 .. $n) {
        if (isprime($p)) {
            $c *= $p ** (multiplicity($p, $n) - multiplicity($p, $k)
                  - multiplicity($p, $n - $k));
        }
    }
    return $c;
}
```

---

###### At [February 9, 2008 3:47 AM](#comment2){:name='comment2'}, Nick said...

BTW, how this algorithm compares in performance to straightforward computing
using infinite precision aritmetics?

---

###### At [March 12, 2008 11:03 AM](#comment3){:name='comment3'}, [ja](http://www.blogger.com/profile/05289656520361260691) said...

It may be faster to compute it in floating point via approximation formulas (for
example Stirling's formula for factorial)

---

###### At [March 12, 2008 1:07 PM](#comment4){:name='comment4'}, [Vít Tuček](http://www.blogger.com/profile/05289656520361260691) said...

Proof of correctness of your algorithm seems nontrivial. However I've found
faster and correct method how to compute (n choose k).

First of all

(n choose k) = (n choose k-1) \* (n-k) / k

This means that either k divides (n choose k-1) or it divides (n-k) for (n
choose k) is natural number.  
Nevertheless it is always true that k divides (n choose k-1)\*(n-k) and hence we
can compute (n choose k) iteratively:

result = 1;  
for(i=1;i<=k;i++) { result = result \* (n-i) / i; }

The overflow can occur in multiplication - in that case we must find out wheter
k divides (n-k) or the intermediate result and do the appropriate division. This
can be done with your GCD function which I presume is fast enough. In order to
minimize test for overflow I suggest you look up some analytical inequalities
for (n choose k) and use them to split the algorithm into two parts - one for
small k which can be done without any checking and one which have to use GCD.

I've done the computation with the Stirling's formula for (2n choose n) and it
suggest that this biggest number that can occur is approximately
2^(2n)\*1/sqrt(pi\*n)

---

###### At [March 12, 2008 1:27 PM](#comment5){:name='comment5'}, [Vít Tuček](http://www.blogger.com/profile/05289656520361260691) said...

I think I've been a bit unclear - here's the code that works in general and
finds correct result if it is possible>

result = 1;  
for(i=1;i<=k;i++){  
 gcd = GCD(i,result);
 if gcd==i then  
 {result = result/i*(n-i);}  
 else result=(n-i)/i*result;  
}

I assume that the arithmetic operations are executed from left - i.e. a/b\*c
means that agets divided by b and the result of this is multiplied by c.

---

###### At [March 12, 2008 1:35 PM](#comment6){:name='comment6'}, Jason said...

Vít Tuček, that's really cool.  Your method is much more straightforward than
mine, and at first glance it looks correct to me.  I will give it a try the next
time I'm working on the code where I need this algorithm.  Thanks!

---

###### At [March 13, 2008 3:13 AM](#comment7){:name='comment7'}, Anonymous said...

I like Vít Tuček's method.  You can also use a MulDiv function that has a
double-precision intermediate result, or expect the compiler to optimize "result
= (long long) result * (n-i) / i", or use assembly language directly.  On x86
that would be something like:

```assembly
    mov eax, 1
    mov ecx, n
    dec ecx
here:
    inc ebx
    mul ecx
    div ebx
    loop here
```

---

###### At [March 13, 2008 10:50 PM](#comment7){:name='comment7'}, Anonymous said...

I am not sure I understand Vít Tuček's method. You say that either k divides (n
choose k-1) or it divides (n-k), for (n choose k) is natural number, and
certainly this is true for k prime. But what if k is not prime?

Example: put n=5 and k=4. We have (n choose k-1) = (5 choose 3) = (5 choose 2) = 10. But k=4 neither divides n-k=1 nor 10.

If we blindly apply your algorithm (as a computer would), we get the result (5
choose 4) = 1/4 times (5 choose 3) = 0 times 10 = 0.

---

###### At [March 13, 2008 11:17 PM](#comment8){:name='comment7'}, Anonymous said...

ack! throughout, for n-k substitute (n-k+1). So, for example, my second
paragraph should end "But k=4 neither divides n-k+1=2 nor 10." Sorry about that.

My confusion remains, though. The result is still (5 choose 4) = 2/4 times 10 = 0.

---

###### At [March 14, 2008 7:01 AM](#comment9){:name='comment9'}, [Vít Tuček](http://www.blogger.com/profile/05289656520361260691) said...

Sorry for confusion. My algorithm only works for k <= n/2.

---

###### At [March 14, 2008 12:42 PM](#comment10){:name='comment10'}, Anonymous said...

What changes when k &lt;= n/2?

Example: Let us calculate 21 choose 4. (11 choose 3 happens to be a multiple of 4, so this is how I adjust.) Where n=21 and k=4,

n choose (k-1) = 21 choose 3 = (21&sdot;20&sdot;19)/(3&sdot;2&sdot;1) =
7&sdot;10&sdot;19, where &sdot; denotes multiplication. So 21 choose 3 is not a
multiple of 4.

n - k + 1 = 21 - 4 + 1 = 18 = 2&sdot;9, also not a multiple of 4.

Blindly applying your code, we get (21 choose 4) = 18/4 times 7&sdot;10&sdot;19
= 4&sdot;7&sdot;10&sdot;19 = 8&sdot;5&sdot;7&sdot;19, which is not a multiple of
3. Unfortunately, in reality (21 choose 4) = 5&sdot;7&sdot;9&sdot;19.

---

###### At [March 16, 2008 12:00 PM](#comment11){:name='comment11'}, [Vít Tuček](http://www.blogger.com/profile/05289656520361260691) said...

You are right. One needs to multiply first and divide later, because some of the
prime factors of k can be hidden inside (n choose k-1) and others in n-k.

So either that problem can be solved by some clever programming techniques as
somebody above pointed out, or you can test for overflow before multiplication
and conditionaly perform some nifty cancelations.

I left the details for readers as an exercise ;-)

---

###### At [September 10, 2009 3:38 PM](#comment12){:name='comment12'}, [Chandra](http://www.blogger.com/profile/10253178591825021165) said...

If a method works well for k &lt; (n/2), that should be enough! After all, (n
choose k) equals (n choose (n - k)). One way of putting meaning in this is the
task of choosing k objects from n is the same as the task of &quot;not
choosing&quot; or &quot;leaving out&quot; (n-k) objects in n.  
-Chandra

---

###### At [January 8, 2010 1:43 PM](#comment13){:name='comment13'}, Anonymous said...

The Stirling's formula idea (suggested above) sounds weird, but it could work
well.  The stirling approximation to the factorial is really quite good.  It can
even be given error bars (see Feller's book on probability - vol.1 - an old
book) which could be checked to see if they encompass exactly one integer.  This
might also be in Concrete Math by Knuth et al.
