import sys
import Parsing

# Tokens.
class a(Parsing.Token):
    "%token"
class b(Parsing.Token):
    "%token"
class c(Parsing.Token):
    "%token"

# Non-terminal definitions.
class S(Parsing.Nonterm):
    "%start"
    def reduce_aAa(self, a_a, A, a_b):
	"%reduce a A a"

    def reduce_bAb(self, b_a, A, b_b):
	"%reduce b A b"

    def reduce_aBb(self, a, B, b):
	"%reduce a B b"

    def reduce_bBa(self, b, B, a):
	"%reduce b B a"

class A(Parsing.Nonterm):
    "%nonterm"
    def reduce_cC(self, c, C):
	"%reduce c C"

class B(Parsing.Nonterm):
    "%nonterm"
    def reduce_cc(self, c_a, c_b):
	"%reduce c c"

class C(Parsing.Nonterm):
    "%nonterm"
    def reduce_c(self, c):
	"%reduce c"

class TestParser(Parsing.Glr):
    def __init__(self, spec):
	Parsing.Glr.__init__(self, spec)

spec = Parsing.Spec(sys.modules[__name__], skinny=False)
print spec
parser = TestParser(spec)
parser.verbose = True
