import sys
import Parsing

# Tokens.
class c(Parsing.Token):
    "%token"

# Non-terminal definitions.
class S(Parsing.Nonterm):
    "%start"
    def reduceA(self, A):
	"%reduce A"
    def reduceB(self, B):
	"%reduce B"

    def merge(self, a, b):
	if isinstance(a, A):
	    return a
	else:
	    return b

class A(Parsing.Nonterm):
    "%nonterm [split]"
    def reduce(self, c):
	"%reduce c"

class B(Parsing.Nonterm):
    "%nonterm [split]"
    def reduce(self, c):
	"%reduce c"

class TestParser(Parsing.Glr):
    def __init__(self, spec):
	Parsing.Glr.__init__(self, spec)

spec = Parsing.Spec(sys.modules[__name__], skinny=False, logFile="Parsing_g.log")
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
