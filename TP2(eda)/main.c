#include <stdio.h>
#include "AllegroDraw.h"
#include "Backend.h"

/* Create and register Allegro Timers 'main' and 'animations'. 
 * The event queue must be created. 
 * If a timer is already created, it will destroy it and recreate it.
 * Returns 0 on success, 1 otherwise.
 */
int createAndRegisterTimers(FrontData* fd);

int main(int argc, char* argv[]) {

	int backend_return = 0;
	FrontData front_data;

	if (initFrontEnd(&front_data)) {

		printf("Error initializing front_data\n");
	}


	if (initAllegro(&front_data.disp, &front_data.small_font, &front_data.big_font)) {

		printf("Error initializing Allegro components.\n");
	}

	// ev queue
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
	
	while (!front_data.request.exit) {
		if(front_data.request.restart) restartFrontEnd(&front_data);

		// (Re)Create and register timers. Then start them
		if (createAndRegisterTimers(&front_data)) {
			destroyFrontEnd(&front_data);
			return 1;
		}

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

			// Loop until user requests something
			while (!front_data.request.exit && !front_data.request.restart)
			{
				al_stop_timer(front_data.timer.animations);
				manageEvents(NULL, &front_data);
			}
		}
		else if (front_data.game_mode == MODE1 && !front_data.request.exit && !front_data.request.restart) {

			drawFinalTime(&front_data);

			// Loop until user requests something
			while (!front_data.request.exit && !front_data.request.restart)
			{
				al_stop_timer(front_data.timer.animations);
				manageEvents(NULL, &front_data);
			}
		}
	}
	destroyFrontEnd(&front_data);

	return 0;
}

int
createAndRegisterTimers(FrontData * fd) {
	if (fd == NULL) return 1;

	// Destroy timers on need
	if (fd->timer.main != NULL) {
		al_destroy_timer(fd->timer.main);
	}
	if (fd->timer.animations != NULL) {
		al_destroy_timer(fd->timer.animations);
	}

    // Creation
	fd->timer.main = al_create_timer(2.0 / FPS);
	if (fd->timer.main == NULL) {
		printf("Error creating timer.\n");
		destroyFrontEnd(fd);
		return 1;
	}
	fd->timer.animations = al_create_timer(5.0 / FPS); // Change this to speed up MODE1 animations
	if (fd->timer.animations == NULL) {
		printf("Error creating timer.\n");
		destroyFrontEnd(fd);
		return 1;
	}

	// Register in event queue
	al_register_event_source(fd->evqueue,
							al_get_timer_event_source(fd->timer.main));
	al_register_event_source(fd->evqueue,
							al_get_timer_event_source(fd->timer.animations));

	return 0;
}