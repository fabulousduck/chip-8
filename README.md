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

### decoding the opcode

Now that we have our opcode `2F5B`, we need to check what it means.

The following table describes what the placeholder values are and represent down below in the opcode table

#### Notation

| notation | explanation |
| -------- | ----------- |
| X / Y    | These are placeholders for an int between 0-F |
| N        | This is used when a number is imidiate |
| NN       | This is used to represent a byte of data |
| NNN      | This is used to represent an address |

#### Opcodes

| opcode   | explanation |
| -------- | ----------- | 
| 0XXX     | This is used for special operations. See special operations table |
| 1XXX     | This is used for abosulte jumps. I.E : `12A8` == `JMP $2A8` |
| 2XXX     | Jumps to a subroutine. See Subroutine jumps. |
| 3XXX     | Does a `SKIP.EQ` with the latter byte being the source. I.E : `3A00` == `SKIP.EQ V[A], #$00`. |
| 4XXX     | Does a `SKIP.NE` with the latter byte being the source. I.E : `4800` == `SKIP.NE V[8], #$00` |
| 5XXX     | Does a `SKIP.EQ` with the latter byte being the index for the registers(`V`). I.E `5A70` == `SKIP.EQ V[A], V[7]` |
| 9XXX     | Does a `SKIP.NE` with the latter byte being the index for the registers(`V`). I.E `93B0` == `SKIP.NE, V[2], V[B]` |
| 8XXX     | Register operations. See register operations table for exact definitions |
|


#### register ops

| opcode | explanation |
| ------ | ----------- |
| 8XX0   | `MOV`: Example: `83A0` == `MOV V3, VA` |
| 8XX1   | `OR`: Example: `83A1` == `OR V3, VA` |
| 8XX2   | `AND`: Example: `83A2` == `AND V3, VA` |
| 8XX3   | `XOR`: Example: 83A3` = `XOR V3, VA` (must alternate)|
| 8XX4   | `ADD` on `VF`: Example: `83B4` == `ADD V3, VB` |
| 8XX5   | `SUB` on `VF`: Example: `83B5` == `ADD V3, VB` |
| 