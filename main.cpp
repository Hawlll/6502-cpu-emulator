#include <iostream>

struct CPU { // emulated after 6502. 8 bit data, 16 bit memory address space. little endian multi-byte ordering (low => High)
    uint8_t mem[65536];
    uint16_t PC;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t status;
    uint8_t N_FLAG; // negative flag
    uint8_t Z_FLAG; // zero flag
    uint8_t C_FLAG; // carry flag (did operation result in needing another bit)
    uint8_t V_FLAG; // overflow flag (did operation result in outside signed bit range)
    uint8_t cycles;
    uint8_t instruction;

    void Initialize() {
        PC = 0x8000;
        A = 0;
        X = 0;
        Y = 0;
        status = 0b00000000;
        N_FLAG = 0b10000000;
        Z_FLAG = 0b00000010;
        C_FLAG = 0b00000001;
        V_FLAG = 0b01000000;
        instruction = 0;
        cycles = 0;
    }

    void Clock() {
        if (cycles > 0) {
            cycles--;
        }
        else {
            throw std::runtime_error("Out of cycles for instruction: " + std::format("{:#X}\n", (int)instruction));
        }
    }

    uint8_t Fetch(uint16_t Address) {
        uint8_t value = mem[Address];
        return value;
    }

    void Store(uint16_t Address, uint8_t Value) {
        mem[Address] = Value;
    }

    void SetZFLAG(uint8_t reg) {
        if (reg == 0) {
            status |= Z_FLAG;
        }
        else {
            status &= ~Z_FLAG;
        }
    }


    void SetNFLAG(uint8_t reg) {
        if ((reg & (1 << 7)) > 0) {
            status |= N_FLAG;
        }
        else {
            status &= ~N_FLAG;
        }
    }

    void SetCFLAG(uint16_t operation_result) {
        if ((operation_result & 0x100) > 0) {
            status |= C_FLAG;
        }
        else {
            status &= ~C_FLAG;
        }
    }

    void SetVFLAG(uint8_t operand_1, uint8_t operand_2, uint8_t reg) {
        uint8_t operand_1_sign = (operand_1 & (1 << 7)) > 0;
        uint8_t operand_2_sign = (operand_2 & (1 << 7)) > 0;
        uint8_t reg_sign = (reg & (1 << 7)) > 0;

        if ((operand_1_sign == operand_2_sign) && (operand_1_sign != reg_sign)) {
            status |= V_FLAG;
        }
        else {
            status &= ~V_FLAG;
        }
    }

    void Execute(uint8_t cycls) {

        cycles = cycls;

        uint8_t opcode = Fetch(PC);
        instruction = opcode;

        PC++;
        Clock();

        switch (opcode) {
            case 0xA9: // LDA (Load into Accumulator register) - take immediate 1 byte after opcode and place into accumulator register
                A = Fetch(PC);
                PC++;
                Clock();

                SetZFLAG(A);
                SetNFLAG(A);
                break;
            case 0x8D: { // STA (Store Accumulator) - take value from accumulator and store into 16 bit address formed with next two bytes
                uint8_t low = Fetch(PC);
                PC++;
                Clock();

                uint8_t high = Fetch(PC);
                PC++;
                Clock();

                uint16_t address = (high << 8) | low;

                Store(address, A);
                Clock();

                break;
            }
            case 0xA2: // LDX(Load into X register) - take immediate value and load into x register
                X = Fetch(PC);
                PC++;
                Clock();

                SetZFLAG(X);
                SetNFLAG(X);
                break;
            case 0x69: { // ADC (Add with carry) - take immediate value, carry, and add into accumulator register
                uint8_t immediate = Fetch(PC);
                PC++;
                Clock();

                uint8_t carry = (status & C_FLAG) > 0;

                uint16_t operation_result = A + immediate + carry;
                uint8_t A_prev = A;

                A = operation_result & 0xFF;

                SetNFLAG(A);
                SetZFLAG(A);
                SetCFLAG(operation_result);
                SetVFLAG(immediate, A_prev, A);
                break;
            }

            default:
                break;

        }
    }
};


int main()
{
    CPU cpu;
    cpu.Initialize();

    // inline instruction - testing LDA
    cpu.mem[0x8000] = 0xA9;
    cpu.mem[0x8001] = 0x7F;
    cpu.Execute(2);

    // inline instruction - testing STA
    //cpu.mem[0x8002] = 0x8D;
    //cpu.mem[0x8003] = 0x00;
    //cpu.mem[0x8004] = 0x90;
    //cpu.Execute();

    // inline instruction - testing LDX
    //cpu.mem[0x8005] = 0xA2;
    //cpu.mem[0x8006] = 0x06;
    //cpu.Execute();

    // inline instruction - testing ADC
    cpu.mem[0x8002] = 0x69;
    cpu.mem[0x8003] = 0x01;
    cpu.Execute(2);

    uint8_t value = cpu.mem[0x9000];

    std::cout << "Accumlator Register: " << (int)cpu.A << std::endl;
    std::cout << "mem[0x9000] = " << (int)value << std::endl;
    std::cout << "X register: " << (int)cpu.X << std::endl;
    std::cout << "Program Counter: " << cpu.PC << std::endl;

    std::cout << "Status: ";


    for (int i = 7; i > -1; i--) {
        int on = (cpu.status & (1 << (i))) > 0;
        std::cout << on;
    }
    std::cout << std::endl;

    return 0;
}
