#pragma once
#include <vector>
#include <cstdint>

struct Memory {
    std::vector<uint8_t> mem;
    uint8_t mem_size_kb;

    Memory (uint16_t KiB) : mem(1024 * KiB, 0), mem_size_kb(KiB) {}

    uint8_t Read(uint16_t Address) {
        return mem[Address];
    }

    void Write(uint16_t Address, uint8_t Value) {
        mem[Address] = Value;
    }
};
