#pragma once
#include "Memory.h"
#include "Video.h"

struct Bus {

    Memory& mem;
    Video& video;

    Bus (Memory& memory, Video& vd) : mem(memory), video(vd) {}

    uint8_t Read(uint16_t Address) {

        if (Address >= 0xE000 && Address <= 0xE0FF) {
            return video.Read(Address - 0xE000);
        }

        return mem.Read(Address);
    }

    void Write(uint16_t Address, uint8_t Value) {

        if (Address >= 0xE000 && Address <= 0xE0FF) {
            video.Write(Address - 0xE000, Value);
        }
        else {
            mem.Write(Address, Value);
        }
    }


};
