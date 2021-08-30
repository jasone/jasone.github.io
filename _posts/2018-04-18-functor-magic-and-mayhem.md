---
layout: post
title: Functor Magic and Mayhem
categories: blog
---

For the past several months I have been programming primarily in
[OCaml](http://ocaml.org/), and it has been a magical experience, though the
learning curve has often been unsettlingly steep.  Recently I spent most of a
day wrapping my head around what was to me an inscrutable type error, followed
by understanding and initial frustration with what seemed like an unnecessary
API landmine, and finally a realization that the API has merit that more
experienced OCaml developers surely leverage.

Minimal background: As an extended OCaml learning exercise I am writing a
computer player for a Scrabble-like board game, in a similar vein to the most
excellent [Quackle](http://people.csail.mit.edu/jasonkb/quackle/).  As a side
effect of having largely learned OCaml via [Real World
OCaml](https://realworldocaml.org/v1/en/html/index.html), I am using the [Jane
Street Core library](https://github.com/janestreet/core), a choice that provides
an excellent foundation for practical software development, but at the cost of a
yet-steeper learning curve.

In a Scrabble game engine, we need to deal with tiles, and bags of tiles.  Core
makes it really easy to enhance basic data types with type-safe collections
support using functors like `Comparable.Make`.  Following is a stripped-down
implementation that compiles, but note the curious `Int.()` wrappers around the
two integer comparisons.

```ocaml
open Core

module T = struct
  type letter =
  | A | B | C | D | E | F | G | H | I | J | K | L | M
  | N | O | P | Q | R | S | T | U | V | W | X | Y | Z
  [@@deriving sexp, compare]

  type t =
  | Blank
  | Letter of letter
  [@@deriving sexp, compare]
end
include T
include Comparable.Make(T)

module Bag = struct
  type tile = t
  type t = int Map.t

  let empty = Map.empty

  let length t =
    Map.fold t ~init:0 ~f:(fun ~key ~data acc -> acc + data)

  let pick t =
    let n = Random.int (length t) in
    let _, tile_opt = begin
      Map.fold t ~init:(0, None) ~f:(fun ~key ~data (i, tile) ->
        let i' = i + data in
(*-->*) match Int.(i' <= n) with
        | true -> i', None
        | false -> i', Some key
      )
    end in
    let t' = match tile_opt with
      | None -> t
      | Some tile ->
        Map.change t tile ~f:(function
(*-->*)   | Some v when Int.(v > 1) -> Some (v - 1)
          | _ -> None
        )
    in
    t', tile_opt

  let put t tiles =
    List.fold tiles ~init:t ~f:(fun t tile ->
      Map.change t tile ~f:(function
        | Some v -> Some (v + 1)
        | None -> Some (1)
      )
    )
end
```

What happens if we omit the `Int()` wrappers?  We get compiler error messages.
```
File "tile.ml", line 31, characters 14-16:
Error: This expression has type int but an expression was expected of type
         T.t
```

What does that mean?  In short, it means comparison operators can't compare
integers!  The `Comparable.Make` functor creates comparison operators that are
specific to the tile data type, and since the `Bag` code follows within the same
module, all comparison operators are specific to the tile type.

The `Int.()` syntax causes the `Int` module to be opened locally for the
expression within the parentheses, i.e. the `Int` module's contents are
flattened into the enclosed expression's namespace.  That means that within the
local opens of `Int`, we are using integer-specific comparisons, and similarly
`Poly.()` (a Core module) would cause us to use the default polymorphic
comparison operators that are built into OCaml.

It took me a long time to figure out what was going on, because I actually wrote
the bag code as a separate `Bag` module before deciding to move it inside `Tile`
and make it `Tile.Bag`.  I went from working Bag code to a compiler error that
made no sense to me.  I have found it really important to OCaml develoment to
iterate in a way that keeps the code always compilable, so that when something
surprising happens, introspection via [utop](https://github.com/diml/utop) and
[Merlin](https://github.com/ocaml/merlin) is possible.  In this case I should
have disabled the `Tile.Bag` code, compiled the `Tile` module, and used utop to
see what `Comparable.Make` had done to the `Tile` namespace.

A better grasp of comparison operators would have helped me too.  OCaml moves
mountains in its type inference engine, but part of the way it infers types is
by avoiding type polymorphism for operators... in some cases but not others.
For example, the `+` addition operator can only operate on integers (and `+.`
only on floats), which gives the compiler opportunities to infer variable types
based solely on addition operations.  The language designers chose not to use
such a heavy hand with comparison operators, though in retrospect it seems there
is
[disagreement](https://discuss.ocaml.org/t/monomorphic-comparison-operator-of-janestreet-base-library/1585/2)
on whether this was a good choice.

And this brings us back to the operators defined by the `Comparable.Make`
functor.  I saw this as a misfeature when I first realized what was causing
compilation errors, but I'm betting experienced OCaml developers who use Core
get warm fuzzies when comparing tiles is as simple as e.g.
```ocaml
let ascending (a, b) =
  match Tile.(a < b) with
  | true -> a, b
  | false -> b, a
```
