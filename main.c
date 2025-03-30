//
// Created by Bitter Nectar on 29.03.2025.
//

#include <stdio.h>
#include <string.h>

#include "/opt/homebrew/include/SDL2/SDL.h"
#include "instructions.h"
#include "constants.h"
#include "chip8.h"

// Загрузка программы в RAM
void load_program(Chip_8 *chip_8, const uint8_t *program, const size_t program_size) {
    if (START_RAM_ADDR + program_size > RAM_SIZE) {
        printf("Не хватает оперативной памяти для загрузки программы: %lu Б\n", program_size);
        exit(1);
    }
    memcpy(&chip_8->RAM[START_RAM_ADDR], program, program_size);
}

// Отрисовка дисплея с использованием SDL
void draw_buffer(Chip_8 *chip, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Черный фон
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Белые пиксели
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            if (chip->display_buffer[y][x]) {
                SDL_Rect rect = {x * 10, y * 10, 10, 10};  // Увеличение пикселей в 10 раз
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

// Сопоставление клавиш SDL с клавишами Chip-8
uint8_t map_key(SDL_Keycode key) {
    switch (key) {
        case SDLK_1: return 0x1; case SDLK_2: return 0x2; case SDLK_3: return 0x3; case SDLK_4: return 0xC;
        case SDLK_q: return 0x4; case SDLK_w: return 0x5; case SDLK_e: return 0x6; case SDLK_r: return 0xD;
        case SDLK_a: return 0x7; case SDLK_s: return 0x8; case SDLK_d: return 0x9; case SDLK_f: return 0xE;
        case SDLK_z: return 0xA; case SDLK_x: return 0x0; case SDLK_c: return 0xB; case SDLK_v: return 0xF;
        default: return 0xFF;  // Неизвестная клавиша
    }
}

int main(int argc, char **argv) {
    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL не удалось инициализировать! Ошибка: %s\n", SDL_GetError());
        return 1;
    }

    // Создание окна
    SDL_Window *window = SDL_CreateWindow("Chip-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        DISPLAY_WIDTH * 10, DISPLAY_HEIGHT * 10, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Не удалось создать окно! Ошибка: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Создание рендерера
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Не удалось создать рендерер! Ошибка: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    char* file_name = argv[1];
    // Загрузка ROM
    size_t file_size;
    const uint8_t *program = load_rom(file_name, &file_size);
    if (!program) {
        printf("Не удалось загрузить ROM.\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Инициализация Chip-8
    Chip_8 chip_8 = {0};
    init_chip_8(&chip_8);
    load_program(&chip_8, program, file_size);

    // Основной цикл
    int running = 1;
    const int FPS = 60;
    const int frame_delay = 1000 / FPS;
    Uint32 frame_start, frame_time;
    Uint32 last_timer_update = SDL_GetTicks();

    while (running) {
        frame_start = SDL_GetTicks();

        // Обработка событий
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
            } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                uint8_t key = map_key(e.key.keysym.sym);
                if (key != 0xFF) {
                    chip_8.keyboard[key] = (e.type == SDL_KEYDOWN) ? 1 : 0;
                }
            }
        }

        // Выполнение инструкций
        int result = execute_instructions(&chip_8);
        if (result == -1) {
            running = 0;
        }

        // Обновление таймеров (60 Гц)
        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_timer_update >= 1000 / 60) {
            if (chip_8.DT > 0) chip_8.DT--;
            if (chip_8.ST > 0) chip_8.ST--;
            last_timer_update = current_time;
        }

        // Отрисовка
        draw_buffer(&chip_8, renderer);

        // Контроль FPS
        frame_time = SDL_GetTicks() - frame_start;
        if (frame_delay > frame_time) {
            SDL_Delay(frame_delay - frame_time);
        }
    }

    // Освобождение ресурсов
    free((void *)program);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
