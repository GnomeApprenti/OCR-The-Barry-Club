#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>
#include <math.h>
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* temp_ = IMG_Load(path);
    SDL_Surface* surface_f = SDL_ConvertSurfaceFormat(temp_, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(temp_);

    return surface_f;
}

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    Uint32 average = 0.3 * r + 0.59 * g + 0.11 * b;

    r = average;
    g = average;
    b = average;

    return SDL_MapRGB(format, r, g, b);
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface -> pixels; // array of pixels
    int len = surface->w * surface->h; // length of the array

    SDL_PixelFormat* format = surface->format;

    SDL_LockSurface(surface);

    for (int i = 0 ; i < len ; i++)
    {
        pixels[i] = pixel_to_grayscale(pixels[i], format);
    }

    SDL_UnlockSurface(surface);
}
Uint32 pixel_to_binarise(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);

    moyenne = r+g+b/3;
    if (moyenne > 127)
        r=g=b=255;
    else
        r=g=b=0;
    return SDL_MapRGB(format, r, g, b);
}

void surface_to_binarise(SDL_Surface* surface)
{
    Uint32* pixels = surface -> pixels; // array of pixels
    int len = surface->w * surface->h; // length of the array

    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);

    for (int i = 0 ; i < len ; i++)
    {
        pixels[i] = pixel_to_grayscale(pixels[i], format);
    }

    SDL_UnlockSurface(surface);
}
int main(int argc, char** argv)
{
	if (argc == 0 )
	{
		errx(EXIT_FAILURE, "Usage: image-file");
	}
	char* path = argv[1];
	SDL_Surface* surface = load_image(path);
	surface_to_grayscale(surface);
    surface_to_binarise(surface);
	IMG_SaveJPG(surface, "trait.jpg", 90);
    return 0;
}

