// Copyright (C) 2003, Matt Conover (mconover@gmail.com)
#include "cpu.h"
#include <assert.h>

// NOTE: this assumes default scenarios (i.e., we assume CS/DS/ES/SS and flat
// and all have a base of 0 and limit of 0xffffffff, we don't try to verify
// that in the GDT)
//
// TODO: use inline assembly to get selector for segment
// Segment = x86 segment register (SEG_ES = 0, SEG_CS = 1, ...)
uint8_t *GetAbsoluteAddressFromSegment(uint8_t Segment, uint32_t Offset)
{
    switch (Segment)
    {
        // Windows uses a flat address space (except FS for x86 and GS for x64)
        case 0: // SEG_ES
        case 1: // SEG_CS
        case 2: // SEG_SS
        case 3: // SEG_DS
            return (uint8_t *)(size_t)Offset;
        case 4: // SEG_FS
        case 5: // SEG_GS
            return (uint8_t *)(size_t)Offset;
            // Note: we're really supposed to do this, but get_teb is not implemented
            // in this bastardized version of the disassembler.
            // return (uint8_t *)get_teb() + Offset;
        default:
            assert(0);
            return (uint8_t *)(size_t)Offset;
    }
}
