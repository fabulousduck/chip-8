# The machine
The machine is the wrapper around the opcode interpreter that manages boot and shutdown procedures. It is responsible for loading ROM's into machine ROM for the interpreter to read out of. It also forms a layer for the interpreter and the SDL graphics layer. 

## Boot sequence
The boot sequence of the chip-8 is quite simple as it only consists of a few steps. These steps are listed below.

* **1) Boot the machine**
* **2) Load the font data**
* **3) Load a game ROM**

### Power machine off
The machine starts in the off state as this makes it possible to swap between ROMS without having to turn the entire emulator on and off again. There are also certain ROMS that make use of the fact that they can turn the machine off which we dont want

### Load the font data
The font data needs to be loaded into ROM to avoid game ROMS accidently overriding this data. If the font data is corrupted by another game there is no way for another rom to check its validity. Font data is loaded into the lower 200 bytes of the machines ROM.

### Load a game
If the emulator was started with the `-f` flag and provided a valid game ROM path, we can load the game into the game ROM area starting at 0x200. If a game is not present this stage will be skipped at boot and deferred until a game rom has been selected.

## ROM layout
The layout of the machine's rom is designed in such a way that it is safe for specific regions of ROM to be reserved for the machine. Data such as fonts and boot sequence variables are stored in such reserved ROM spaces. These reserved areas of ROM are not accessible to programs loaded into the machine. These checks are performed by the machine itself. Below can be seen a diagram that displays all the sectors of the machines ROM as well as the registers in the machine.

<img src="https://i.imgur.com/8w6IzWm.png">