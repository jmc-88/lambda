# lambda

Untyped lambda calculus implementation.

Features:
* immutable data,
* eager argument evaluation and call-by-value semantics,
* left-to-right associativity for applications,
	* i.e. `a b c` is equivalent to `(a b) c`
* native support for N-ary functions,
* tail call optimization,
* single-line comments (with the `#` character),
* language utilities (see below),
* extra types:
	* 32 bit signed integers
	* 64 bit signed integers
	* double-precision floating point numbers
	* complex numbers (pairs of double-precision floating point numbers)
	* strings

Booleans are Church-encoded.

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

# a list of 4 integers: 3, 6, 2, 5
list 3 (list 6 (list 2 (list 5)))

# printing a list to standard output
Z lambda print_list, list . and (print (head list)) (and (print ', ') (print_list (tail list)))
```

## Language Utilities

The following predefined terms are readily available and you can use them without defining them (descriptions in angle brackets indicate native code implementation):

```
true   = lambda x, y . x
false  = lambda x, y . y

not  = lambda a . a false true
and  = lambda a, b . a b false
or   = lambda a, b . a true b
xor  = lambda a, b . a (not b) b

if  = lambda condition, then, else . condition then else

Z  = lambda f . (lambda x . f (lambda v . x x v)) (lambda x . f (lambda v . x x v))

=   = <value comparison>
!=  = <negated value comparison>
<   = <less than>
<=  = <less than or equal to>
>   = <greater than>
>=  = <greater than or equal to>

+   = <binary sum>
-   = <binary subraction>
*   = <multiplication>
/   = <division>
**  = <power>
~   = <bitwise NOT>
&   = <bitwise AND>
|   = <bitwise OR>
^   = <bitwise XOR>

pair    = lambda x, y, z . z x y
first   = lambda pair . pair lambda x, y . x
second  = lambda pair . pair lambda x, y . y

list  = pair
head  = first
tail  = second

print    = <prints the string argument to standard output, returns true>
println  = <prints the string argument followed by a line terminator to standard output, returns true>
input    = <reads a line from standard input and returns it as a string, excluding the line terminator>
```

## Known Issues

The `=` comparison operator doesn't work on booleans because they are Church-encoded.

No garbage collection yet, the interpreter will leak the world until you terminate it (this is going to be fixed in a future release).
