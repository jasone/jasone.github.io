import sys
import Parsing

# Precedences.
class p1(Parsing.Precedence):
    "%left"
class p2(Parsing.Precedence):
    "%left >p1"

# Tokens.
class plus(Parsing.Token):
    "%token [p1]"
class star(Parsing.Token):
    "%token [p2]"
class id(Parsing.Token):
    "%token"

# Non-terminal definitions.
class S(Parsing.Nonterm):
    "%start"
    def reduce(self, E):
	"%reduce E"
	print "%r ::= %r" % (self, E)

class E(Parsing.Nonterm):
    "%nonterm"
    def reduceId(self, id):
	"%reduce id"
	print "%r ::= %r" % (self, id)
    def reducePlus(self, EA, plus, EB):
	"%reduce E plus E [p1]"
	print "%r ::= %r %r %r" % (self, EA, plus, EB)
    def reduceStar(self, EA, star, EB):
	"%reduce E star E [p2]"
	print "%r ::= %r %r %r" % (self, EA, star, EB)

class TestParser(Parsing.Glr):
    def __init__(self, spec):
	Parsing.Glr.__init__(self, spec)

spec = Parsing.Spec(sys.modules[__name__], skinny=False)
print spec
parser = TestParser(spec)
#parser.verbose = True

print "Input: id * id + id * id"
parser.token(id(parser))
parser.token(star(parser))
parser.token(id(parser))
parser.token(plus(parser))
parser.token(id(parser))
parser.token(star(parser))
parser.token(id(parser))
parser.eoi()

print "Start: %r" % parser.start
