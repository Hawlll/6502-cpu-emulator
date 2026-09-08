# 6502-cpu-emulator

This repo emulates the 6502, a 1975 microprocessor known for its simplicity. It uses 64KB of memory, has a 8-bit data width, has a 16-bit address width, and little endian multi-byte ordering. It has 56 instructions with multiple addressing modes for each instruction (I plan to implement them all). The goal eventually is to connect video graphics to this and draw some pixels!

<img width="835" height="440" alt="image" src="https://github.com/user-attachments/assets/f5d9703e-c264-4f10-a0a3-f090f668ca50" />

To create the diagonal in the image, the following program was executed by the emulated cpu

LDA 0x00  
LDX 0x00  
LDY 0x00  

// write to gpu mapped memory  
PHA  
TAX  
LDA 0x01  
STA 0x00 0xE0  
PLA  

CMP  
BEQ  

LDX 0x00  
JSR  
  INX  
  CPX 0x0F  
  BEQ  
  JMP  
RTS  
STX  
ADC  
STY  
ADC  
JMP  

Specifically, the STA instruction writes to the video mapped memory via the bus. What you see in the image is the VRAM represented as '.' and "#". "#" means the value at the address is 1.
