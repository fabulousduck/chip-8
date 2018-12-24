# notes on bitwise operations


## bitmasking

bitmasking is basically the operation of hiding a a part of a bit vlue

I.E

if we wish to get 0x2F0 from 0xA2F0 we need to do the following

1010001011110000 //0xA2F0


0000111111111111 //0x0FFF


----------------- AND

0000001011110000 //0x2F0


In typical c code this would look a little like this:

```c
unsigned int I = opcode & 0x0FFF;   //set I to the value of the resulting opcode
pc += 2;                            //because we need to move the opcode pointer
```

note: to inverse any of these the opposite of all of these operations can be
applied.


THE BIT BELOW THE COMPARISON WILL BE SET TO THE RESULT OF THE EXPRESSION WHERE TRUE IS 1 AND FALSE IS
0

an operation will compare bit by bit top to bottom in an expression to see if
the statement yields true or false and behaves according to the statement above

## multiplying

If we wish to multiply a binary value by a factor of 2's
we need to shift the value to the left by one bit.

For example


1000 == decimal(8)

1000 << 1 == 10000

10000 == decimal(16)

## deviding

Deviding works just like multiplication except here we use the inverse (deviding
is the inverse of multiplying)

For example

1000 == decimal(8)

1000 >> 1 == 100

100 == decimal(4)


## setting a bit in a value in position N to be on

To set a bit at a position N in a binary value to 1
we must do the following

First we create a new value to compare with.
In this value we will insert the desired value into a position N

for Example:

value << N

0000000 << 5 == 0100000

now we use a bitwise OR operation on the old value

1010101
0100000
-------- AND ( & )
111010

now the 1 value has been set at the desired position


## turning a bit in a value off

To turn off a value at a position N in a binary value we must do the following

First we create a new value where the N value is 0 and all the others are 1 like
so


newValue = ~(1 << N)

to examplify this

101111 = ~(1 < 4)

now we can apply an and value to this with our old value on top

111010
101111
------- AND
101010

this happens because 1 AND 0 is 0, so now the bit is 0



## checking wether a bit is 1 or 0 at position `N` in set `S`

checking wether a value is 1 or zero is done by comparing the result to 0

RESULT == 0 == IS NOT 1

RESULT == 1 == IS 1


This is done because the RESULT string is a string that marks on positon N with
either 1 or 0 if the value specified is 1 or 0 or not.

00000 is equal to decimal(0)

but

01000 is not equal to decimal(0)

to achieve such a result we must create a new value where the position we want
to check is one like so

1 << N

to examplify

1 << 5 == 0100000

now we perform an AND operation with this value at the bottom

1010101
0100000
-------- AND

0000000

0000000 = decimal(0) so it is NOT 1 thus NOT SET

but the oposite is also true

1110101
0100000
-------- AND
0100000

0100000 != decimal(0) so it is IS 1 thus IS SET


## flipping a bit in a value at position N

































