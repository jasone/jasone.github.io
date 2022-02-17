import sys
import Parsing

# Tokens.
class c(Parsing.Token):
    "%token"
class d(Parsing.Token):
    "%token"

# Non-terminal definitions.
class S(Parsing.Nonterm):
    "%start"
    def reduce(self, CA, CB):
	"%reduce C C"
	print "%r ::= %r %r" % (self, CA, CB)

class C(Parsing.Nonterm):
    "%nonterm"
    def reduceA(self, c, C):
	"%reduce c C"
	print "%r ::= %r %r" % (self, c, C)
    def reduceB(self, d):
	"%reduce d"
	print "%r ::= %r" % (self, d)

class TestParser(Parsing.Glr):
    def __init__(self, spec):
	Parsing.Glr.__init__(self, spec)

spec = Parsing.Spec(sys.modules[__name__], skinny=False)
print spec
parser = TestParser(spec)
parser.verbose = True

#print "Input: id * id + id"
#parser.token(id(parser))
#parser.token(star(parser))
#parser.token(id(parser))
#parser.token(plus(parser))
#parser.token(id(parser))
#parser.eoi()
#
#print "Start: %r" % parser.start
