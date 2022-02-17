import sys
import Parsing

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

# Non-terminal definitions.
class E(Parsing.Nonterm):
    "%start"
    def reduceA(self, E, plus, T):
	"%reduce E plus T"
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

class F(Parsing.Nonterm):
    "%nonterm"
    def reduceA(self, lparen, E, rparen):
	"%reduce lparen E rparen"
	print "%r ::= %r %r %r" % (self, lparen, E, rparen)
    def reduceB(self, id):
	"%reduce id"
	print "%r ::= %r" % (self, id)

class TestParser(Parsing.Glr):
    def __init__(self, spec):
	Parsing.Glr.__init__(self, spec)

#===============================================================================
print "===> Original"

spec = Parsing.Spec(sys.modules[__name__], skinny=False)
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

#===============================================================================
print "===> Direct Pickle"

import cPickle
specPickle = cPickle.dumps(spec, protocol=cPickle.HIGHEST_PROTOCOL)
spec2 = cPickle.loads(specPickle)
print spec2

parser = TestParser(spec2)
parser.verbose = True

print "Input: id * id + id"
parser.token(id(parser))
parser.token(star(parser))
parser.token(id(parser))
parser.token(plus(parser))
parser.token(id(parser))
parser.eoi()

print "Start: %r" % parser.start

#===============================================================================
print "===> Disk Pickle"

spec3 = Parsing.Spec(sys.modules[__name__], "Parsing_j.pickle")
print spec3

parser = TestParser(spec3)
parser.verbose = True

print "Input: id * id + id"
parser.token(id(parser))
parser.token(star(parser))
parser.token(id(parser))
parser.token(plus(parser))
parser.token(id(parser))
parser.eoi()

print "Start: %r" % parser.start
