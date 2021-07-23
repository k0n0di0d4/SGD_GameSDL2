/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include "classes.h"
//The dimensions of the level
const int LEVEL_WIDTH = 1920;
const int LEVEL_HEIGHT = 480;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
inits initt;
//music to play in the background
static const char *MY_COOL_MP3 = "LESSGO/LESS GO.mp3";
//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

//Scene textures
extern LTexture gStreamingTexture;
extern LTexture gBGTexture;

//Animation stream
extern DataStream gDatastream;


int main( int argc, char* args[] )
{
	int counter = 0;
	//Start up SDL and create window
	if( !initt.init(SCREEN_WIDTH, SCREEN_HEIGHT) )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !initt.loadMedia(counter) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The player that you control
			Player player;

			//wall
			SDL_Rect wall;
			wall.x = 0;
			wall.y = 405;
			wall.w = 1920;
			wall.h = 75;

			//The camera area
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//handling music 
				int mp3result = 0;
				int flags = MIX_INIT_MP3;
				
				if (SDL_Init(SDL_INIT_AUDIO) < 0) {
					exit(1);
    			}
				if (flags != ( mp3result = Mix_Init(flags))) {
					exit(1);
				}
				Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
				Mix_Music *music = Mix_LoadMUS(MY_COOL_MP3);
				Mix_PlayMusic(music, -1);
				//end of handling music
			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the Player
					player.handleEvent( e );
				}
				
				//Move the Player
				player.move(wall, LEVEL_WIDTH, LEVEL_HEIGHT);

				//Center the camera over the Player
				camera.x = ( player.getPosX() + Player::playerWidth / 2 ) - SCREEN_WIDTH / 2;
				camera.y = ( player.getPosY() + Player::playerHeight / 2 ) - SCREEN_HEIGHT / 2;

				//Keep the camera in bounds
				if( camera.x < 0 )
				{
					camera.x = 0;
				}
				if( camera.y < 0 )
				{
					camera.y = 0;
				}
				if( camera.x > LEVEL_WIDTH - camera.w )
				{
					camera.x = LEVEL_WIDTH - camera.w;
				}
				if( camera.y > LEVEL_HEIGHT - camera.h )
				{
					camera.y = LEVEL_HEIGHT - camera.h;
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background
				gBGTexture.render( 0, 0, &camera );

				//Render and creates an image of Player in at the center of the screen
				//gStreamingTexture.render( ( SCREEN_WIDTH - gStreamingTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gStreamingTexture.getHeight() ) / 2 );

				//Render objects
				player.render( camera.x, camera.y );

				//Update screen
				SDL_RenderPresent( gRenderer );

				if(player.getPosX() == LEVEL_WIDTH - 100){
					player.beZero();
					counter++;
					break;
				}
			}
			Mix_FreeMusic(music);
		}
		if(counter > 0){
			if( !initt.loadMedia(counter) )
			{
				printf( "Failed to load media!\n" );
			}
			else
			{
				//Main loop flag
				bool quit = false;

				//Event handler
				SDL_Event e;

				//The player that you control
				Player player;

				//wall
				SDL_Rect wall;
				wall.x = 0;
				wall.y = 405;
				wall.w = 1920;
				wall.h = 75;

				//The camera area
				SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

				//handling music 
				int mp3result = 0;
				int flags = MIX_INIT_MP3;
				
				if (SDL_Init(SDL_INIT_AUDIO) < 0) {
					exit(1);
    			}
				if (flags != ( mp3result = Mix_Init(flags))) {
					exit(1);
				}
				Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
				Mix_Music *music = Mix_LoadMUS(MY_COOL_MP3);
				Mix_PlayMusic(music, -1);
				//end of handling music
				//While application is running
				while( !quit )
				{
					//Handle events on queue
					while( SDL_PollEvent( &e ) != 0 )
					{
						//User requests quit
						if( e.type == SDL_QUIT )
						{
							quit = true;
						}

						//Handle input for the Player
						player.handleEvent( e );
					}
				
					//Move the Player
					player.move(wall, LEVEL_WIDTH, LEVEL_HEIGHT);

					//Center the camera over the Player
					camera.x = ( player.getPosX() + Player::playerWidth / 2 ) - SCREEN_WIDTH / 2;
					camera.y = ( player.getPosY() + Player::playerHeight / 2 ) - SCREEN_HEIGHT / 2;

					//Keep the camera in bounds
					if( camera.x < 0 )
					{
						camera.x = 0;
					}
					if( camera.y < 0 )
					{
						camera.y = 0;
					}
					if( camera.x > LEVEL_WIDTH - camera.w )
					{
						camera.x = LEVEL_WIDTH - camera.w;
					}
					if( camera.y > LEVEL_HEIGHT - camera.h )
					{
						camera.y = LEVEL_HEIGHT - camera.h;
					}

					//Clear screen
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					SDL_RenderClear( gRenderer );

					//Render background
					gBGTexture.render( 0, 0, &camera );

					//Render and creates an image of Player in at the center of the screen
					//gStreamingTexture.render( ( SCREEN_WIDTH - gStreamingTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gStreamingTexture.getHeight() ) / 2 );

					//Render objects
					player.render( camera.x, camera.y );

					//Update screen
					SDL_RenderPresent( gRenderer );
				}
				Mix_FreeMusic(music);
			}
		}
	}
	
	//Free resources and close SDL
	initt.close();

	return 0;
}
