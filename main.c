#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "line_detection.h"

int main(int argc, char *argv[]){
    char* blur = "gaussian_blur.jpg";
    char* sobel = "sobel_operator.jpg";
    char* double_tresh = "double_treshold.jpg";
    char* hyste = "hysteresis.jpg";

    if (argc == 1)
    {
        errx(EXIT_FAILURE, "Usage: image-file");
    }
    char* path  = argv[1];
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        errx(EXIT_FAILURE,"%s",SDL_GetError());
    }
    else
    {
        SDL_Surface* surface  = IMG_Load(path);
        SDL_Surface* surface2 = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGB888,0);

        //GAUSSIAN BLUR
        SDL_Surface* blurred = gaussian_blur(surface2);
        IMG_SaveJPG(blurred,blur,90);

        //SOBEL OPERATOR
        SDL_Surface* sob_op = sobel_operator(blurred);
        IMG_SaveJPG(sob_op,sobel,90);

        //DOUBLE TRESHOLD
        SDL_Surface* double_tresholded = double_tresholding(sob_op);
        IMG_SaveJPG(double_tresholded,double_tresh,90);

        //HYSTERESIS
        SDL_Surface* hysteresised = hysteresis(double_tresholded);
        IMG_SaveJPG(hysteresised,hyste,90);

        //FREE SURFACES
        SDL_FreeSurface(surface);
        SDL_FreeSurface(surface2);
        SDL_FreeSurface(blurred);
        SDL_FreeSurface(sob_op);
        SDL_FreeSurface(double_tresholded);
        SDL_FreeSurface(hysteresised);

        return 0;
    }
}

