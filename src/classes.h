#include <SDL2/SDL.h>
#include <string>
#include <vector>

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		
		//Image dimensions
		int mWidth;
		int mHeight;
};

//A test animation stream
class DataStream
{
	public:
		//Initializes internals
		DataStream();

		//Loads initial data
		bool loadMedia();

		//Deallocator
		void free();

		//Gets current frame data
		void* getBuffer();

	private:
		//Internal data
		SDL_Surface* mImages[ 4 ];
		int mCurrentImage;
		int mDelayFrames;
};

class Player
{
	public:
		//thrash function
		void poggers();
		
		//dimensions of player
		static const int playerWidth = 64;
		static const int playerHeight = 205;

		//maximum axic velocity of the player
		static const int playerVelocity = 10;

		//Initializes the variables
		Player();
		
		//Takes key presses and adjusts the Player's velocity
		void handleEvent( SDL_Event& e );

		//Moves the Player
		void move(SDL_Rect& wall, const int LEVEL_WIDTH, const int LEVEL_HEIGHT);

		//set Player's X position to 0
		void beZero();

		//Shows the Player on the screen relative to the camera
		void render( int camX, int camY );
		
		//getter for mColliders
		std::vector<SDL_Rect>& getColliders();

		//Position accessors
		int getPosX();
		int getPosY();

		//is jumping
		bool jumping = false;
		float jumpVel = 2;
		float gravity = 0.2f;


	private:
		//The X and Y offsets of the dot
		int mPosX, mPosY;
		//The velocity of the dot
		int mVelX, mVelY;

		SDL_Rect mCollider;

};

class inits
{   
    public:
        //Starts up SDL and creates window
        bool init(const int SCREEN_WIDTH, const int SCREEN_HEIGHT);

        //Loads media
        bool loadMedia(int counter);

        //Frees media and shuts down SDL
        void close();

        //Box set collision detector
        bool checkCollision( SDL_Rect a, SDL_Rect b );
};
