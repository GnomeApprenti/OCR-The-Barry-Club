#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "line_detection.h"

int main(int argc, char *argv[]){
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    }

    // - Create a surface from the colored image.
    char* path = argv[1];
    SDL_Surface* surface = load_image(path);

    SDL_Surface* res_surface = gaussian_blur(surface);


    // Try to save image as pgn
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Failed to initialize SDL_image: %s\n", IMG_GetError());
        return 1;
    }

    if (IMG_SavePNG(res_surface, "test" ) == -1) {
        printf("Failed to save surface as PNG: %s\n", IMG_GetError());
        return 1;
    }

    IMG_Quit();
    SDL_FreeSurface(surface);
    SDL_Quit();
    return 0;
}

