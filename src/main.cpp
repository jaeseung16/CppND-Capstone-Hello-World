#include <iostream>
#include <fstream>
#include <complex>
#include <SDL.h>

float width = 600;
float height = 600;

int value(int x, int y) {
    std::complex<float> point((float)x/width-1.5, (float)y/height-0.5);
    std::complex<float> z(0,0);
    int nb_iter = 0;
    while (abs(z) < 2 && nb_iter <= 20) {
        z = z * z + point;
        nb_iter ++;
    }
    if (nb_iter < 20) {
        return (255 * nb_iter) / 20;
    } else {
        return 0;
    }
}

int main() {
    std::cout << "Hello World!" << "\n";

    std::ofstream my_Image("mandelbrot.ppm");

    if (my_Image.is_open()) {
        my_Image << "P3\n" << width << " " << height << " 255\n";
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height ; y++) {
                int val = value(x, y);
                my_Image << val << ' ' << 0 << ' ' << 0 << "\n";
            }
        }
        my_Image.close();
    } else {
        std::cout << "Could not open the file\n";
    }

    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("SDL2 Displaying Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface *image = SDL_LoadBMP("mandelbrot.bmp");
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    while(!quit) {
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
        }
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}