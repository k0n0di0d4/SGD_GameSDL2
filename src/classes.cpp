#include "classes.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
extern inits initt;
//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//Animation stream
	DataStream gDataStream;

	//Scene textures
	LTexture gStreamingTexture;
	LTexture gBGTexture;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image from a specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
			//Color key image
        	SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

			newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
			if( newTexture == NULL )
			{
				printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				mWidth = loadedSurface->w;
				mHeight = loadedSurface->h;
			}


		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

DataStream::DataStream()
{
	mImages[ 0 ] = NULL;
	mImages[ 1 ] = NULL;
	mImages[ 2 ] = NULL;
	mImages[ 3 ] = NULL;

	mCurrentImage = 0;
	mDelayFrames = 16;
}

bool DataStream::loadMedia()
{
	bool success = true;

	for( int i = 0; i < 4; ++i )
	{
		char path[ 64 ] = "";
		sprintf( path, "LESSGO/student%d.png", i );

		SDL_Surface* loadedSurface = IMG_Load( path );
		if( loadedSurface == NULL )
		{
			printf( "Unable to load %s! SDL_image error: %s\n", path, IMG_GetError() );
			success = false;
		}
		else
		{
			mImages[ i ] = SDL_ConvertSurfaceFormat( loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0 );
		}

		SDL_FreeSurface( loadedSurface );
	}

	return success;
}

void DataStream::free()
{
	for( int i = 0; i < 4; ++i )
	{
		SDL_FreeSurface( mImages[ i ] );
	}
}

void* DataStream::getBuffer()
{
	--mDelayFrames;
	if( mDelayFrames == 0 )
	{
		++mCurrentImage;
		mDelayFrames = 16;
	}

	if( mCurrentImage == 4 )
	{
		mCurrentImage = 0;
	}

	return mImages[ mCurrentImage ]->pixels;
}

Player::Player()
{
	//Initialize the offsets
    mPosX = 0;
    mPosY = 404 - 204;

	//Initialize the Collider
	mCollider.w = playerWidth;
	mCollider.h = playerHeight;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Player::poggers(){
}

void Player::handleEvent( SDL_Event& e )
{
	int velocity = playerVelocity;
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            //case SDLK_UP: mVelY -= playerVelocity; break;
            //case SDLK_DOWN: mVelY += playerVelocity; break;
			//case SDLK_LCTRL: velocity = 30;
            case SDLK_LEFT: mVelX -= velocity; break;
            case SDLK_RIGHT: mVelX += velocity; break;
			case SDLK_SPACE: if(!jumping) jumping = true; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            //case SDLK_UP: mVelY += playerVelocity; break;
            //case SDLK_DOWN: mVelY -= playerVelocity; break;
			//case SDLK_LCTRL: velocity = playerVelocity;
            case SDLK_LEFT: mVelX += velocity; break;
            case SDLK_RIGHT: mVelX -= velocity; break;
        }
    }
}

void Player::move( SDL_Rect& wall, const int LEVEL_WIDTH, const int LEVEL_HEIGHT)
{
    //Move the Player left or right
    mPosX += mVelX;
	mCollider.x = mPosX;

    //If the Player went too far to the left or right or collided
    if( ( mPosX < 0 ) || ( mPosX + playerWidth > LEVEL_WIDTH ) || initt.checkCollision(mCollider, wall))
    {
        //Move back
        mPosX -= mVelX;
		mCollider.x = mPosX;
    }
    //Move the Player up or down
    mPosY += mVelY;
	mCollider.y = mPosY;

    //If the Player collidedwent too far up or down
    if( ( mPosY < 0 ) || ( mPosY + playerHeight > LEVEL_HEIGHT || initt.checkCollision(mCollider, wall) ) )
    {
        //Move back
		if(jumping){
			mPosY -=5;
			mCollider.y = mPosY;
		}
		else{
        	mPosY -= mVelY;
			mCollider.y = mPosY;
		}
		
		jumpVel = 2;
		jumping = false;
		mVelY = 0;
    }
	if(jumping){
		mVelY -= jumpVel;
		jumpVel -= gravity;
	}
	//printf("%d\n", mPosY);
	
}

void Player::beZero()
{
	mPosX = 0;
	mCollider.x = mPosX;
	mPosY = 404 - 204;
	mCollider.y = mPosY;
}
void Player::render( int camX, int camY)
{
    //Show the Player relative to the camera
	gStreamingTexture.render( mPosX - camX, mPosY - camY , 0 , 0.0, 0, SDL_FLIP_HORIZONTAL);
}

int Player::getPosX()
{
	return mPosX;
}

int Player::getPosY()
{
	return mPosY;
}

bool inits::init(const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Seed random
		srand( SDL_GetTicks() );

		//Create window
		gWindow = SDL_CreateWindow( "LESS GO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool inits::loadMedia(int counter)
{
	//Loading success flag
	bool success = true;

	for(int i = 0; i < 4; i++){
		//Load Player texture
		std::string pog = "LESSGO/student" + std::to_string(i) + ".png";
		std::cout << ">>" << pog << "<<" << std::endl;
		if( !gStreamingTexture.loadFromFile(pog) )
		{
			printf( "Failed to create streaming texture!\n" );
			success = false;
		}	
	}
	
	if (counter == 0){
		//Load background texture
		if( !gBGTexture.loadFromFile( "LESSGO/Level1Map.png" ) )
		{
			printf( "Failed to load background texture!\n" );
			success = false;
		}
	}
	else if (counter > 0){
		if( !gBGTexture.loadFromFile( "LESSGO/Level2Map.png" ) )
		{
			printf( "Failed to load background texture!\n" );
			success = false;
		}
	}

	//Load data stream
	if( !gDataStream.loadMedia() )
	{
		printf( "Unable to load data stream!\n" );
		success = false;
	}

	return success;
}

void inits::close()
{
	//Free loaded images
	gStreamingTexture.free();
	gDataStream.free();
	gBGTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool inits::checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}