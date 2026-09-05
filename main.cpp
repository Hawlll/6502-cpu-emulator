#include "6502.h"

int main()
{
    CPU cpu;
    cpu.Initialize();

    //inline instruction - loop
    // load val in to X for num of iterations
    // call CPX and then BEQ
    // otherwise DEX


    std::cout << "Accumlator Register: " << (int)cpu.A << std::endl;
    std::cout << "X register: " << (int)cpu.X << std::endl;
    std::cout << "Y register: " << (int)cpu.Y << std::endl;
    std::cout << "Program Counter: " << std::format("{:#X}\n", (int)cpu.PC) << std::endl;

    std::cout << "Status: ";


    for (int i = 7; i > -1; i--) {
        int on = (cpu.status & (1 << (i))) > 0;
        std::cout << on;
    }
    std::cout << std::endl;

    return 0;
}
