/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Code Implemented by Mr Neville Kalebi , //////////////////////////////////////////////////////////////////////////////////////////////
// Updated code will be added Soon , please add and configure as you wish , with new feature and add comments so we can learn from it,///
// Thanks Eshells 2020 .......///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "SDL2/SDL.h"
#include "constants.h"

////////////////////////////////////////////////////////////////////////////////
//Global Vars
///////////////////////////////////////////////////////////////////////////////

int game_is_running = FALSE;
SDL_Window* window =NULL;
SDL_Renderer* renderer =NULL;
int last_frame_time = 0;


//////////////////////////////////////////////////////////////////////////////
// Declare two game objects for the ball and the paddle
////////////////////////////////////////////////////////////////////////////

struct ball {
        float x;
        float y;
        float width;
        float height;
        float vel_x;
        float vel_y;
        } ball,paddle;

////////////////////////////////////////////////////////////////////////////
// Function to initialize our SDL window
//////////////////////////////////////////////////////////////////////////

int  initialize_window(void) {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
                fprintf(stderr, "Error initializing SDL. \n");
                return FALSE;
        }

        window =SDL_CreateWindow(
                NULL,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                800,
                600,
                SDL_WINDOW_BORDERLESS
         );

        if(!window) {
        fprintf(stderr, "Error creating SDL window. \n");
        return FALSE;
        }

        renderer =SDL_CreateRenderer(window,-1,0);
        if(!renderer) {
                fprintf(stderr, "Error creating SDL Renderer. \n");
                return FALSE;
        }

        return TRUE;
}

/////////////////////////////////////////////////////////////////
// Function to poll SDL events and process keyboard input    ////
/////////////////////////////////////////////////////////////////

void process_input(void) {
        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type) {

        case SDL_QUIT:
                game_is_running = FALSE;
                break;
        case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                        game_is_running = FALSE;
                if (event.key.keysym.sym == SDLK_LEFT)
                        paddle.vel_x =-400;
                if (event.key.keysym.sym == SDLK_RIGHT)
                        paddle.vel_x =+400;

                break;
        case SDL_KEYUP:
                // Reset paddle velocity based on left /right arrow keys

                  if (event.key.keysym.sym == SDLK_LEFT)
                        paddle.vel_x =0;
                  if (event.key.keysym.sym == SDLK_RIGHT)
                        paddle.vel_x =0;
        break;

        }
}

////////////////////////////////////////////////////////////////////
// Setup function that runs once at the begining of our program
/////////////////////////////////////////////////////////////////////

void setup(void) {
        //initialize the ball object moving down at constant velocity

        ball.width=15;
        ball.height=15;
        ball.x =20;
        ball.y=20;
        ball.vel_x =300;
        ball.vel_y =300;

//initialize the paddle position at the bottom of the screen
        paddle.width =100;
        paddle.height =20;
        paddle.x = (WINDOW_WIDTH / 2 ) - (paddle.width /2);
        paddle.y = WINDOW_HEIGHT - 40;
        paddle.vel_x =0;
        paddle.vel_y = 0;
}



/////////////////////////////////////////////////////////////
// Update function with a fixed time tamp
//////////////////////////////////////////////////

void update(void) {
        int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

        if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
                SDL_Delay(time_to_wait);

        //get delta time factor convewrted to seconds to be used to update objects
        float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

        //Store the millisec of the current frame
        last_frame_time = SDL_GetTicks();

        //update ball position and paddle position
        ball.x +=ball.vel_x * delta_time;
        ball.y +=ball.vel_y * delta_time;
        paddle.x += paddle.vel_x * delta_time;
        paddle.y += paddle.vel_y * delta_time;



//Check for ball collision with walls
        if(ball.x <= 0 || ball.x + ball.width >= WINDOW_WIDTH)
                ball.vel_x =-ball.vel_x;

        if(ball.y <0)
                ball.vel_y =-ball.vel_y;

// Check for ball collision with paddle
if(ball.y + ball.height >= paddle.y && ball.x + ball.width >= paddle.x && ball.x <= paddle.x + paddle.width)
        ball.vel_y =-ball.vel_y;

//Prevent paddle from moving outside the boundaries of the window
if(paddle.x <=0)
        paddle.x =0;
if(paddle.x >= WINDOW_WIDTH - paddle.width)
        paddle.x =WINDOW_WIDTH - paddle.width;


//Check for game over when ball hits the bottom part of the screen
if(ball.y + ball.height > WINDOW_HEIGHT) {
        ball.x = WINDOW_WIDTH / 2;
        ball.y = 0;
        }


}

///////////////////////////////////////////////////////////////////////
// Render function to draw game objects in the SDL window
///////////////////////////////////////////////////////////////////

void render() {
        SDL_SetRenderDrawColor(renderer , 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //TODO : Here is where we can start drwaing our game objects
        //Draw Rectangle
        SDL_Rect ball_rect = {
                (int)ball.x,
                (int)ball.y,
                (int)ball.width,
                (int)ball.height
        };

        SDL_SetRenderDrawColor(renderer , 255,255,255,255);
        SDL_RenderFillRect(renderer, &ball_rect);


        //Draw a rectangle  fio paddle object

        SDL_Rect paddle_rect = {
        (int)paddle.x,
        (int)paddle.y,
        (int)paddle.width,
        (int)paddle.height
        };
        SDL_SetRenderDrawColor(renderer, 255 ,255,255,255);
        SDL_RenderFillRect(renderer, &paddle_rect);


        SDL_RenderPresent(renderer);
}

//////////////////////////////////////////////////////////////////
// function to dstroy SDL window and renderer
///////////////////////////////////////////////////////////////


void destroy_window() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
}


//////////////////////////////////////////////////////////////
// Main Function
////////////////////////////////////////////////////////////

int main() {
        game_is_running = initialize_window();

        setup();

        while(game_is_running) {
                process_input();
                update();
                render();
        }

        destroy_window();




        return 0;
}





