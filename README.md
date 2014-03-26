# lambda

Untyped lambda calculus implementation.

Features:
* immutable data,
* eager argument evaluation and call-by-value semantics,
* left-to-right associativity for applications,
** i.e. `a b c` is equivalent to `(a b) c`
* native support for N-ary functions,
* tail call optimization,
* language utilities (see below),
* extra types:
** 32 bit signed integers
** 64 bit signed integers
** double-precision floating point numbers
** complex numbers (pairs of double-precision floating point numbers)
** strings

Booleans are Church-encoded. All the data is immutable.

Single-line comments support through the `#` character.

Some examples:

```
# identity function
lambda x . x

# identity function applied to itself
(lambda x . x)(lambda x . x)

# the easiest way to loop the interpreter
(lambda x . x x)(lambda x . x x)

# factorial
Z lambda factorial, n . (< n 1) 1 (* n (factorial (- n 1)))

# fibonacci
Z lambda fibonacci, i . (< i 2) 1 (+ (fibonacci (- i 1)) (fibonacci (- i 2)))
```

## Language Utilities

The following predefined terms are readily available and you can use them without defining them (descriptions in angle brackets indicate native code implementation):

```
true = lambda x, y . x
false = lambda x, y . y

not = lambda a . a false true
and = lambda a, b . a b false
or = lambda a, b . a true b
xor = lambda a, b . a (not b) b

if = lambda cond, then, else . cond then else

Z = lambda f . (lambda x . f (lambda v . x x v)) (lambda x . f (lambda v . x x v))

= = <value comparison>
!= = <negated value comparison>
< = <less than>
<= = <less than or equal to>
> = <greater than>
>= = <greater than or equal to>

+ = <binary sum>
- = <binary subraction>
* = <multiplication>
/ = <division>
** = <power>
~ = <bitwise NOT>
& = <bitwise AND>
| = <bitwise OR>
^ = <bitwise XOR>
```

## Known Issues

The `=` comparison operator doesn't work on booleans because they are Church-encoded.
