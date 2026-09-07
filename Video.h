
struct Video {

    uint8_t vram[256]{}; // 1 page of memory

    uint8_t Read(uint16_t Address) {
        return vram[Address];
    }

    void Write(uint16_t Address, uint8_t Value) {
        vram[Address] = Value;
    }

};
