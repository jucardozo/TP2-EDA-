#include <stdio.h>
#include "AllegroDraw.h"
#include "Backend.h"

int main(int argc, char* argv[]) {

	int backend_return = 0;
	FrontData front_data;

	if (initFrontEnd(&front_data)) {

		printf("Error initializing front_data\n");
	}


	if (initAllegro(&front_data.disp, &front_data.small_font, &front_data.big_font)) {

		printf("Error initializing Allegro components.\n");
	}

	// Timers and ev queue
	front_data.timer.main = al_create_timer(2.0 / FPS);
	if (front_data.timer.main == NULL) {
		printf("Error creating timer.\n");
		destroyFrontEnd(&front_data);
		return 1;
	}
	front_data.timer.animations = al_create_timer(5.0 / FPS); // Change this to speed up MODE1 animations
	if (front_data.timer.animations == NULL) {
		printf("Error creating timer.\n");
		destroyFrontEnd(&front_data);
		return 1;
	}

	front_data.evqueue = al_create_event_queue();
	if (front_data.evqueue == NULL) {
		printf("Error creating event queue.\n");
		destroyFrontEnd(&front_data);
		return 1;
	}
	// Register events
	// Display
	al_register_event_source(front_data.evqueue,
							al_get_display_event_source(front_data.disp));
	// Keyboard
	al_register_event_source(front_data.evqueue,
							al_get_keyboard_event_source());
	// Timers
	al_register_event_source(front_data.evqueue,
							al_get_timer_event_source(front_data.timer.main));
	al_register_event_source(front_data.evqueue,
							al_get_timer_event_source(front_data.timer.animations));

	while (!front_data.request.exit) {
		if(front_data.request.restart) restartFrontEnd(&front_data);

		al_start_timer(front_data.timer.main);
		al_start_timer(front_data.timer.animations);

		backend_return = initBackend(argc, argv, gatherBackendData, &front_data);
		// Only ignore "FAILURE" if restart was requested
		if(backend_return == FAILURE && !front_data.request.restart) {
			destroyFrontEnd(&front_data);
			if (front_data.request.exit) {
				// User requested exit, so there's no problem here :)
				return 0;
			}
			// BACKEND FAILED FOR SOME REASON! :O
			return 1;
		}

		front_data.times_count++;		//Here is the total number of "robots_number" that has been used

		if (front_data.game_mode == MODE2 && !front_data.request.exit && !front_data.request.restart ) {

			drawFunction(&front_data, front_data.times_count);

			while (!front_data.request.exit && !front_data.request.restart)
			{
				manageEvents(NULL, &front_data);
			}
		}
		else if (front_data.game_mode == MODE1 && !front_data.request.exit && !front_data.request.restart) {

			drawFinalTime(&front_data);

			while (!front_data.request.exit && !front_data.request.restart)
			{
				manageEvents(NULL, &front_data);
			}
		}
	}
	destroyFrontEnd(&front_data);

	return 0;
}