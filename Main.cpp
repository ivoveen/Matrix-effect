#include <SDL.h> // window
#include <SDL_ttf.h> // printing text
#include <time.h> // time for seeding
#include <stdlib.h> //randomizer
#include <iostream> // error msg
#include <sstream> // string builder

using namespace std;


int a[44][6];
Uint8 color_rgb[44][2];





void CreateDroplet(int amount) {
	int speed;
	Uint8 color_g = 0;
	Uint8 color_rb = 0;
	int factor;

	for (int i = 1; i <= amount; i++) {
		speed = rand() % 4; // 1 through 3
		a[i][0] = speed;

	
		a[i][1] = i; //xpos
		
		
		
		a[i][2] = 1; //lengte

		factor = (rand() % 10 + 5); // 0.1 through 0.19
		a[i][3] = factor;


		color_g = 255;
		color_rb =  (rand() % 100 + 79);
		
		color_rgb[i][0] = color_g;
		color_rgb[i][1] = color_rb;

	}
}


void ResetDroplet(int DropNr) {
	int speed;
	Uint8 color_g = 0;
	Uint8 color_rb = 0;
	int factor;
	
		speed = rand() % 3; // 0 through 2
		a[DropNr][0] = speed;

		//int x = rand() % 43 + 1; //
		
		
		a[DropNr][1] = DropNr; //xpos
		

		a[DropNr][2] = 1; //lengte

		factor = (rand() % 10 + 5); // 0.1 through 0.19
		a[DropNr][3] = factor;


		color_g = 255;
		color_rb = (rand() % 100 + 79);

		color_rgb[DropNr][0] = color_g;
		color_rgb[DropNr][1] = color_rb;

	
}

int main(int argc, char** argv)
{
	bool quit = false;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	SDL_Window* window = SDL_CreateWindow("SDL_ttf in SDL2",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,
		480, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	TTF_Font* font = TTF_OpenFont("arial.ttf", 15);
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface* surface = TTF_RenderText_Solid(font, "A", color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Event event;

	int texW = 0;
	int texH = 0;
	int texX = 0;
	int texY = 0;
	int frames = 0;

	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { texX, texY, texW, texH };


	//matrix vars
	srand(time(NULL));
	SDL_Rect rect;
	char c;
	int r;

	Uint8 fG;
	Uint8 fR;
	double factor;

	CreateDroplet(43);

	while (!quit)
	{
		


		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				// Break out of the loop on quit
				quit = true;
			}
		}

		if (frames == 1000) {

			for (int i = 0; i < 43; i++) {
				int speed = a[i][0];
			
				int currentwait = a[i][4];

				if (currentwait == speed) { //is it the turn of this droplet
					
					int x = a[i][1] * 15;
					int length = a[i][2];

					double colorfactor = a[i][3];


					Uint8 r1 = color_rgb[i][1];
					Uint8 g1 = color_rgb[i][0];
					int y;

					bool visible = true;

					for (int j = 0; j < length; j++) { // draw every char of the droplet
						//-----------------------------------------------------------------------------
						//color

						 factor = (colorfactor /100) * (length -j);

						 if (factor <= 1) { //is this part of the droplet visible
							 //-----------------------------------------------------------------------------
							 //color

							 fR = r1 + factor * (0 - r1); // a + t (b - a)
							 fG = g1 + factor * (0 - g1);


							 color = { fR, fG, fR };



							 //-----------------------------------------------------------------------------
							 //randomize character
							 
							 std::stringstream s;
							 r = rand() % 100;   // generate a random number
							 c = '?' + r;            // Convert to a character from a-z
							 s << c;
							 //-----------------------------------------------------------------------------
							 //calculate y pos
							 y = j * 15;


							 //-----------------------------------------------------------------------------
							 //remove old text
							 rect.x = x;
							 rect.y = y;
							 rect.w = 15;
							 rect.h = 15;

							 SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
							 SDL_RenderFillRect(renderer, &rect);

							 //-----------------------------------------------------------------------------
							 //final printing
							 surface = TTF_RenderText_Solid(font, s.str().c_str(), color);
							 texture = SDL_CreateTextureFromSurface(renderer, surface);
							 SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
							 dstrect = { x, y, texW, texH };
							 SDL_RenderCopy(renderer, texture, NULL, &dstrect);
							 SDL_DestroyTexture(texture);
							 SDL_FreeSurface(surface);

							 //-----------------------------------------------------------------------------
							 // spawn a new droplet possibly
							 if (visible == true) { //this is the last visible piece of the drop
								 if (y > 480) {
									 ResetDroplet(i);
								 }
								
								 //-----------------------------------------------------------------------------
								 //remove dead piece of droplet
								 rect.x = x;
								 rect.y = y - 15;
								 rect.w = 15;
								 rect.h = 15;

								 SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
								 SDL_RenderFillRect(renderer, &rect);

								



								visible = false;
							 }
						 }
						
						
						

					}
					a[i][4] = -1; //wait
					a[i][2] ++; //length

					//cerr << "number" << i << "| x " << x/15 << " | speed " << speed << "| factor " << colorfactor << endl;
				}
				a[i][4] += 1; //wait counter

			}
			
			SDL_RenderPresent(renderer);
			frames = 0;
		}

		frames++;




	}
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	return 0;
}



