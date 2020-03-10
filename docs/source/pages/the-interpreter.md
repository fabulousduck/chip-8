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