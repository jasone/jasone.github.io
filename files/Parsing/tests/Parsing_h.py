import sys
import Parsing

class TokenI(Parsing.Token):
    "%token i"
class TokenPlus(Parsing.Token):
    "%token plus [split]"
class TokenStar(Parsing.Token):
    "%token star [split]"

class S(Parsing.Nonterm):
    "%start"

    def __repr__(self):
	return "%r" % self.E

    def reduce(self, E):
	"%reduce E"
	self.E = E

class E(Parsing.Nonterm):
    "%nonterm"

    def __repr__(self):
	if self.variant == "i":
	    return "i"
	elif self.variant == "plus":
	    return "(%r + %r)" % (self.EA, self.EB)
	elif self.variant == "star":
	    return "(%r * %r)" % (self.EA, self.EB)
	else:
	    assert False

    def merge(self, other):
	if self.variant == "plus":
	    return self
	elif other.variant == "plus":
	    return other
	else:
	    return self

    def reduceI(self, i):
	"%reduce i"
	self.variant = "i"
	self.i = i

    def reducePlus(self, EA, plus, EB):
	"%reduce E plus E [split]"
	self.variant = "plus"
	self.EA = EA
	self.plus = plus
	self.EB = EB

    def reduceStar(self, EA, star, EB):
	"%reduce E star E [split]"
	self.variant = "star"
	self.EA = EA
	self.star = star
	self.EB = EB

class TestParser(Parsing.Glr):
    def __init__(self, spec):
	Parsing.Glr.__init__(self, spec)

spec = Parsing.Spec(sys.modules[__name__], skinny=False)
print spec
parser = TestParser(spec)
parser.verbose = True

print "Input: i + i * i"
parser.token(TokenI(parser))
parser.token(TokenPlus(parser))
parser.token(TokenI(parser))
parser.token(TokenStar(parser))
parser.token(TokenI(parser))
parser.eoi()

assert len(parser.start) == 1
print parser.start[0]
