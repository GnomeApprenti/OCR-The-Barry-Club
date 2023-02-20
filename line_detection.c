#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "line_detection.h"

// TOOLS
/*
static void swap(size_t *xp, size_t *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

static int array_max(int arr[], int len){
    int max = arr[0];

    for (int i = 1; i < len; i++){
        if (arr[i] > max){
            max = arr[i];
        }
    }

    return max;
}

static int array_min(int arr[], int len){
    int min = arr[0];

    for (int i = 1; i < len; i++){
        if (arr[i] < min){
            min = arr[min];
        }
    }

    return min;
}

// Sort using insertion sort
static void array_sort(int arr[][2], int len) {
    for (int i = 0; i < len; i++) {

        int key = arr[i][0];
        int value = arr[i][1];


        int j = i - 1;
        while (j >= 0 && arr[j][0] > key) {

            arr[j + 1][0] = arr[j][0];
            arr[j + 1][1] = arr[j][1];
            j--;
        }


        arr[j + 1][0] = key;
        arr[j + 1][1] = value;
    }
}

static void array_sort_2(int arr[], int len){
    for (int i = 1; i < len; i++) {

        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }

        arr[j + 1] = key;
    }
}

static int compute_treshold(int accumulator[][360], int diag, int len){
    int values[len];
    int values_i = 0;

    for (int i = 0; i < diag; i++) {
        for (int j = 0; j < 360; j++) {
            values[values_i] = accumulator[i][j];
            values_i++;
        }
    }
    array_sort_2(values, len);

    return values[values_i/2];
}


static void crop(SDL_Surface* input, SDL_Surface* output){
    //TODO
}*/

// NOT STATIC TOOLS
SDL_Surface* load_image(const char* path){
    SDL_Surface* temp_surface = IMG_Load(path);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(temp_surface, SDL_PIXELFORMAT_RGB888, 0);
    return surface;
}



// PART 1: CANNY EDGE
//
//STEP 1: GAUSSIAN BLUR for noise reduction
SDL_Surface* gaussian_blur(SDL_Surface* surface){
    // Gaussian kernel
    int kernel[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};

    //The surface we return
    SDL_Surface* res_surface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0, 0, 0, 0);

    //The arrays of pixels
    Uint32* pixels = surface->pixels;
    Uint32* res_pixels = res_surface->pixels;
    Uint8 r, g, b;

    //All variables
    int w = surface->w;
    int h = surface->h;
    // sum, kernel_index, num_in_sum, i, i2, j, j2, coord, blurred_color;

    // For each pixel in the image
    for (int i = 0; i < w; i++){
		for (int j = 0; i < j; j++){

            int sum = 0;
            int kernel_index = 0;
            int num_in_sum = 0;

            //For each cell in the 3x3 matrix around (i,j) pixel
            for (int i2 = -1; i2 < 2; i2++){
                for (int j2 = -1; j2 < 2; j2++){

                    //We found its coordinates in the pixel array
                    int coord = ((i+i2) * w + (j+j2));

                    //If the coordinates are in the array (the top left pixel of the top left corner doesn't exist)
                    if (coord > 0 && coord < w*h){

                        //We get the rgb values of the pixel, multiply it by our kernel
                        SDL_GetRGB(pixels[coord], surface->format, &r, &g, &b);
                        sum+= kernel[kernel_index] * floor(0.3 * r + 0.59 * g + 0.11 * b);
                        //sum += kernel[kernel_index] * r;
                        num_in_sum += kernel[kernel_index];
                    }
                    kernel_index++;
                }
            }

            int coord = i * w + j;
            int blurred_color = sum / num_in_sum;

            // Update the color of the (i,j) pixel in our res_pixels array and so in res_surface image
            Uint32 color = SDL_MapRGB(surface->format, blurred_color, blurred_color, blurred_color);
            res_pixels[coord] = color;
		}
	}
    free(surface);
    return res_surface;
}

//STEP 2: SOBEL OPERATOR for gradient calculation and NON-MAX SUPPRESSION for thinner edges
SDL_Surface* sobel_operator(SDL_Surface* surface){
    //Kernels
    int gx_kernel[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy_kernel[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // The surface we return
    SDL_Surface* res_surface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0, 0, 0, 0);

    // We get all the pixels' arrays and initialize r, g and b
    Uint32* pixels = surface->pixels;
    Uint32* res_pixels = res_surface->pixels;
    Uint8 r, g, b;

    // All variables
    int w = surface->w;
    int h = surface->h;

    // For the non-maximum suppression
    int orientation[w*h];
    int magnitude[w*h];
    int non_max_suppression[w*h];
    int nms_build_arrays_index = 0;

    // For each pixel in the image
    for (int i = 0; i < w; i++){
        for (int j = 0; j < h; j++){

            int gradient_x = 0;
            int gradient_y = 0;
            int gradient_kernel_index = 0;

            //For each cell in the 3x3 matrix around (i,j) pixel
            for (int i2 = -1; i2 < 2; i2++){
                for (int j2 = -1; j2 < 2; j2++){

                    //We found its coordinates in the pixel array
                    int coord = ((i+i2) * w + (j+j2));

                    //If the coordinates are in the array (the top left pixel of the top left corner doesn't exist)
                    if (coord > 0 && coord < w*h){

                        // We get the RGB values, calculate it's average and add it to our gradients
                        SDL_GetRGB(pixels[coord], surface->format, &r, &g, &b);
                        //int average = floor(0.3 * r + 0.59 * g + 0.11 * b);
                        //gradient_x+= gx_kernel[gradient_kernel_index] * average;
                        //gradient_y+= gy_kernel[gradient_kernel_index] * average;
                        gradient_x+= gx_kernel[gradient_kernel_index] * r;
                        gradient_y+= gy_kernel[gradient_kernel_index] * r;
                    }
                    gradient_kernel_index++;
                }
            }

            // Update the magnitude array
            magnitude[nms_build_arrays_index] = sqrt(gradient_x * gradient_x + gradient_y * gradient_y);

            // Update the orientation array
            orientation[nms_build_arrays_index] = atan(gradient_x/gradient_y);

            nms_build_arrays_index++;
        }
    }

    // Non-maximum suppression
    int magnitude_treshold = 200; //floor(0.8 * array_max(magnitude, w*h));

    for (int nms_i = 0; nms_i < w; nms_i++){
        for (int nms_j = 0; nms_j < h; nms_j++){

            int nms_coord = nms_i * w + nms_j;
            int act_magnitude = res_pixels[nms_coord];
            int act_orientation = orientation[nms_coord];
            int neighbour1;
            int neighbour2;

            if ((0 <= act_orientation && act_orientation < M_PI_4) || (7 * M_PI_4 <= act_orientation && act_orientation <= M_PI)) {
                neighbour1 = magnitude[(nms_i + 1) * w + nms_j];
                neighbour2 = magnitude[(nms_i - 1) * w + nms_j];
            }
            else if (M_PI_4 <= act_orientation && act_orientation < 3 * M_PI_4) {
                neighbour1 = magnitude[(nms_j + 1) + (nms_i + 1) * w];
                neighbour2 = magnitude[(nms_j - 1) + (nms_i - 1) * w];
            }
            else if (3 * M_PI_4 <= act_orientation && act_orientation < 5 * M_PI_4) {
                neighbour1 = magnitude[(nms_j + 1) + nms_i * w];
                neighbour2 = magnitude[(nms_j - 1) + nms_i * w];
            }
            else {
                neighbour1 = magnitude[(nms_j + 1) + (nms_i - 1) * w];
                neighbour2 = magnitude[(nms_j - 1) + (nms_i + 1) * w];
            }

            // Suppression du non-maximum
            if (act_magnitude >= neighbour1 && act_magnitude >= neighbour2 && act_magnitude >= magnitude_treshold) {
                non_max_suppression[nms_coord] = act_magnitude;
            } else {
                non_max_suppression[nms_coord] = 0;
            }
        }
    }

    // Create the answer
    for (int res_i = 0; res_i < w*h; res_i++){
        int act_grad = non_max_suppression[res_i];
        int color_used = 0;

        //Probably useless case
        if (act_grad > 255){
            color_used = 255;
        }
        //Probably useless case
        else if (act_grad < 0){
            color_used = 0;
        }
        else{
            color_used = act_grad;
        }

        Uint32 color = SDL_MapRGB(surface->format, color_used, color_used, color_used);
        res_pixels[res_i] = color;
    }

    free(surface);
    return res_surface;
}

//STEP 3: DOUBLE TRESHOLDING to have only black (non-edge), white(edge) or grey(weak edge) pixels;
SDL_Surface* double_tresholding(SDL_Surface* surface){   //, int min_treshold, int max_treshold){
    //Answer
    SDL_Surface* res_surface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0, 0, 0, 0);

    // Variables
    Uint32* pixels = surface->pixels;
    Uint32* res_pixels = res_surface->pixels;
    Uint8 r, g, b;
    int w = surface->w;
    int h = surface->h;
    int max_treshold = 200; //floor(0.8 * array_max(pixels, w*h));
    int min_treshold = 150; //floor(1.2 * array_min(pixels, w*h));

    for(int i = 0; i < w*h; i++){
        SDL_GetRGB(pixels[i], surface->format, &r, &g, &b);
        int average = floor(0.3 * r + 0.59 * g + 0.11 * b);
        int color_used = 125;

        if (average < min_treshold){
            color_used = 0;
        }
        else if(average > max_treshold){
            color_used = 255;
        }

        Uint32 color = SDL_MapRGB(surface->format, color_used, color_used, color_used);
        res_pixels[i] = color;
    }

    free(surface);
    return res_surface;
}

//STEP 4: HYSTERESIS to link edges and correct imperfections
SDL_Surface* hysteresis(SDL_Surface* surface){
    //Answer
    SDL_Surface* res_surface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0, 0, 0, 0);

    // Variables
    Uint32* pixels = surface->pixels;
    Uint32* res_pixels = res_surface->pixels;
    Uint8 r, g, b;
    int w = surface->w;
    int h = surface->h;
    int edges[w*h];

    //MARK STRONG EDGES
    for (int i = 0; i < w*h; i++){
        SDL_GetRGB(pixels[i], surface->format, &r, &g, &b);

        if (r == 255){
            edges[i] = 255;
        }
        else{
            edges[i] = 0;
        }
    }

    //TRACE WEAK EDGES
    for (int i2 = 0; i2 < w; i2++){
        for (int j2 = 0; j2 < h; j2++){

            int coord = i2 * w + j2;
            SDL_GetRGB(pixels[coord], surface->format, &r, &g, &b);

            if (r == 125){
                for (int i3 = -1; i3 <= 1; i3++) {
                    for (int j3 = -1; j3 <= 1; j3++) {
                        int coords = (j2 + j3) + (i2 + i3) * w;
                        if(edges[coords] == 255){
                            edges[coord] = 255;
                        }
                    }
                }
            }
        }
    }

    for (int res_i = 0; res_i < w*h; res_i++){
        int hysteresis_color = edges[res_i];
        Uint32 color = SDL_MapRGB(surface->format, hysteresis_color, hysteresis_color, hysteresis_color);
        res_pixels[res_i] = color;
    }

    free(surface);
    return res_surface;
}



// PART 2 : HOUGH TRANSFORM for line detection
//
//STEP 1: WE GET THE ACCUMULATOR
int* hough_accumulator(SDL_Surface* surface){
    int w = surface->w;
    int h = surface->h;
    int diag = sqrt(w*w + h*h);
    Uint32* pixels = surface->pixels;
    Uint8 r, g, b;

    // Set all accumulator's values to 0
    int* accumulator = (int*) malloc((diag * 360) * sizeof(int));
    for (int acc_i = 0; acc_i < diag*360; acc_i++) {
        accumulator[acc_i] = 0;
    }

    // For all pixels
    for (int i = 0; i < w; i++){
        for (int j = 0; j < h; j++){

            //Find the coordinates of the current pixel in the array and get its values
            int coord = i * w + j;
            SDL_GetRGB(pixels[coord], surface->format, &r, &g, &b);

            // If the pixel is white (an edge)
            if (r+g+b == 765){

                //For all thetas
                for (int theta = -180; theta < 180; theta++){

                    // We calculate its rhos and add 1 in the accumulator at (rho,theta) index
                    int rho = i*cos(theta) + (j*w)* sin(theta);
                    accumulator[rho * 360 + theta] +=1;
                }
            }
        }
    }

    return accumulator;
}

/*
//STEP 2: EXTRACT MAXIMUMS
int[][2] hough_maximums(int[][360], int diag){
    //TODO
}

//STEP 3: EXTRACT VERTICAL AND HORIZONTAL LINES AND SORT THEM
int[][4] hough_vertical_lines(int[][2] maximums, int len){
    //TODO
}

int[][4] hough_horizontal_lines(int[][2] maixmums, int len){
    //TODO
}



// PART 3: FINISH PROCESS
//
//STEP 1: LOCATE PATTERN AND EXTRACT ALL CELLS
int[81][8] extract_cells(int[][4] h_lines, int[][4] v_lines, int len_v, int len_h){
    //TODO
}

//STEP 2: BUILD ARRAY
char[11][11] build_array(int[81][8] cells){
    //TODO
}*/
