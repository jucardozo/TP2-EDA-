#include <stdio.h>
#include "AllegroDraw.h"
#include "Backend.h"

int main(int argc, char* argv[]) {

	FrontData front_data;

	if (initFrontEnd(&front_data)) {

		printf("Error initializing front_data\n");
	}

	ALLEGRO_DISPLAY* disp;
	ALLEGRO_FONT* fontTTF;

	if (initAllegro(&disp, &fontTTF)) {

		printf("Error initializing allegro components\n");
	}

	front_data.font = fontTTF;

	initBackend(argc, argv, publishStatus, &front_data);

	if (1) {	//MODE2

		drawFunction2(&front_data);
	}
	else if (0) {		//MODE1

		//IMPRIMO EL TIEMPO TOTAL EN TICKS, CASTEADO A INT
	}

	destroyFrontEnd(&front_data);

	return 0;
}