//
// Created by Bitter Nectar on 30.03.2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"
#include "utils.h"

// Загрузка ROM
const uint8_t *load_rom(char *file_name, size_t *file_size) {
    FILE *rom = fopen(file_name, "rb");
    if (!rom) {
        perror("Ошибка открытия файла");
        return NULL;
    }

    fseek(rom, 0, SEEK_END);
    *file_size = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    uint8_t *buffer = malloc(*file_size);
    if (!buffer) {
        fclose(rom);
        return NULL;
    }

    size_t read = fread(buffer, 1, *file_size, rom);
    fclose(rom);

    if (read != *file_size) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

// Расширенные операции
void handle_extended_cat(Chip_8 *chip_8, uint16_t full_instruction) {
    const uint8_t x = X(full_instruction);

    if (full_instruction == LD_VX_DT(x)) {
        chip_8->V[x] = chip_8->DT;
        LOG_LD_VX_DT(x);
        return;
    }

    if (full_instruction == LD_VX_K(x)) {
        const uint8_t k = getchar();
        chip_8->V[x] = k;
        LOG_LD_VX_K(x);
        return;
    }

    if (full_instruction == LD_DT_VX(x)) {
        chip_8->DT = chip_8->V[x];
        LOG_LD_DT_VX(x);
        return;
    }

    if (full_instruction == LD_ST_VX(x)) {
        chip_8->ST = chip_8->V[x];
        LOG_LD_ST_VX(x);
        return;
    }

    if (full_instruction == ADD_I_VX(x)) {
        chip_8->I += chip_8->V[x];
        ADD_I_VX(x);
        return;
    }

    if (full_instruction == LD_F_VX(x)) {
        chip_8->RAM[chip_8->I] = chip_8->RAM[chip_8->V[x]];
        LOG_LD_F_VX(x);
        return;
    }

    if (full_instruction == LD_B_VX(x)) {
        const uint8_t b = chip_8->V[x] % 100;
        const uint8_t c = (chip_8->V[x] - b * 100) % 10;
        const uint8_t d = (chip_8->V[x] - b * 100 - c * 10) % 10;

        chip_8->RAM[chip_8->I] = b;
        chip_8->RAM[chip_8->I + 1] = c;
        chip_8->RAM[chip_8->I + 2] = d;

        LOG_LD_B_VX(x);
        return;
    }

    if (full_instruction == LD_I_VX(x)) {
        for (uint8_t i = 0; i <= x; i++) {
            const uint8_t value = chip_8->V[x];
            chip_8->RAM[chip_8->I + i] = value;
        }
        LOG_LD_I_VX(x);
        return;
    }

    if (full_instruction == LD_VX_I(x)) {
        for (uint8_t i = 0; i <= x; i++) {
            const uint8_t value = chip_8->RAM[chip_8->I + i];
            chip_8->V[i] = value;
        }
        LOG_LD_VX_I(x);
    }
}

// Операции с двумя регистрами
void handle_reg_op(Chip_8 *chip_8, uint16_t full_instruction) {
    const uint8_t reg_op = N(full_instruction);
    const uint8_t x = X(full_instruction);
    const uint8_t y = Y(full_instruction);
    switch (reg_op) {
        case 0: chip_8->V[x] = chip_8->V[y]; LOG_LD_VX_VY(x, y); break;
        case 1: chip_8->V[x] |= chip_8->V[y]; LOG_OR_VX_VY(x, y); break;
        case 2: chip_8->V[x] &= chip_8->V[y]; LOG_AND_VX_VY(x, y); break;
        case 3: chip_8->V[x] ^= chip_8->V[y]; LOG_XOR_VX_VY(x, y); break;
        case 4: {
            uint16_t temp = chip_8->V[x] + chip_8->V[y];
            chip_8->VF = (temp > 255) ? 1 : 0;
            chip_8->V[x] = temp & 0xFF;
            LOG_ADD_VX_VY(x, y);
            break;
        }
        case 5: {
            chip_8->VF = (chip_8->V[x] >= chip_8->V[y]) ? 1 : 0;
            chip_8->V[x] -= chip_8->V[y];
            LOG_SUB_VX_VY(x, y);
            break;
        }
        case 6: {
            chip_8->VF = chip_8->V[x] & 0x1;
            chip_8->V[x] >>= 1;
            LOG_SHR_VX(x);
            break;
        }
        case 7: {
            chip_8->VF = (chip_8->V[y] >= chip_8->V[x]) ? 1 : 0;
            chip_8->V[x] = chip_8->V[y] - chip_8->V[x];
            LOG_SUBN_VX_VY(x, y);
            break;
        }
        case 0xE: {
            chip_8->VF = (chip_8->V[x] >> 7) & 0x1;
            chip_8->V[x] <<= 1;
            LOG_SHL_VX(x);
            break;
        }
        default: UNKNOW_INSTRUCTION;
    }
}

// Загрузка спрайта в буфер дисплея
void load_sprite_in_display_buffer(Chip_8 *chip_8, const uint8_t x, const uint8_t y, const uint8_t n, uint16_t sprite_address) {
    chip_8->VF = 0;
    for (uint8_t i = 0; i < n; i++) {
        uint8_t sprite_line = chip_8->RAM[sprite_address + i];
        for (uint8_t j = 0; j < 8; j++) {
            uint8_t y_coord = (chip_8->V[y] + i) % DISPLAY_HEIGHT;
            uint8_t x_coord = (chip_8->V[x] + j) % DISPLAY_WIDTH;
            uint8_t px_value = (sprite_line >> (7 - j)) & 0x1;
            uint8_t old_px = chip_8->display_buffer[y_coord][x_coord];
            chip_8->display_buffer[y_coord][x_coord] ^= px_value;
            if (old_px == 1 && chip_8->display_buffer[y_coord][x_coord] == 0) {
                chip_8->VF = 1;
            }
        }
    }
}

// Исполнение инструкций
int execute_instructions(Chip_8 *chip_8) {
    if (chip_8->PC >= RAM_SIZE || chip_8->PC < START_RAM_ADDR) {
        printf("PC вышел за пределы памяти.\n");
        return -1;
    }

    const uint16_t address = chip_8->PC;
    const uint16_t full_instruction = (chip_8->RAM[address] << 8) | chip_8->RAM[address + 1];
    const uint8_t opcode = OPCODE(full_instruction);

    switch (full_instruction) {
        case CLS: {
            memset(chip_8->display_buffer, 0, sizeof(chip_8->display_buffer));
            LOG_CLS();
            break;
        }
        case RET: {
            if (chip_8->SP == 0) {
                printf("Стек пуст.\n");
                return -1;
            }
            chip_8->PC = chip_8->stack[--chip_8->SP];
            LOG_RET();
            return 0;
        }
    }

    switch (opcode) {
        case CAT_SYS: break;  // Игнорируем
        case CAT_JMP: chip_8->PC = NNN(full_instruction); LOG_JMP(NNN(full_instruction)); return 0;
        case CAT_CALL: {
            if (chip_8->SP >= STACK_SIZE) {
                printf("Переполнение стека.\n");
                return -1;
            }
            chip_8->stack[chip_8->SP++] = chip_8->PC + PC_STEP;
            chip_8->PC = NNN(full_instruction);
            LOG_CALL(NNN(full_instruction));
            return 0;
        }
        case CAT_SE: {
            if (chip_8->V[X(full_instruction)] == NN(full_instruction)) chip_8->PC += PC_STEP;
            LOG_SE_VX_KK(X(full_instruction), NN(full_instruction));
            break;
        }
        case CAT_SNE: {
            if (chip_8->V[X(full_instruction)] != NN(full_instruction)) chip_8->PC += PC_STEP;
            LOG_SNE_VX_KK(X(full_instruction), NN(full_instruction));
            break;
        }
        case CAT_SE_REG: {
            if (chip_8->V[X(full_instruction)] == chip_8->V[Y(full_instruction)]) chip_8->PC += PC_STEP;
            LOG_SE_VX_VY(X(full_instruction), Y(full_instruction));
            break;
        }
        case CAT_LD: {
            chip_8->V[X(full_instruction)] = NN(full_instruction);
            LOG_LD_VX_KK(X(full_instruction), NN(full_instruction));
            break;
        }
        case CAT_ADD: {
            chip_8->V[X(full_instruction)] += NN(full_instruction);
            LOG_ADD_VX_KK(X(full_instruction), NN(full_instruction));
            break;
        }
        case CAT_REG_OP: handle_reg_op(chip_8, full_instruction); break;
        case CAT_SNE_REG: {
            if (chip_8->V[X(full_instruction)] != chip_8->V[Y(full_instruction)]) chip_8->PC += PC_STEP;
            LOG_SNE_VX_VY(X(full_instruction), Y(full_instruction));
            break;
        }
        case CAT_LD_I: chip_8->I = NNN(full_instruction); LOG_LD_I(NNN(full_instruction)); break;
        case CAT_JMP_V0: chip_8->PC = NNN(full_instruction) + chip_8->V[0]; LOG_JMP_V0(NNN(full_instruction)); return 0;
        case CAT_RND: {
            chip_8->V[X(full_instruction)] = (rand() % 256) & NN(full_instruction);
            LOG_RND_VX_KK(X(full_instruction), NN(full_instruction));
            break;
        }
        case CAT_DRW: {
            load_sprite_in_display_buffer(chip_8, X(full_instruction), Y(full_instruction), N(full_instruction), chip_8->I);
            LOG_DRW_VX_VY_N(X(full_instruction), Y(full_instruction), N(full_instruction));
            break;
        }
        case CAT_SKP: {
            if (full_instruction == SKP_VX(X(full_instruction))) {
                if (chip_8->keyboard[chip_8->V[X(full_instruction)]]) chip_8->PC += PC_STEP;
                LOG_SKP_VX(X(full_instruction));
            } else if (full_instruction == SKNP_VX(X(full_instruction))) {
                if (!chip_8->keyboard[chip_8->V[X(full_instruction)]]) chip_8->PC += PC_STEP;
                LOG_SKNP_VX(X(full_instruction));
            }
            break;
        }
        case CAT_EXTENDED: handle_extended_cat(chip_8, full_instruction); break;
        default: UNKNOW_INSTRUCTION;
    }
    chip_8->PC += PC_STEP;
    return 0;
}