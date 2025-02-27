// Copyright (C) 2004, Matt Conover (mconover@gmail.com)
//
// The opcode tables in this file are based off the Intel Instruction Set Reference
// and an assortment of disassemblers, primarily libdisasm (by mammon)

#ifndef DISASM_X86_TABLES
#define DISASM_X86_TABLES

#define X86_GET_REG(val) ((val) & 7)
#define X86_GET_REG64(val) ((GET_REX_B(X86Instruction->rex_b) << 3) | ((val) & 7))

#define GET_MODRM_MOD(a) (((a) >> 6) & 3) // bits 6, 7
#define GET_MODRM_REG(a) (((a) >> 3) & 7) // bits 3, 4, 5
#define GET_MODRM_EXT(a) (((a) >> 3) & 7) // bits 3, 4, 5
#define GET_MODRM_RM(a)     ((a) & 7) // bits 0, 1, 2

#define GET_SIB_SCALE(a) (((a) >> 6) & 3) // bits 6, 7
#define GET_SIB_INDEX(a) (((a) >> 3) & 7) // bits 3, 4, 5
#define GET_SIB_BASE(a)  ((a) & 7) // bits 0, 1, 2

#define REX_PREFIX_START 0x40
#define REX_PREFIX_END 0x4F
#define GET_REX_W(r) (((r) & 8) >> 3) // bit 3
#define GET_REX_R(r) (((r) & 4) >> 2) // bit 2
#define GET_REX_X(r) (((r) & 2) >> 1) // bit 1
#define GET_REX_B(r) ((r) & 1)        // bit 0
#define REX_MASK(n) ((n >> 16) & 0x0F) // bits 0-3

// Groupings to make the opcode table more readible
#define NOARGS { 0, 0, 0 }
#define NOCOND 0
#define NOGROUP NULL
#define NOACTION 0
#define NOCHANGE 0
#define IGNORED 0
#define NOINSTR NOGROUP, CPU_UNKNOWN, 0, "", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED
#define GROUP CPU_UNKNOWN, 0, "", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED
#define PREFIX NOGROUP, CPU_UNKNOWN, ITYPE_EXT_PREFIX, "", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED
#define EXT_FPU CPU_UNKNOWN, ITYPE_EXT_FPU, "", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED 
#define EXT_64 CPU_UNKNOWN, ITYPE_EXT_64, "", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED 
#define EXT_SUFFIX(a, b, c) CPU_UNKNOWN, ITYPE_EXT_SUFFIX, "", { a, b, c }, NOCOND, NOCHANGE, NOACTION, IGNORED
#define EXT_MODRM CPU_UNKNOWN, ITYPE_EXT_MODRM, "", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED

#define SET_MODRM(modrm, src) \
{ \
    (modrm).mod = GET_MODRM_MOD(src); \
    (modrm).reg = GET_MODRM_REG(src); \
    (modrm).rm = GET_MODRM_RM(src); \
}

#define SET_REX_MODRM(rex_modrm, rex, modrm) \
{ \
    (rex_modrm).rm = ((rex).b << 3) | (modrm).rm; \
    (rex_modrm).reg = ((rex).r << 3) | (modrm).reg; \
}

#define SET_SIB(sib, src) \
{ \
    (sib).scale = GET_SIB_SCALE(src); \
    (sib).index = GET_SIB_INDEX(src); \
    (sib).base =  GET_SIB_BASE(src); \
}

#define SET_REX_SIB(rex_sib, rex, sib) \
{ \
    (rex_sib).index = ((rex).x << 3) | (sib).index; \
    (rex_sib).base = ((rex).b << 3) | (sib).base; \
}

#define SET_REX(rex, src) \
{ \
    (rex).w = GET_REX_W(src);    \
    (rex).r = GET_REX_R(src);    \
    (rex).x = GET_REX_X(src);    \
    (rex).b = GET_REX_B(src); \
}

// Addressing modes
#define AMODE_A    0x00010000   
#define AMODE_C    0x00020000
#define AMODE_D    0x00030000
#define AMODE_E    0x00040000
#define AMODE_G    0x00050000
#define AMODE_I    0x00060000
#define AMODE_J    0x00070000
#define AMODE_M    0x00080000
#define AMODE_O    0x00090000
#define AMODE_P    0x000A0000
#define AMODE_Q    0x000B0000
#define AMODE_R    0x000C0000
#define AMODE_S    0x000D0000
#define AMODE_T    0x000E0000
#define AMODE_V    0x000F0000
#define AMODE_W    0x00100000
#define AMODE_X    0x00110000
#define AMODE_Y    0x00120000
#define AMODE_PR   0x00130000
#define AMODE_VR   0x00140000
#define AMODE_xlat 0x00150000

// Operand types
#define OPTYPE_a    0x01000000
#define OPTYPE_b    0x02000000
#define OPTYPE_d    0x03000000
#define OPTYPE_p    0x04000000
#define OPTYPE_q    0x05000000
#define OPTYPE_dt   0x06000000
#define OPTYPE_v    0x07000000
#define OPTYPE_w    0x08000000
#define OPTYPE_ps   0x09000000 // packed 128-bit single real
#define OPTYPE_pd   0x0A000000 // packed 128-bit double real
#define OPTYPE_pb   0x0B000000 // packed BCD (10 bytes, 18-bit precision)
#define OPTYPE_ss   0x0C000000 // scalar single real
#define OPTYPE_sd   0x0D000000 // scalar double real
#define OPTYPE_se   0x0E000000 // scalar extended real
#define OPTYPE_fev  0x0F000000 // FPU environment (28 bytes if 32-bit modes, 14 bytes in 16-bit mode)
#define OPTYPE_fst1 0x10000000 // FPU state (108 bytes in 32-bit modes, 94 bytes in 16-bit real mode)
#define OPTYPE_fst2 0x11000000 // FPU/MMX/XMM/MXCSR state (512 bytes)
#define OPTYPE_z    0x12000000
#define OPTYPE_o    0x13000000
#define OPTYPE_dq   0x14000000 // OPTYPE_d or OPTYPE_o
#define OPTYPE_mw   0x15000000 // word if memory, register size otherwise
#define OPTYPE_sso  0x16000000 // OPTYPE_ss or OPTYPE_o
#define OPTYPE_sdo  0x17000000 // OPTYPE_ss or OPTYPE_o
#define OPTYPE_cpu  0x18000000 // pointer to CPU state structure
#define OPTYPE_lea  0x19000000 // size set by other operand
// NOTE: if you change this, you must also update OptypeHandlers[] in disasm_x86.c
// Be sure to preserve the ordering

//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// Registers
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/*static char *Addressing16[8] = {"bx+si","bx+di","bp+si","bp+di","si","di","bp","bx"};
static char *MMX_Registers[8] = {"mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7"};
static char *SSE_Registers[8] = {"xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7"};
static char *DR_Registers[8] = {"dr0", "dr1", "dr2", "dr3", "dr4", "dr5", "dr6", "dr7"};
static char *CR_Registers[8] = {"cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7"};
static char *TR_Registers[8] = {"tr0", "tr1", "tr2", "tr3", "tr4", "tr5", "tr6", "tr7"};
static char *FPU_Registers[8] = {"st(0)", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)"};*/
static char *Segments[8] = {"es", "cs", "ss", "ds", "fs", "gs", "ERROR", "ERROR"};
/*static char *Registers8[8] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" };
static char *Registers16[8] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di" };
static char *Registers32[8] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };
static char *REX_Registers8[16] = {"al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b" };
static char *REX_Registers16[16] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w" };
static char *REX_Registers32[16] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d" };
static char *REX_Registers64[16] = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15" };*/
static char *DataSizes[8+1] = {"byte ptr", "word ptr", "dword ptr", "6_byte ptr", "qword ptr", "10_byte ptr", "INVALID PTR", "INVALID PTR", "oword ptr"};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// FPU constants
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

uint8_t float_0[10]   = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t float_1[10] =   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x3F };
uint8_t float_l2t[10] = { 0xFE, 0x8A, 0x1B, 0xCD, 0x4B, 0x78, 0x9A, 0xD4, 0x00, 0x40 };
uint8_t float_l2e[10] = { 0xBC, 0xF0, 0x17, 0x5C, 0x29, 0x3B, 0xAA, 0xB8, 0xFF, 0x3F };
uint8_t float_pi[10]  = { 0x35, 0xC2, 0x68, 0x21, 0xA2, 0xDA, 0x0F, 0xC9, 0x00, 0x40 };
uint8_t float_lg2[10] = { 0x99, 0xF7, 0xCF, 0xFB, 0x84, 0x9A, 0x20, 0x9A, 0xFD, 0x3F };
uint8_t float_ln2[10] = { 0xAC, 0x79, 0xCF, 0xD1, 0xF7, 0x17, 0x72, 0xB1, 0xFE, 0x3F };

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// Tables
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

extern X86_OPCODE X86_Opcodes_2[0x100];
extern X86_OPCODE X86_Group_1_80[8], X86_Group_1_81[8], X86_Group_1_82[8], X86_Group_1_83[8], X86_Group_2_C0[8], X86_Group_2_C1[8], X86_Group_2_D0[8], X86_Group_2_D1[8], X86_Group_2_D2[8], X86_Group_2_D3[8], X86_Group_3_F6[8], X86_Group_3_F7[8], X86_Group_4[8], X86_Group_5[8], X86_Group_6[8], X86_Group_7[8], X86_Group_8[8], X86_Group_9[8], X86_Group_10[8], X86_Group_11[8], X86_Group_12_C6[8], X86_Group_12_C7[8], X86_Group_13[8], X86_Group_14[8], X86_Group_15[8], X86_Group_16[8], X86_Group_17[8], X86_Group_P[8];
extern X86_OPCODE X86_SSE[0x300], X86_SSE2_Group_13[24], X86_SSE2_Group_14[24], X86_SSE2_Group_15[24];
extern X86_OPCODE X86_ESC_0[0x48], X86_ESC_1[0x48], X86_ESC_2[0x48], X86_ESC_3[0x48], X86_ESC_3[0x48], X86_ESC_4[0x48], X86_ESC_5[0x48], X86_ESC_6[0x48], X86_ESC_7[0x48];
extern X86_OPCODE X86_3DNOW_0F[0x100];
extern X86_OPCODE X86_0F01_ModRM[0x100];
extern X86_OPCODE X86_Opcode_63[2], X86_Opcode_0F05[2];

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// Opcode tables
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

X86_OPCODE X86_Opcodes_1[0x100] = // 1 byte opcodes
{
    { NOGROUP, CPU_I386, ITYPE_ADD, "add", { AMODE_E | OPTYPE_b | OP_DST, AMODE_G | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "add", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "add", { AMODE_G | OPTYPE_b | OP_DST, AMODE_E | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "add", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "add", { OPTYPE_REG_AL | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "add", { OPTYPE_REG_xAX_BIG | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OPTYPE_ES | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OPTYPE_ES | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x07 */
    { NOGROUP, CPU_I386, ITYPE_OR, "or", { AMODE_E | OPTYPE_b | OP_DST, AMODE_G | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x08 */
    { NOGROUP, CPU_I386, ITYPE_OR, "or", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x09 */
    { NOGROUP, CPU_I386, ITYPE_OR, "or", { AMODE_G | OPTYPE_b | OP_DST, AMODE_E | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x0A */
    { NOGROUP, CPU_I386, ITYPE_OR, "or", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x0B */
    { NOGROUP, CPU_I386, ITYPE_OR, "or", { OPTYPE_REG_AL | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x0C */
    { NOGROUP, CPU_I386, ITYPE_OR, "or", { OPTYPE_REG_xAX_BIG | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x0D */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OPTYPE_CS | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x0E */
    { X86_Opcodes_2, GROUP }, /* 0x0F */
    { NOGROUP, CPU_I386, ITYPE_ADD, "adc", { AMODE_E | OPTYPE_b | OP_DST, AMODE_G | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x10 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "adc", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x11 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "adc", { AMODE_G | OPTYPE_b | OP_DST, AMODE_E | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x12 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "adc", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x13 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "adc", { OPTYPE_REG_AL | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x14 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "adc", { OPTYPE_REG_xAX_BIG | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x15 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OPTYPE_SS | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x16 */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OPTYPE_SS | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x17 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sbb", { AMODE_E | OPTYPE_b | OP_DST, AMODE_G | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x18 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sbb", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x19 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sbb", { AMODE_G | OPTYPE_b | OP_DST, AMODE_E | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x1A */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sbb", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x1B */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sbb", { OPTYPE_REG_AL | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x1C */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sbb", { OPTYPE_REG_xAX_BIG | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x1D */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OPTYPE_DS | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x1E */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OPTYPE_DS | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x1F */
    { NOGROUP, CPU_I386, ITYPE_AND, "and", { AMODE_E | OPTYPE_b | OP_DST, AMODE_G | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x20 */
    { NOGROUP, CPU_I386, ITYPE_AND, "and", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x21 */
    { NOGROUP, CPU_I386, ITYPE_AND, "and", { AMODE_G | OPTYPE_b | OP_DST, AMODE_E | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x22 */
    { NOGROUP, CPU_I386, ITYPE_AND, "and", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x23 */
    { NOGROUP, CPU_I386, ITYPE_AND, "and", { OPTYPE_REG_AL | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x24 */
    { NOGROUP, CPU_I386, ITYPE_AND, "and", { OPTYPE_REG_xAX_BIG | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x25 */
    { PREFIX }, /* 0x26 */
    { NOGROUP, CPU_I386, ITYPE_BCDCONV, "daa", { OPTYPE_REG_AL | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_AF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x27 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sub", { AMODE_E | OPTYPE_b | OP_DST, AMODE_G | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x28 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sub", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x29 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sub", { AMODE_G | OPTYPE_b | OP_DST, AMODE_E | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x2A */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sub", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x2B */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sub", { OPTYPE_REG_AL | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x2C */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sub", { OPTYPE_REG_xAX_BIG | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x2D */
    { PREFIX }, /* 0x2E */
    { NOGROUP, CPU_I386, ITYPE_BCDCONV, "das", { OPTYPE_REG_AL | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_AF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x2F */
    { NOGROUP, CPU_I386, ITYPE_XOR, "xor", { AMODE_E | OPTYPE_b | OP_DST, AMODE_G | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x30 */
    { NOGROUP, CPU_I386, ITYPE_XOR, "xor", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x31 */
    { NOGROUP, CPU_I386, ITYPE_XOR, "xor", { AMODE_G | OPTYPE_b | OP_DST, AMODE_E | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x32 */
    { NOGROUP, CPU_I386, ITYPE_XOR, "xor", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x33 */
    { NOGROUP, CPU_I386, ITYPE_XOR, "xor", { OPTYPE_REG_AL | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x34 */
    { NOGROUP, CPU_I386, ITYPE_XOR, "xor", { OPTYPE_REG_xAX_BIG | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x35 */
    { PREFIX }, /* 0x36 */
    { NOGROUP, CPU_I386, ITYPE_BCDCONV, "aaa", { OPTYPE_REG_AL | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_AF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x37 */
    { NOGROUP, CPU_I386, ITYPE_CMP, "cmp", { AMODE_E | OPTYPE_b | OP_SRC, AMODE_G | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x38 */
    { NOGROUP, CPU_I386, ITYPE_CMP, "cmp", { AMODE_E | OPTYPE_v | OP_SRC, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x39 */
    { NOGROUP, CPU_I386, ITYPE_CMP, "cmp", { AMODE_G | OPTYPE_b | OP_SRC, AMODE_E | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x3A */
    { NOGROUP, CPU_I386, ITYPE_CMP, "cmp", { AMODE_G | OPTYPE_v | OP_SRC, AMODE_E | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x3B */
    { NOGROUP, CPU_I386, ITYPE_CMP, "cmp", { OPTYPE_REG_AL | OP_SIGNED | OP_SRC, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x3C */
    { NOGROUP, CPU_I386, ITYPE_CMP, "cmp", { OPTYPE_REG_xAX_BIG | OP_SIGNED | OP_SRC, AMODE_I | OPTYPE_z | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x3D */
    { PREFIX }, /* 0x3E */
    { NOGROUP, CPU_I386, ITYPE_BCDCONV, "aas", { OPTYPE_REG_AL | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_AF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x3F */
    { NOGROUP, CPU_I386, ITYPE_INC, "inc", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x40 */
    { NOGROUP, CPU_I386, ITYPE_INC, "inc", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x41 */
    { NOGROUP, CPU_I386, ITYPE_INC, "inc", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x42 */
    { NOGROUP, CPU_I386, ITYPE_INC, "inc", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x43 */
    { NOGROUP, CPU_I386, ITYPE_INC, "inc", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x44 */
    { NOGROUP, CPU_I386, ITYPE_INC, "inc", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x45 */
    { NOGROUP, CPU_I386, ITYPE_INC, "inc", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x46 */
    { NOGROUP, CPU_I386, ITYPE_INC, "inc", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x47 */
    { NOGROUP, CPU_I386, ITYPE_DEC, "dec", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x48 */
    { NOGROUP, CPU_I386, ITYPE_DEC, "dec", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x49 */
    { NOGROUP, CPU_I386, ITYPE_DEC, "dec", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x4A */
    { NOGROUP, CPU_I386, ITYPE_DEC, "dec", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x4B */
    { NOGROUP, CPU_I386, ITYPE_DEC, "dec", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x4C */
    { NOGROUP, CPU_I386, ITYPE_DEC, "dec", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x4D */
    { NOGROUP, CPU_I386, ITYPE_DEC, "dec", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x4E */
    { NOGROUP, CPU_I386, ITYPE_DEC, "dec", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x4F */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OP_REG | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x50 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OP_REG | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x51 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OP_REG | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x52 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OP_REG | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x53 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OP_REG | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x54 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OP_REG | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x55 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OP_REG | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x56 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OP_REG | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x57 */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OP_REG | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x58 */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OP_REG | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x59 */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OP_REG | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5A */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OP_REG | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5B */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OP_REG | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5C */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OP_REG | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5D */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OP_REG | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5E */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OP_REG | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5F */
    { NOGROUP, CPU_I386, ITYPE_PUSHA, "pushad", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x60 */
    { NOGROUP, CPU_I386, ITYPE_POPA, "popad", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x61 */
    { NOGROUP, CPU_I386, ITYPE_BOUNDS, "bound", { AMODE_G | OPTYPE_v | OP_SRC, AMODE_M | OPTYPE_a | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x62 */
    { X86_Opcode_63, EXT_64 }, /* 0x63 */
    { PREFIX }, /* 0x64 */
    { PREFIX }, /* 0x65 */
    { PREFIX }, /* 0x66 */
    { PREFIX }, /* 0x67 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { AMODE_I | OPTYPE_z | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x68 */
    { NOGROUP, CPU_I386, ITYPE_MUL, "imul", { AMODE_G | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_E | OPTYPE_v | OP_SIGNED | OP_SRC, AMODE_I | OPTYPE_z | OP_SIGNED | OP_SRC }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x69 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x6A */
    { NOGROUP, CPU_I386, ITYPE_MUL, "imul", { AMODE_G | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_E | OPTYPE_v | OP_SIGNED | OP_SRC, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x6B */
    { NOGROUP, CPU_I386, ITYPE_IN, "insb", { AMODE_Y | OPTYPE_b | OP_DST, OPTYPE_REG_DX | OP_SYS | OP_SRC, 0 }, COND_D, NOCHANGE, xDI_DEC | xCX_REP_DEC, xDI_INC | xCX_REP_DEC }, /* 0x6C */
    { NOGROUP, CPU_I386, ITYPE_IN, "insd", { AMODE_Y | OPTYPE_z | OP_DST, OPTYPE_REG_DX | OP_SYS | OP_SRC, 0 }, COND_D, NOCHANGE, xDI_DECx | xCX_REP_DEC, xDI_INCx | xCX_REP_DEC }, /* 0x6D */
    { NOGROUP, CPU_I386, ITYPE_OUT, "outsb", { OPTYPE_REG_DX | OP_SYS | OP_DST, AMODE_X | OPTYPE_b | OP_SRC, 0 }, COND_D, NOCHANGE, xSI_DEC | xCX_REP_DEC, xSI_INC  | xCX_REP_DEC }, /* 0x6E */
    { NOGROUP, CPU_I386, ITYPE_OUT, "outsd", { OPTYPE_REG_DX | OP_SYS | OP_DST, AMODE_X | OPTYPE_z | OP_SRC, 0 }, COND_D, NOCHANGE, xSI_DECx | xCX_REP_DEC, xSI_INCx  | xCX_REP_DEC}, /* 0x6F */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jo", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_O, NOCHANGE, OP1_DST, NOACTION }, /* 0x70 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jno", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_NO, NOCHANGE, OP1_DST, NOACTION }, /* 0x71 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jb", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_C, NOCHANGE, OP1_DST, NOACTION }, /* 0x72 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jnb", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_NC, NOCHANGE, OP1_DST, NOACTION }, /* 0x73 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jz", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_Z, NOCHANGE, OP1_DST, NOACTION }, /* 0x74 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jnz", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_NZ, NOCHANGE, OP1_DST, NOACTION }, /* 0x75 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jbe", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_BE, NOCHANGE, OP1_DST, NOACTION }, /* 0x76 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "ja", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_A, NOCHANGE, OP1_DST, NOACTION }, /* 0x77 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "js", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_S, NOCHANGE, OP1_DST, NOACTION }, /* 0x78 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jns", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_NS, NOCHANGE, OP1_DST, NOACTION }, /* 0x79 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jpe", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_PE, NOCHANGE, OP1_DST, NOACTION }, /* 0x7A */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jpo", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_PO, NOCHANGE, OP1_DST, NOACTION }, /* 0x7B */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jl", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_L, NOCHANGE, OP1_DST, NOACTION }, /* 0x7C */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jge", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_GE, NOCHANGE, OP1_DST, NOACTION }, /* 0x7D */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jle", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_LE, NOCHANGE, OP1_DST, NOACTION }, /* 0x7E */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jg", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_G, NOCHANGE, OP1_DST, NOACTION }, /* 0x7F */
    { X86_Group_1_80, GROUP }, /* 0x80 Eb Ib */
    { X86_Group_1_81, GROUP }, /* 0x81 Ev Iz */
    { X86_Group_1_82, GROUP }, /* 0x82 Eb Ib */
    { X86_Group_1_83, GROUP }, /* 0x83 Ev Ib */
    { NOGROUP, CPU_I386, ITYPE_TEST, "test", { AMODE_E | OPTYPE_b | OP_SRC, AMODE_G | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x84 */
    { NOGROUP, CPU_I386, ITYPE_TEST, "test", { AMODE_E | OPTYPE_v | OP_SRC, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x85 */
    { NOGROUP, CPU_I386, ITYPE_XCHG, "xchg", { AMODE_E | OPTYPE_b | OP_SRC | OP_DST, AMODE_G | OPTYPE_b | OP_SRC | OP_DST, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x86 */
    { NOGROUP, CPU_I386, ITYPE_XCHG, "xchg", { AMODE_E | OPTYPE_v | OP_SRC | OP_DST, AMODE_G | OPTYPE_v | OP_SRC | OP_DST, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x87 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_E | OPTYPE_b | OP_DST, AMODE_G | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x88 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x89 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_G | OPTYPE_b | OP_DST, AMODE_E | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x8A */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_v | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x8B */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_E | OPTYPE_mw | OP_DST, AMODE_S | OPTYPE_w | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x8C */
    { NOGROUP, CPU_I386, ITYPE_LEA, "lea", { AMODE_G | OPTYPE_v | OP_DST, AMODE_M | OPTYPE_lea | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x8D */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_S | OPTYPE_w | OP_DST, AMODE_E | OPTYPE_w | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x8E */
    { X86_Group_10, GROUP }, /* 0x8F */
    { NOGROUP, CPU_I386, ITYPE_NOP, "nop", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x90 */
    { NOGROUP, CPU_I386, ITYPE_XCHG, "xchg", { OPTYPE_REG_xAX_BIG | OP_SRC | OP_DST, OP_REG | OP_SRC | OP_DST, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x91 */
    { NOGROUP, CPU_I386, ITYPE_XCHG, "xchg", { OPTYPE_REG_xAX_BIG | OP_SRC | OP_DST, OP_REG | OP_SRC | OP_DST, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x92 */
    { NOGROUP, CPU_I386, ITYPE_XCHG, "xchg", { OPTYPE_REG_xAX_BIG | OP_SRC | OP_DST, OP_REG | OP_SRC | OP_DST, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x93 */
    { NOGROUP, CPU_I386, ITYPE_XCHG, "xchg", { OPTYPE_REG_xAX_BIG | OP_SRC | OP_DST, OP_REG | OP_SRC | OP_DST, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x94 */
    { NOGROUP, CPU_I386, ITYPE_XCHG, "xchg", { OPTYPE_REG_xAX_BIG | OP_SRC | OP_DST, OP_REG | OP_SRC | OP_DST, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x95 */
    { NOGROUP, CPU_I386, ITYPE_XCHG, "xchg", { OPTYPE_REG_xAX_BIG | OP_SRC | OP_DST, OP_REG | OP_SRC | OP_DST, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x96 */
    { NOGROUP, CPU_I386, ITYPE_XCHG, "xchg", { OPTYPE_REG_xAX_BIG | OP_SRC | OP_DST, OP_REG | OP_SRC | OP_DST, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x97 */
    { NOGROUP, CPU_I386, ITYPE_SZCONV, "cwde", { OPTYPE_REG_xAX_BIG | OP_SIGNED | OP_DST, OPTYPE_REG_xAX_SMALL | OP_SIGNED | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x98 */
    { NOGROUP, CPU_I386, ITYPE_SZCONV, "cdq", { OPTYPE_xDX_HI_xAX_LO | OP_SIGNED | OP_DST, OPTYPE_REG_xAX_BIG | OP_SIGNED | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x99 */
    { NOGROUP, CPU_I386, ITYPE_CALL, "call", { AMODE_A | OPTYPE_p | OP_SRC | OP_EXEC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x9A */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "wait", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x9B */
    { NOGROUP, CPU_I386, ITYPE_PUSHF, "pushf", { OPTYPE_xFLAGS | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x9C */
    { NOGROUP, CPU_I386, ITYPE_POPF, "popf", { OPTYPE_xFLAGS | OP_DST, 0, 0 }, NOCOND, FLAG_ALL_MOD, NOACTION, IGNORED }, /* 0x9D */
    { NOGROUP, CPU_I386, ITYPE_MOV, "sahf", { OPTYPE_FLAGS | OP_DST, OPTYPE_REG_AH | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_AF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x9E */
    { NOGROUP, CPU_I386, ITYPE_MOV, "lahf", { OPTYPE_REG_AH | OP_DST, OPTYPE_FLAGS | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x9F */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OPTYPE_REG_AL | OP_DST, AMODE_O | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xA0 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OPTYPE_REG_xAX_BIG | OP_DST, AMODE_O | OPTYPE_v | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xA1 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_O | OPTYPE_b | OP_DST, OPTYPE_REG_AL | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xA2 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_O | OPTYPE_v | OP_DST, OPTYPE_REG_xAX_BIG | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xA3 */
    { NOGROUP, CPU_I386, ITYPE_STRMOV, "movsb", { AMODE_Y | OPTYPE_b | OP_DST, AMODE_X | OPTYPE_b | OP_SRC, 0 }, COND_D, NOCHANGE, xDI_DEC | xSI_DEC | xCX_REP_DEC, xDI_INC | xSI_INC | xCX_REP_DEC}, /* 0xA4 */
    { NOGROUP, CPU_I386, ITYPE_STRMOV, "movsd", { AMODE_Y | OPTYPE_z | OP_DST, AMODE_X | OPTYPE_z | OP_SRC, 0 }, COND_D, NOCHANGE, xDI_DECx | xSI_DECx| xCX_REP_DEC, xDI_INCx | xSI_INCx | xCX_REP_DEC }, /* 0xA5 */
    { NOGROUP, CPU_I386, ITYPE_STRCMP, "cmpsb", { AMODE_X | OPTYPE_b | OP_SRC, AMODE_Y | OPTYPE_b | OP_SRC, 0 }, COND_D, FLAG_COMMON_MOD, xDI_DEC | xSI_DEC | xCX_REP_DEC, xDI_INC | xSI_INC | xCX_REP_DEC }, /* 0xA6 */
    { NOGROUP, CPU_I386, ITYPE_STRCMP, "cmpsd", { AMODE_X | OPTYPE_z | OP_SRC, AMODE_Y | OPTYPE_z | OP_SRC, 0 }, COND_D, FLAG_COMMON_MOD, xDI_DECx | xSI_DECx | xCX_REP_DEC, xDI_INCx | xSI_INCx | xCX_REP_DEC}, /* 0xA7 */
    { NOGROUP, CPU_I386, ITYPE_TEST, "test", { OPTYPE_REG_AL | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0xA8 */
    { NOGROUP, CPU_I386, ITYPE_TEST, "test", { OPTYPE_REG_xAX_BIG | OP_SRC, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0xA9 */
    { NOGROUP, CPU_I386, ITYPE_STRSTOR, "stosb", { AMODE_Y | OPTYPE_b | OP_DST, OPTYPE_REG_AL | OP_SRC, 0 }, COND_D, NOCHANGE, xDI_DEC | xSI_DEC | xCX_REP_DEC, xDI_INC | xSI_INC | xCX_REP_DEC }, /* 0xAA */
    { NOGROUP, CPU_I386, ITYPE_STRSTOR, "stosd", { AMODE_Y | OPTYPE_z | OP_DST, OPTYPE_REG_xAX_BIG | OP_SRC, 0 }, COND_D, NOCHANGE, xDI_DECx | xSI_DECx | xCX_REP_DEC, xDI_INCx | xSI_INCx | xCX_REP_DEC }, /* 0xAB */
    { NOGROUP, CPU_I386, ITYPE_STRLOAD, "lodsb", { OPTYPE_REG_AL | OP_DST, AMODE_X | OPTYPE_b | OP_SRC, 0 }, COND_D, NOCHANGE, xSI_DEC | xCX_REP_DEC, xSI_INC | xCX_REP_DEC }, /* 0xAC */
    { NOGROUP, CPU_I386, ITYPE_STRLOAD, "lodsd", { OPTYPE_REG_xAX_BIG | OP_DST, AMODE_X | OPTYPE_z | OP_SRC, 0 }, COND_D, NOCHANGE, xSI_DECx | xCX_REP_DEC, xSI_INCx | xCX_REP_DEC }, /* 0xAD */
    { NOGROUP, CPU_I386, ITYPE_STRCMP, "scasb", { OPTYPE_REG_AL | OP_SRC, AMODE_Y | OPTYPE_b | OP_SRC, 0 }, COND_D, FLAG_COMMON_MOD, xDI_DEC | xCX_REP_DEC, xDI_INC | xCX_REP_DEC }, /* 0xAE */
    { NOGROUP, CPU_I386, ITYPE_STRCMP, "scasd", { OPTYPE_REG_xAX_BIG | OP_SRC, AMODE_Y | OPTYPE_z | OP_SRC, 0 }, COND_D, FLAG_COMMON_MOD, xDI_DECx, xDI_INCx }, /* 0xAF */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OPTYPE_REG8 | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB0 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OPTYPE_REG8 | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB1 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OPTYPE_REG8 | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB2 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OPTYPE_REG8 | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB3 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OPTYPE_REG8 | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB4 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OPTYPE_REG8 | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB5 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OPTYPE_REG8 | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB6 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OPTYPE_REG8 | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB7 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OP_REG | OP_DST, AMODE_I | OPTYPE_v | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB8 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OP_REG | OP_DST, AMODE_I | OPTYPE_v | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB9 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OP_REG | OP_DST, AMODE_I | OPTYPE_v | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xBA */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OP_REG | OP_DST, AMODE_I | OPTYPE_v | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xBB */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OP_REG | OP_DST, AMODE_I | OPTYPE_v | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xBC */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OP_REG | OP_DST, AMODE_I | OPTYPE_v | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xBD */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OP_REG | OP_DST, AMODE_I | OPTYPE_v | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xBE */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { OP_REG | OP_DST, AMODE_I | OPTYPE_v | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xBF */
    { X86_Group_2_C0, GROUP }, /* 0xC0 Eb Ib */
    { X86_Group_2_C1, GROUP }, /* 0xC1 Ev Ib */
    { NOGROUP, CPU_I386, ITYPE_RET, "ret", { AMODE_I | OPTYPE_w | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC2 */
    { NOGROUP, CPU_I386, ITYPE_RET, "ret", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC3 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "les", { AMODE_G | OPTYPE_z | OP_DST, AMODE_M | OPTYPE_p | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC4 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "lds", { AMODE_G | OPTYPE_z | OP_DST, AMODE_M | OPTYPE_p | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC5 */
    { X86_Group_12_C6, GROUP }, /* 0xC6 Eb Ib */
    { X86_Group_12_C7, GROUP }, /* 0xC7 Ev Iz */
    { NOGROUP, CPU_I386, ITYPE_ENTER, "enter", { OPTYPE_REG_xBP | OP_SRC | OP_DST, AMODE_I | OPTYPE_w | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC8 */
    { NOGROUP, CPU_I386, ITYPE_LEAVE, "leave", { OPTYPE_REG_xBP | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC9 */
    { NOGROUP, CPU_I386, ITYPE_RET, "retf", { AMODE_I | OPTYPE_w | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xCA */
    { NOGROUP, CPU_I386, ITYPE_RET, "retf", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xCB */
    { NOGROUP, CPU_I386, ITYPE_DEBUG, "int3", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xCC */
    { NOGROUP, CPU_I386, ITYPE_TRAP, "int", { AMODE_I | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xCD */
    { NOGROUP, CPU_I386, ITYPE_OFLOW, "into", NOARGS, NOCOND, NOCHANGE, NOACTION, NOACTION }, /* 0xCE */
    { NOGROUP, CPU_I386, ITYPE_TRAPRET, "iret", NOARGS, NOCOND, FLAG_ALL_MOD, NOACTION, IGNORED }, /* 0xCF */
    { X86_Group_2_D0, GROUP }, /* 0xD0 Eb, 1 */
    { X86_Group_2_D1, GROUP }, /* 0xD1 Ev, 1 */
    { X86_Group_2_D2, GROUP }, /* 0xD2 Eb, CL */
    { X86_Group_2_D3, GROUP }, /* 0xD3 Ev, CL */
    { NOGROUP, CPU_I386, ITYPE_BCDCONV, "aam", { OPTYPE_REG_AX | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD, NOACTION, IGNORED }, /* 0xD4 */
    { NOGROUP, CPU_I386, ITYPE_BCDCONV, "aad", { OPTYPE_REG_AX | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD, NOACTION, IGNORED }, /* 0xD5 */
    { NOGROUP, CPU_I386, ITYPE_ARITH, "salc", { OPTYPE_REG_AL | OP_DST, OPTYPE_FF | OP_SRC, OPTYPE_0 | OP_SRC }, COND_C, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0xD6 */
    { NOGROUP, CPU_I386, ITYPE_XLAT, "xlat", { OPTYPE_REG_AL | OP_DST, AMODE_xlat | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xD7 */
    { X86_ESC_0, EXT_FPU }, /* 0xD8 */
    { X86_ESC_1, EXT_FPU }, /* 0xD9 */
    { X86_ESC_2, EXT_FPU }, /* 0xDA */
    { X86_ESC_3, EXT_FPU }, /* 0xDB */
    { X86_ESC_4, EXT_FPU }, /* 0xDC */
    { X86_ESC_5, EXT_FPU }, /* 0xDD */
    { X86_ESC_6, EXT_FPU }, /* 0xDE */
    { X86_ESC_7, EXT_FPU }, /* 0xDF */
    { NOGROUP, CPU_I386, ITYPE_LOOPCC, "loopnz", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_REG_xCX_BIG_NZ | COND_NZ, NOCHANGE, OP1_DST | xCX_DEC, NOACTION }, /* 0xE0 */
    { NOGROUP, CPU_I386, ITYPE_LOOPCC, "loopz", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_REG_xCX_BIG_NZ | COND_Z, NOCHANGE, OP1_DST | xCX_DEC, NOACTION }, /* 0xE1 */
    { NOGROUP, CPU_I386, ITYPE_LOOPCC, "loop", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_REG_xCX_BIG_NZ, NOCHANGE, OP1_DST | xCX_DEC, NOACTION }, /* 0xE2 */
    { NOGROUP, CPU_I386, ITYPE_LOOPCC, "jecxz", { AMODE_J | OPTYPE_b | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_REG_xCX_BIG_Z, NOCHANGE, OP1_DST, NOACTION }, /* 0xE3 */
    { NOGROUP, CPU_I386, ITYPE_IN, "in", { OPTYPE_REG_AL | OP_DST, AMODE_I | OPTYPE_b | OP_SYS | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, NOACTION }, /* 0xE4 */
    { NOGROUP, CPU_I386, ITYPE_IN, "in", { OPTYPE_REG_xAX_BIG | OP_DST, AMODE_I | OPTYPE_b | OP_SYS | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, NOACTION }, /* 0xE5 */
    { NOGROUP, CPU_I386, ITYPE_OUT, "out", { AMODE_I | OPTYPE_b | OP_SYS | OP_DST, OPTYPE_REG_AL | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, NOACTION }, /* 0xE6 */
    { NOGROUP, CPU_I386, ITYPE_OUT, "out", { AMODE_I | OPTYPE_b | OP_SYS | OP_DST, OPTYPE_REG_xAX_BIG | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, NOACTION }, /* 0xE7 */
    { NOGROUP, CPU_I386, ITYPE_CALL, "call", { AMODE_J | OPTYPE_z | OP_SRC | OP_EXEC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xE8 */
    { NOGROUP, CPU_I386, ITYPE_BRANCH, "jmp", { AMODE_J | OPTYPE_z | OP_SRC | OP_EXEC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xE9 */
    { NOGROUP, CPU_I386, ITYPE_BRANCH, "jmp", { AMODE_A | OPTYPE_p | OP_SRC | OP_EXEC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xEA */
    { NOGROUP, CPU_I386, ITYPE_BRANCH, "jmp", { AMODE_J | OPTYPE_b | OP_SRC | OP_EXEC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xEB */
    { NOGROUP, CPU_I386, ITYPE_IN, "in", { OPTYPE_REG_AL | OP_DST, OPTYPE_REG_DX | OP_SYS | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xEC */
    { NOGROUP, CPU_I386, ITYPE_IN, "in", { OPTYPE_REG_xAX_BIG | OP_DST, OPTYPE_REG_DX | OP_SYS | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xED */
    { NOGROUP, CPU_I386, ITYPE_OUT, "out", { OPTYPE_REG_DX | OP_SYS | OP_DST, OPTYPE_REG_AL | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xEE */
    { NOGROUP, CPU_I386, ITYPE_OUT, "out", { OPTYPE_REG_DX | OP_SYS | OP_DST, OPTYPE_REG_xAX_BIG | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xEF */
    { PREFIX }, /* 0xF0 */
    { NOGROUP, CPU_I386, ITYPE_DEBUG, "int1", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, // aka icebp /* 0xF1 */
    { PREFIX }, /* 0xF2 */
    { PREFIX }, /* 0xF3 */
    { NOGROUP, CPU_I386, ITYPE_HALT, "hlt", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xF4 */
    { NOGROUP, CPU_I386, ITYPE_TOGCF, "cmc", { OPTYPE_FLAGS | OP_DST, 0, 0 }, NOCOND, FLAG_CF_TOG, IGNORED }, /* 0xF5 */
    { X86_Group_3_F6, GROUP }, /* 0xF6 Eb */
    { X86_Group_3_F7, GROUP }, /* 0xF7 Ev */
    { NOGROUP, CPU_I386, ITYPE_CLEARCF, "clc", { OPTYPE_FLAGS | OP_DST, 0, 0 }, NOCOND, FLAG_CF_CLR, NOACTION, IGNORED }, /* 0xF8 */
    { NOGROUP, CPU_I386, ITYPE_SETCF, "stc", { OPTYPE_FLAGS | OP_DST, 0, 0 }, NOCOND, FLAG_CF_SET, NOACTION, IGNORED }, /* 0xF9 */
    { NOGROUP, CPU_I386, ITYPE_CLEARIF, "cli", { OPTYPE_FLAGS | OP_DST, 0, 0 }, NOCOND, FLAG_IF_CLR, NOACTION, IGNORED }, /* 0xFA */
    { NOGROUP, CPU_I386, ITYPE_SETIF, "sti", { OPTYPE_FLAGS | OP_DST, 0, 0 }, NOCOND, FLAG_IF_SET, NOACTION, IGNORED }, /* 0xFB */
    { NOGROUP, CPU_I386, ITYPE_CLEARDF, "cld", { OPTYPE_FLAGS | OP_DST, 0, 0 }, NOCOND, FLAG_DF_CLR, NOACTION, IGNORED }, /* 0xFC */
    { NOGROUP, CPU_I386, ITYPE_SETDF, "std", { OPTYPE_FLAGS | OP_DST, 0, 0 }, NOCOND, FLAG_DF_SET, NOACTION, IGNORED }, /* 0xFD */
    { X86_Group_4, GROUP }, /* 0xFE */
    { X86_Group_5, GROUP }, /* 0xFF */
};

X86_OPCODE X86_Opcodes_2[0x100] = // 2 byte opcodes
{
    { X86_Group_6, GROUP }, /* 0x00 */
    { X86_0F01_ModRM, EXT_MODRM }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "lar", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_w | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "lsl", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_w | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOINSTR }, /* 0x04 */
    { X86_Opcode_0F05, EXT_64 }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "clts", { OPTYPE_CR0 | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_SYSCALLRET, "sysret", { OPTYPE_STAR_MSR | OP_MSR | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x07 */
    // 0F 07 could also be this undocumented instruction on older CPUs:
    // { NOGROUP, CPU_I386, ITYPE_SYSTEM, "loadall", { AMODE_Y | OPTYPE_cpu | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x07 */
    { NOGROUP, CPU_I486, ITYPE_SYSTEM, "invd", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x08 */
    { NOGROUP, CPU_I486, ITYPE_SYSTEM, "wbinvd", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x09 */
    { NOINSTR }, /* 0x0A */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_INVALID, "undef" /* aka UD2 */, NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x0B */
    { NOINSTR }, /* 0x0C */
    { X86_Group_P, GROUP }, /* 0x0D */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "femms", { OPTYPE_FPU_TAG | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x0E */
    { X86_3DNOW_0F, EXT_SUFFIX(AMODE_P | OPTYPE_q, AMODE_Q | OPTYPE_q, 0) }, /* 0x0F */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MOV, "movups", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x10 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MOV, "movups", { AMODE_W | OPTYPE_ps | OP_DST, AMODE_V | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x11 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MOV, "movlps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x12 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MOV, "movlps", { AMODE_M | OPTYPE_q | OP_DST, AMODE_V | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x13 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "unpcklps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x14 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "unpckhps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x15 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MOV, "movhps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x16 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MOV, "movhps", { AMODE_M | OPTYPE_q | OP_DST, AMODE_V | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x17 */
    { X86_Group_17, GROUP }, /* 0x18 */
    { NOINSTR }, /* 0x19 */
    { NOINSTR }, /* 0x1A */
    { NOINSTR }, /* 0x1B */
    { NOINSTR }, /* 0x1C */
    { NOINSTR }, /* 0x1D */
    { NOINSTR }, /* 0x1E */
    { NOINSTR }, /* 0x1F */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_R | OPTYPE_dq | OP_DST, AMODE_C | OPTYPE_dq | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x20 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_R | OPTYPE_dq | OP_DST, AMODE_D | OPTYPE_dq | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x21 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_C | OPTYPE_dq | OP_DST, AMODE_R | OPTYPE_dq | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x22 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_D | OPTYPE_dq | OP_DST, AMODE_R | OPTYPE_dq | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x23 */
    { NOINSTR }, /* 0x24 */
    //{ NOGROUP, CPU_I486, ITYPE_MOV, "mov", { AMODE_R | OPTYPE_d | OP_DST, AMODE_T | OPTYPE_d | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x24 */
    { NOINSTR }, /* 0x25 */
    { NOINSTR }, /* 0x26 */
    //{ NOGROUP, CPU_I486, ITYPE_MOV, "mov", { AMODE_T | OPTYPE_d | OP_DST, AMODE_R | OPTYPE_d | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x26 */
    { NOINSTR }, /* 0x27 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MOV, "movaps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x28 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MOV, "movaps", { AMODE_W | OPTYPE_ps | OP_DST, AMODE_V | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x29 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "cvtpi2ps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x2A */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MOV, "movntps", { AMODE_M | OPTYPE_o | OP_DST, AMODE_V | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x2B */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "cvttps2pi", { AMODE_P | OPTYPE_q | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x2C */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "cvtps2pi", { AMODE_P | OPTYPE_q | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x2D */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_CMP, "ucomiss", { AMODE_V | OPTYPE_ss | OP_SRC, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_CF_MOD | FLAG_PF_MOD | FLAG_OF_CLR | FLAG_SF_CLR | FLAG_AF_CLR, NOACTION, IGNORED }, /* 0x2E */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_CMP, "comiss", { AMODE_V | OPTYPE_ps | OP_SRC, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_CF_MOD | FLAG_PF_MOD | FLAG_OF_CLR | FLAG_SF_CLR | FLAG_AF_CLR, NOACTION, IGNORED }, /* 0x2F */
    { NOGROUP, CPU_PENTIUM, ITYPE_SYSTEM, "wrmsr", { OPTYPE_REG_ECX | OP_MSR | OP_DST, OPTYPE_EDX_HI_EAX_LO | OP_SRC, 0 } , NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x30 */
    { NOGROUP, CPU_PENTIUM, ITYPE_SYSTEM, "rdtsc", { OPTYPE_EDX_HI_EAX_LO | OP_DST, OPTYPE_TSC | OP_MSR | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x31 */
    { NOGROUP, CPU_PENTIUM, ITYPE_SYSTEM, "rdmsr", { OPTYPE_EDX_HI_EAX_LO | OP_DST, OPTYPE_REG_ECX | OP_MSR | OP_SRC, 0 } , NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x32 */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_OTHER, "rdpmc", { OPTYPE_EDX_HI_EAX_LO | OP_DST, OPTYPE_REG_ECX | OP_SYS | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x33 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSCALL, "sysenter", { OPTYPE_CS_MSR | OP_MSR | OP_SRC, OPTYPE_EIP_MSR | OP_MSR | OP_SRC, OPTYPE_ESP_MSR | OP_MSR | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x34 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSCALLRET, "sysexit", { OPTYPE_CS_MSR | OP_MSR | OP_SRC, OPTYPE_EIP_MSR | OP_MSR | OP_SRC, OPTYPE_ESP_MSR | OP_MSR | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x35 */
    { NOINSTR }, /* 0x36 */
    { NOINSTR }, /* 0x37 */
    { NOINSTR }, /* 0x38 */
    { NOINSTR }, /* 0x39 */
    { NOINSTR }, /* 0x3A */
    { NOINSTR }, /* 0x3B */
    { NOINSTR }, /* 0x3C */
    { NOINSTR }, /* 0x3D */
    { NOINSTR }, /* 0x3E */
    { NOINSTR }, /* 0x3F */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovo", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_O, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x40 */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovno", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_NO, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x41 */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovc", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_C, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x42 */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovnc", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_NC, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x43 */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovz", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_Z, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x44 */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovnz", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_NZ, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x45 */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovbe", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_BE, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x46 */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmova", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_A, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x47 */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovs", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_S, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x48 */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovns", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_NS, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x49 */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovpe", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_PE, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x4A */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovpo", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_PO, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x4B */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovl", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_L, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x4C */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovge", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_GE, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x4D */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovle", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_LE, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x4E */
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_MOVCC, "cmovg", { AMODE_G | OPTYPE_v | OP_COND_DST, AMODE_E | OPTYPE_v | OP_COND_SRC, 0 }, COND_G, NOCHANGE, OP1_DST | OP2_SRC, NOACTION }, /* 0x4F */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "movmskps", { AMODE_G | OPTYPE_d | OP_DST, AMODE_VR | OPTYPE_ps| OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x50 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "sqrtps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x51 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "rsqrtps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x52 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "rcpps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x53 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_AND, "andps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x54 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_AND, "andnps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x55 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_OR, "orps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x56 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_XOR, "xorps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x57 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_ADD, "addps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x58 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MUL, "mulps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x59 */
    { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtps2pd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5A */
    { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtdq2ps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5B */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_SUB, "subps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5C */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "minps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5D */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_DIV, "divps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5E */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "maxps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x5F */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "punpcklbw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_d | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x60 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "punpcklwd", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_d | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x61 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "punpckldq", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_d | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x62 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "packsswb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x63 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_CMP, "pcmpgtb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x64 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_CMP, "pcmpgtw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x65 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_CMP, "pcmpgtd", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x66 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "packuswb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x67 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "punpckhbw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_d | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x68 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "punpckhwd", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_d | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x69 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "punpckhdq", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_d | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x6A */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "packssdw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x6B */
    { NOINSTR }, /* 0x6C */
    { NOINSTR }, /* 0x6D */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_MOV, "movd", { AMODE_P | OPTYPE_q | OP_DST, AMODE_E | OPTYPE_dq | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x6E */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_MOV, "movq", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x6F */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "pshufw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x70 */
    { X86_Group_13, GROUP }, /* 0x71 */
    { X86_Group_14, GROUP }, /* 0x72 */
    { X86_Group_15, GROUP }, /* 0x73 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_CMP, "pcmpeqb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x74 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_CMP, "pcmpeqw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x75 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_CMP, "pcmpeqd", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x76 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "emms", { OPTYPE_FPU_TAG | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x77 */
    { NOINSTR }, /* 0x78 */
    { NOINSTR }, /* 0x79 */
    { NOINSTR }, /* 0x7A */
    { NOINSTR }, /* 0x7B */
    { NOINSTR }, /* 0x7C */
    { NOINSTR }, /* 0x7D */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_MOV, "movd", { AMODE_E | OPTYPE_dq | OP_DST, AMODE_P | OPTYPE_dq | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x7E */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_MOV, "movq", { AMODE_Q | OPTYPE_q | OP_DST, AMODE_P | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x7F */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jo", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_O, NOCHANGE, OP1_DST, NOACTION }, /* 0x80 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jno", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_NO, NOCHANGE, OP1_DST, NOACTION }, /* 0x81 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jb", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_C, NOCHANGE, OP1_DST, NOACTION }, /* 0x82 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jnb", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_NC, NOCHANGE, OP1_DST, NOACTION }, /* 0x83 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jz", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_Z, NOCHANGE, OP1_DST, NOACTION }, /* 0x84 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jnz", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_NZ, NOCHANGE, OP1_DST, NOACTION }, /* 0x85 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jbe", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_BE, NOCHANGE, OP1_DST, NOACTION }, /* 0x86 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "ja", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_A, NOCHANGE, OP1_DST, NOACTION }, /* 0x87 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "js", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_S, NOCHANGE, OP1_DST, NOACTION }, /* 0x88 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jns", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_NS, NOCHANGE, OP1_DST, NOACTION }, /* 0x89 */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jpe", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_PE, NOCHANGE, OP1_DST, NOACTION }, /* 0x8A */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jpo", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_PO, NOCHANGE, OP1_DST, NOACTION }, /* 0x8B */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jl", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_L, NOCHANGE, OP1_DST, NOACTION }, /* 0x8C */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jge", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_GE, NOCHANGE, OP1_DST, NOACTION }, /* 0x8D */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jle", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_LE, NOCHANGE, OP1_DST, NOACTION }, /* 0x8E */
    { NOGROUP, CPU_I386, ITYPE_BRANCHCC, "jg", { AMODE_J | OPTYPE_z | OP_SRC | OP_COND_EXEC, 0, 0 }, COND_G, NOCHANGE, OP1_DST, NOACTION }, /* 0x8F */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "seto", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_O, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x90 */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setno", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_NO, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x91 */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setb", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_C, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x92 */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setnb", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_NC, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x93 */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "sete", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_Z, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x94 */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setne", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_NZ, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x95 */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setbe", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_BE, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x96 */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "seta", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_A, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x97 */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "sets", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_S, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x98 */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setns", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_NS, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x99 */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setpe", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_PE, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x9A */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setpo", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_PO, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x9B */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setl", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_L, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x9C */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setge", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_GE, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x9D */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setle", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_LE, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x9E */
    { NOGROUP, CPU_I386, ITYPE_MOVCC, "setg", { AMODE_E | OPTYPE_b | OP_COND_DST, OPTYPE_1 | OP_COND_SRC, OPTYPE_0 | OP_COND_SRC }, COND_G, NOCHANGE, OP1_DST | OP2_SRC, OP1_DST | OP3_SRC }, /* 0x9F */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OPTYPE_FS | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xA0 */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OPTYPE_FS | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xA1 */
    { NOGROUP, CPU_PENTIUM, ITYPE_CPUID, "cpuid", { OPTYPE_EDX_ECX_EBX_EAX | OP_DST, OPTYPE_REG_xAX_BIG | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xA2 */
    { NOGROUP, CPU_I386, ITYPE_BITTEST, "bt", { AMODE_E | OPTYPE_v | OP_SRC, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0xA3 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "shld", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0xA4 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "shld", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, OPTYPE_REG_CL | OP_SRC }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0xA5 */
    { NOINSTR }, /* 0xA6 */
    { NOINSTR }, /* 0xA7 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { OPTYPE_GS | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xA8 */
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { OPTYPE_GS | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xA9 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "rsm", NOARGS, NOCOND, FLAG_ALL_MOD, NOACTION, IGNORED }, /* 0xAA */
    { NOGROUP, CPU_I386, ITYPE_BITTEST, "bts", { AMODE_E | OPTYPE_v | OP_SRC, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0xAB */
    { NOGROUP, CPU_I386, ITYPE_SHR, "shrd", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0xAC */
    { NOGROUP, CPU_I386, ITYPE_SHR, "shrd", { AMODE_E | OPTYPE_v | OP_DST, AMODE_G | OPTYPE_v | OP_SRC, OPTYPE_REG_CL | OP_SRC }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0xAD */
    { X86_Group_16, GROUP }, /* 0xAE */
    { NOGROUP, CPU_I386, ITYPE_MUL, "imul", { AMODE_G | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_E | OPTYPE_v | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0xAF */
    { NOGROUP, CPU_I486, ITYPE_XCHGCC, "cmpxchg", { AMODE_E | OPTYPE_b | OP_SRC | OP_COND_DST, OPTYPE_REG_AL | OP_SRC | OP_COND_DST, AMODE_G | OPTYPE_b | OP_COND_SRC }, COND_OP1_EQ_OP2, FLAG_COMMON_MOD, OP1_DST | OP3_SRC, OP2_DST | OP1_SRC }, /* 0xB0 */
    { NOGROUP, CPU_I486, ITYPE_XCHGCC, "cmpxchg", { AMODE_E | OPTYPE_v | OP_SRC | OP_COND_DST, OPTYPE_REG_xAX_BIG | OP_SRC | OP_COND_DST, AMODE_G | OPTYPE_v | OP_COND_SRC }, COND_OP1_EQ_OP2, FLAG_COMMON_MOD, OP1_DST | OP3_SRC, OP2_DST | OP1_SRC }, /* 0xB1 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "lss", { AMODE_G | OPTYPE_z | OP_DST, AMODE_M | OPTYPE_p | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB2 */
    { NOGROUP, CPU_I386, ITYPE_BITTEST, "btr", { AMODE_E | OPTYPE_v | OP_SRC, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0xB3 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "lfs", { AMODE_G | OPTYPE_z | OP_DST, AMODE_M | OPTYPE_p | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB4 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "lgs", { AMODE_G | OPTYPE_z | OP_DST, AMODE_M | OPTYPE_p | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB5 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "movzx", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB6 */
    { NOGROUP, CPU_I386, ITYPE_MOV, "movzx", { AMODE_G | OPTYPE_v | OP_DST, AMODE_E | OPTYPE_w | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xB7 */
    { NOINSTR }, /* 0xB8 */
    { X86_Group_11, GROUP }, /* 0xB9 */
    { X86_Group_8, GROUP }, /* 0xBA */
    { NOGROUP, CPU_I386, ITYPE_BITTEST, "btc", { AMODE_E | OPTYPE_v | OP_SRC, AMODE_G | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0xBB */
    { NOGROUP, CPU_I386, ITYPE_BITTEST, "bsf", { AMODE_G | OPTYPE_v | OP_SRC, AMODE_E | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD, NOACTION, IGNORED }, /* 0xBC */
    { NOGROUP, CPU_I386, ITYPE_BITTEST, "bsr", { AMODE_G | OPTYPE_v | OP_SRC, AMODE_E | OPTYPE_v | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD, NOACTION, IGNORED }, /* 0xBD */
    { NOGROUP, CPU_I386, ITYPE_MOV, "movsx", { AMODE_G | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_E | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xBE */
    { NOGROUP, CPU_I386, ITYPE_MOV, "movsx", { AMODE_G | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_E | OPTYPE_w | OP_SIGNED | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xBF */
    { NOGROUP, CPU_I486, ITYPE_XCHGADD, "xadd", { AMODE_E | OPTYPE_b | OP_SRC | OP_DST, AMODE_G | OPTYPE_b | OP_SRC | OP_DST, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0xC0 */
    { NOGROUP, CPU_I486, ITYPE_XCHGADD, "xadd", { AMODE_E | OPTYPE_v | OP_SRC | OP_DST, AMODE_G | OPTYPE_v | OP_SRC | OP_DST, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0xC1 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_CMP, "cmpps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0xC2 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MOV, "movnti", { AMODE_M | OPTYPE_dq | OP_DST, AMODE_G | OPTYPE_dq | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC3 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "pinsrw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_E | OPTYPE_w | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC4 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "pextrw", { AMODE_G | OPTYPE_d | OP_DST, AMODE_PR | OPTYPE_q | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC5 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "shufps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC6 */
    { X86_Group_9, GROUP }, /* 0xC7 */
    { NOGROUP, CPU_I486, ITYPE_XCHG, "bswap", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC8 */
    { NOGROUP, CPU_I486, ITYPE_XCHG, "bswap", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC9 */
    { NOGROUP, CPU_I486, ITYPE_XCHG, "bswap", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xCA */
    { NOGROUP, CPU_I486, ITYPE_XCHG, "bswap", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xCB */
    { NOGROUP, CPU_I486, ITYPE_XCHG, "bswap", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xCC */
    { NOGROUP, CPU_I486, ITYPE_XCHG, "bswap", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xCD */
    { NOGROUP, CPU_I486, ITYPE_XCHG, "bswap", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xCE */
    { NOGROUP, CPU_I486, ITYPE_XCHG, "bswap", { OP_REG | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xCF */
    { NOINSTR }, /* 0xD0 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psrlw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xD1 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psrld", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xD2 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psrlq", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xD3 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_ADD, "paddq", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xD4 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_MUL, "pmullw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xD5 */
    { NOINSTR }, /* 0xD6 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "pmovmskb", { AMODE_G | OPTYPE_d | OP_DST, AMODE_PR | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xD7 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_SUB, "psubusb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xD8 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_SUB, "psubusw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xD9 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "pminub", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xDA */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_AND, "pand", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xDB */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_ADD, "paddusb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xDC */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_ADD, "paddusw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xDD */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "pmaxub", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xDE */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_AND, "pandn", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xDF */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "pavgb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xE0 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psraw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xE1 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psrad", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xE2 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "pavgw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xE3 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MUL, "pmulhuw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xE4 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_MUL, "pmulhw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xE5 */
    { NOINSTR }, /* 0xE6 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MOV, "movntq", { AMODE_M | OPTYPE_q | OP_DST, AMODE_P | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xE7 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_SUB, "psubsb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xE8 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_SUB, "psubsw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xE9 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "pminsw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xEA */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_OR, "por", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xEB */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_ADD, "paddsb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xEC */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_ADD, "paddsw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xED */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "pmaxsw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xEE */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_XOR, "pxor", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xEF */
    { NOINSTR }, /* 0xF0 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psllw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xF1 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "pslld", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xF2 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psllq", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xF3 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_MUL, "pmuludq", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xF4 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_ADD, "pmaddwd", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xF5 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "psadbw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xF6 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "maskmovq", { AMODE_P | OPTYPE_q | OP_DST, AMODE_PR | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xF7 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_SUB, "psubb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xF8 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_SUB, "psubw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xF9 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_SUB, "psubd", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xFA */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_SUB, "psubq", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xFB */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_ADD, "paddb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xFC */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_ADD, "paddw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xFD */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX_ADD, "paddd", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xFE */
    { NOINSTR } /* 0xFF */,
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// Groups
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

X86_OPCODE X86_Group_1_80[8] = // 80
{
    { NOGROUP, CPU_I386, ITYPE_ADD, "add", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_OR, "or", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "adc", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sbb", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_AND, "and", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sub", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_XOR, "xor", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_CMP, "cmp", { AMODE_E | OPTYPE_b | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_1_81[8] = // 81
{
    { NOGROUP, CPU_I386, ITYPE_ADD, "add", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_OR, "or", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "adc", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sbb", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_AND, "and", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sub", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_XOR, "xor", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_CMP, "cmp", { AMODE_E | OPTYPE_v | OP_SRC, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_1_82[8] = // 82
{
    { NOGROUP, CPU_I386, ITYPE_ADD, "add", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_OR, "or", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "adc", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sbb", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_AND, "and", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sub", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_XOR, "xor", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_CMP, "cmp", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_SRC, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_1_83[8] = // 83
{
    { NOGROUP, CPU_I386, ITYPE_ADD, "add", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_OR, "or", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_ADD, "adc", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sbb", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_AND, "and", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_SUB, "sub", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_XOR, "xor", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_CMP, "cmp", { AMODE_E | OPTYPE_v | OP_SIGNED | OP_SRC, AMODE_I | OPTYPE_b | OP_SIGNED | OP_SRC, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_2_C0[8] = // C0
{
    { NOGROUP, CPU_I386, ITYPE_ROL, "rol", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "ror", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_ROL, "rcl", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "rcr", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "shl", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "shr", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "sal", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "sar", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_2_C1[8] = // C1
{
    { NOGROUP, CPU_I386, ITYPE_ROL, "rol", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "ror", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_ROL, "rcl", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "rcr", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "shl", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "shr", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "sal", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "sar", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_2_D0[8] = // D0
{
    { NOGROUP, CPU_I386, ITYPE_ROL, "rol", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "ror", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_ROL, "rcl", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "rcr", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "shl", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "shr", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "sal", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "sar", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_2_D1[8] = // D1
{
    { NOGROUP, CPU_I386, ITYPE_ROL, "rol", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "ror", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_ROL, "rcl", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "rcr", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "shl", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "shr", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "sal", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "sar", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_1 | OP_SRC, 0 }, NOCOND, FLAG_OF_MOD | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_2_D2[8] = // D2
{
    { NOGROUP, CPU_I386, ITYPE_ROL, "rol", { AMODE_E | OPTYPE_b | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "ror", { AMODE_E | OPTYPE_b | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_ROL, "rcl", { AMODE_E | OPTYPE_b | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "rcr", { AMODE_E | OPTYPE_b | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "shl", { AMODE_E | OPTYPE_b | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "shr", { AMODE_E | OPTYPE_b | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "sal", { AMODE_E | OPTYPE_b | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "sar", { AMODE_E | OPTYPE_b | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_2_D3[8] = // D3
{
    { NOGROUP, CPU_I386, ITYPE_ROL, "rol", { AMODE_E | OPTYPE_v | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "ror", { AMODE_E | OPTYPE_v | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_ROL, "rcl", { AMODE_E | OPTYPE_v | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_ROR, "rcr", { AMODE_E | OPTYPE_v | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "shl", { AMODE_E | OPTYPE_v | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "shr", { AMODE_E | OPTYPE_v | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_SHL, "sal", { AMODE_E | OPTYPE_v | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_SHR, "sar", { AMODE_E | OPTYPE_v | OP_DST, OPTYPE_REG_CL | OP_SRC, 0 }, NOCOND, FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_3_F6[8] = // F6
{
    { NOGROUP, CPU_I386, ITYPE_TEST, "test", { AMODE_E | OPTYPE_b | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_TEST, "test", { AMODE_E | OPTYPE_b | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_NOT, "not", { AMODE_E | OPTYPE_b | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_NEG, "neg", { AMODE_E | OPTYPE_b | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_MUL, "mul", { OPTYPE_REG_AX | OP_DST, AMODE_E | OPTYPE_b | OP_SRC, OPTYPE_REG_AL | OP_SRC }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_MUL, "imul", { OPTYPE_REG_AX | OP_SIGNED | OP_DST, AMODE_E | OPTYPE_b | OP_SIGNED | OP_SRC, OPTYPE_REG_AL | OP_SIGNED | OP_SRC }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_DIV, "div", { OPTYPE_REG_AX | OP_DST, AMODE_E | OPTYPE_b | OP_SRC, OPTYPE_REG_AX | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_DIV, "idiv", { OPTYPE_REG_AX | OP_SIGNED | OP_DST, AMODE_E | OPTYPE_b | OP_SIGNED | OP_SRC, OPTYPE_REG_AX | OP_SIGNED | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED } /* 0x07 */,
};

X86_OPCODE X86_Group_3_F7[8] = // F7
{
    { NOGROUP, CPU_I386, ITYPE_TEST, "test", { AMODE_E | OPTYPE_v | OP_SRC, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_TEST, "test", { AMODE_E | OPTYPE_v | OP_SRC, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, FLAG_OF_CLR | FLAG_SF_MOD | FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_CLR, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_NOT, "not", { AMODE_E | OPTYPE_v | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_NEG, "neg", { AMODE_E | OPTYPE_v | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_MUL, "mul", { OPTYPE_xDX_HI_xAX_LO | OP_DST, AMODE_E | OPTYPE_v | OP_SRC, OPTYPE_REG_xAX_BIG | OP_SRC }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_MUL, "imul", { OPTYPE_xDX_HI_xAX_LO | OP_SIGNED | OP_DST, AMODE_E | OPTYPE_v | OP_SIGNED | OP_SRC, OPTYPE_REG_xAX_BIG | OP_SRC }, NOCOND, FLAG_OF_MOD | FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_DIV, "div", { OPTYPE_xDX_HI_xAX_LO | OP_DST, AMODE_E | OPTYPE_v | OP_SRC, OPTYPE_REG_xAX_BIG | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_DIV, "idiv", { OPTYPE_xDX_HI_xAX_LO | OP_SIGNED | OP_DST, AMODE_E | OPTYPE_v | OP_SIGNED | OP_SRC, OPTYPE_REG_xAX_BIG | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_4[8] = // FE
{
    { NOGROUP, CPU_I386, ITYPE_INC, "inc", { AMODE_E | OPTYPE_b | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_DEC, "dec", { AMODE_E | OPTYPE_b | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x01 */
    { NOINSTR }, /* 0x03 */
    { NOINSTR }, /* 0x04 */
    { NOINSTR }, /* 0x05 */
    { NOINSTR }, /* 0x06 */
    { NOINSTR } /* 0x07 */
};

X86_OPCODE X86_Group_5[8] = // FF
{
    { NOGROUP, CPU_I386, ITYPE_INC, "inc", { AMODE_E | OPTYPE_v | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_DEC, "dec", { AMODE_E | OPTYPE_v | OP_SRC | OP_DST, 0, 0 }, NOCOND, FLAG_OF_MOD|FLAG_SF_MOD|FLAG_ZF_MOD|FLAG_AF_MOD|FLAG_PF_MOD, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_CALL, "call", { AMODE_E | OPTYPE_v | OP_SRC | OP_EXEC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_CALL, "call", { AMODE_E | OPTYPE_p | OP_SRC | OP_EXEC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_BRANCH, "jmp", { AMODE_E | OPTYPE_v | OP_SRC | OP_EXEC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_BRANCH, "jmp", { AMODE_E | OPTYPE_p | OP_SRC | OP_EXEC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_PUSH, "push", { AMODE_E | OPTYPE_v | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOINSTR }, /* 0x07 */
};

X86_OPCODE X86_Group_6[8] = // 0F 00
{
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "sldt", { AMODE_E | OPTYPE_mw | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "str", { AMODE_E | OPTYPE_mw | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "lldt", { AMODE_E | OPTYPE_w | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "ltr", { AMODE_E | OPTYPE_w | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "verr", { AMODE_E | OPTYPE_w | OP_SRC, 0, 0 }, NOCOND, FLAG_ZF_MOD, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "verw", { AMODE_E | OPTYPE_w | OP_SRC, 0, 0 }, NOCOND, FLAG_ZF_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_IA64, ITYPE_BRANCH, "jmpe", { AMODE_E | OPTYPE_v | OP_SRC | OP_EXEC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOINSTR } /* 0x07 */
};

X86_OPCODE X86_Group_7[8] = // 0F 01
{
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "sgdt", { AMODE_M | OPTYPE_dt | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "sidt", { AMODE_M | OPTYPE_dt | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "lgdt", { AMODE_M | OPTYPE_dt | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "lidt", { AMODE_M | OPTYPE_dt | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "smsw", { AMODE_E | OPTYPE_mw | OP_DST, OPTYPE_CR0 | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x04 */
    { NOINSTR }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "lmsw", { OPTYPE_CR0 | OP_DST, AMODE_E | OPTYPE_w | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I486, ITYPE_SYSTEM, "invlpg", { AMODE_M | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_8[8] = // 0F BA
{
    { NOINSTR }, /* 0x00 */
    { NOINSTR }, /* 0x01 */
    { NOINSTR }, /* 0x02 */
    { NOINSTR }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_BITTEST, "bt", { AMODE_E | OPTYPE_v | OP_SRC | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_BITTEST, "bts", { AMODE_E | OPTYPE_v | OP_SRC | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_BITTEST, "btr", { AMODE_E | OPTYPE_v | OP_SRC | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_BITTEST, "btc", { AMODE_E | OPTYPE_v | OP_SRC | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, FLAG_CF_MOD, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_9[8] = // 0F C7
{
    { NOINSTR }, /* 0x00 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_XCHGCC, "cmpxchg8b", { AMODE_M | OPTYPE_q | OP_SRC | OP_COND_DST, OPTYPE_xDX_HI_xAX_LO | OP_SRC | OP_COND_DST, OPTYPE_xCX_HI_xBX_LO | OP_COND_SRC }, COND_OP1_EQ_OP2, FLAG_ZF_MOD, OP1_DST | OP3_SRC, OP2_DST | OP1_SRC }, /* 0x02 */
    { NOINSTR }, /* 0x03 */
    { NOINSTR }, /* 0x04 */
    { NOINSTR }, /* 0x05 */
    { NOINSTR }, /* 0x06 */
    { NOINSTR }, /* 0x07 */
};

X86_OPCODE X86_Group_10[8] = // 8F (NOTE: AMD64 labels this Group 1A)
{
    { NOGROUP, CPU_I386, ITYPE_POP, "pop", { AMODE_E | OPTYPE_v | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x00 */
    { NOINSTR }, /* 0x01 */
    { NOINSTR }, /* 0x02 */
    { NOINSTR }, /* 0x03 */
    { NOINSTR }, /* 0x04 */
    { NOINSTR }, /* 0x05 */
    { NOINSTR }, /* 0x06 */
    { NOINSTR }, /* 0x07 */
};

X86_OPCODE X86_Group_11[8] = // 0F B9 (NOTE: AMD64 labels this Group 10)
{
    { NOGROUP, CPU_I386, ITYPE_INVALID, "undef", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_I386, ITYPE_INVALID, "undef", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_I386, ITYPE_INVALID, "undef", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_I386, ITYPE_INVALID, "undef", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_I386, ITYPE_INVALID, "undef", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_I386, ITYPE_INVALID, "undef", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_I386, ITYPE_INVALID, "undef", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_I386, ITYPE_INVALID, "undef", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED } /* 0x07 */
};

X86_OPCODE X86_Group_12_C6[8] = // C6 (NOTE: AMD64 labels this Group 11)
{
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_E | OPTYPE_b | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0xC6 */
    { NOINSTR }, /* 0x01 */
    { NOINSTR }, /* 0x02 */
    { NOINSTR }, /* 0x03 */
    { NOINSTR }, /* 0x04 */
    { NOINSTR }, /* 0x05 */
    { NOINSTR }, /* 0x06 */
    { NOINSTR }, /* 0x07 */
};

X86_OPCODE X86_Group_12_C7[8] = // C7 (NOTE: AMD64 labels this Group 11)
{
    { NOGROUP, CPU_I386, ITYPE_MOV, "mov", { AMODE_E | OPTYPE_v | OP_DST, AMODE_I | OPTYPE_z | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x00 */
    { NOINSTR }, /* 0x01 */
    { NOINSTR }, /* 0x02 */
    { NOINSTR }, /* 0x03 */
    { NOINSTR }, /* 0x04 */
    { NOINSTR }, /* 0x05 */
    { NOINSTR }, /* 0x06 */
    { NOINSTR }, /* 0x07 */
};

// NOTE: the X86_SSE2_* is only followed if it is a 3-byte opcode (e.g., prefix is 66, F2, or F3)
X86_OPCODE X86_Group_13[8] = // 0F 71 (NOTE: AMD64 labels this Group 12)
{
    { NOINSTR }, /* 0x00 */
    { NOINSTR }, /* 0x01 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psrlw", { AMODE_PR | OPTYPE_q | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOINSTR }, /* 0x03 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psraw", { AMODE_PR | OPTYPE_q | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x04 */
    { NOINSTR }, /* 0x05 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psllw", { AMODE_PR | OPTYPE_q | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOINSTR }, /* 0x07 */
};

X86_OPCODE X86_Group_14[8] = // 0F 72 (NOTE: AMD64 labels this Group 13)
{
    { NOINSTR }, /* 0x00 */
    { NOINSTR }, /* 0x01 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psrld", { AMODE_PR | OPTYPE_q | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOINSTR }, /* 0x03 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psrad", { AMODE_PR | OPTYPE_q | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x04 */
    { NOINSTR }, /* 0x05 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "pslld", { AMODE_PR | OPTYPE_q | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOINSTR }, /* 0x07 */
};

X86_OPCODE X86_Group_15[8] = // 0F 73 (NOTE: AMD64 labels this Group 14)
{
    { NOINSTR }, /* 0x00 */
    { NOINSTR }, /* 0x01 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psrlq", { AMODE_PR | OPTYPE_q | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psrldq", { AMODE_PR | OPTYPE_q | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x03 */
    { NOINSTR }, /* 0x04 */
    { NOINSTR }, /* 0x05 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "psllq", { AMODE_PR | OPTYPE_q | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_MMX, "pslldq", { AMODE_PR | OPTYPE_q | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED } /* 0x07 */
};

X86_OPCODE X86_Group_16[8] = // 0F AE (NOTE: AMD64 labels this Group 15)
{
    { NOGROUP, CPU_PENTIUM2, ITYPE_FPU, "fxsave", { AMODE_M | OPTYPE_fst2 | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_FPU, "fxrstor", { AMODE_M | OPTYPE_fst2 | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "ldmxcsr", { AMODE_M | OPTYPE_d | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "stmxcsr", { AMODE_M | OPTYPE_d | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x03 */
    { NOINSTR }, /* 0x04 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSTEM, "lfence", NOARGS, NOCOND, NOCHANGE, SERIALIZE_READ, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSTEM, "mfence", NOARGS, NOCOND, NOCHANGE, SERIALIZE_ALL, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSTEM, "sfence", NOARGS, NOCOND, NOCHANGE, SERIALIZE_WRITE, IGNORED } /* 0x07 */
};

X86_OPCODE X86_Group_17[8] = // 0F 18 (NOTE: AMD64 labels this Group 16)
{
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSTEM, "prefetchnta", { AMODE_E | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x00 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSTEM, "prefetcht0", { AMODE_E | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSTEM, "prefetcht1", { AMODE_E | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSTEM, "prefetcht2", { AMODE_E | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x03 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSTEM, "hintnop", { AMODE_E | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x04 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSTEM, "hintnop", { AMODE_E | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x05 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSTEM, "hintnop", { AMODE_E | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
    { NOGROUP, CPU_PENTIUM2, ITYPE_SYSTEM, "hintnop", { AMODE_E | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x07 */
};

X86_OPCODE X86_Group_P[8] = // 0F 0D
{
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "prefetch", { AMODE_E | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x01 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "prefetchw", { AMODE_E | OPTYPE_b | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
    { NOINSTR }, /* 0x03 */
    { NOINSTR }, /* 0x04 */
    { NOINSTR }, /* 0x05 */
    { NOINSTR }, /* 0x06 */
    { NOINSTR }, /* 0x07 */
};


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// FPU (ESC) opcodes
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


X86_OPCODE X86_ESC_0[0x48] = // D8 
{
    //
    // ModRM < C0
    // Index 0x00-0x07 = opcode extension
    //

    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_ss | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x00
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_ss | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x01
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcom", { AMODE_M | OPTYPE_ss | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // 0x02
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcomp", { AMODE_M | OPTYPE_ss | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // 0x03
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_ss | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x04
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_ss | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x05
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_ss | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x06
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_ss | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x07

    //
    // ModRM >= C0: ST(0) <- ST(0) + ST(i)
    // Index 0x08-0x47 = ModRM 0xC0-0xFF
    //

    // C0-C7
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x7
    // C8-CF
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xF
    // D0-D7
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x7
    // D8-DF
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xF
    // E0-E7
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x7
    // E8-EF
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xF
    // F0-F7
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x7
    // F8-FF
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }  // xF
};


X86_OPCODE X86_ESC_1[0x48] = // D9
{
    //
    // ModRM < C0
    // Index 0x00-0x07 = opcode extension
    //

    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld", { AMODE_M | OPTYPE_ss | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // 0x00
    { NOINSTR }, // 0x01
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fst", { AMODE_M | OPTYPE_ss | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x02
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstp", { AMODE_M | OPTYPE_ss | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // 0x03
    { NOGROUP, CPU_I287, ITYPE_FLOADENV, "fldenv", { AMODE_M | OPTYPE_fev | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // 0x04
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fldcw", { OPTYPE_FPU_CONTROL | OP_DST, AMODE_M | OPTYPE_w | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // 0x05
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstenv", { AMODE_M | OPTYPE_fev | OP_DST, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // 0x06
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fnstcw", { AMODE_M | OPTYPE_w | OP_DST, OPTYPE_FPU_CONTROL | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // 0x07

    //
    // ModRM >= C0
    // Index 0x08-0x47 = ModRM 0xC0-0xFF
    //

    // C0-C7
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld", { OPTYPE_STx | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld", { OPTYPE_STx | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld", { OPTYPE_STx | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld", { OPTYPE_STx | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld", { OPTYPE_STx | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld", { OPTYPE_STx | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld", { OPTYPE_STx | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld", { OPTYPE_STx | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x7
    // C8-CF
    { NOGROUP, CPU_I287, ITYPE_FEXCH, "fxch", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC | OP_DST, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FEXCH, "fxch", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC | OP_DST, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FEXCH, "fxch", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC | OP_DST, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FEXCH, "fxch", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC | OP_DST, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FEXCH, "fxch", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC | OP_DST, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FEXCH, "fxch", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC | OP_DST, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FEXCH, "fxch", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC | OP_DST, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FEXCH, "fxch", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_STx | OP_SRC | OP_DST, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xF
    // D0-D7
    { NOGROUP, CPU_I287, ITYPE_FPU, "fnop", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
    { NOINSTR }, // x1
    { NOINSTR }, // x2
    { NOINSTR }, // x3
    { NOINSTR }, // x4
    { NOINSTR }, // x5
    { NOINSTR }, // x6
    { NOINSTR }, // x7
    // D8-DF
    { NOINSTR }, // x8
    { NOINSTR }, // x9
    { NOINSTR }, // xA
    { NOINSTR }, // xB
    { NOINSTR }, // xC
    { NOINSTR }, // xD
    { NOINSTR }, // xE
    { NOINSTR }, // xF
    // E0-E7
    { NOGROUP, CPU_I287, ITYPE_FPU, "fchs", { OPTYPE_ST0 | OP_SRC | OP_DST, 0, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FPU, "fabs", { OPTYPE_ST0 | OP_SRC | OP_DST, 0, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x1
    { NOINSTR }, // x2
    { NOINSTR }, // x3
    { NOGROUP, CPU_I287, ITYPE_FPU, "ftst", { OPTYPE_ST0 | OP_SRC, OPTYPE_FLDZ | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FPU, "fxam", { OPTYPE_ST0 | OP_SRC, 0, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x5
    { NOINSTR }, // x6
    { NOINSTR }, // x7
    // E8-EF
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld1", { OPTYPE_FLD1 | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fldl2t", { OPTYPE_FLDL2T | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fldl2e", { OPTYPE_FLDL2E | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fldpi", { OPTYPE_FLDPI | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fldlg2", { OPTYPE_FLDLG2| OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fldln2", { OPTYPE_FLDLN2 | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fldz", { OPTYPE_FLDZ | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // xE
    { NOINSTR }, // xF
    // F0-F7
    { NOGROUP, CPU_I287, ITYPE_FPU, "f2xm1", { OPTYPE_ST0 | OP_SRC | OP_DST, 0, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FPU, "fyl2x", { OPTYPE_ST1 | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FPU, "fptan", { OPTYPE_ST0 | OP_SRC | OP_DST, 0, 0 }, NOCOND, FPU_C1_MOD|FPU_C2_MOD, FPU_STACK_PUSH, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FPU, "fpatan", { OPTYPE_ST1 | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC , 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FPU, "fxtract", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_ST1 | OP_DST, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x4
    { NOGROUP, CPU_I387, ITYPE_FPU, "fprem1", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_ST1 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FPU, "fdecstp", NOARGS, NOCOND, FPU_C1_MOD, FPU_STACK_DEC, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FPU, "fincstp", NOARGS, NOCOND, FPU_C1_MOD, FPU_STACK_INC, IGNORED }, // x7
    // F8-FF
    { NOGROUP, CPU_I287, ITYPE_FPU, "fprem", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_ST1 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FPU, "fyl2xp1", { OPTYPE_ST1 | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FPU, "fsqrt", { OPTYPE_ST0 | OP_SRC | OP_DST, 0, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FPU, "fsincos", { OPTYPE_ST0 | OP_SRC | OP_DST, 0, 0 }, NOCOND, FPU_C1_MOD|FPU_C2_MOD, FPU_STACK_PUSH, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FPU, "frndint", { OPTYPE_ST0 | OP_SRC | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FPU, "fscale", { OPTYPE_ST0 | OP_SRC | OP_DST, OPTYPE_ST1 | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FPU, "fsin", { OPTYPE_ST0 | OP_SRC | OP_DST, 0, 0 }, NOCOND, FPU_C1_MOD|FPU_C2_MOD, NOACTION, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FPU, "fcos", { OPTYPE_ST0 | OP_SRC | OP_DST, 0, 0 }, NOCOND, FPU_C1_MOD|FPU_C2_MOD, NOACTION, IGNORED } // xF
};


X86_OPCODE X86_ESC_2[0x48] = // DA
{
    //
    // ModRM < C0
    // Index 0x00-0x07 = opcode extension
    //

    { NOGROUP, CPU_I287, ITYPE_FADD, "fiadd", { OPTYPE_ST0 | OP_SIGNED | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fimul", { OPTYPE_ST0 | OP_SIGNED | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "ficom", { AMODE_M | OPTYPE_sd | OP_SIGNED | OP_SRC, OPTYPE_ST0 | OP_SRC | OP_DST, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "ficomp", { AMODE_M | OPTYPE_sd | OP_SIGNED | OP_SRC, OPTYPE_ST0 | OP_SRC | OP_DST, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fisub", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fisubr", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fidiv", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fidivr", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x7

    //
    // ModRM >= C0
    // Index 0x08-0x47 = ModRM 0xC0-0xFF
    //

    // C0-C7
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_B, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x0
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_B, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x1
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_B, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x2
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_B, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x3
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_B, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x4
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_B, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x5
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_B, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x6
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_B, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x7
    // C8-CF
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmove", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_E, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x8
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmove", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_E, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x9
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmove", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_E, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xA
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmove", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_E, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xB
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmove", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_E, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xC
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmove", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_E, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xD
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmove", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_E, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xE
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmove", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_E, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xF
    // D0-D7
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_BE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x0
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_BE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x1
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_BE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x2
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_BE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x3
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_BE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x4
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_BE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x5
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_BE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x6
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_BE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x7
    // D8-DF
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_U, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x8
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_U, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x9
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_U, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xA
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_U, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xB
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_U, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xC
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_U, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xD
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_U, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xE
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FMOVCC, "fcmovu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_COND_SRC, 0 }, COND_U, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xF
    // E0-E7
    { NOINSTR }, // x0
    { NOINSTR }, // x1
    { NOINSTR }, // x2
    { NOINSTR }, // x3
    { NOINSTR }, // x4
    { NOINSTR }, // x5
    { NOINSTR }, // x6
    { NOINSTR }, // x7
    // E8-EF
    { NOINSTR }, // x8
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucompp", { OPTYPE_ST1 | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP2, IGNORED }, // x9
    { NOINSTR }, // xA
    { NOINSTR }, // xB
    { NOINSTR }, // xC
    { NOINSTR }, // xD
    { NOINSTR }, // xE
    { NOINSTR }, // xF
    // F0-F7
    { NOINSTR }, // x0
    { NOINSTR }, // x1
    { NOINSTR }, // x2
    { NOINSTR }, // x3
    { NOINSTR }, // x4
    { NOINSTR }, // x5
    { NOINSTR }, // x6
    { NOINSTR }, // x7
    // F8-FF
    { NOINSTR }, // x8
    { NOINSTR }, // x9
    { NOINSTR }, // xA
    { NOINSTR }, // xB
    { NOINSTR }, // xC
    { NOINSTR }, // xD
    { NOINSTR }, // xE
    { NOINSTR }  // xF
};



X86_OPCODE X86_ESC_3[0x48] = // DB
{
    //
    // ModRM < C0
    // Index 0x00-0x07 = opcode extension
    //

    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fild", { AMODE_M | OPTYPE_d | OP_SIGNED | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // 0x00
    { NOGROUP, CPU_PENTIUM4, ITYPE_FSTORE, "fisttp", { AMODE_M | OPTYPE_d | OP_SIGNED | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // 0x01
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fist", { AMODE_M | OPTYPE_d | OP_SIGNED | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x02
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fistp", { AMODE_M | OPTYPE_d | OP_SIGNED | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // 0x03
    { NOINSTR }, // 0x04
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld", { AMODE_M | OPTYPE_se | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // 0x05
    { NOINSTR }, // 0x06
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstp", { AMODE_M | OPTYPE_se | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // 0x07

    //
    // ModRM >= C0
    // Index 0x08-0x47 = ModRM 0xC0-0xFF
    //

    // C0-C7
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NB, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x0
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NB, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x1
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NB, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x2
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NB, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x3
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NB, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x4
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NB, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x5
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NB, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x6
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnb", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NB, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x7
    // C8-CF
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovne", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x8
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovne", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x9
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovne", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xA
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovne", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xB
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovne", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xC
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovne", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xD
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovne", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xE
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovne", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xF
    // D0-D7
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NBE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x0
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NBE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x1
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NBE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x2
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NBE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x3
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NBE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x4
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NBE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x5
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NBE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x6
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnbe", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NBE, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x7
    // D8-DF
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NU, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x8
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NU, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // x9
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NU, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xA
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NU, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xB
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NU, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xC
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NU, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xD
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NU, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xE
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FPU, "fcmovnu", { OPTYPE_ST0 | OP_COND_DST, OPTYPE_STx | OP_SRC, 0 }, COND_NU, FPU_C1_MOD, OP1_DST | OP2_SRC, NOACTION }, // xF
    // E0-E7
    { NOINSTR }, // x0
    { NOINSTR }, // x1
    { NOGROUP, CPU_I287, ITYPE_FPU, "fnclex", { OPTYPE_FPU_STATUS | OP_DST, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FPU, "finit", { OPTYPE_FPU_STATUS | OP_DST, 0, 0 }, NOCOND, FPU_ALL_CLR, NOACTION, IGNORED }, // x3
    { NOINSTR }, // x4
    { NOINSTR }, // x5
    { NOINSTR }, // x6
    { NOINSTR }, // x7
    // E8-EF
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x8
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x9
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // xA
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // xB
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // xC
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // xD
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // xE
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // xF
    // F0-F7
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x3
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomi", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, NOACTION, IGNORED }, // x7
    // F8-FF
    { NOINSTR }, // x8
    { NOINSTR }, // x9
    { NOINSTR }, // xA
    { NOINSTR }, // xB
    { NOINSTR }, // xC
    { NOINSTR }, // xD
    { NOINSTR }, // xE
    { NOINSTR }  // xF
};

X86_OPCODE X86_ESC_4[0x48] = // DC
{
    //
    // ModRM < C0
    // Index 0x00-0x07 = opcode extension
    //

    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x00
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x01
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcom", { AMODE_M | OPTYPE_sd | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // 0x02
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcomp", { AMODE_M | OPTYPE_sd | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // 0x03
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x04
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x05
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x06
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_sd | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x07

    //
    // ModRM >= C0
    // Index 0x08-0x47 = ModRM 0xC0-0xFF
    //

    // C0-C7
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FADD, "fadd", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x7
    // C8-CF
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmul", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xF
    // D0-D7
    { NOINSTR }, // x0
    { NOINSTR }, // x1
    { NOINSTR }, // x2
    { NOINSTR }, // x3
    { NOINSTR }, // x4
    { NOINSTR }, // x5
    { NOINSTR }, // x6
    { NOINSTR }, // x7
    // D8-DF
    { NOINSTR }, // x8
    { NOINSTR }, // x9
    { NOINSTR }, // xA
    { NOINSTR }, // xB
    { NOINSTR }, // xC
    { NOINSTR }, // xD
    { NOINSTR }, // xE
    { NOINSTR }, // xF
    // E0-E7
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x7
    // E8-EF
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsub", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xF
    // F0-F7
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivr", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x7
    // F8-FF
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdiv", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED } // xF
};


X86_OPCODE X86_ESC_5[0x48] = // DD
{
    //
    // ModRM < C0
    // Index 0x00-0x07 = opcode extension
    //
    //

    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fld", { AMODE_M | OPTYPE_sd | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // x0
    { NOGROUP, CPU_PENTIUM4, ITYPE_FSTORE, "fisttp", { AMODE_M | OPTYPE_d | OP_SIGNED | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fst", { AMODE_M | OPTYPE_sd | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstp", { AMODE_M | OPTYPE_sd | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FRESTORE, "frstor", { AMODE_M | OPTYPE_fst1 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x4
    { NOINSTR }, // x5
    { NOGROUP, CPU_I287, ITYPE_FSAVE, "fsave", { AMODE_M | OPTYPE_fst1 | OP_DST, 0 }, NOCOND, FPU_ALL_CLR, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fnstsw", { AMODE_M | OPTYPE_w | OP_DST, OPTYPE_FPU_STATUS | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x7

    //
    // ModRM >= C0
    // Index 0x08-0x47 = ModRM 0xC0-0xFF
    //

    // C0-C7
    { NOGROUP, CPU_I287, ITYPE_FPU, "ffree", { OPTYPE_FPU_TAG | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FPU, "ffree", { OPTYPE_FPU_TAG | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FPU, "ffree", { OPTYPE_FPU_TAG | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FPU, "ffree", { OPTYPE_FPU_TAG | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FPU, "ffree", { OPTYPE_FPU_TAG | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FPU, "ffree", { OPTYPE_FPU_TAG | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FPU, "ffree", { OPTYPE_FPU_TAG | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FPU, "ffree", { OPTYPE_FPU_TAG | OP_DST, OPTYPE_STx | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x7
    // C8-CF
    { NOINSTR }, // x8
    { NOINSTR }, // x9
    { NOINSTR }, // xA
    { NOINSTR }, // xB
    { NOINSTR }, // xC
    { NOINSTR }, // xD
    { NOINSTR }, // xE
    { NOINSTR }, // xF
    // D0-D7
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fst", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fst", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fst", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fst", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fst", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fst", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fst", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fst", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // x7
    // D8-DF
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstp", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstp", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstp", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstp", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstp", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstp", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstp", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fstp", { OPTYPE_STx | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xF
    // E0-E7
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x0
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x1
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x2
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x3
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x4
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x5
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x6
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucom", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // x7
    // E8-EF
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x8
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x9
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fucomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xB
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xC
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xD
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xE
    { NOGROUP, CPU_I387, ITYPE_FCOMP, "fucomp", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xF
    // F0-F7
    { NOINSTR }, // x0
    { NOINSTR }, // x1
    { NOINSTR }, // x2
    { NOINSTR }, // x3
    { NOINSTR }, // x4
    { NOINSTR }, // x5
    { NOINSTR }, // x6
    { NOINSTR }, // x7
    // F8-FF
    { NOINSTR }, // x8
    { NOINSTR }, // x9
    { NOINSTR }, // xA
    { NOINSTR }, // xB
    { NOINSTR }, // xC
    { NOINSTR }, // xD
    { NOINSTR }, // xE
    { NOINSTR }  // xF
};


X86_OPCODE X86_ESC_6[0x48] = // DE
{
    //
    // ModRM < C0
    // Index 0x00-0x07 = opcode extension
    //

    { NOGROUP, CPU_I287, ITYPE_FADD, "fiadd", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_w | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x00
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fimul", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_w | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x01
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "ficom", { AMODE_M | OPTYPE_w | OP_SIGNED | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, NOACTION, IGNORED }, // 0x02
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "ficomp", { AMODE_M | OPTYPE_w | OP_SIGNED | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // 0x03
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fisub", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_w | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x04
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fisubr", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_w | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x05
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fidiv", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_w | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x06
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fidivr", { OPTYPE_ST0 | OP_SRC | OP_DST, AMODE_M | OPTYPE_w | OP_SIGNED | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x07
    //
    // ModRM >= C0
    // Index 0x08-0x47 = ModRM 0xC0-0xFF
    //

    // C0-C7
    { NOGROUP, CPU_I287, ITYPE_FADD, "faddp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FADD, "faddp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FADD, "faddp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FADD, "faddp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FADD, "faddp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FADD, "faddp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FADD, "faddp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FADD, "faddp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x7
    // C8-CF
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmulp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmulp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmulp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmulp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmulp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmulp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmulp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FMUL, "fmulp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xF
    // D0-D7
    { NOINSTR }, // x0
    { NOINSTR }, // x1
    { NOINSTR }, // x2
    { NOINSTR }, // x3
    { NOINSTR }, // x4
    { NOINSTR }, // x5
    { NOINSTR }, // x6
    { NOINSTR }, // x7
    // D8-DF
    { NOINSTR }, // x8
    { NOGROUP, CPU_I287, ITYPE_FCOMP, "fcompp", { OPTYPE_ST1 | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_ALL_MOD, FPU_STACK_POP2, IGNORED }, // x9
    { NOINSTR }, // 0xA
    { NOINSTR }, // 0xB
    { NOINSTR }, // 0xC
    { NOINSTR }, // 0xD
    { NOINSTR }, // 0xE
    { NOINSTR }, // 0xF
    // E0-E7
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x7
    // E8-EF
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FSUB, "fsubp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xF
    // F0-F7
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x0
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x1
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x2
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x3
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x4
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x5
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x6
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivrp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x7
    // F8-FF
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x8
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // x9
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xA
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xB
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xC
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xD
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // xE
    { NOGROUP, CPU_I287, ITYPE_FDIV, "fdivp", { OPTYPE_STx | OP_SRC | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }  // xF
};


X86_OPCODE X86_ESC_7[0x48] = // DF
{
    //
    // ModRM < C0
    // Index 0x00-0x07 = opcode extension
    //

    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fild", { AMODE_M | OPTYPE_w | OP_SIGNED | OP_SRC, 0, 0}, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // 0x00
    { NOGROUP, CPU_PENTIUM4, ITYPE_FSTORE, "fisttp", { AMODE_M | OPTYPE_w | OP_SIGNED | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // 0x01
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fist", { AMODE_M | OPTYPE_w | OP_SIGNED | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, NOACTION, IGNORED }, // 0x02
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fistp", { AMODE_M | OPTYPE_w | OP_SIGNED | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // 0x03
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fbld", { AMODE_M | OPTYPE_pb | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // 0x04
    { NOGROUP, CPU_I287, ITYPE_FLOAD, "fild", { AMODE_M | OPTYPE_q | OP_SIGNED | OP_SRC, 0, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_PUSH, IGNORED }, // 0x05
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fbstp", { AMODE_M | OPTYPE_pb | OP_DST, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // 0x06
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fistp", { AMODE_M | OPTYPE_q | OP_SIGNED | OP_DST, OPTYPE_ST0 | OP_SRC, 0}, NOCOND, FPU_C1_MOD, FPU_STACK_POP, IGNORED }, // 0x07

    //
    // ModRM >= C0
    // Index 0x08-0x47 = ModRM 0xC0-0xFF
    //
    // C0-C7
    { NOINSTR }, // x0
    { NOINSTR }, // x1
    { NOINSTR }, // x2
    { NOINSTR }, // x3
    { NOINSTR }, // x4
    { NOINSTR }, // x5
    { NOINSTR }, // x6
    { NOINSTR }, // x7
    // C8-CF
    { NOINSTR }, // x8
    { NOINSTR }, // x9
    { NOINSTR }, // xA
    { NOINSTR }, // xB
    { NOINSTR }, // xC
    { NOINSTR }, // xD
    { NOINSTR }, // xE
    { NOINSTR }, // xF
    // D0-D7
    { NOINSTR }, // x0
    { NOINSTR }, // x1
    { NOINSTR }, // x2
    { NOINSTR }, // x3
    { NOINSTR }, // x4
    { NOINSTR }, // x5
    { NOINSTR }, // x6
    { NOINSTR }, // x7
    // D8-DF
    { NOINSTR }, // x8
    { NOINSTR }, // x9
    { NOINSTR }, // xA
    { NOINSTR }, // xB
    { NOINSTR }, // xC
    { NOINSTR }, // xD
    { NOINSTR }, // xE
    { NOINSTR }, // xF
    // E0-E7
    { NOGROUP, CPU_I287, ITYPE_FSTORE, "fnstsw", { OPTYPE_REG_AX | OP_DST, OPTYPE_FPU_STATUS | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
    { NOINSTR }, // x1
    { NOINSTR }, // x2
    { NOINSTR }, // x3
    { NOINSTR }, // x4
    { NOINSTR }, // x5
    { NOINSTR }, // x6
    { NOINSTR }, // x7
    // E8-EF
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x8
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x9
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xA
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xB
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xC
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xD
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xE
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fucomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // xF
    // F0-F7
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x0
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x1
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x2
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x3
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x4
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x5
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x6
    { NOGROUP, CPU_PENTIUM_PRO, ITYPE_FCOMP, "fcomip", { OPTYPE_STx | OP_SRC, OPTYPE_ST0 | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_PF_MOD | FLAG_CF_MOD | FPU_ALL_MOD, FPU_STACK_POP, IGNORED }, // x7
    // F8-FF
    { NOINSTR }, // x8
    { NOINSTR }, // x9
    { NOINSTR }, // xA
    { NOINSTR }, // xB
    { NOINSTR }, // xC
    { NOINSTR }, // xD
    { NOINSTR }, // xE
    { NOINSTR }  // xF
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// SSE opcodes
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

X86_OPCODE X86_SSE[0x300] =
{
    // prefix 0x66 (operand size)
        /* 0x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 1x */
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movupd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movupd", { AMODE_W | OPTYPE_pd | OP_DST, AMODE_V | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x1
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movlpd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_M | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x2
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movlpd", { AMODE_M | OPTYPE_q | OP_DST, AMODE_V | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x3
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "unpcklpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x4
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "unpckhpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movhpd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_M | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "mpvhpd", { AMODE_M | OPTYPE_q | OP_DST, AMODE_V | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 2x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movapd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x8
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movapd", { AMODE_W | OPTYPE_o | OP_DST, AMODE_V | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x9
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtpi2pd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xA
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movntpd", { AMODE_M | OPTYPE_o | OP_DST, AMODE_V | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xB
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvttpd2pi", { AMODE_P | OPTYPE_q | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtpd2pi", { AMODE_P | OPTYPE_q | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_CMP, "ucomisd", { AMODE_V | OPTYPE_sd | OP_SRC, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_CF_MOD | FLAG_PF_MOD | FLAG_OF_CLR | FLAG_SF_CLR | FLAG_AF_CLR, NOACTION, IGNORED }, // xE
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_CMP, "comisd", { AMODE_V | OPTYPE_pd | OP_SRC, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD | FLAG_CF_MOD | FLAG_PF_MOD | FLAG_OF_CLR | FLAG_SF_CLR | FLAG_AF_CLR, NOACTION, IGNORED }, // xF

        /* 3x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 4x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 5x */
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "movmskpd", { AMODE_G | OPTYPE_d | OP_DST, AMODE_VR | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "sqrtpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_AND, "andpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x4
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_AND, "andnpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_OR, "orpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_XOR, "xorpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x7
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "addpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x8
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MUL, "mulpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x9
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtpd2ps", { AMODE_V | OPTYPE_ps | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xA
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtps2dq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xB
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_SUB, "subpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "minpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_DIV, "divpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xE
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "maxpd", { AMODE_V | OPTYPE_pd | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xF

        /* 6x */
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "punpcklbw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "punpcklwd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x1
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "punpckldq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x2
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "packsswb", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x3
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pcmpgtb", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x4
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pcmpgtw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pcmpgtd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "packuswb", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x7
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "punpckhbw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x8
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "punpckhwd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x9
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "punpckhdq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xA
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "packssdw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xB
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "punpcklqdq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "punpckhqdq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_E | OPTYPE_dq | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xE
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movdqa", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xF

        /* 7x */
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pshufd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { X86_SSE2_Group_13, GROUP }, // x1
      { X86_SSE2_Group_14, GROUP }, // x2
      { X86_SSE2_Group_15, GROUP }, // x3
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_CMP, "pcmpeqb", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x4
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_CMP, "pcmpeqw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_CMP, "pcmpeqd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE3_ADD, "haddpd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE3_SUB, "hsubpd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movd", { AMODE_E | OPTYPE_dq | OP_DST, AMODE_V | OPTYPE_dq | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xE
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movdqa", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xF

        /* 8x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 9x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Ax */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Bx */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Cx */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_CMP, "cmppd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, // x2
      { NOINSTR }, // x3
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pinsrw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_E | OPTYPE_w | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x4
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pextrw", { AMODE_G | OPTYPE_d | OP_DST, AMODE_VR| OPTYPE_o | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "shufpd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Dx */
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE3, "addsubpd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psrlw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x1
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psrld", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x2
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psrlq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x3
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "paddq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x4
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MUL, "pmullw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movq", { AMODE_W | OPTYPE_q | OP_DST, AMODE_V | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pmovmskb", { AMODE_G | OPTYPE_d | OP_DST, AMODE_VR| OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x7
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_SUB, "psubusb", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x8
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_SUB, "psubusw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x9
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pminub", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xA
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_AND, "pand", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xB
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "paddusb", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "paddusw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pmaxub", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xE
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_AND, "pandn", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xF

        /* Ex */
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pavgb", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psraw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x1
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psrad", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x2
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pavgw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x3
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MUL, "pmulhuw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x4
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MUL, "pmulhw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvttpd2dq", { AMODE_V | OPTYPE_q | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movntdq", { AMODE_M | OPTYPE_o | OP_DST, AMODE_V | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x7
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_SUB, "psubsb", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x8
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_SUB, "psubsw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x9
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pminsw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xA
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_OR, "por", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xB
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "paddsb", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "paddsw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pmaxuw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xE
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_XOR, "pxor", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xF

        /* Fx */
      { NOINSTR }, // x0
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psllw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x1
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pslld", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x2
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psllq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x3
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MUL, "pmuludq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x4
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "pmaddwd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psadbw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "maskmovdqu", { AMODE_V | OPTYPE_o | OP_DST, AMODE_VR| OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x7
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_SUB, "psubb", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x8
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_SUB, "psubw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x9
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_SUB, "psubd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xA
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_SUB, "psubq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xB
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "paddb", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "paddw", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "paddd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xE
      { NOINSTR }, // xF

    // prefix 0xf2 (repne)
        /* 0x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 1x */
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movsd", { AMODE_V | OPTYPE_sdo | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movsd", { AMODE_W | OPTYPE_sd | OP_DST, AMODE_V | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x1
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE3_MOV, "movddup", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 2x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtsi2sd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_E | OPTYPE_dq | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xA
      { NOINSTR }, // xB
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvttsd2si", { AMODE_G | OPTYPE_dq | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtsd2si", { AMODE_G | OPTYPE_dq | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 3x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 4x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 5x */
      { NOINSTR }, // x0
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "sqrtsd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "addsd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x8
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MUL, "mulsd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x9
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtsd2ss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xA
      { NOINSTR }, // xB
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_SUB, "subsd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "minsd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_DIV, "divsd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xE
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "maxsd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xF

        /* 6x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movdqa", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xF

        /* 7x */
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pshuflw", { AMODE_V | OPTYPE_q | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE3_ADD, "haddps", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE3_SUB, "hsubps", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 8x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 9x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Ax */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Bx */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Cx */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_CMP, "cmpsd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_W | OPTYPE_sd | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Dx */
      { NOGROUP, CPU_PENTIUM3, ITYPE_SSE3, "addsubps", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movdq2q", { AMODE_P | OPTYPE_q | OP_DST, AMODE_VR | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Ex */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtpd2dq", { AMODE_V | OPTYPE_q | OP_DST, AMODE_W | OPTYPE_pd | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Fx */
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE3, "lddqu", { AMODE_V | OPTYPE_o | OP_DST, AMODE_M | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

    // prefix 0xf3 (rep)
        /* 0x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 1x */
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movss", { AMODE_V | OPTYPE_sso | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movss", { AMODE_W | OPTYPE_ss | OP_DST, AMODE_V | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x1
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE3_MOV, "movsldup", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE3_MOV, "movshdup", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 2x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "cvtsi2ss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_E | OPTYPE_dq | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xA
      { NOINSTR }, // xB
      { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "cvttss2si", { AMODE_G | OPTYPE_dq | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM3, ITYPE_SSE, "cvtss2si", { AMODE_G | OPTYPE_dq | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 3x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 4x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 5x */
      { NOINSTR }, // x0
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "sqrtss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x1
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "rsqrtss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x2
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "rcpss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_ADD, "addss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x8
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MUL, "mulss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x9
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtss2sd", { AMODE_V | OPTYPE_sd | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xA
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvttps2dq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_ps | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xB
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_SUB, "subss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xC
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "minss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xD
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_DIV, "divss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xE
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "maxss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xF

        /* 6x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movdqu", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xF

        /* 7x */
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pshufhw", { AMODE_V | OPTYPE_q | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movq", { AMODE_V | OPTYPE_q | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xE
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movdqu", { AMODE_W | OPTYPE_o | OP_DST, AMODE_V | OPTYPE_o | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // xF

        /* 8x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* 9x */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Ax */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Bx */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Cx */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOGROUP, CPU_PENTIUM3, ITYPE_SSE_CMP, "cmpss", { AMODE_V | OPTYPE_ss | OP_DST, AMODE_W | OPTYPE_ss | OP_SRC, AMODE_I | OPTYPE_b | OP_SRC }, NOCOND, FLAG_COMMON_MOD, NOACTION, IGNORED }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Dx */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2_MOV, "movq2dq", { AMODE_V | OPTYPE_o | OP_DST, AMODE_PR | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Ex */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "cvtdq2pd", { AMODE_V | OPTYPE_o | OP_DST, AMODE_W | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR }, // xF

        /* Fx */
      { NOINSTR }, // x0
      { NOINSTR }, // x1
      { NOINSTR }, // x2
      { NOINSTR }, // x3
      { NOINSTR }, // x4
      { NOINSTR }, // x5
      { NOINSTR }, // x6
      { NOINSTR }, // x7
      { NOINSTR }, // x8
      { NOINSTR }, // x9
      { NOINSTR }, // xA
      { NOINSTR }, // xB
      { NOINSTR }, // xC
      { NOINSTR }, // xD
      { NOINSTR }, // xE
      { NOINSTR } // xF
};

X86_OPCODE X86_SSE2_Group_13[24] = // 66/F2/F3 0F 71
{
    // prefix 0x66 (operand size)
        { NOINSTR }, /* 0x00 */
        { NOINSTR }, /* 0x01 */
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psrlw", { AMODE_VR | OPTYPE_o | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
        { NOINSTR }, /* 0x03 */
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psraw", { AMODE_VR | OPTYPE_o | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x04 */
        { NOINSTR }, /* 0x05 */
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psllw", { AMODE_VR | OPTYPE_o | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
        { NOINSTR }, /* 0x07 */

    // prefix 0xf2 (repne)
        { NOINSTR }, /* 0x00 */
        { NOINSTR }, /* 0x01 */
        { NOINSTR }, /* 0x02 */
        { NOINSTR }, /* 0x03 */
        { NOINSTR }, /* 0x04 */
        { NOINSTR }, /* 0x05 */
        { NOINSTR }, /* 0x06 */
        { NOINSTR }, /* 0x07 */

    // prefix 0xf3 (rep)
        { NOINSTR }, /* 0x00 */
        { NOINSTR }, /* 0x01 */
        { NOINSTR }, /* 0x02 */
        { NOINSTR }, /* 0x03 */
        { NOINSTR }, /* 0x04 */
        { NOINSTR }, /* 0x05 */
        { NOINSTR }, /* 0x06 */
        { NOINSTR }, /* 0x07 */
};

X86_OPCODE X86_SSE2_Group_14[24] = // 66/F2/F3 0F 72
{
    // prefix 0x66 (operand size)
        { NOINSTR }, /* 0x00 */
        { NOINSTR }, /* 0x01 */
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psrld", { AMODE_VR | OPTYPE_o | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
        { NOINSTR }, /* 0x03 */
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psrad", { AMODE_VR | OPTYPE_o | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x04 */
        { NOINSTR }, /* 0x05 */
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pslld", { AMODE_VR | OPTYPE_o | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
        { NOINSTR }, /* 0x07 */

    // prefix 0xf2 (repne)
        { NOINSTR }, /* 0x00 */
        { NOINSTR }, /* 0x01 */
        { NOINSTR }, /* 0x02 */
        { NOINSTR }, /* 0x03 */
        { NOINSTR }, /* 0x04 */
        { NOINSTR }, /* 0x05 */
        { NOINSTR }, /* 0x06 */
        { NOINSTR }, /* 0x07 */

    // prefix 0xf3 (rep)
        { NOINSTR }, /* 0x00 */
        { NOINSTR }, /* 0x01 */
        { NOINSTR }, /* 0x02 */
        { NOINSTR }, /* 0x03 */
        { NOINSTR }, /* 0x04 */
        { NOINSTR }, /* 0x05 */
        { NOINSTR }, /* 0x06 */
        { NOINSTR }, /* 0x07 */
};

X86_OPCODE X86_SSE2_Group_15[24] =
{
    // prefix 0x66 (operand size)
        { NOINSTR }, /* 0x00 */
        { NOINSTR }, /* 0x01 */
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psrlq", { AMODE_VR | OPTYPE_o | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x02 */
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psrldq", { AMODE_VR | OPTYPE_o | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x03 */
        { NOINSTR }, /* 0x04 */
        { NOINSTR }, /* 0x05 */
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "psllq", { AMODE_VR | OPTYPE_o | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x06 */
        { NOGROUP, CPU_PENTIUM4, ITYPE_SSE2, "pslldq", { AMODE_VR | OPTYPE_o | OP_DST, AMODE_I | OPTYPE_b | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0x07 */

    // prefix 0xf2 (repne)
        { NOINSTR }, /* 0x00 */
        { NOINSTR }, /* 0x01 */
        { NOINSTR }, /* 0x02 */
        { NOINSTR }, /* 0x03 */
        { NOINSTR }, /* 0x04 */
        { NOINSTR }, /* 0x05 */
        { NOINSTR }, /* 0x06 */
        { NOINSTR }, /* 0x07 */

    // prefix 0xf3 (rep)
        { NOINSTR }, /* 0x00 */
        { NOINSTR }, /* 0x01 */
        { NOINSTR }, /* 0x02 */
        { NOINSTR }, /* 0x03 */
        { NOINSTR }, /* 0x04 */
        { NOINSTR }, /* 0x05 */
        { NOINSTR }, /* 0x06 */
        { NOINSTR }, /* 0x07 */
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// 3DNow opcodes
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

X86_OPCODE X86_3DNOW_0F[0x100] =
{
    { NOINSTR }, /* 00 */
    { NOINSTR }, /* 01 */
    { NOINSTR }, /* 02 */
    { NOINSTR }, /* 03 */
    { NOINSTR }, /* 04 */
    { NOINSTR }, /* 05 */
    { NOINSTR }, /* 06 */
    { NOINSTR }, /* 07 */
    { NOINSTR }, /* 08 */
    { NOINSTR }, /* 09 */
    { NOINSTR }, /* 0A */
    { NOINSTR }, /* 0B */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pi2fw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0C */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pi2fd", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 0D */
    { NOINSTR }, /* 0E */
    { NOINSTR }, /* 0F */
    { NOINSTR }, /* 10 */
    { NOINSTR }, /* 11 */
    { NOINSTR }, /* 12 */
    { NOINSTR }, /* 13 */
    { NOINSTR }, /* 14 */
    { NOINSTR }, /* 15 */
    { NOINSTR }, /* 16 */
    { NOINSTR }, /* 17 */
    { NOINSTR }, /* 18 */
    { NOINSTR }, /* 19 */
    { NOINSTR }, /* 1A */
    { NOINSTR }, /* 1B */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pf2iw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 1C */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pf2id", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 1D */
    { NOINSTR }, /* 1E */
    { NOINSTR }, /* 1F */
    { NOINSTR }, /* 20 */
    { NOINSTR }, /* 21 */
    { NOINSTR }, /* 22 */
    { NOINSTR }, /* 23 */
    { NOINSTR }, /* 24 */
    { NOINSTR }, /* 25 */
    { NOINSTR }, /* 26 */
    { NOINSTR }, /* 27 */
    { NOINSTR }, /* 28 */
    { NOINSTR }, /* 29 */
    { NOINSTR }, /* 2A */
    { NOINSTR }, /* 2B */
    { NOINSTR }, /* 2C */
    { NOINSTR }, /* 2D */
    { NOINSTR }, /* 2E */
    { NOINSTR }, /* 2F */
    { NOINSTR }, /* 30 */
    { NOINSTR }, /* 31 */
    { NOINSTR }, /* 32 */
    { NOINSTR }, /* 33 */
    { NOINSTR }, /* 34 */
    { NOINSTR }, /* 35 */
    { NOINSTR }, /* 36 */
    { NOINSTR }, /* 37 */
    { NOINSTR }, /* 38 */
    { NOINSTR }, /* 39 */
    { NOINSTR }, /* 3A */
    { NOINSTR }, /* 3B */
    { NOINSTR }, /* 3C */
    { NOINSTR }, /* 3D */
    { NOINSTR }, /* 3E */
    { NOINSTR }, /* 3F */
    { NOINSTR }, /* 40 */
    { NOINSTR }, /* 41 */
    { NOINSTR }, /* 42 */
    { NOINSTR }, /* 43 */
    { NOINSTR }, /* 44 */
    { NOINSTR }, /* 45 */
    { NOINSTR }, /* 46 */
    { NOINSTR }, /* 47 */
    { NOINSTR }, /* 48 */
    { NOINSTR }, /* 49 */
    { NOINSTR }, /* 4A */
    { NOINSTR }, /* 4B */
    { NOINSTR }, /* 4C */
    { NOINSTR }, /* 4D */
    { NOINSTR }, /* 4E */
    { NOINSTR }, /* 4F */
    { NOINSTR }, /* 50 */
    { NOINSTR }, /* 51 */
    { NOINSTR }, /* 52 */
    { NOINSTR }, /* 53 */
    { NOINSTR }, /* 54 */
    { NOINSTR }, /* 55 */
    { NOINSTR }, /* 56 */
    { NOINSTR }, /* 57 */
    { NOINSTR }, /* 58 */
    { NOINSTR }, /* 59 */
    { NOINSTR }, /* 5A */
    { NOINSTR }, /* 5B */
    { NOINSTR }, /* 5C */
    { NOINSTR }, /* 5D */
    { NOINSTR }, /* 5E */
    { NOINSTR }, /* 5F */
    { NOINSTR }, /* 60 */
    { NOINSTR }, /* 61 */
    { NOINSTR }, /* 62 */
    { NOINSTR }, /* 63 */
    { NOINSTR }, /* 64 */
    { NOINSTR }, /* 65 */
    { NOINSTR }, /* 66 */
    { NOINSTR }, /* 67 */
    { NOINSTR }, /* 68 */
    { NOINSTR }, /* 69 */
    { NOINSTR }, /* 6A */
    { NOINSTR }, /* 6B */
    { NOINSTR }, /* 6C */
    { NOINSTR }, /* 6D */
    { NOINSTR }, /* 6E */
    { NOINSTR }, /* 6F */
    { NOINSTR }, /* 70 */
    { NOINSTR }, /* 71 */
    { NOINSTR }, /* 72 */
    { NOINSTR }, /* 73 */
    { NOINSTR }, /* 74 */
    { NOINSTR }, /* 75 */
    { NOINSTR }, /* 76 */
    { NOINSTR }, /* 77 */
    { NOINSTR }, /* 78 */
    { NOINSTR }, /* 79 */
    { NOINSTR }, /* 7A */
    { NOINSTR }, /* 7B */
    { NOINSTR }, /* 7C */
    { NOINSTR }, /* 7D */
    { NOINSTR }, /* 7E */
    { NOINSTR }, /* 7F */
    { NOINSTR }, /* 80 */
    { NOINSTR }, /* 81 */
    { NOINSTR }, /* 82 */
    { NOINSTR }, /* 83 */
    { NOINSTR }, /* 84 */
    { NOINSTR }, /* 85 */
    { NOINSTR }, /* 86 */
    { NOINSTR }, /* 87 */
    { NOINSTR }, /* 88 */
    { NOINSTR }, /* 89 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pfnacc", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 8A */
    { NOINSTR }, /* 8B */
    { NOINSTR }, /* 8C */
    { NOINSTR }, /* 8D */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pfpnacc", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 8E */
    { NOINSTR }, /* 8F */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW_CMP, "pfcmpge", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 90 */
    { NOINSTR }, /* 91 */
    { NOINSTR }, /* 92 */
    { NOINSTR }, /* 93 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pfmin", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 94 */
    { NOINSTR }, /* 95 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pfrcp", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 96 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pfrsqrt", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 97 */
    { NOINSTR }, /* 98 */
    { NOINSTR }, /* 99 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW_SUB, "pfsub", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 9A */
    { NOINSTR }, /* 9B */
    { NOINSTR }, /* 9C */
    { NOINSTR }, /* 9D */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW_ADD, "pfadd", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* 9E */
    { NOINSTR }, /* 9F */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW_CMP, "pfcmpgt", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* A0 */
    { NOINSTR }, /* A1 */
    { NOINSTR }, /* A2 */
    { NOINSTR }, /* A3 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pfmax", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* A4 */
    { NOINSTR }, /* A5 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pfrcpit1", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* A6 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pfrsqit1", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* A7 */
    { NOINSTR }, /* A8 */
    { NOINSTR }, /* A9 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW_SUB, "pfsubr", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* AA */
    { NOINSTR }, /* AB */
    { NOINSTR }, /* AC */
    { NOINSTR }, /* AD */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pfacc", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* AE */
    { NOINSTR }, /* AF */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW_CMP, "pfcmpeq", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* B0 */
    { NOINSTR }, /* B1 */
    { NOINSTR }, /* B2 */
    { NOINSTR }, /* B3 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW_MUL, "pfmul", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* B4 */
    { NOINSTR }, /* B5 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pfrcpit2", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* B6 */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW_MUL, "pmulhrw", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* B7 */
    { NOINSTR },         /* B8 */
    { NOINSTR }, /* B9 */
    { NOINSTR }, /* BA */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW_XCHG, "pswapd", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* BB */
    { NOINSTR }, /* BC */
    { NOINSTR }, /* BD */
    { NOINSTR }, /* BE */
    { NOGROUP, CPU_AMD_K6_2, ITYPE_3DNOW, "pavgb", { AMODE_P | OPTYPE_q | OP_DST, AMODE_Q | OPTYPE_q | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, /* BF */
    { NOINSTR }, /* C0 */
    { NOINSTR }, /* C1 */
    { NOINSTR }, /* C2 */
    { NOINSTR }, /* C3 */
    { NOINSTR }, /* C4 */
    { NOINSTR }, /* C5 */
    { NOINSTR }, /* C6 */
    { NOINSTR }, /* C7 */
    { NOINSTR }, /* C8 */
    { NOINSTR }, /* C9 */
    { NOINSTR }, /* CA */
    { NOINSTR }, /* CB */
    { NOINSTR }, /* CC */
    { NOINSTR }, /* CD */
    { NOINSTR }, /* CE */
    { NOINSTR }, /* CF */
    { NOINSTR }, /* D0 */
    { NOINSTR }, /* D1 */
    { NOINSTR }, /* D2 */
    { NOINSTR }, /* D3 */
    { NOINSTR }, /* D4 */
    { NOINSTR }, /* D5 */
    { NOINSTR }, /* D6 */
    { NOINSTR }, /* D7 */
    { NOINSTR }, /* D8 */
    { NOINSTR }, /* D9 */
    { NOINSTR }, /* DA */
    { NOINSTR }, /* DB */
    { NOINSTR }, /* DC */
    { NOINSTR }, /* DD */
    { NOINSTR }, /* DE */
    { NOINSTR }, /* DF */
    { NOINSTR }, /* E0 */
    { NOINSTR }, /* E1 */
    { NOINSTR }, /* E2 */
    { NOINSTR }, /* E3 */
    { NOINSTR }, /* E4 */
    { NOINSTR }, /* E5 */
    { NOINSTR }, /* E6 */
    { NOINSTR }, /* E7 */
    { NOINSTR }, /* E8 */
    { NOINSTR }, /* E9 */
    { NOINSTR }, /* EA */
    { NOINSTR }, /* EB */
    { NOINSTR }, /* EC */
    { NOINSTR }, /* ED */
    { NOINSTR }, /* EE */
    { NOINSTR }, /* EF */
    { NOINSTR }, /* F0 */
    { NOINSTR }, /* F1 */
    { NOINSTR }, /* F2 */
    { NOINSTR }, /* F3 */
    { NOINSTR }, /* F4 */
    { NOINSTR }, /* F5 */
    { NOINSTR }, /* F6 */
    { NOINSTR }, /* F7 */
    { NOINSTR }, /* F8 */
    { NOINSTR }, /* F9 */
    { NOINSTR }, /* FA */
    { NOINSTR }, /* FB */
    { NOINSTR }, /* FC */
    { NOINSTR }, /* FD */
    { NOINSTR }, /* FE */
    { NOINSTR }    /* FF */
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// 64-bit replacement opcodes
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

X86_OPCODE X86_Opcode_63[2] =
{
    { NOGROUP, CPU_I386, ITYPE_SYSTEM, "arpl", { AMODE_E | OPTYPE_w | OP_SRC, AMODE_G | OPTYPE_w | OP_SRC, 0 }, NOCOND, FLAG_ZF_MOD, NOACTION, IGNORED }, // !ARCH_AMD64
    { NOGROUP, CPU_AMD64, ITYPE_MOV, "movsxd", { AMODE_G | OPTYPE_v | OP_SIGNED | OP_DST, AMODE_E | OPTYPE_d | OP_SIGNED | OP_SRC, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED } // ARCH_AMD64
};

X86_OPCODE X86_Opcode_0F05[2] =
{
    { NOGROUP, CPU_AMD_K6_2, ITYPE_SYSCALL, "syscall", { OPTYPE_STAR_MSR | OP_MSR | OP_SRC, OPTYPE_CSTAR_MSR | OP_MSR | OP_SRC, OPTYPE_FMASK_MSR | OP_MSR | OP_SRC }, NOCOND, FLAG_ZF_MOD, NOACTION, IGNORED }, // !ARCH_AMD64
    { NOGROUP, CPU_AMD64, ITYPE_SYSCALL, "syscall", { OPTYPE_STAR_MSR | OP_MSR | OP_SRC, OPTYPE_LSTAR_MSR | OP_MSR | OP_SRC, OPTYPE_FMASK_MSR | OP_MSR | OP_SRC }, NOCOND, NOCHANGE, NOACTION, IGNORED } // ARCH_AMD64
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// Other 3 byte opcodes
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

// Three byte opcodes where the third opcode byte is ModRM
X86_OPCODE X86_0F01_ModRM[0x100] = 
{
    /* 0x */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* 1x */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* 2x */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* 3x */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* 4x */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* 5x */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* 6x */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* 7x */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* 8x */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* 9x */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* Ax */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* Bx */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* Cx */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { NOGROUP, CPU_PRESCOTT, ITYPE_SYSTEM, "monitor", NOARGS, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x8
  { NOGROUP, CPU_PRESCOTT, ITYPE_SYSTEM, "mwait", NOARGS, NOCOND, NOCHANGE, SERIALIZE_ALL, IGNORED }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* Dx */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* Ex */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { X86_Group_7, GROUP }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }, // xF

    /* Fx */
  { X86_Group_7, GROUP }, // x0
  { X86_Group_7, GROUP }, // x1
  { X86_Group_7, GROUP }, // x2
  { X86_Group_7, GROUP }, // x3
  { X86_Group_7, GROUP }, // x4
  { X86_Group_7, GROUP }, // x5
  { X86_Group_7, GROUP }, // x6
  { X86_Group_7, GROUP }, // x7
  { NOGROUP, CPU_AMD64, ITYPE_SYSTEM, "swapgs", { OPTYPE_KERNELBASE_MSR | OP_MSR | OP_SRC, 0, 0 }, NOCOND, NOCHANGE, NOACTION, IGNORED }, // x8
  { X86_Group_7, GROUP }, // x9
  { X86_Group_7, GROUP }, // xA
  { X86_Group_7, GROUP }, // xB
  { X86_Group_7, GROUP }, // xC
  { X86_Group_7, GROUP }, // xD
  { X86_Group_7, GROUP }, // xE
  { X86_Group_7, GROUP }  // xF
};

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// Sanity checking tables
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

#define S2 1 // SSE2
#define S3 2 // SSE3
uint8_t X86_ModRM_1[0x100] =
{
  //      x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF
  /* 0x */ 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 
  /* 1x */ 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 
  /* 2x */ 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 
  /* 3x */ 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 
  /* 4x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  /* 5x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  /* 6x */ 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 
  /* 7x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  /* 8x */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  /* 9x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  /* Ax */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  /* Bx */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  /* Cx */ 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
  /* Dx */ 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 
  /* Ex */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  /* Fx */ 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1
};
uint8_t X86_ModRM_2[0x100] =
{
  //      x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF
  /* 0x */ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 
  /* 1x */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
  /* 2x */ 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 
  /* 3x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  /* 4x */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  /* 5x */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  /* 6x */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  /* 7x */ 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 
  /* 8x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  /* 9x */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  /* Ax */ 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 
  /* Bx */ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 
  /* Cx */ 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 
  /* Dx */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  /* Ex */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  /* Fx */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0
};

uint8_t X86_SSE_2[0x100] = 
{
  /*      x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF        */
  /* 0x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 1x */ 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 2x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0,
  /* 3x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 4x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 5x */ 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
  /* 6x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
  /* 7x */ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
  /* 8x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 9x */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* Ax */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* Bx */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* Cx */ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* Dx */ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* Ex */ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* Fx */ 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0
};

// Indicates if a LOCK prefix is allowed
// The following are allowed:
// add, adc, and, btc, btr, bts, cmpxchg, cmpxchg8, dec, inc, 
// neg, not, or, sbb, sub, xor, xadd, xchg
#define GR 2
uint8_t X86_LockPrefix_1[0x100] =
{
  //       x0  x1  x2  x3  x4  x5  x6  x7  x8  x9  xA  xB  xC  xD  xE  xF
  /* 0x */  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0, 
  /* 1x */  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0, 
  /* 2x */  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0,  0,  0, 
  /* 3x */  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 4x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 5x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 6x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 7x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 8x */ GR, GR, GR, GR,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 9x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* Ax */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* Bx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* Cx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* Dx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* Ex */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* Fx */  0,  0,  0,  0,  0,  0, GR, GR,  0,  0,  0,  0,  0,  0, GR, GR
};

uint8_t X86_LockPrefix_2[0x100] =
{
  //       x0  x1  x2  x3  x4  x5  x6  x7  x8  x9  xA  xB  xC  xD  xE  xF
  /* 0x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 1x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 2x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 3x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 4x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 5x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 6x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 7x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 8x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* 9x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* Ax */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0, 
  /* Bx */  1,  1,  0,  1,  0,  0,  0,  0,  0,  0, GR,  1,  0,  0,  0,  0, 
  /* Cx */  1,  1,  0,  0,  0,  0,  0, GR,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* Dx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* Ex */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
  /* Fx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

uint8_t X86_LockPrefix_Groups[17][8] =
{
//  x0  x1  x2  x3  x4  x5  x6  x7
    {  1,  1,  1,  1,  1,  1,  1,  0 }, // group 1
    {  0,  0,  0,  0,  0,  0,  0,  0 }, // group 2
    {  0,  0,  1,  1,  0,  0,  0,  0 }, // group 3
    {  1,  1,  0,  0,  0,  0,  0,  0 }, // group 4
    {  1,  1,  0,  0,  0,  0,  0,  0 }, // group 5
    {  0,  0,  0,  0,  0,  0,  0,  0 }, // group 6
    {  0,  0,  0,  0,  0,  0,  0,  0 }, // group 7
    {  0,  0,  0,  0,  1,  1,  1,  1 }, // group 8
    {  0,  1,  0,  0,  0,  0,  0,  0 }, // group 9
    {  0,  0,  0,  0,  0,  0,  0,  0 }, // group 10
    {  0,  0,  0,  0,  0,  0,  0,  0 }, // group 11
    {  0,  0,  0,  0,  0,  0,  0,  0 }, // group 12
    {  0,  0,  0,  0,  0,  0,  0,  0 }, // group 13
    {  0,  0,  0,  0,  0,  0,  0,  0 }, // group 14
    {  0,  0,  0,  0,  0,  0,  0,  0 }, // group 15
    {  0,  0,  0,  0,  0,  0,  0,  0 }, // group 16
    {  0,  0,  0,  0,  0,  0,  0,  0 }, // group 17
};

#define X86_MAX_GROUP 19
uint8_t X86_Groups_1[0x100] = // one-byte opcodes
{
    /*       x0  x1  x2  x3  x4  x5  x6  x7  x8  x9  xA  xB  xC  xD  xE  xF */
    /* 0x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 0x */
    /* 1x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 1x */
    /* 2x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 2x */
    /* 3x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 3x */
    /* 4x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 4x */
    /* 5x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 5x */
    /* 6x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 6x */
    /* 7x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 7x */
    /* 8x */  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10, /* 8x */
    /* 9x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 9x */
    /* Ax */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Ax */
    /* Bx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Bx */
    /* Cx */  2,  2,  0,  0,  0,  0, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0, /* Cx */
    /* Dx */  2,  2,  2,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Dx */
    /* Ex */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Ex */
    /* Fx */  0,  0,  0,  0,  0,  0,  3,  3,  0,  0,  0,  0,  0,  0,  4,  5  /* Fx */
};

// 19 = Group P
// 20 = 3DNow
uint8_t X86_Groups_2[0x100] = // two-byte opcodes
{
    /*       x0  x1  x2  x3  x4  x5  x6  x7  x8  x9  xA  xB  xC  xD  xE  xF */
    /* 0x */  6,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0, 19, /* 0x */
    /* 1x */  0,  0,  0,  0,  0,  0,  0,  0, 17,  0,  0,  0,  0,  0,  0,  0, /* 1x */
    /* 2x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 2x */
    /* 3x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 3x */
    /* 4x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 4x */
    /* 5x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 5x */
    /* 6x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 6x */
    /* 7x */  0, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 7x */
    /* 8x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 8x */
    /* 9x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 9x */
    /* Ax */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 16,  0, /* Ax */
    /* Bx */  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  8,  0,  0,  0,  0,  0, /* Bx */
    /* Cx */  0,  0,  0,  0,  0,  0,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0, /* Cx */
    /* Dx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Dx */
    /* Ex */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Ex */
    /* Fx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0  /* Fx */
};

// Indicate which 1-byte opcodes are invalid with a 16-bit operand size
uint8_t X86_Invalid_Op16_1[0x100] = 
{
    /*       x0  x1  x2  x3  x4  x5  x6  x7  x8  x9  xA  xB  xC  xD  xE  xF */
    /* 0x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 0x */
    /* 1x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 1x */
    /* 2x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 2x */
    /* 3x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 3x */
    /* 4x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 4x */
    /* 5x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 5x */
    /* 6x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 6x */
    /* 7x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 7x */
    /* 8x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 8x */
    /* 9x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 9x */
    /* Ax */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Ax */
    /* Bx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Bx */
    /* Cx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Cx */
    /* Dx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Dx */
    /* Ex */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Ex */
    /* Fx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0  /* Fx */
};

// Indicate which 2-byte opcodes are invalid with a 16-bit operand size
uint8_t X86_Invalid_Op16_2[0x100] =
{
    /*       x0  x1  x2  x3  x4  x5  x6  x7  x8  x9  xA  xB  xC  xD  xE  xF */
    /* 0x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 0x */
    /* 1x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 1x */
    /* 2x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 2x */
    /* 3x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 3x */
    /* 4x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 4x */
    /* 5x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 5x */
    /* 6x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 6x */
    /* 7x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 7x */
    /* 8x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 8x */
    /* 9x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 9x */
    /* Ax */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Ax */
    /* Bx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Bx */
    /* Cx */  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1, /* Cx */
    /* Dx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Dx */
    /* Ex */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Ex */
    /* Fx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0  /* Fx */
};

// Indicate which 1-byte opcodes are invalid with a 64-bit address size
uint8_t X86_Invalid_Addr64_1[0x100] =
{
    /*       x0  x1  x2  x3  x4  x5  x6  x7  x8  x9  xA  xB  xC  xD  xE  xF */
    /* 0x */  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  1,  0, /* 0x */
    /* 1x */  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0, /* 1x */
    /* 2x */  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  1, /* 2x */
    /* 3x */  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  1, /* 3x */
    /* 4x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 4x */
    /* 5x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 5x */
    /* 6x */  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 6x */
    /* 7x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 7x */
    /* 8x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 8x */
    /* 9x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0, /* 9x */
    /* Ax */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Ax */
    /* Bx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Bx */
    /* Cx */  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0, /* Cx */
    /* Dx */  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Dx */
    /* Ex */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0, /* Ex */
    /* Fx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0  /* Fx */
};

// Indicate which 2-byte opcodes are invalid with a 64-bit address size
uint8_t X86_Invalid_Addr64_2[0x100] =
{
    /*       x0  x1  x2  x3  x4  x5  x6  x7  x8  x9  xA  xB  xC  xD  xE  xF */
    /* 0x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 0x */
    /* 1x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 1x */
    /* 2x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 2x */
    /* 3x */  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 3x */
    /* 4x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 4x */
    /* 5x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 5x */
    /* 6x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 6x */
    /* 7x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 7x */
    /* 8x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 8x */
    /* 9x */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* 9x */
    /* Ax */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Ax */
    /* Bx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Bx */
    /* Cx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Cx */
    /* Dx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Dx */
    /* Ex */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, /* Ex */
    /* Fx */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0  /* Fx */
};

#endif // DISASM_X86_TABLES