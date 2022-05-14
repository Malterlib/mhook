// Copyright (C) 2003, Matt Conover (mconover@gmail.com)
#ifndef CPU_H
#define CPU_H
#ifdef __cplusplus
extern "C" {
#endif
#pragma pack(push,1)

//#include <windows.h>
#include "misc.h"

////////////////////////////////////////////////////////
// System descriptors
////////////////////////////////////////////////////////

#define GDT_NULL 0
#define GDT_R0_CODE 0x08
#define GDT_R0_DATA 0x10
#define GDT_R3_CODE 0x18
#define GDT_R3_DATA 0x20
#define GDT_TSS 0x28
#define GDT_PCR 0x30
#define GDT_R3_TEB 0x38
#define GDT_VDM 0x40
#define GDT_LDT 0x48
#define GDT_DOUBLEFAULT_TSS 0x50
#define GDT_NMI_TSS 0x58

// 16-bit GDT entries:
// TODO: #define GDT_ABIOS_UNKNOWN   0x60  (22F30-32F2F)
#define GDT_ABIOS_VIDEO 0x68
#define GDT_ABIOS_GDT   0x70 // descriptor describing ABIOS GDT itself
#define GDT_ABIOS_NTOS  0x78 // first 64K of NTOSKRNL
#define GDT_ABIOS_CDA   0xE8 // common data area
#define GDT_ABIOS_CODE  0xF0 // KiI386AbiosCall
#define GDT_ABIOS_STACK 0xF8

#define SELECTOR_RPL_MASK 0x03 // bits 0-1
#define SELECTOR_LDT      0x04 // bit 2

// for data selectors
#define DATA_ACCESS_MASK       (1<<0)
#define DATA_WRITE_ENABLE_MASK (1<<1)
#define DATA_EXPAND_DOWN_MASK  (1<<2)

// for code selectors
#define CODE_ACCESS_MASK       (1<<0)
#define CODE_READ_MASK         (1<<1)
#define CODE_CONFORMING_MASK   (1<<2)
#define CODE_FLAG              (1<<3)

#define TASK_GATE      5
#define INTERRUPT_GATE 6
#define TRAP_GATE      7

typedef struct _IDT_ENTRY
{
   uint16_t LowOffset;
   uint16_t Selector;
   uint8_t Ignored : 5;
   uint8_t Zero : 3;
   uint8_t Type : 3;
   uint8_t Is32Bit : 1;
   uint8_t Ignored2 : 1;
   uint8_t DPL : 2;
   uint8_t Present : 1;
   uint16_t HighOffset;
#ifdef _WIN64
   uint32_t HighOffset64;
   uint32_t Reserved;
#endif
} IDT_ENTRY, TRAP_GATE_ENTRY;

typedef struct _CALL_GATE_ENTRY
{
   uint16_t LowOffset;
   uint16_t Selector;
   uint8_t ParameterCount: 4;
   uint8_t Ignored : 3;
   uint8_t Type : 5;
   uint8_t DPL : 2;
   uint8_t Present : 1;
   uint16_t HighOffset;
#ifdef _WIN64
   uint32_t HighOffset64;
   uint32_t Reserved;
#endif
} CALL_GATE_ENTRY;

typedef struct _TASK_GATE_ENTRY
{
   uint16_t Ignored;
   uint16_t Selector;
   uint8_t Ignored2 : 5;
   uint8_t Zero : 3;
   uint8_t Type : 5;
   uint8_t DPL : 2;
   uint8_t Present : 1;
   uint16_t Ignored3;
} TASK_GATE_ENTRY;

typedef struct _DESCRIPTOR_ENTRY
{
    uint16_t  LimitLow;
    uint16_t  BaseLow;
    uint8_t   BaseMid;
    uint8_t   Type : 4;        // 10EWA (code), E=ExpandDown, W=Writable, A=Accessed
                             // 11CRA (data), C=Conforming, R=Readable, A=Accessed
    uint8_t   System : 1;      // if 1 then it is a gate or LDT
    uint8_t   DPL : 2;         // descriptor privilege level; 
                             // for data selectors, MAX(CPL, RPL) must be <= DPL to access (or else GP# fault)
                             // for non-conforming code selectors (without callgate), MAX(CPL, RPL) must be <= DPL to access (or else GP# fault)
                             // for conforming code selectors, MAX(CPL, RPL) must be >= DPL (i.e., CPL 0-2 cannot access if DPL is 3)
                             // for non-conforming code selectors (with call gate), DPL indicates lowest privilege allowed to access gate
    uint8_t   Present : 1;
    uint8_t   LimitHigh : 4;
    uint8_t   Available: 1;    // aka AVL
    uint8_t   Reserved : 1;
    uint8_t   Is32Bit : 1;     // aka B flag
    uint8_t   Granularity : 1; // aka G flag
    uint8_t   BaseHi : 8;
#ifdef _WIN64
   uint32_t HighOffset64;
   uint32_t Reserved2;
#endif
} DESCRIPTOR_ENTRY;

typedef struct _GATE_ENTRY
{
   uint16_t LowOffset;
   uint8_t Skip;
   uint8_t Type : 5;
   uint8_t DPL : 2;
   uint8_t Present : 1;
   uint16_t HighOffset;
#ifdef _WIN64
   uint32_t HighOffset64;
   uint32_t Reserved;
#endif
} GATE_ENTRY;

// TODO: update for X64
typedef struct _PTE_ENTRY
{
    uint32_t Present : 1;
    uint32_t Write : 1;
    uint32_t Owner : 1; // E.g., user mode or supervisor mode
    uint32_t WriteThrough : 1;
    uint32_t CacheDisable : 1;
    uint32_t Accessed : 1;
    uint32_t Dirty : 1;
    uint32_t PAT : 1;
    uint32_t Global : 1;
    uint32_t CopyOnWrite : 1;
    uint32_t Prototype : 1;
    uint32_t Transition : 1;
    uint32_t Address : 20;
} PTE_ENTRY;

// TODO: update for X64
typedef struct _PDE_ENTRY
{
    uint32_t Present : 1;
    uint32_t Write : 1;
    uint32_t Owner : 1;
    uint32_t WriteThrough : 1;
    uint32_t CacheDisable : 1;
    uint32_t Accessed : 1;
    uint32_t Reserved1 : 1;
    uint32_t PageSize : 1;
    uint32_t Global : 1;
    uint32_t Reserved : 3;
    uint32_t Address : 20;
} PDE_ENTRY;

// TODO: update for X64
typedef struct _IO_ACCESS_MAP
{
    uint8_t DirectionMap[32];
    uint8_t IoMap[8196];
} IO_ACCESS_MAP;

#define MIN_TSS_SIZE FIELD_OFFSET(TSS_ENTRY, IoMaps)
// TODO: update for X64
typedef struct _TSS_ENTRY
{
    uint16_t  Backlink;
    uint16_t  Reserved0;
    uint32_t   Esp0;
    uint16_t  Ss0;
    uint16_t  Reserved1;
    uint32_t   NotUsed1[4];
    uint32_t   CR3;
    uint32_t   Eip;
    uint32_t   NotUsed2[9];
    uint16_t  Es;
    uint16_t  Reserved2;
    uint16_t  Cs;
    uint16_t  Reserved3;
    uint16_t  Ss;
    uint16_t  Reserved4;
    uint16_t  Ds;
    uint16_t  Reserved5;
    uint16_t  Fs;
    uint16_t  Reserved6;
    uint16_t  Gs;
    uint16_t  Reserved7;
    uint16_t  LDT;
    uint16_t  Reserved8;
    uint16_t  Flags;
    uint16_t  IoMapBase;
    IO_ACCESS_MAP IoMaps[1];
    uint8_t IntDirectionMap[32];
} TSS_ENTRY;

// TODO: update for X64
typedef struct _TSS16_ENTRY
{
    uint16_t  Backlink;
    uint16_t  Sp0;
    uint16_t  Ss0;
    uint16_t  Sp1;
    uint16_t  Ss1;
    uint16_t  Sp2;
    uint16_t  Ss3;
    uint16_t  Ip;
    uint16_t  Flags;
    uint16_t  Ax;
    uint16_t  Cx;
    uint16_t  Dx;
    uint16_t  Bx;
    uint16_t  Sp;
    uint16_t  Bp;
    uint16_t  Si;
    uint16_t  Di;
    uint16_t  Es;
    uint16_t  Cs;
    uint16_t  Ss;
    uint16_t  Ds;
    uint16_t  LDT;
} TSS16_ENTRY;

// TODO: update for X64
typedef struct _GDT_ENTRY
{
    uint16_t  LimitLow;
    uint16_t  BaseLow;
    union {
        struct {
            uint8_t   BaseMid;
            uint8_t   Flags1;
            uint8_t   Flags2;
            uint8_t   BaseHi;
        } Bytes;
        struct {
            uint32_t   BaseMid : 8;
            uint32_t   Type : 5;
            uint32_t   Dpl : 2;
            uint32_t   Pres : 1;
            uint32_t   LimitHi : 4;
            uint32_t   Sys : 1;
            uint32_t   Reserved_0 : 1;
            uint32_t   Default_Big : 1;
            uint32_t   Granularity : 1;
            uint32_t   BaseHi : 8;
        } Bits;
    } HighWord;
} GDT_ENTRY;

uint8_t *GetAbsoluteAddressFromSegment(uint8_t Segment, uint32_t Offset);

#pragma pack(pop)
#ifdef __cplusplus
}
#endif
#endif // CPU_H