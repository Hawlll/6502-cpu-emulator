#include "6502.h"
#include "Memory.h"
#include <iostream>

int main()
{
    CPU cpu;
    Memory ram(64);

    cpu.Initialize();


    //inline instruction - loop
    /*loop

      LDX

      CPX
      BEQ => jump to address if zero flag is on
      DEX
      JMP
    */

    uint8_t num_iterations = 0x01;



    ram.Write(0x8000, 0xA2); // LDX
    ram.Write(0x8001, num_iterations);
    cpu.Clock(ram);
    cpu.Clock(ram);

    ram.Write(0x8002, 0xE0); // CPX
    ram.Write(0x8003, 0x00);

    ram.Write(0x8004, 0xF0); // BEQ
    ram.Write(0x8005, 0x09);

    ram.Write(0x8006, 0xCA); // DEX

    ram.Write(0x8007, 0x4C); // JMP
    ram.Write(0x8008, 0x02);
    ram.Write(0x8009, 0x80);

    while (true) {
        char in;
        std::cout << "Type c to clock CPU: ";
        std::cin >> in;

        if (in == 'c') {
            std::cout << std::endl;
            std::cout << "Cycles: " << (int)cpu.cycles << std::endl;
            std::cout << "X register: " << (int)cpu.X << std::endl;
            std::cout << "Program Counter: " << std::format("{:#X}\n", (int)cpu.PC);
            std::cout << "Status: ";


            for (int i = 7; i > -1; i--) {
                int on = (cpu.status & (1 << (i))) > 0;
                std::cout << on;
            }
            std::cout << std::endl;
            std::cout << std::endl;
            cpu.Clock(ram);

        }
    }

    return 0;
}
