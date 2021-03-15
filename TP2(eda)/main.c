#include <stdio.h>
#include "AllegroDraw.h"
#include "Backend.h"

int main(int argc, char* argv[]) {

	FrontData front_data;

	if (initFrontEnd(&front_data)) {

		printf("Error initializing front_data\n");
	}

	ALLEGRO_DISPLAY* disp;
	ALLEGRO_FONT* smallfont;
	ALLEGRO_FONT* bigfont;

	if (initAllegro(&disp, &smallfont, &bigfont)) {

		printf("Error initializing allegro components\n");
	}

	front_data.small_font = smallfont;
	front_data.big_font = bigfont;

	if ((initBackend(argc, argv, publishStatus, &front_data)) == FAILURE) {

		printf("Backend failure\n");
	}

	front_data.times_count++;		//Here is the total number of "robots_number" that has been used

	if (front_data.game_mode == MODE2) {	

		drawFunction(&front_data, front_data.times_count);
		
		if (front_data.times_count >= 40) {

			drawFunction(&front_data, front_data.times_count/2);
		}
	}
	else if (front_data.game_mode == MODE1) {		

		drawFinalTime(&front_data);
	}
	else {

		printf("Error in the game_mode definition\n");
	}

	destroyFrontEnd(&front_data);

	//destroyAllegro(&disp, &smallfont, &bigfont);

	return 0;
}