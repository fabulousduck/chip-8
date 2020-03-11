# The interpreter
The interpreter is at the core of the chip-8 emulator. It takes a 2 byte opcode and executes it accordingly. Per cycle, one opcode is executed. The interpreter is the only part of the entire emulator that has access to the internal registers and read only memory of the emulator. Below is documentation on all the opcodes that it can execute and what effects they have on the emulator.

## Opcodes
Within the chip-8 system, opcodes are 4 bytes long. Each opcode has 4 prefix bits ranging from 0 - F. These act as their identifiers. The following notation is used to denote what an opcode can contain.

* **NNN:   address**
* **NN:    8-bit constant**
* **N:     4-bit constant**
* **X - Y: 4-bit register identifiers**
* **PC:    Program counter**
* **I:     16bit register (For memory address) (Similar to void pointer)**
* **VN:    One of the 16 available variables. N may be 0 to F (hexadecimal)**


<table id="opcodes">
    <tr>
        <td>Opcode</td>
        <td>Type</td>
        <td>C pseudo</td>
        <td>Details</td>
    </tr>
    <tr>
        <td>0NNN</td>
        <td>Call</td>
        <td></td>
        <td>Calls RCA 1802 program at address NNN. Not necessary for most ROMs.</td>
    </tr>
    <tr>
      <td>00E0</td>
      <td>Display</td>
      <td>disp_clear() </td>
      <td>Clears the screen. </td>
    </tr>
    <tr>
      <td>00EE</td>
      <td>return</td>
      <td>return;</td>
      <td>Returns from a subroutine.</td>
    </tr>
    <tr>
      <td>1NNN</td>
      <td>Flow</td>
      <td>goto NNN;</td>
      <td>Jumps to address NNN.</td>
    </tr>
    <tr>
      <td>2NNN</td>
      <td>Flow</td>
      <td>*(0xNNN)()</td>
      <td>Calls subroutine at NNN.</td>
    </tr> 
    <tr>
      <td>3XNN</td>
      <td>Cond</td>
      <td>if(Vx==NN)</td>
      <td>Skips the next instruction if VX equals NN.</td>
    </tr>
    <tr>
      <td>4XNN</td>
      <td>Cond</td>
      <td>if(Vx!=NN)</td>
      <td>Skips the next instruction if VX doesn't equal NN.</td>
    </tr> 
    <tr>
      <td>5XY0</td>
      <td>Cond</td>
      <td>if(Vx==Vy)</td>
      <td>Skips the next instruction if VX equals VY.</td>
    </tr> 
    <tr>
      <td>6XNN</td>
      <td>Const</td>
      <td>Vx = NN</td>
      <td>Sets VX to NN.</td>
    </tr>
    <tr>
      <td>7XNN</td>
      <td>Const</td>
      <td>Vx += NN</td>
      <td>Adds NN to VX. (Carry flag is not changed) </td>
    </tr>
    <tr>
      <td>8XY0 </td>
      <td>Assign</td>
      <td>Vx = VY</td>
      <td>Sets VX to the value of VY.</td>
    </tr>
    <tr>
      <td>8XY1</td>
      <td>BitOp</td>
      <td>Vx=Vx|Vy</td>
      <td>Sets VX to VX OR VY. </td>
    </tr>
    <tr>
      <td>8XY2</td>
      <td>BitOp</td>
      <td>Vx=Vx&Vy</td>
      <td>Sets VX to VX AND VY. </td>
    </tr>
    <tr>
      <td>8XY3</td>
      <td>BitOp</td>
      <td>Vx=Vx^Vy</td>
      <td>Sets VX to VX XOR VY. </td>
    </tr>
    <tr>
      <td>8XY4</td>
      <td>Math</td>
      <td>Vx -= Vy</td>
      <td>Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.</td>
    </tr>
    <tr>
      <td>8XY5</td>
      <td>Math</td>
      <td>Vx -= Vy</td>
      <td>Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.</td>
    </tr>
    <tr>
      <td>8XY6</td>
      <td>Math</td>
      <td>Vx >>= 1</td>
      <td>Stores the least significant bit of VX in VF and then shifts VX to the right by 1.</td>
    </tr>
    <tr>
      <td>8XY7</td>
      <td>Math</td>
      <td>Vx = Vy - Vx</td>
      <td>Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.</td>
    </tr>
    <tr>
      <td>8XYE</td>
      <td>Math</td>
      <td>Vx <<= 1</td>
      <td>Stores the most significant bit of VX in VF and then shifts VX to the left by 1.</td>
    </tr>
    <tr>
      <td>9XY0</td>
      <td>Cond</td>
      <td>if(Vx!=Vy)</td>
      <td>Skips the next instruction if VX doesn't equal VY.</td>
    </tr>
    <tr>
      <td>ANNN</td>
      <td>MEM</td>
      <td>I = NNN</td>
      <td>Sets I to the address NNN</td>
    </tr>
    <tr>
      <td>BNNN</td>
      <td>Flow</td>
      <td>PC = V0 + NNN</td>
      <td>Jumps to the address NNN plus V0.</td>
    </tr>
    <tr>
      <td>CXNN</td>
      <td>Rand</td>
      <td>Vx = rand()&NN </td>
      <td>Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.</td>
    </tr>
    <tr>
      <td>DXYN</td>
      <td>Disp</td>
      <td>draw(Vx,Vy,N) </td>
      <td>Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen</td>
    </tr>
    <tr>
      <td>EX9E</td>
      <td>KeyOp</td>
      <td>if(key()==Vx)</td>
      <td>Skips the next instruction if the key stored in VX is pressed. </td>
    </tr>
    <tr>
      <td>EXA1</td>
      <td>KeyOp</td>
      <td>if(key()!=Vx)</td>
      <td>Skips the next instruction if the key stored in VX isn't pressed.</td>
    </tr>
    <tr>
      <td>FX07</td>
      <td>Timer</td>
      <td>Vx = get_delay()</td>
      <td>Sets VX to the value of the delay timer.</td>
    </tr>
    <tr>
      <td>FX0A</td>
      <td>KeyOp</td>
      <td>Vx = get_key()</td>
      <td>A key press is awaited, and then stored in VX.</td>
    </tr>
    <tr>
      <td>FX15</td>
      <td>Timer</td>
      <td>delay_timer(Vx) </td>
      <td>Sets the delay timer to VX.</td>
    </tr>
    <tr>
      <td>FX18</td>
      <td>Sound</td>
      <td>sound_timer(Vx)</td>
      <td>Sets the sound timer to VX.</td>
    </tr>
    <tr>
      <td>FX1E</td>
      <td>MEM</td>
      <td>I +=Vx</td>
      <td>Adds VX to I. VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't.</td>
    </tr>
    <tr>
      <td>FX29</td>
      <td>MEM</td>
      <td>I=sprite_addr[Vx]</td>
      <td>Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font. </td>
    </tr>
    <tr>
      <td>FX33</td>
      <td>BCD</td>
      <td>set_BCD(Vx);</br>
        *(I+0)=BCD(3);</br>
        *(I+1)=BCD(2);</br>
        *(I+2)=BCD(1); 
      </td>
      <td>Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)</td>
    </tr>
    <tr>
      <td>FX55</td>
      <td>MEM</td>
      <td>reg_dump(Vx,&I)</td>
      <td>Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.</td>
    </tr>
    <tr>
      <td>FX65</td>
      <td>MEM</td>
      <td>reg_load(Vx,&I)</td>
      <td>Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.</td>
    </tr>
</table>


<style>
#opcodes {
  font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
  border-collapse: collapse;
  width: 100%;
}

#opcodes td, #opcodes th {
  border: 1px solid #ddd;
  padding: 8px;
}

#opcodes tr:nth-child(even){background-color: #f2f2f2;}

#opcodes tr:hover {background-color: #ddd;}

#opcodes th {
  padding-top: 12px;
  padding-bottom: 12px;
  text-align: left;
  background-color: #4CAF50;
  color: white;
}
</style>