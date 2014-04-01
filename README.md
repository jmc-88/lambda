# lambda

Untyped lambda calculus implementation.

Features:
* immutable data,
* eager argument evaluation and call-by-value semantics,
* macros (functions with lazy evaluation and call-by-name semantics),
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
Z lambda factorial, n . if (< n 1) 1 (* n (factorial (- n 1)))

# fibonacci
Z lambda fibonacci, i . if (< i 2) 1 (+ (fibonacci (- i 1)) (fibonacci (- i 2)))

# a list of 4 integers: 3, 6, 2, 5
list 3 (list 6 (list 2 (list 5 nil)))

# printing a list to standard output
Z lambda print_list, list . and (head list (lambda head . print head) false) (tail list (lambda tail . and (print ', ') (print_list tail)) false)

# scanning a list looking for a value
Z lambda search_list, list, value . or (head list (lambda head . = head value) false) (tail list (lambda tail . search_list tail value) false)
```

## Language Utilities

The following predefined terms are readily available and you can use them without defining them (descriptions in angle brackets indicate native code implementation):

```
true   = lambda x, y . x
false  = lambda x, y . y

not  = macro a . a false true
and  = macro a, b . a (lambda x . b) (lambda x . false) nil
or   = macro a, b . a (lambda x . true) (lambda x . b) nil
xor  = macro a, b . a (lambda x . not b) (lambda x . b) nil

if  = macro condition, then, else . condition (lambda x . then) (lambda x . else) nil

Z  = lambda f . (lambda x . f lambda v . x x v) (lambda x . f lambda v . x x v)

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
first   = macro pair . pair lambda first, second . first
second  = macro pair . pair lambda first, second . second

list  = lambda element, next, f, g . f element next
nil   = Z lambda nil, f, g . g nil
head  = macro list, f, g . list f lambda x . g
tail  = macro list, f, g . list (lambda element, next . f next) lambda x . g

exit     = <immediately terminates the program and uses the argument as return code>
throw    = <throws the argument as an exception, terminates the program>
input    = <ignores the argument, reads a line from standard input and returns it as a string, excluding the line terminator>
print    = <prints the string argument to standard output, returns true>
println  = <prints the string argument followed by a line terminator to standard output, returns true>
error    = <prints the string argument to standard error, returns true>
errorln  = <prints the string argument followed by a line terminator to standard error, returns true>
```

## Known Issues

The `=` and `!=` comparison operators don't work on booleans because they are Church-encoded.

Exceptions may be thrown using the `throw` term (which has native implementation) but there's no way to catch them (yet - a future release will add it).
