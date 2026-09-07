#pragma once
#include "Memory.h"

struct Bus {

    Memory& mem;

    Bus (Memory& memory) : mem(memory) {}

    uint8_t Read(uint16_t Address) {
        return mem.Read(Address);
    }

    void Write(uint16_t Address, uint8_t Value) {
        mem.Write(Address, Value);
    }


};
