#===============================================================================
#
# Ambiguous grammar test.
#
#===============================================================================

import sys
import Parsing

# Tokens.
class equal(Parsing.Token):
    "%token"
class star(Parsing.Token):
    "%token"
class id(Parsing.Token):
    "%token"

# Non-terminal definitions.
class S(Parsing.Nonterm):
    "%start"

    def reduceA(self, L, equal, R):
	"%reduce L equal R"

    def reduceB(self, R):
	"%reduce R"

class L(Parsing.Nonterm):
    "%nonterm"
    def reduceA(self, star, R):
	"%reduce star R"
    def reduceB(self, id):
	"%reduce id"

class R(Parsing.Nonterm):
    "%nonterm"
    def reduce(self, L):
	"%reduce L"

class TestParser(Parsing.Glr):
    def __init__(self, spec):
	Parsing.Glr.__init__(self, spec)

spec = Parsing.Spec(sys.modules[__name__], skinny=False)
print spec
#parser = TestParser(spec)
#parser.verbose = True
