/*
To use the emulator as an assembler (the output file will have the same name is the input with ".machine" concatenated onto the end):

        /share/cs321/legv8emul <legv8 assembly file> -a

To run the emulator in binary emulation mode:
        /share/cs321/legv8emul <legv8 binary file> -b
*/
#include <iostream>
#include <fstream>
#include <cstdint>
#include <endian.h>
#include <string>

static int label_count = 0;

typedef struct instruction
{
    std::string mnemonic;
    uint8_t Rd;
    uint8_t Rn;
    uint8_t Rm;
    uint8_t Rt;
    uint8_t shamt;
    uint16_t offset;
    int32_t imm;
    int type;
} instruction_t;

instruction_t decode(uint32_t inst)
{
    uint32_t opcode_11 = (inst >> 21) & 0x7FF;
    uint32_t opcode_10 = (inst >> 22) & 0x3FF;
    uint32_t opcode_8 = (inst >> 24) & 0xFF;
    uint32_t opcode_6 = (inst >> 26) & 0x3F;
    instruction_t i = {};

    // R-type
    if (opcode_11 == 0b10001011000)
    {
        i.mnemonic = "ADD";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 0;
    }
    else if (opcode_11 == 0b10001010000)
    {
        i.mnemonic = "AND";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 0;
    }
    else if (opcode_11 == 0b10011011000) // MUL
    {
        i.mnemonic = "MUL";
        i.Rd = inst & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 0;
    }
    else if (opcode_11 == 0b11001010000)
    {
        i.mnemonic = "EOR";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 0;
    }
    else if (opcode_11 == 0b11010011011)
    {
        i.mnemonic = "LSL";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.type = 0;
    }
    else if (opcode_11 == 0b10101010000)
    {
        i.mnemonic = "ORR";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 0;
    }
    else if (opcode_11 == 0b11010011010)
    {
        i.mnemonic = "LSR";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.type = 0;
    }
    else if (opcode_11 == 0b11001011000)
    {
        i.mnemonic = "SUB";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 0;
    }
    else if (opcode_11 == 0b11101011000)
    {
        i.mnemonic = "SUBS";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 0;
    }
    else if (opcode_11 == 0b11111111101)
    {
        i.mnemonic = "PRNT";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 5;
    }
    else if (opcode_11 == 0b11111111100)
    {
        i.mnemonic = "PRNL";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 5;
    }
    else if (opcode_11 == 0b11111111110)
    {
        i.mnemonic = "DUMP";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 0;
    }
    else if (opcode_11 == 0b11111111111)
    {
        i.mnemonic = "HALT";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 5;
    }
    else if (opcode_11 == 0b11010110000)
    {
        i.mnemonic = "BR";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.shamt = (inst >> 10) & 0x3F;
        i.Rm = (inst >> 16) & 0x1F;
        i.type = 0;
    }
    // D-type
    else if (opcode_11 == 0b11111000010)
    {
        i.mnemonic = "LDUR";
        i.Rt = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        if (((inst >> 20) & 0x1) == 1)
        {
            int masked = (signed)(inst >> 12) & 0x1ff;
            masked = (signed)(masked ^ 0xFffffE00);
            i.imm = masked;
        }
        else
        {
            i.imm = (inst >> 12) & 0x1ff;
        }
        i.type = 1;
    }
    else if (opcode_11 == 0b11111000000)
    {
        i.mnemonic = "STUR";
        i.Rt = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        if (((inst >> 20) & 0x1) == 1)
        {
            int masked = (signed)(inst >> 12) & 0x1ff;
            masked = (signed)(masked ^ 0xFffffE00);
            i.imm = masked;
        }
        else
        {
            i.imm = (inst >> 12) & 0x1ff;
        }
        i.type = 1;
    }
    // I-type
    else if (opcode_10 == 0b1001000100)
    {
        i.mnemonic = "ADDI";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.imm = (inst >> 10) & 0x3FF;
        i.type = 2;
    }
    else if (opcode_10 == 0b1001001000)
    {
        i.mnemonic = "ANDI";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.imm = (inst >> 10) & 0x3FF;
        i.type = 2;
    }
    else if (opcode_10 == 0b1101001000)
    {
        i.mnemonic = "EORI";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.imm = (inst >> 10) & 0x3FF;
        i.type = 2;
    }
    else if (opcode_10 == 0b1011001000)
    {
        i.mnemonic = "ORRI";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.imm = (inst >> 10) & 0x3FF;
        i.type = 2;
    }
    else if (opcode_10 == 0b1101000100)
    {
        i.mnemonic = "SUBI";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.imm = (inst >> 10) & 0x3FF;
        i.type = 2;
    }
    else if (opcode_10 == 0b1111000100)
    {
        i.mnemonic = "SUBIS";
        i.Rd = (inst >> 0) & 0x1F;
        i.Rn = (inst >> 5) & 0x1F;
        i.imm = (inst >> 10) & 0x3FF;
        i.type = 2;
    }

    // B-type

    else if (opcode_6 == 0b000101)
    {
        i.mnemonic = "B";
        int masked = (signed)(inst & 0x3ffffff);
        masked = (signed)(masked ^ 0xFC000000);
        i.imm = masked;
        i.type = 3;
    }
    else if (opcode_6 == 0b100101)
    {
        i.mnemonic = "BL";
        int masked = (signed)(inst & 0x3ffffff);
        masked = (signed)(masked ^ 0xFC000000);
        i.imm = masked;
        i.type = 3;
    }
    // CB-type
    else if (opcode_8 == 0b10110100)
    {
        i.mnemonic = "CBZ";
        i.Rt = (inst >> 0) & 0x1F;
        i.imm = (inst >> 5) & 0x7FFFF;
        if (i.imm & 0x40000)
            i.imm |= 0xFFF80000;
        i.type = 4;
    }
    else if (opcode_8 == 0b01010100)
    {
        i.mnemonic = "B.";
        i.Rt = (inst >> 0) & 0x1F;
        i.imm = (inst >> 5) & 0x7FFFF;
        if (i.imm & 0x40000)
        {
            i.imm |= 0xFFF80000;
        }
        i.type = 4;
    }
    else if (opcode_8 == 0b10110101)
    {
        i.mnemonic = "CBNZ";
        i.Rt = (inst >> 0) & 0x1F;
        i.imm = (inst >> 5) & 0x7FFFF;
        if (i.imm & 0x40000)
        {
            i.imm |= 0xFFF80000;
        }
        i.type = 4;
    }
    return i;
}
std::string get_reg_name(uint8_t reg)
{
    if (reg == 28)
    {
        return "SP";
    }
    if (reg == 30)
    {
        return "LR";
    }
    if (reg == 31)
    {
        return "XZR";
    }
    return "X" + std::to_string(reg);
}

std::string bcond_convert(instruction_t i)
{
    switch (i.Rt)
    {
    case 0x0:
        return "EQ";
    case 0x1:
        return "NE";
    case 0x2:
        return "HS";
    case 0x3:
        return "LO";
    case 0x4:
        return "MI";
    case 0x5:
        return "PL";
    case 0x6:
        return "VS";
    case 0x7:
        return "VC";
    case 0x8:
        return "HI";
    case 0x9:
        return "LS";
    case 0xa:
        return "GE";
    case 0xb:
        return "LT";
    case 0xc:
        return "GT";
    case 0xd:
        return "LE";
    default:
        return "INVALID";
    }
}

void print_instruction(instruction_t inst)
{
    if (inst.type == 0) // R
    {
        if (inst.mnemonic == "LSL" || inst.mnemonic == "LSR")
        {
            std::cout << inst.mnemonic << " " << get_reg_name(inst.Rd) << ", " << get_reg_name(inst.Rn) << ", #" << (int)inst.shamt << std::endl;
        }
        else if (inst.mnemonic == "BR")
        {
            std::cout << inst.mnemonic << " " << get_reg_name(inst.Rn) << std::endl;
        }
        else if (inst.mnemonic == "PRNL" || inst.mnemonic == "DUMP")
        {
            std::cout << inst.mnemonic << std::endl;
        }
        else
        {
            std::cout << inst.mnemonic << " " << get_reg_name(inst.Rd) << ", " << get_reg_name(inst.Rn) << ", " << get_reg_name(inst.Rm) << std::endl;
        }
    }
    else if (inst.type == 1) // D
    {
        std::cout << inst.mnemonic << " " << get_reg_name(inst.Rt) << ", [" << get_reg_name(inst.Rn) << ", #" << (int)inst.imm << "]" << std::endl;
    }
    else if (inst.type == 2) // I
    {
        std::cout << inst.mnemonic << " " << get_reg_name(inst.Rd) << ", " << get_reg_name(inst.Rn) << ", #" << (int)inst.imm << std::endl;
    }
    else if (inst.type == 3) // B
    {
        std::cout << inst.mnemonic << " label" << ++label_count << std::endl;
    }
    else if (inst.type == 4) // CB
    {
        if (inst.mnemonic == "B.")
        {
            std::cout << inst.mnemonic << bcond_convert(inst) << ", label" << ++label_count << std::endl;
        }
        else
        {
            std::cout << inst.mnemonic << " " << get_reg_name(inst.Rt) << ", label" << ++label_count << std::endl;
        }
    }
    else if (inst.type == 5)
    {
        if (inst.mnemonic == "PRNT")
        {
            std::cout << inst.mnemonic << " " << get_reg_name(inst.Rd) << std::endl;
        }
        else
        {
            std::cout << inst.mnemonic << std::endl;
        }
    }
    else
    {
        std::cout << "error" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    std::ifstream infile(argv[1], std::ios::binary);
    if (!infile)
    {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    uint32_t raw;
    while (infile.read(reinterpret_cast<char *>(&raw), sizeof(raw)))
    {
        uint32_t inst = be32toh(raw);
        instruction_t decoded_inst = decode(inst);
        print_instruction(decoded_inst);
    }

    return 0;
}
