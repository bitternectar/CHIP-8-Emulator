//
// Created by Bitter Nectar on 30.03.2025.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define RAM_SIZE       4096
#define REG_COUNT      16
#define STACK_SIZE     16
#define DISPLAY_WIDTH  64
#define DISPLAY_HEIGHT 32
#define START_RAM_ADDR 0x200
#define KEYBOARD_SIZE  16
#define INSTRUCTIONS_DELAY  500000000
#define TIMER_DELAY  1000000000
#define PC_STEP             2

#define UNKNOW_INSTRUCTION      printf("Неизвестная инструкция\n")
#define RAM_ERROR(program_size) printf("Не хватает оперативной памяти для загрузки программы: %lu Б", (program_size) * 2);
#define END_OF_PROGRAM          printf("Конец программы.\n");

#endif //CONSTANTS_H
