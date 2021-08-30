---
layout: post
title: More forgotten parser generation algorithms
categories: blog
---

When writing a grammar specification that is input to a parser generator, the
most natural way of describing the grammar is often ambiguous.  There are two
solutions: 1) rewrite the grammar to be less obvious, or 2) use precedence rules
to disambiguate conflicting actions.  In practice, I find myself using both
approaches, according to which is least distasteful for the ambiguity at hand.

However, I have lately run into a series of bugs in the
[Lyken](http://www.lyken.net/) parser that are a result of the following steps:
1) disambiguate the grammar using precedences, 2) continue grammar development.
What happens is that the precedence specifications added in step (1) end up
being inadvertently employed to disambiguate additions made in step (2), but in
many cases not as I would have chosen, were I presented with an ambiguity to
resolve.  The result is obscured bugs that only show up when parsing code that
exercises the appropriate broken portions of the parser.

I started thinking about how to avoid these masked ambiguities, and realized
that in many cases it is impossible, due to the precedence machinery provided by
virtually every parser generator in existence (if there is any precedence
support at all).  Here is a typical set of precedence specifications as
supported by YACC.

```
%left '+' '-'
%left '*' '/'
```

The operator sets are listed from lowest to highest precedence so that
multiplication/division has higher precedence than addition/subtraction.

For simple examples, it is hard to see what is wrong with this scheme, but for
more complex grammars, there is a problem: It is impossible to declare a
precedence relationship between a production and, say, addition/subtraction,
without incidentally declaring a precedence relationship with _every other
precedence_.  In essence, we are stuck with a linearization of what should
really be a directed acyclic graph (DAG) of precedence relationships.

Apparently, before LR parsing became the norm, there was a more limited method
called "precedence parsing".  The precedence support we have in LR-family parser
generators apparently was added to subsume precedence parsing, making pure
precedence parsing completely obsolete.  The problem is, we are stuck with a
special case of precedence parsing, even though the general case was worked out
and published ([Gray 1973](#gray1973)).

I have implemented DAG-based precedence specification support in my parser
generator, and indeed it solves the problem described earlier.  Since the DAG
can be disjoint, it is possible to, for example, disambiguate a reduce/reduce
conflict without any possibility of masking conflicts due to later grammar
additions.  Naturally, much care is still required when using precedence
specifications for disambiguation, but with the DAG-based approach, at least I
am no longer hobbled by an unnecessary limitation.

### References

[Gray, James N. and Michael A. Harrison (1973) Canonical Precedence Schemes.
JACM 20(2), 214-234.](#gray1973){:name='gray1973'}

### Comments

###### At [December 7, 2008 2:50 PM](#comment1){:name='comment1'}, Erik Sørensen said...

That is also something that has bothered me...  and in my parser engine plans,
I've also envisioned a DAG precedence approach.

Nice to see that someone's actually done this.
