# note about exponentiation
given the expression "1+2^3^2", we want "(1 + 4^(3^2))", but get "(1 + (4^3)^2)"

left-associative order of operations gives us:
            +
        1       ^
              ^   2
             4 3

but we want exponentiation to be right-associative:
            +
        1       ^
              4   ^
                 3 2

so we need a tree rotation
but we can't rotate the upper ^
because that's what the root is pointing to
so we need to do a non standard rotation

but only if it's two chained ^s: "1+2^3+2" truly is
            +
        1       +
              ^   2
             2 3
