// Copyright (C) 2004, Matt Conover (mconover@gmail.com)
#undef NDEBUG
#include <assert.h>
//#include <windows.h>
#include "disasm.h"
#include <string.h>

#ifdef NO_SANITY_CHECKS
#define NDEBUG
#undef assert
#define assert(x)
#endif

//////////////////////////////////////////////////////////////////////
// Global variables
//////////////////////////////////////////////////////////////////////

ARCHITECTURE_FORMAT SupportedArchitectures[] =
{
    { ARCH_X86, &X86 },
    { ARCH_X86_16, &X86 },
    { ARCH_X64, &X86 },
    { ARCH_UNKNOWN, NULL }
};

typedef struct _DISASM_ARG_INFO
{
    INSTRUCTION *MatchedInstruction;
    bool MatchPrefix;
    U8 *Opcode;
    U32 OpcodeLength;
    INSTRUCTION_TYPE InstructionType;
    U32 Count;
} DISASM_ARG_INFO;

//////////////////////////////////////////////////////////////////////
// Function prototypes
//////////////////////////////////////////////////////////////////////

bool InitInstruction(INSTRUCTION *Instruction, DISASSEMBLER *Disassembler);
static struct _ARCHITECTURE_FORMAT *GetArchitectureFormat(ARCHITECTURE_TYPE Type);

//////////////////////////////////////////////////////////////////////
// Disassembler setup
//////////////////////////////////////////////////////////////////////

bool InitDisassembler(DISASSEMBLER *Disassembler, ARCHITECTURE_TYPE Architecture)
{
    ARCHITECTURE_FORMAT *ArchFormat;

    memset(Disassembler, 0, sizeof(DISASSEMBLER));
    Disassembler->Initialized = DISASSEMBLER_INITIALIZED;

    ArchFormat = GetArchitectureFormat(Architecture);
    if (!ArchFormat) { assert(0); return false; }
    Disassembler->ArchType = ArchFormat->Type;
    Disassembler->Functions = ArchFormat->Functions;
    return true;
}

void CloseDisassembler(DISASSEMBLER *Disassembler)
{
    memset(Disassembler, 0, sizeof(DISASSEMBLER));
}

//////////////////////////////////////////////////////////////////////
// Instruction setup
//////////////////////////////////////////////////////////////////////

bool InitInstruction(INSTRUCTION *Instruction, DISASSEMBLER *Disassembler)
{
    memset(Instruction, 0, sizeof(INSTRUCTION));
    Instruction->Initialized = INSTRUCTION_INITIALIZED;
    Instruction->Disassembler = Disassembler;
    memset(Instruction->String, ' ', MAX_OPCODE_DESCRIPTION-1);
    Instruction->String[MAX_OPCODE_DESCRIPTION-1] = '\0';
    return true;
}

// If Decode = false, only the following fields are valid:
// Instruction->Length, Instruction->Address, Instruction->Prefixes, Instruction->PrefixCount,
// Instruction->OpcodeBytes, Instruction->Instruction->OpcodeLength, Instruction->Groups,
// Instruction->Type, Instruction->OperandCount
//
// If Disassemble = true, then Instruction->String is valid (also requires Decode = true)
//
// WARNING: This will overwrite the previously obtained instruction
INSTRUCTION *GetInstruction(DISASSEMBLER *Disassembler, U64 VirtualAddress, U8 *Address, U32 Flags)
{
    if (Disassembler->Initialized != DISASSEMBLER_INITIALIZED) { assert(0); return NULL; }
    assert(Address);
    InitInstruction(&Disassembler->Instruction, Disassembler);
    Disassembler->Instruction.Address = Address;
    Disassembler->Instruction.VirtualAddressDelta = VirtualAddress - (U64)Address;
    if (!Disassembler->Functions->GetInstruction(&Disassembler->Instruction, Address, Flags))
    {
        assert(Disassembler->Instruction.Address == Address);
        assert(Disassembler->Instruction.Length < MAX_INSTRUCTION_LENGTH);

        // Save the address that failed, in case the lower-level disassembler didn't
        Disassembler->Instruction.Address = Address;
        Disassembler->Instruction.ErrorOccurred = true;
        return NULL;
    }
    return &Disassembler->Instruction;
}

///////////////////////////////////////////////////////////////////////////
// Miscellaneous
///////////////////////////////////////////////////////////////////////////

static ARCHITECTURE_FORMAT *GetArchitectureFormat(ARCHITECTURE_TYPE Type)
{
    ARCHITECTURE_FORMAT *Format;
    for (Format = SupportedArchitectures; Format->Type != ARCH_UNKNOWN; Format++)
    {
        if (Format->Type == Type) return Format;
    }

    assert(0);
    return NULL;
}

