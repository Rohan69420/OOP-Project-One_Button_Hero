#pragma once
#include "Game.h"
//SUGGESTION: DONOT DECLARE FUNCTIONS IF YOU DONT YET HAVE A USE FOR IT 


class textureClass //texture wrapper
{
public:
    //Initializes variables
   // textureClass();

    //Deallocates memory
   // ~textureClass();

    //Loads image at specified path
    bool loadFromFile(SDL_Renderer* gRenderer, std::string path);

    //Creates image from font string
    bool loadFromRenderedText(SDL_Renderer* gRenderer, std::string textureText, SDL_Color textColor,TTF_Font *gfont);

    //Deallocates texture
   // void free();

    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //Set blending
    //void setBlendMode(SDL_BlendMode blending);

    //Set alpha modulation
    //void setAlpha(Uint8 alpha);

    //Renders texture at given point
    void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void renderResized(SDL_Renderer* gRenderer, SDL_Rect*, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    //Gets image dimensions
    int getWidth();
    int getHeight();

private:
    //The actual hardware texture
    SDL_Texture* mTexture;
    SDL_Renderer* renderer;
    SDL_Rect Rect;

    //Image dimensions
    int mWidth;
    int mHeight;
};