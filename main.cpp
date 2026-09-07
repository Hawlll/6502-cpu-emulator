#include "6502.h"
#include "Bus.h"
#include "Memory.h"
#include <iostream>

int main()
{
    CPU cpu;
    Memory ram(64);
    Video gpu;
    Bus bus(ram, gpu);

    cpu.Initialize();


    //inline instruction - loop
    /*loop

      LDX

      CPX
      BEQ => jump to address if zero flag is on
      DEX
      JMP
    */

    bus.Write(0x8000, 0xA9);
    bus.Write(0x8001, 0x01);
    cpu.Clock(bus);
    cpu.Clock(bus);


    uint16_t test_vram_locs[] = {
        0xE033, 0xE034, 0xE035, 0xE036,

        0xE043,
        0xE053,
        0xE063,
        0xE073,
        0xE083,
        0xE093,
        0xE0A3,
        0xE0B3,

        0xE047,
        0xE057,
        0xE067,

        0xE073, 0xE074, 0xE075, 0xE076,

        0xE087,
        0xE097,
        0xE0A7,

        0xE0B3, 0xE0B4, 0xE0B5, 0xE0B6
    };

    for (size_t i = 0; i < std::size(test_vram_locs); i++) {
        bus.Write(0x8002 + (3 * i), 0x8D);
        bus.Write(0x8003 + (3 * i), test_vram_locs[i] & 0x00FF);
        bus.Write(0x8004 + (3 * i), (test_vram_locs[i] & 0xFF00) >> 8);
    }

    for (int i = 0; i < (int)std::size(test_vram_locs) * 4; i++) {
        cpu.Clock(bus);
    }

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            uint8_t val_in_vram = bus.Read(0xE000 + (x*16) + y);
            if (val_in_vram == 0x01) {
                std::cout << "#";
            }
            else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }

//    uint8_t num_iterations = 0x01;
//
//
//
//    bus.Write(0x8000, 0xA2); // LDX
//    bus.Write(0x8001, num_iterations);
//    cpu.Clock(bus);
//    cpu.Clock(bus);
//
//    bus.Write(0x8002, 0xE0); // CPX
//    bus.Write(0x8003, 0x00);
//
//    bus.Write(0x8004, 0xF0); // BEQ
//    bus.Write(0x8005, 0x09);
//
//    bus.Write(0x8006, 0xCA); // DEX
//
//    bus.Write(0x8007, 0x4C); // JMP
//    bus.Write(0x8008, 0x02);
//    bus.Write(0x8009, 0x80);
//
//    while (true) {
//        char in;
//        std::cout << "Type c to clock CPU: ";
//        std::cin >> in;
//
//        if (in == 'c') {
//            std::cout << std::endl;
//            std::cout << "Cycles: " << (int)cpu.cycles << std::endl;
//            std::cout << "X register: " << (int)cpu.X << std::endl;
//            std::cout << "Program Counter: " << std::format("{:#X}\n", (int)cpu.PC);
//            std::cout << "Status: ";
//
//
//            for (int i = 7; i > -1; i--) {
//                int on = (cpu.status & (1 << (i))) > 0;
//                std::cout << on;
//            }
//            std::cout << std::endl;
//            std::cout << std::endl;
//            cpu.Clock(bus);
//
//        }
//    }

    return 0;
}
