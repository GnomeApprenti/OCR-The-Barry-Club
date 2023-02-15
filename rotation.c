#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <err.h>
#include <math.h>

SDL_Surface* SDL_RotationCentral(SDL_Surface* origine, float angle)
{
 SDL_Surface* destination;
 int i;
 int j;
 int mx, my, mxdest, mydest;
 int bx, by;
 float angle_radian;
 float tcos;
 float tsin;
 double largeurdest;
 double hauteurdest;
 

 angle_radian = -angle * M_PI / 180.0;
 

 tcos = cos(angle_radian);
 tsin = sin(angle_radian);
 

 largeurdest=   ceil(origine->w * fabs(tcos) + origine->h * fabs(tsin)),
 hauteurdest=   ceil( origine->w * fabs(tsin) + origine->h * fabs(tcos)),
 
 
 destination = SDL_CreateRGBSurface(0, largeurdest, hauteurdest, 
    origine->format->BitsPerPixel,origine->format->Rmask, origine->format->Gmask, 
    origine->format->Bmask, origine->format->Amask);
 Uint32* pixels = destination->pixels;
 for(int i =0; i< destination->w* destination->h;i++)
 {
    pixels[i] = SDL_MapRGB(destination->format,255,255,255);
 }
 if(destination==NULL)
  return NULL;
 

 mxdest = destination->w/2.;
 mydest = destination->h/2.;
 mx = origine->w/2.;
 my = origine->h/2.;
 
 for(j=0;j<destination->h;j++)
  for(i=0;i<destination->w;i++)
  {
 
   bx = (ceil (tcos * (i-mxdest) + tsin * (j-mydest) + mx));
   by = (ceil (-tsin * (i-mxdest) + tcos * (j-mydest) + my));
   /* on vérifie que l'on ne sort pas des bords*/
   if (bx>=0 && bx< origine->w && by>=0 && by< origine->h)
   {
     SDL_ChangePixel(origine,destination, bx, by, i, j); 
     } 
 
 }
 
return destination;
}

int main(int argc,char** argv )
{
    char* out = "Rot.jpg";
    if (argc == 1)
    {
        errx(EXIT_FAILURE, "Usage: image-file");
    }
    char* path  = argv[1];
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        errx(EXIT_FAILURE,"%s",SDL_GetError());
    }
    if (strcmp( argv[2], "--auto") == 0)
    {
        AutoRation(path);
    }
    else
    {
        SDL_Surface* surface  = IMG_Load(path); 
        float f = atof(argv[2]);
        SDL_Surface* surface2 = SDL_ConvertSurfaceFormat(surface,SDL_PIXELFORMAT_RGB888,0);
        SDL_Surface* destination = SDL_RotationCentral(surface2, f);
        SDL_FreeSurface(surface);
        IMG_SaveJPG(destination,out,90);
        SDL_FreeSurface(surface2);
    }
}
