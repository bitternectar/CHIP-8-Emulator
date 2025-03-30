//
// Created by Bitter Nectar on 29.03.2025.
//

#ifndef UTILS_H
#define UTILS_H

/*
 * Битовые маски
 */
#define OPCODE_MASK 0xF000
#define X_MASK      0x0F00
#define Y_MASK      0x00F0
#define N_MASK      0x000F
#define NN_MASK     0x00FF
#define NNN_MASK    0x0FFF

#define OPCODE(op) ((op) >> 12)
#define X(op)      (((op) & X_MASK) >> 8)
#define Y(op)      (((op) & Y_MASK) >> 4)
#define N(op)      ((op) & N_MASK)
#define NN(op)     ((op) & NN_MASK)
#define NNN(op)    ((op) & NNN_MASK)

/*
 * Debug logging control
 * Set to 1 to enable logging, 0 to disable
 */
#define CHIP8_DEBUG 1

/*
 * Debug logging macros
 */
#if CHIP8_DEBUG
    #define LOG_INSTR(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define LOG_INSTR(fmt, ...)
#endif

/*
 * Instruction logging macros
 */
#define LOG_SYS(nnn)        LOG_INSTR("SYS 0x%03X\n", (nnn))
#define LOG_CLS()           LOG_INSTR("CLS\n")
#define LOG_RET()           LOG_INSTR("RET\n")
#define LOG_JMP(nnn)        LOG_INSTR("JMP 0x%03X\n", (nnn))
#define LOG_CALL(nnn)       LOG_INSTR("CALL 0x%03X\n", (nnn))
#define LOG_SE_VX_KK(x, kk) LOG_INSTR("SE V%X, 0x%02X\n", (x), (kk))
#define LOG_SNE_VX_KK(x, kk) LOG_INSTR("SNE V%X, 0x%02X\n", (x), (kk))
#define LOG_SE_VX_VY(x, y)  LOG_INSTR("SE V%X, V%X\n", (x), (y))
#define LOG_SNE_VX_VY(x, y) LOG_INSTR("SNE V%X, V%X\n", (x), (y))
#define LOG_LD_VX_KK(x, kk) LOG_INSTR("LD V%X, 0x%02X; V%X = %d\n", (x), (kk), (x), (kk))
#define LOG_ADD_VX_KK(x, kk) LOG_INSTR("ADD V%X, 0x%02X; V%X += %d\n", (x), (kk), (x), (kk))
#define LOG_LD_VX_VY(x, y)  LOG_INSTR("LD V%X, V%X; V%X = V%X (%d)\n", (x), (y), (x), (y), (y))
#define LOG_OR_VX_VY(x, y)  LOG_INSTR("OR V%X, V%X; V%X |= V%X\n", (x), (y), (x), (y))
#define LOG_AND_VX_VY(x, y) LOG_INSTR("AND V%X, V%X; V%X &= V%X\n", (x), (y), (x), (y))
#define LOG_XOR_VX_VY(x, y) LOG_INSTR("XOR V%X, V%X; V%X ^= V%X\n", (x), (y), (x), (y))
#define LOG_ADD_VX_VY(x, y) LOG_INSTR("ADD V%X, V%X; V%X += V%X\n", (x), (y), (x), (y))
#define LOG_SUB_VX_VY(x, y) LOG_INSTR("SUB V%X, V%X; V%X -= V%X\n", (x), (y), (x), (y))
#define LOG_SHR_VX(x)       LOG_INSTR("SHR V%X; V%X >>= 1\n", (x), (x))
#define LOG_SUBN_VX_VY(x, y) LOG_INSTR("SUBN V%X, V%X; V%X = V%X - V%X\n", (x), (y), (x), (y), (x))
#define LOG_SHL_VX(x)       LOG_INSTR("SHL V%X; V%X <<= 1\n", (x), (x))
#define LOG_LD_I(nnn)       LOG_INSTR("LD I, 0x%03X; I = %d\n", (nnn), (nnn))
#define LOG_JMP_V0(nnn)     LOG_INSTR("JMP V0, 0x%03X; PC = V0 + %d\n", (nnn), (nnn))
#define LOG_RND_VX_KK(x, kk) LOG_INSTR("RND V%X, 0x%02X; V%X = random & 0x%02X\n", (x), (kk), (x), (kk))
#define LOG_DRW_VX_VY_N(x, y, n) LOG_INSTR("DRW V%X, V%X, %d; Draw at (%d,%d) %d bytes\n", (x), (y), (n), (x), (y), (n))
#define LOG_SKP_VX(x)       LOG_INSTR("SKP V%X; Skip if key V%X (%d) pressed\n", (x), (x), (x))
#define LOG_SKNP_VX(x)      LOG_INSTR("SKNP V%X; Skip if key V%X (%d) not pressed\n", (x), (x), (x))
#define LOG_LD_VX_DT(x)     LOG_INSTR("LD V%X, DT; V%X = delay timer (%d)\n", (x), (x), (x))
#define LOG_LD_VX_K(x)      LOG_INSTR("LD V%X, K; Wait for key press -> V%X\n", (x), (x))
#define LOG_LD_DT_VX(x)     LOG_INSTR("LD DT, V%X; delay timer = V%X (%d)\n", (x), (x), (x))
#define LOG_LD_ST_VX(x)     LOG_INSTR("LD ST, V%X; sound timer = V%X (%d)\n", (x), (x), (x))
#define LOG_ADD_I_VX(x)     LOG_INSTR("ADD I, V%X; I += V%X (%d)\n", (x), (x), (x))
#define LOG_LD_F_VX(x)      LOG_INSTR("LD F, V%X; I = sprite addr for digit V%X (%d)\n", (x), (x), (x))
#define LOG_LD_B_VX(x)      LOG_INSTR("LD B, V%X; Store BCD of V%X at I,I+1,I+2\n", (x), (x))
#define LOG_LD_I_VX(x)      LOG_INSTR("LD [I], V%X; Store V0-V%X in memory at I\n", (x), (x))
#define LOG_LD_VX_I(x)      LOG_INSTR("LD V%X, [I]; Load V0-V%X from memory at I\n", (x), (x))

#endif //UTILS_H
