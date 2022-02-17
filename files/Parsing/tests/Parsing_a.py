import sys
import Parsing

# Precedences.
class P1(Parsing.Precedence):
    "%left p1"
class p2(Parsing.Precedence):
    "%left >p1"

# Tokens.
class TokenPlus(Parsing.Token):
    "%token plus"
class TokenStar(Parsing.Token):
    "%token star"
class TokenLparen(Parsing.Token):
    "%token lparen"
class TokenRparen(Parsing.Token):
    "%token rparen [p2]"
class TokenId(Parsing.Token):
    "%token id"

# Non-terminal definitions.
class NontermE(Parsing.Nonterm):
    "%start E"
    def reduceA(self, E, plus, T):
	"%reduce E plus T [p1]"
	print "%r ::= %r %r %r" % (self, E, plus, T)

    def reduceB(self, T):
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

class NontermF(Parsing.Nonterm):
    "%nonterm F [p2]"
    def reduceA(self, lparen, E, rparen):
	"%reduce lparen E rparen"
	print "%r ::= %r %r %r" % (self, lparen, E, rparen)
    def reduceB(self, id):
	"%reduce id [split]"
	print "%r ::= %r" % (self, id)

class TestParser(Parsing.Lr):
    def __init__(self, spec):
	Parsing.Lr.__init__(self, spec)

spec = Parsing.Spec(sys.modules[__name__])
print spec
parser = TestParser(spec)

print "Input: id * id + id"
parser.token(TokenId(parser))
parser.token(TokenStar(parser))
parser.token(TokenId(parser))
parser.token(TokenPlus(parser))
parser.token(TokenId(parser))
parser.eoi()

print "Start: %r" % parser.start
