#ifndef CHIP8EMU_CHIP8_H
#define CHIP8EMU_CHIP8_H
#include <cstdint>
#include <vector>
#include <fstream>
#include <chrono>
#include <random>

class chip8 {
public:
    //REFERENCE---------------------------------------------------------------------------------------------------------
    /*TYPE GUIDE:
     uint8_t = 0xFF (1 byte)
     uint16_t = 0xFFFF  (2 bytes)
     uint32_t = 0xFFFFFFFF  (4 bytes)
     uint64_t = 0xFFFFFFFFFFFFFFFF  (8 bytes)

     nnn or addr- a 12-bit value, lowest 12 bits of instruction
     n or nibble- a 4-bit value, lowest 4 bits of the instruction
     x- a 4-bit value, the lower 4 bits of the high byte of the instruction
     y- a 4-bit value, the upper 4 bits of the low byte of the instruction
     kk or byte- an 8-bit value, the lowest 8 bits of the instruction
     */

    //DATA MEMBERS------------------------------------------------------------------------------------------------------
    //memory
    //0x000 to 0xFFF (0 to 4095)
    //first 512 bytes, 0x000 to 0x1FF RESERVED FOR ORIGINAL INTERPRETER
        //x200- start of most Chip-8 programs
        //x600- start of ETI 660 Chip-8 programs
        //xFFF- end of Chip-8 RAM
    uint8_t memory[4096]= {};
    uint8_t display[128*64] = {};

    //registers
    //16 general purpose 8-bit registers, V0 to VF
        //VF should not be used by any program
    uint16_t registers[16] = {}; //V0 through VF

    //16 bit register, I to store memory addresses
    uint16_t I; //index register

    //delay and sound timers
        //when the register is non zero, they are decremented at rate of 60Hz
    uint8_t delay_timer;
    uint8_t sound_timer;

    //pseudo-registers
        //program counter (PC) 16-bit, stores currently executing address
    uint16_t PC; //program counter
        //stack pointer (SP) 8-bit, point to topmost level of stack
    uint8_t SP;

    //stack
        //array of 16 16-bit values
        //stores address interpreter should return to when finished with a subroutine
    std::vector<uint16_t> stack;

    //font
    uint8_t fontset[80] =
    {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    //INSTRUCTIONS------------------------------------------------------------------------------------------------------
    //ROM: a binary program code will all the data to execute it
    void loadROM(const char* filename);
    chip8();


    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;

    //0nnn - SYS addr
        //Jump to a machine code routine at nnn


};

void chip8::loadROM(const char *filename) {
    std::ifstream ifile(filename, std::ios::binary | std::ios::ate);
    if (ifile.is_open()){
        std::streampos size = ifile.tellg();
        char* buffer = new char[size];
        //put at the beginning
        ifile.seekg(0, std::ios::beg);
        //read in buffer
        ifile.read(buffer, size);
        ifile.close();
        for (int i = 0; i < size; i++){
            memory[0x200 + i] = buffer[i];
        }
        delete[] buffer;
    }
}

chip8::chip8() {
    PC = 0x200;

    for (int i = 0; i < 80; i++){
        memory[0x50 + i] = fontset[i];
    }
}


#endif //CHIP8EMU_CHIP8_H
