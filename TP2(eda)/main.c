#include "AllegroDraw.h"
#include "Backend.h"

int main(int argc, char* argv[]) {

	FrontData front_data;

	if (!initFrontEnd(&front_data)) {

		printf("Error initializing front_data\n");
	}

	ALLEGRO_DISPLAY* disp;
	ALLEGRO_FONT* font;

	if (initAllegro(&disp, &font)) {

		printf("Error initializing allegro components\n");
	}	

	initBackend(argc, argv, publishStatus, &front_data);

	destroyFrontEnd(&font);

	return 0;
}