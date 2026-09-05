#include "6502.h"
#include <iostream>

int main()
{
    CPU cpu;
    cpu.Initialize();

    //inline instruction - loop
    /*loop

      LDX

      CPX
      BEQ => jump to address if zero flag is on
      DEX
      JMP
    */

    uint8_t num_iterations = 0x05;



    cpu.mem[0x8000] = 0xA2; // LDX
    cpu.mem[0x8001] = num_iterations;
    cpu.Clock();
    cpu.Clock();

    cpu.mem[0x8002] = 0xE0; // CPX
    cpu.mem[0x8003] = 0x00;

    cpu.mem[0x8004] = 0xF0; // BEQ
    cpu.mem[0x8005] = 0x04;

    cpu.mem[0x8006] = 0xCA; // DEX

    cpu.mem[0x8007] = 0x4C; // JMP
    cpu.mem[0x8008] = 0x02;
    cpu.mem[0x8009] = 0x80;

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
            cpu.Clock();

        }
    }

    return 0;
}
