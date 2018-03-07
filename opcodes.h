#define PIX_WIDTH 10
#define PIX_HEIGHT 10
#define EMU_WIDTH 64 * PIX_WIDTH
#define EMU_HEIGHT 32 * PIX_HEIGHT
#define OPCODE_COUNT 35

typedef enum {CALL, DISPLAY, FLOW, COND, ASSIGN, BITOP, MATH, MEM,                  DISP, KEYOP, TIMER, SOUND, BCD, CONST, RAND} callType;

typedef struct opcode_t {
    char * opcode;
    callType type;
} Opcode;

//memory map for the emu
//  0x000 - 0x1FF : Emu interpreter (including the font set)
//  0x050 - 0x0A0 : Used for the 4x5 pixel font set ( 0 - F)
//  0x200 - 0xFFF : Program rom and work RAM
typedef struct emu_t
{
    unsigned char gfx[EMU_WIDTH * EMU_HEIGHT];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned char memory[4096];
    unsigned char key[16];      //keypad state (this is hex based)
    unsigned char V[16];        //chip-8 only has 15 registers, E register is the carry flag
    unsigned short I;           //this is the index register
    unsigned short pc;          //this is the program counter
    unsigned short stack[16];   //16 stack levels
    unsigned short sp;          //the stack pointer
    Opcode opcodes[OPCODE_COUNT];         //opcode storage
} Emu;
