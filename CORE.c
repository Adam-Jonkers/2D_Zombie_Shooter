#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <png.h>

#include "CORE.h"

int get_random_number(int min, int max)
{
    return (rand() % (max - min + 1) + min);
}

vec2_t normalise_vec2(vec2_t v) 
{
    float length = length_vec2(v); 
    return (vec2_t){v.x / length, v.y / length};
} 

vec2_t floor_vec2(vec2_t v) 
{
    return (vec2_t){floor(v.x), floor(v.y)};
}

vec2_t add_vec2(vec2_t v1, vec2_t v2) 
{
    return (vec2_t){v1.x + v2.x, v1.y + v2.y};
}

vec2_t subtract_vec2(vec2_t v1, vec2_t v2) 
{
    return (vec2_t){v1.x - v2.x, v1.y - v2.y};
}

vec2_t divide_vec2(vec2_t v, float f) 
{
    return (vec2_t){v.x / f, v.y / f};
}

vec2_t multiply_vec2(vec2_t v, float f) 
{
    return (vec2_t){v.x * f, v.y * f};
}

float length_vec2(vec2_t v) 
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

float dot_product(vec2_t v1, vec2_t v2) 
{
    return v1.x * v2.x + v1.y * v2.y;
}

SDL_Texture* load_texture(char* path, SDL_Renderer* renderer)
{
    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void load_animation(Animation_t *animation, char* path, SDL_Renderer* renderer)
{
    for (int i = 0; i <= animation->length; i++)
    {
        char fullPath[100];
        strcpy(fullPath, path);
        char index[20];
        sprintf(index, "%d", i);
        strcat(fullPath, index);
        strcat(fullPath, ".png");
        animation->animation[i] = load_texture(fullPath, renderer);
    }
}

static pixel_t* get_pixel(bitmap_t* bitmap, int x, int y)
{
    return &bitmap->pixels[bitmap->width * y + x];
}

int save_png_to_file (bitmap_t* bitmap, const char* path)
{ 
    FILE* fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte** row_pointers = NULL;

    int status = 1;
    int pixel_size = 3;
    int depth = 8;

    fp = fopen(path, "wb");
    if (!fp) {
        goto fopen_failed;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        goto png_failure;
    }

    png_set_IHDR(png_ptr,
                 info_ptr,
                 bitmap->width,
                 bitmap->height,
                 depth,
                 PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    
    row_pointers = png_malloc(png_ptr, bitmap->height * sizeof(png_byte*));
    for (y = 0; y < bitmap->height; y++) {
        png_byte* row = png_malloc(png_ptr, sizeof(uint8_t) * bitmap->width * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < bitmap->width; x++) {
            pixel_t* pixel = get_pixel(bitmap, x, y);
            *row++ = pixel->red;
            *row++ = pixel->green;
            *row++ = pixel->blue;
        }
    }

    png_init_io(png_ptr, fp);
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    status = 0;

    for (y = 0; y < bitmap->height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);

    png_failure:
    png_create_info_struct_failed:
        png_destroy_write_struct(&png_ptr, &info_ptr);

    png_create_write_struct_failed:
        fclose(fp);

    fopen_failed:
        return status;
}

void load_Text(Text_t* text, SDL_Renderer* renderer)
{
    SDL_Surface* textSurface = NULL;
    if (text->font == NULL)
    {
        printf("Font not setup\n");
    } else {
        textSurface = TTF_RenderText_Solid(text->font, text->text, text->textColor);
    }
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        text->texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if (text->texture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            text->textBox.w = textSurface->w;
            text->textBox.h = textSurface->h;
        }
    }
    SDL_FreeSurface(textSurface);
}

void load_Font(TTF_Font** font, char* path)
{
    *font = TTF_OpenFont(path, 15);
    if (*font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    } 
}