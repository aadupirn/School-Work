## Homework 5
### Aadu Pirn, aap75

### Problem 1
 
#### Part I

##### a) A = {w | w starts with 0}
- 011000
- x = 0
- y = 11
- z = 000

##### b) A = {w | w ends with 1}
- 000111
- x = 000
- y = 11
- z = 1

##### c) A = {w | w starts with a 0 and ends with a 1}
- 001001
- x = 0
- y = 0100
- z = 1

##### d) A = {w | w contains 11 as a substring}
- 001100
- x = 00
- y = 11
- z = 00

#### Part II

##### a)
- 011000
##### b)
- 000111
##### c)
- 001001 
##### d)
- 001100
##### e)
- 01110111
##### f)
- 0011001100110011
##### g)
- 0000111
##### h)
- aaabbc

### Problem 2

#### Part I

##### a) A = {w#w#w | w ∈ {a, b}∗}
- (abaa)^p (abaa)^p (abaa)^p
- if y = ba then it will break the first condition.
- therefor A is not a regular expression.

##### b) A = {a^ib^i | j = 2^i}
- a^p b^(2^p)
- if y is b then it will break the first condition
- therefor A is not a regular expression.

#### Part II
- 1.73 says that 0^p1^p cannot be pumped with respect to B which is {0^n1^n|n>=0}. Pumped strings in 0^p^p all are in 0 * 1 *.
