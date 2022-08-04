#include "TextureManager.h"

bool textureClass::loadFromFile(SDL_Renderer *gRenderer,string path) {

	//free();
	//NOT defined yet

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		cout << "Unable to load image %s! SDL_image Error: " << path.c_str() << IMG_GetError() << endl;
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) //check this part for error, gave error although working last time, BUT NEED THIS TO GET DIMENSIONS
		{
			cout << "Unable to create texture from % s!SDL Error : " << path.c_str() << SDL_GetError() << endl;
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}
void textureClass::render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
	//REPLACED SRCRECT FROM CLIP TO NULL
	//USE SDL_FLIP_NONE for no flip
	//USE NULL for center to set rotating point at center of the texture.
}
void textureClass::renderResized(SDL_Renderer* gRenderer, SDL_Rect* Desti, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(gRenderer, mTexture, NULL, Desti, angle, center, flip);
	SDL_RenderPresent(gRenderer);
}
int textureClass::getWidth() {
	return mWidth;
}
int textureClass::getHeight() {
	return mHeight;
}
bool textureClass::loadFromRenderedText(SDL_Renderer* gRenderer, std::string textureText, SDL_Color textColor,TTF_Font *gfont)
{
	//Get rid of preexisting texture
	//free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gfont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
	}
	else
	{
		//Create texture from surface pixels
		//SDL_Renderer* lrenderer;
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << endl;
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w / 2;
			mHeight = textSurface->h / 2;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);

		//Substitute rendering text here to using a seperate render function so that it is more reusable
	}

	//Return success
	return mTexture != NULL;
}