# chip-8
Chip-8 emu in C


## Opcodes
This is a short explenations on how opcodes are decoded


### opcode from memory.

When reading an opcode it is important to understand that an opcode is 2 bytes long. so we will need to concat 2 bytes together to get the full opcode.

The opcodes are pointed at by the **program counter** `pc`.

so the bytes we will want to read are
`memory[pc]` and `memory[pc+1]`.

to add these to together we will need to append the first to the second which is done in the following way: 
`int opcode = memory[pc] << 8 | memory[pc+1];`
What is happening here is the following :

We have memory[pc], which we will assume to be `2F` for now which equals to `0010 1111` in binary.

when we apply a `<<` which is a **left bit shift**, we shift the `LHS` by `RHS` bytes.
In our case this is 8. This results in us ending up with `0010 1111 0000 0000 `.

This is enough space to move `memory[pc+1]` into. So by doing `| memory[pc+1]` we essentially **shift it into the empty 8 bytes**. (We will assume `memory[pc+1]` is '5B');

This results in: `0010 1111 0101 1011` which translates into `2F5B` in hex.

## decoding the opcode

Now that we have our opcode `2F5B`, we need to check what it means.


Before we can do that we need to define a few things about opcodes namely how the table represents them.

### `X`

In our table, X can be any integer in range 0 - F.

This integer represents a register in the register table.

Within the table, we will represent any register in the register table as follows:

`V[X]` where `V` is the register table and `X` the integer obtained from the opcode.

### `Y`

Same as X.

This serves as a variable when we need to for instance compare registers

I.E

`V[X] != V[Y]`

### `N`

`N` is used to represent an `immidiate` vale. Meaning that it is to be interpreted literally.

### `NN`

`NN` is used to represent a byte of data.

### `NNN`

`NNN` is used to represent a memory address.










