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
    uint16_t address;

    void Initialize() {
        PC = 0x8000;
        A = 0x00;
        X = 0x00;
        Y = 0x00;
        status = 0b00000000;
        N_FLAG = 0b10000000;
        Z_FLAG = 0b00000010;
        C_FLAG = 0b00000001;
        V_FLAG = 0b01000000;
        instruction = 0x00;
        cycles = 0x00;
        address = 0x0000;
    }

    void Clock() {
        if (cycles > 0) { // we are performing instruction
            Execute();
        }
        else {
            uint8_t opcode = Fetch(PC);
            cycles = Decode(opcode);
            instruction = opcode;
            PC++;
        }
        cycles--;
    }

    uint8_t Fetch(uint16_t Address) {
        uint8_t value = mem[Address];
        return value;
    }

    uint8_t Decode(uint8_t opcode) { // defined opcodes, return cycles
        switch (opcode) {
            case 0xA9: // LDA (Load into Accumulator register) - take immediate 1 byte after opcode and place into accumulator register
                return 2;
                break;
            case 0xA2: // LDX(Load into X register) - take immediate value and load into x register
                return 2;
                break;
            case 0xA0: // LDY (load immediate into Y register) - take immediate value and load into y register
                return 2;
                break;
            case 0x8D: // STA (Store Accumulator) - take value from accumulator and store into 16 bit address formed with next two bytes
                return 4;
                break;
            case 0x8E: // STX (Store X register) - take value from X register and store into 16 bit address formed with next two bytes
                return 4;
                break;
            case 0x8C: // STY (Store Y register) - take value from Y register and store into 16 bit address formed with next two bytes
                return 4;
                break;
            case 0x69: // ADC (Add with carry) - take immediate value, carry, and add into accumulator register
                return 2;
                break;
            case 0xE8: // INX (Increment X) - Add one to the X register
                return 2;
                break;
            case 0x4C: // JMP (Jump) - Update PC to address formed by next two bytes
                return 3;
                break;
            case 0xAA: // TAX (Transfer A to X) - Load the value in the accumualtor into the X register
                return 2;
                break;
            default:
                throw std::runtime_error("Instruction does not exist: " + std::format("{:#X}\n", (int)opcode));
                break;

        }
    }

    void Execute() {

        switch (instruction) {

            case 0xA9: // LDA (Load into Accumulator register) - take immediate 1 byte after opcode and place into accumulator register

                switch (cycles) {
                    case 1:
                        A = Fetch(PC);
                        SetZFLAG(A);
                        SetNFLAG(A);
                        PC++;
                        break;
                    default:
                        break;
                }
                break;


            case 0xA2: // LDX(Load into X register) - take immediate value and load into x register

                switch (cycles) {
                    case 1:
                        X = Fetch(PC);
                        SetZFLAG(X);
                        SetNFLAG(X);
                        PC++;
                        break;
                    default:
                        break;
                }
                break;


            case 0xA0: // LDY (load immediate into Y register) - take immediate value and load into y register

                switch (cycles) {
                    case 1:
                        Y = Fetch(PC);
                        SetZFLAG(Y);
                        SetNFLAG(Y);
                        PC++;
                        break;
                    default:
                        break;
                }
                break;


            case 0x8D: { // STA (Store Accumulator) - take value from accumulator and store into 16 bit address formed with next two bytes

                switch (cycles) {
                    case 3:
                        address = Fetch(PC); // low byte
                        PC++;
                        break;
                    case 2:
                        address |= Fetch(PC) << 8; // high byte
                        PC++;
                        break;
                    case 1:
                        Store(address, A);
                        break;
                    default:
                        break;
                }
                break;

            }

            case 0x8E: // STX (Store X register) - take value from X register and store into 16 bit address formed with next two bytes

                switch (cycles) {
                    case 3:
                        address = Fetch(PC);
                        PC++;
                        break;
                    case 2:
                        address |= (Fetch(PC) << 8);
                        PC++;
                        break;
                    case 1:
                        Store(address, X);
                        break;
                    default:
                        break;
                }
                break;

            case 0x8C: // STY (Store Y register) - take value from Y register and store into 16 bit address formed with next two bytes

                switch (cycles) {
                    case 3:
                        address = Fetch(PC);
                        PC++;
                        break;
                    case 2:
                        address |= (Fetch(PC) << 8);
                        PC++;
                        break;
                    case 1:
                        Store(address, Y);
                        break;
                    default:
                        break;
                }
                break;


            case 0x69: { // ADC (Add with carry) - take immediate value, carry, and add into accumulator register

                switch (cycles) {
                    case 1: {
                        uint8_t immediate = Fetch(PC);

                        uint8_t carry = (status & C_FLAG) > 0;

                        uint16_t operation_result = A + immediate + carry;
                        uint8_t A_prev = A;

                        A = operation_result & 0xFF;

                        SetNFLAG(A);
                        SetZFLAG(A);
                        SetCFLAG(operation_result);
                        SetVFLAG(immediate, A_prev, A);
                        PC++;
                        break;
                    }
                    default:
                        break;
                }
                break;

            }

            case 0xE8: // INX (Increment X) - Add one to the X register

                switch (cycles) {
                    case 1:
                        X += 1;
                        SetZFLAG(X);
                        SetNFLAG(X);
                        break;
                    default:
                        break;
                }
                break;

            case 0x4C: // JMP (Jump) - Update PC to address formed by next two bytes

                switch (cycles) {
                    case 2:
                        address = Fetch(PC);
                        PC++;
                        break;
                    case 1:
                        address |= (Fetch(PC) << 8);
                        PC = address;
                        break;
                    default:
                        break;
                }
                break;

            case 0xAA: // TAX (Transfer A to X) - Load the value in the accumualtor into the X register

                switch (cycles) {
                    case 1:
                        X = A;
                        break;
                    default:
                        break;
                }
                break;

            default:
                throw std::runtime_error("Instruction does not exist: " + std::format("{:#X}\n", (int)instruction));
                break;
        }
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
};

int main()
{
    CPU cpu;
    cpu.Initialize();

    // inline instruction - testing LDA
    cpu.mem[0x8000] = 0xA9;
    cpu.mem[0x8001] = 0x7F;
    cpu.Clock();
    cpu.Clock();

    // inline instruction - testing STA
    cpu.mem[0x8002] = 0x8D;
    cpu.mem[0x8003] = 0x00;
    cpu.mem[0x8004] = 0x90;
    cpu.Clock();
    cpu.Clock();
    cpu.Clock();
    cpu.Clock();

    // inline instruction - testing LDX
    cpu.mem[0x8005] = 0xA2;
    cpu.mem[0x8006] = 0x06;
    cpu.Clock();
    cpu.Clock();

    // inline instruction - testing ADC
    cpu.mem[0x8007] = 0x69;
    cpu.mem[0x8008] = 0x01;
    cpu.Clock();
    cpu.Clock();

    //inline instruction - testing LDY
    cpu.mem[0x8009] = 0xA0;
    cpu.mem[0x800A] = 0x80;
    cpu.Clock();
    cpu.Clock();

    //inline instruction - testing STX
    cpu.mem[0x800B] = 0x8E;
    cpu.mem[0x800C] = 0x01;
    cpu.mem[0x800D] = 0x70;
    cpu.Clock();
    cpu.Clock();
    cpu.Clock();
    cpu.Clock();

    // inline instruction - testing STY
    cpu.mem[0x800E] = 0x8C;
    cpu.mem[0x800F] = 0x02;
    cpu.mem[0x8010] = 0x70;
    cpu.Clock();
    cpu.Clock();
    cpu.Clock();
    cpu.Clock();

    //inline instruction - testing JMP
    cpu.mem[0x8011] = 0x4C;
    cpu.mem[0x8012] = 0x88;
    cpu.mem[0x8013] = 0x80;
    cpu.Clock();
    cpu.Clock();
    cpu.Clock();

    // inline instruction - testing TAX
    cpu.mem[0x8088] = 0xAA;
    cpu.Clock();
    cpu.Clock();

    // inline instruction - testing INX
    cpu.mem[0x8089] = 0xE8;
    cpu.Clock();
    cpu.Clock();

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
    std::cout << (int)cpu.mem[0x7002] << std::endl;

    return 0;
}
