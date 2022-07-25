#pragma once
#include <SDL_mixer.h>

class gameSounds {
public:
	bool initSounds();
	bool loadAllSounds();
	void closeSounds();
	void playGlobalMusic();
private:
	//			//<<<<<<<<<< < music variables,this should be global tho hmmmmmmmmmm GLOBALL

		//The sound effects that will be used
		Mix_Chunk * gScratch = NULL;
	Mix_Chunk* gHigh = NULL;
	Mix_Chunk* gMedium = NULL;
	Mix_Chunk* gLow = NULL;
};
/* ADDITIONAL DOCUMENTATION FOR THE OPERATIONS THAT WILL BE NEEDED IN THE FUTURE

else if( e.type == SDL_KEYDOWN )
					{
						switch( e.key.keysym.sym )
						{
							//Play high sound effect
							case SDLK_1:
							Mix_PlayChannel( -1, gHigh, 0 ); <<<<<<<<<<<<<<<<<<<<<<<<Playing chunk/channel procedure
							break;

							//Play medium sound effect
							case SDLK_2:
							Mix_PlayChannel( -1, gMedium, 0 );
							break;

							//Play low sound effect
							case SDLK_3:
							Mix_PlayChannel( -1, gLow, 0 );
							break;

							//Play scratch sound effect
							case SDLK_4:
							Mix_PlayChannel( -1, gScratch, 0 );
							break;

							case SDLK_9:
							//If there is no music playing
							if( Mix_PlayingMusic() == 0 )			<<<<<<<<<<<<< check music playing status
							{
								//Play the music
								Mix_PlayMusic( gMusic, -1 );			<<<<<<<<<<<<<<<<<<<<<<<<<<Playing music procedure
							}
							//If music is being played
							else
							{
								//If the music is paused
								if( Mix_PausedMusic() == 1 )
								{
									//Resume the music
									Mix_ResumeMusic();
								}
								//If the music is playing
								else
								{
									//Pause the music
									Mix_PauseMusic();
								}
							}
							break;

							case SDLK_0:
							//Stop the music
							Mix_HaltMusic();				<<<<<<<<<<<<<<stopping music
							break;
						}
					}
*/