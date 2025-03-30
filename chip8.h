//
// Created by Bitter Nectar on 30.03.2025.
//
#pragma once
#include <stdint.h>
#include "constants.h"

typedef struct {
    uint8_t RAM[RAM_SIZE];
    uint8_t V[REG_COUNT];
    uint8_t VF;
    uint8_t DT;
    uint8_t ST;
    uint8_t display_buffer[DISPLAY_HEIGHT][DISPLAY_WIDTH];
    uint16_t I;
    uint16_t PC;
    uint8_t SP;
    uint16_t stack[STACK_SIZE];
    uint8_t keyboard[KEYBOARD_SIZE];
} Chip_8;


void init_chip_8(Chip_8 *chip);