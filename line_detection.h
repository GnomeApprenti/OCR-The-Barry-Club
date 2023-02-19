#ifndef LINE_DETECTION_H
#define LINE_DETECTION_H

//TOOLS
SDL_Surface* load_image(const char* path);

// CANNY EDGE FUNCTIONS
SDL_Surface* gaussian_blur(SDL_Surface* surface);
SDL_Surface* sobel_operator(SDL_Surface* surface);
SDL_Surface* double_tresholding(SDL_Surface* surface);
SDL_Surface* hysteresis(SDL_Surface* surface);

// HOUGH TRANSFORM FUNCTIONS
int* hough_accumulator(SDL_Surface* surface);
/*
int[][2] hough_maximums(int[][360], int diag);
int[][4] hough_vertical_lines(int[][2] maximums, int len);
int[][4] hough_horizontal_lines(int[][2] maixmums, int len);

// FINISH PROCESS FUNCTIONS
int[81][8] extract_cells(int[][4] h_lines, int[][4] v_lines, int len_v, int len_h);
char[11][11] build_array(int[81][8] cells);*/

#endif

//gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` line_detection.c
