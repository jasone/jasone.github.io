import sys
import Parsing

# Tokens.
class a(Parsing.Token):
    "%token"
class b(Parsing.Token):
    "%token"
class c(Parsing.Token):
    "%token"
class d(Parsing.Token):
    "%token"
class e(Parsing.Token):
    "%token"

# Non-terminal definitions.
class S(Parsing.Nonterm):
    "%start"
    def reduceA(self, a, A, d):
	"%reduce a A d"
    def reduceB(self, b, B, d):
	"%reduce b B d"
    def reduceC(self, a, B, e):
	"%reduce a B e"
    def reduceD(self, b, A, e):
	"%reduce b A e"

class A(Parsing.Nonterm):
    "%nonterm"
    def reduce(self, c):
	"%reduce c"

class B(Parsing.Nonterm):
    "%nonterm"
    def reduce(self, c):
	"%reduce c"

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
