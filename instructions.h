#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include "chip8.h"
#include "utils.h"
/*
 * Категории инструкций
 */
#define CAT_SYS        0x0
#define CAT_JMP        0x1
#define CAT_CALL       0x2
#define CAT_SE         0x3
#define CAT_SNE        0x4
#define CAT_SE_REG     0x5
#define CAT_LD         0x6
#define CAT_ADD        0x7
#define CAT_REG_OP     0x8
#define CAT_SNE_REG    0x9
#define CAT_LD_I       0xA
#define CAT_JMP_V0     0xB
#define CAT_RND        0xC
#define CAT_DRW        0xD
#define CAT_SKP        0xE
#define CAT_EXTENDED   0xF

/*
 * Специальные инструкции
 */

/* System */
#define SYS_ADDR(nnn)  (0x0000 | ((nnn) & NNN_MASK))
#define CLS            0x00E0
#define RET            0x00EE

/* Контроль потока выполнения программы */
#define JMP_ADDR(nnn)  (0x1000 | ((nnn) & NNN_MASK))
#define CALL_ADDR(nnn) (0x2000 | ((nnn) & NNN_MASK))

/* Условные инструкции */
#define SE_VX_KK(x, kk)    (0x3000 | (((x) << 8) & X_MASK) | ((kk) & NN_MASK))
#define SNE_VX_KK(x, kk)   (0x4000 | (((x) << 8) & X_MASK) | ((kk) & NN_MASK))
#define SE_VX_VY(x, y)     (0x5000 | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK))
#define SNE_VX_VY(x, y)    (0x9000 | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK))

/* Арифметические инструкции */
#define LD_VX_KK(x, kk)    (0x6000 | (((x) << 8) & X_MASK) | ((kk) & NN_MASK))
#define ADD_VX_KK(x, kk)   (0x7000 | (((x) << 8) & X_MASK) | ((kk) & NN_MASK))

/* Операции с регистром */
#define LD_VX_VY(x, y)     (0x8000 | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK))
#define OR_VX_VY(x, y)     (0x8001 | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK))
#define AND_VX_VY(x, y)    (0x8002 | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK))
#define XOR_VX_VY(x, y)    (0x8003 | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK))
#define ADD_VX_VY(x, y)    (0x8004 | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK))
#define SUB_VX_VY(x, y)    (0x8005 | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK))
#define SHR_VX_VY(x, y)    (0x8006 | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK))
#define SUBN_VX_VY(x, y)   (0x8007 | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK))
#define SHL_VX_VY(x, y)    (0x800E | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK))

/* Операция с памятью */
#define LD_I_ADDR(nnn)     (0xA000 | ((nnn) & NNN_MASK))
#define JMP_V0_ADDR(nnn)   (0xB000 | ((nnn) & NNN_MASK))
#define RND_VX_KK(x, kk)   (0xC000 | (((x) << 8) & X_MASK) | ((kk) & NN_MASK))
#define DRW_VX_VY_N(x, y, n) (0xD000 | (((x) << 8) & X_MASK) | (((y) << 4) & Y_MASK) | ((n) & N_MASK))

/* Ввод */
#define SKP_VX(x)          (0xE09E | (((x) << 8) & X_MASK))
#define SKNP_VX(x)         (0xE0A1 | (((x) << 8) & X_MASK))

/* Расширенные инструкции */
#define LD_VX_DT(x)        (0xF007 | (((x) << 8) & X_MASK))
#define LD_VX_K(x)         (0xF00A | (((x) << 8) & X_MASK))
#define LD_DT_VX(x)        (0xF015 | (((x) << 8) & X_MASK))
#define LD_ST_VX(x)        (0xF018 | (((x) << 8) & X_MASK))
#define ADD_I_VX(x)        (0xF01E | (((x) << 8) & X_MASK))
#define LD_F_VX(x)         (0xF029 | (((x) << 8) & X_MASK))
#define LD_B_VX(x)         (0xF033 | (((x) << 8) & X_MASK))
#define LD_I_VX(x)         (0xF055 | (((x) << 8) & X_MASK))
#define LD_VX_I(x)         (0xF065 | (((x) << 8) & X_MASK))


const uint8_t *load_rom(char *file_name, size_t *file_size);
void handle_extended_cat(Chip_8 *chip_8, uint16_t full_instruction);
void handle_reg_op(Chip_8 *chip_8, uint16_t full_instruction);
void load_sprite_in_display_buffer(Chip_8 *chip_8, const uint8_t x, const uint8_t y, const uint8_t n, uint16_t sprite_address);
int execute_instructions(Chip_8 *chip_8);

#endif // INSTRUCTIONS_H