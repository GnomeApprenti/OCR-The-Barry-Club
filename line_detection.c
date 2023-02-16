#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

// TOOLS
static SDL_Surface* load_image(const char* path){

	SDL_Surface* temp_surface = IMG_Load(path);
	SDL_Surface* surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
	SDL_FreeSurface(temp_surface);
	
	if(surface == NULL){
		return NULL;
	}

	return surface;
}

static void swap(size_t *xp, size_t *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}


// PART 1: CANNY EDGE
//
//STEP 1: GAUSSIAN BLUR for noise reduction

SDL_Surface* gaussian_blur(SDL_Surface* surface){
	int kernel[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
	int size = 3;
	int w = surface->w;
	int h = surface->h;
	int shift, sum, kernel_index, coord, blurred_avg, num_in_sum; 


	SDL_Surface* res_surface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0, 0, 0, 0);

    Uint32 *pixels = surface->pixels;
    Uint32 *new_pixels = new_surface->pixels;
    Uint8 r, g, b;

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
        {
            shift = size / 2;
            sum = 0, num_in_sum = 0, kernel_index = 0;

            for (int s_x = shift - size; s_x < shift; s_x++)
                for (int s_y = shift - size; s_y < shift; s_y++)
                {
                    coord = (y + s_y) * w + x + s_x;
                    if (coord >= 0)
                    {
                        SDL_GetRGB(pixels[coord], surface->format, &r, &g, &b);

                        sum += kernel[kernel_index] * floor(0.3*r + 0.59*g + 0.11*b);
                        //number_in_kernel ++;
						number_in_kernel += kernel[kernel_index];
                    }
                    kernel_index++;
                }

            coord = y * w + x;
            blurred_avg = sum / weight;

            Uint32 color = SDL_MapRGB(surface->format, blurred_avg, blurred_avg, blurred_avg);
            res_pixels[coord] = color;
        }

    return new_surface;
}

//STEP 2: SOBEL OPERATOR for gradient calculation
SDL_Surface* sobel_operator(SDL_Surface* surface){
	int sobelx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	int sobely[] = {-1, -2, 1, 0, 0, 0, 1, 2, 1};
	int size = 3;
	int w = surface->w;
	int h = surface->h;
	int shift, sum, kernel_index, coord, sobel_x, sobel_y;  


}

//STEP 3: NON MAXIMUM SUPPRESSION to have thinner edges
SDL_Surface* non_max_supp(SDL_Surface* surface){
	//TODO
}

//STEP 4: DOUBLE TRESHOLDING to have only black or white
SDL_Surface* double_tresholding(SDL_Surface* surface){
	//TODO
}

//STEP 5: HYSTERESIS to link edges and correct imperfections
SDL_Surface* hysteresis(SDL_Surface* surface){
	//TODO
}

// PART 2 : HOUGH TRANSFORM


// PART 3: FINISH PROCESS

