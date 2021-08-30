---
layout: post
title: A simple Parsing-based parser example
categories: blog
---

As requested by several people, I have uploaded a simple [example
parser](http://www.canonware.com/download/Parsing/examples/example1.py) that
uses the [Parsing](http://www.canonware.com/download/Parsing/Parsing.py) module.
It is pretty self explanatory, so I encourage you to take a look at it, run it,
and experiment with changes.

That done, I should say a bit more about how I actually use the Parsing module.
Well, the first thing I did with it was to write a parser for a parser generator
input language similar to what
[Elkhound](http://www.cs.berkeley.edu/~smcpeak/elkhound/sources/elkhound/index.html)
supports.  The parser translates the input to two output files, Token.py and
Ast.py, which contain code that the Parsing module can generate a parser from.
Here are a few example productions from Lyken's grammar specification:

```
token comment=xclass:"comment" {
    inline Init ${
        self.val = raw
    }$
}

fail pStmtList <{pExpr};
nonterm StmtList=[e] {
    -> Stmt(Stmt, semicolon);
    -> DelimitedExpr(DelimitedExpr) [pStmtList];
    -> ExtendStmt(StmtList, Stmt, semicolon);
    -> ExtendDelimitedExpr(StmtList, DelimitedExpr) [pStmtList];
}

nonterm Stmts {
    -> Empty;
    -> Stmt(Stmt);
    -> StmtList(StmtList);
}

start Module=[S] {
    -> Module(boi, DocStr, ModuleDecl, Version, InitialCBlock, Stmts);
}
```

There are numerous features not represented above, but the general idea should
be apparent.  Note that embedded code can be associated with productions.  This
allows me to do some pretty highly stylized code generation, yet still embed
custom code where necessary.

One of the non-obvious clauses above is the `=[S]` that follows `start Module`.
This is extra annotation that says the Module production provides an outer
lexical scope.  By supporting such custom annotations, I am able to
automatically generate code that deals with many aspects of semantic analysis.
This is also one of the main reasons I haven't seen fit to release the grammar
specification parser -- it is not obvious to me how to generalize such features
in a way that everyone can benefit.  At the moment I am of the opinion that the
low level docstring-based interface to the Parsing module is good for small- to
medium-size parsers, and that for large parsers, you need to write a custom
translator that I can't hope to guess the needs of.
