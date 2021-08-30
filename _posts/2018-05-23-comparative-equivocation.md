---
layout: post
title: Comparative Equivocation
categories: blog
---

As part of my ongoing effort to write a bot player for a Scrabble-like game
engine I recently implemented [Welch's
t-test](https://en.wikipedia.org/wiki/Welch%27s_t-test), a variant of the more
common [Student's t-test](https://en.wikipedia.org/wiki/Student%27s_t-test).
This was my first experience using floating point math in
[OCaml](http://ocaml.org/).  As you will see below, it is relevant that I am
using the wonderful, if questionably opinionated, [Jane Street Core
library](https://github.com/janestreet/core).

Let's skip the nitty gritty t distribution details, and just focus on the
[numerical integration](https://en.wikipedia.org/wiki/Numerical_integration)
code that generated surprising (incorrect!) results.  The approach is to simply
add up a bunch of rectangles under the curve defined by a function, using narrow
enough slices to achieve the desired accuracy.

```ocaml
val t_pdf: x:float -> df:float -> float
(** Compute the pdf for the t distribution at x with df degrees of freedom. *)

val t_inv: x:float -> df:float -> acc:float -> float
(** Compute the inverse cdf for the t distribution, at level (x = 1 - alpha),
    df degrees of freedom, and accuracy bound acc (e.g. 0.001 to compute a
    result accurate to ±0.001). *)
```

```ocaml
let t_inv ~x ~df ~acc =
  assert Float.(x > 0.5);
  assert Float.(x < 1.);
  let rec lambda x df lim h i accum = begin
    let x = (Int.to_float i) *. h in
    let t = t_pdf ~x ~df in
    let accum' = accum +. t *. h in
    match Float.(accum' < lim) with
(*-->                   ^ *)
    | false -> x
    | true -> begin
        let i' = i + 1 in
        lambda x df lim h i' accum'
      end
  end in
  let lim = x -. 0.5 in
  (* Total proportional cumulative error is approximately bounded by h/2. *)
  let h = acc *. 2. in
  let t = t_pdf ~x:0. ~df in
  let accum = t *. h *. 0.5 in
  lambda x df lim h 1 accum
```

Note the peculiar `Float.(x < lim)`, which one would ordinarily write as `x <.
lim`, i.e. using the float-specific comparison operator.  It's written that way
because otherwise the carefully chosen termination condition for numerical
integration is never reached in some cases.

```ocaml
utop # 0.49499990252089 <. 0.49500;;
- : bool = false
utop # Float.(0.49499990252089 < 0.49500);;
- : bool = true
```

If you're using Core, OCaml's `<.` operator is replaced with a
version that considers floats within `robust_comparison_tolerance` (`1E-7`) to
be equivalent!  This is my opinion really unfortunate, for two reasons.  First,
it hides magic in a seemingly innocuous operator, and second, there is no
value for `robust_comparison_tolerance` that is generally applicable.

As an aside, the way I actually work around this in my own code is to `open
Basis` rather than `open Core` in every source file, and fix up general
problems.  `basis.ml` is now:

```ocaml
include Core
include Int.Replace_polymorphic_compare

let (>=.) = Float.(>=)
let (<=.) = Float.(<=)
let (=.) = Float.(=)
let (>.) = Float.(>)
let (<.) = Float.(<)
let (<>.) = Float.(<>)
```
