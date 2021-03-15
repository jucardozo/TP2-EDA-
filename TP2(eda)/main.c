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

	initBackend(argc, argv, publishStatus, &front_data);

	if (front_data.game_mode == MODE2) {	

		drawFunction2(&front_data);
	}
	else if (front_data.game_mode == MODE1) {		

		drawFinalTime(&front_data);
	}
	else {

		printf("Error in the game_mode definition\n");
	}

	destroyFrontEnd(&front_data);

	return 0;
}