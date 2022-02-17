import sys
import Parsing

class foo(Parsing.Precedence):
    "%left"

# Tokens.
class plus(Parsing.Token):
    "%token"
class star(Parsing.Token):
    "%token"
class lparen(Parsing.Token):
    "%token"
class rparen(Parsing.Token):
    "%token"
class id(Parsing.Token):
    "%token"
class bar(Parsing.Token):
    "%token"

# Non-terminal definitions.
class E(Parsing.Nonterm):
    "%start"
    def reduceA(self, E, plus, T):
	"%reduce E plus T"
	print "%r ::= %r %r %r" % (self, E, plus, T)

    def reduceB(self, T):
	"%reduce T"
	print "%r ::= %r" % (self, T)

    def reduceC(self, T):
	"%reduce T"
	print "%r ::= %r" % (self, T)

class T(Parsing.Nonterm):
    "%nonterm"
    def reduceA(self, T, star, F):
	"%reduce T star F"
	print "%r ::= %r %r %r" % (self, T, star, F)
    def reduceB(self, F):
	"%reduce F"
	print "%r ::= %r" % (self, F)

class F(Parsing.Nonterm):
    "%nonterm"
    def reduceA(self, lparen, E, rparen):
	"%reduce lparen E rparen"
	print "%r ::= %r %r %r" % (self, lparen, E, rparen)
    def reduceB(self, id):
	"%reduce id"
	print "%r ::= %r" % (self, id)

class Biz(Parsing.Nonterm):
    "%nonterm"

    def reduceA(self):
	"%reduce [foo]"

    def reduceB(self):
	"%reduce bar"

class TestParser(Parsing.Glr):
    def __init__(self, spec):
	Parsing.Glr.__init__(self, spec)

spec = Parsing.Spec(sys.modules[__name__], logFile="Parsing_i.log",
  skinny=False)
print spec
parser = TestParser(spec)
parser.verbose = True

print "Input: id * id + id"
parser.token(id(parser))
parser.token(star(parser))
parser.token(id(parser))
parser.token(plus(parser))
parser.token(id(parser))
parser.eoi()

print "Start: %r" % parser.start

