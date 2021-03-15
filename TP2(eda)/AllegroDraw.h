/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "Backend.h"

 /*******************************************************************************
  * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
  ******************************************************************************/

#define KEY_SEEN		(1)
#define KEY_RELEASED	(2)
#define KEY_READY		((KEY_SEEN | KEY_RELEASED) & KEY_RELEASED)

#define FPS				(30)
#define SCREENWIDHT		(1200)
#define SCREENHEIGHT	(600)
#define BORDE_WIDTH		(25)

/*Structure given to both drawing functions, because I need some of the data that's given*/
typedef struct {
	int game_mode;
	long double* times_recorded;
	int times_count;
	int robots_used;

	ALLEGRO_DISPLAY* disp;
	ALLEGRO_EVENT_QUEUE* evqueue;
	struct {
		ALLEGRO_TIMER* main;
		ALLEGRO_TIMER* animations;
	} timer;
	unsigned char * keyboard_keys;	// Size: ALLEGRO_KEY_MAX

	ALLEGRO_FONT* small_font;
	ALLEGRO_FONT* big_font;
	struct {					// For options in this struct: 0 means disabled, != 0 means enabled.
		int exit;				// Shall the program stop execution
		int restart;			// Shall the program restart itself
		int lock_front;			// Lock to avoid returning to backend and keep animations "smooth". Controlled by animations timer
	} request;

} FrontData;

/*Initializes the data that will be given to the drawing functions, that is time recorded and time count*/
int initFrontEnd(void* front_data);

/*Initializes all Allegro addons, the display and the font*/
int initAllegro(ALLEGRO_DISPLAY** disp, ALLEGRO_FONT** font1, ALLEGRO_FONT** font2);

/* Restart FrontData structure. Assumes initAllegro has been run already. */
int restartFrontEnd(FrontData* fd);

/*Destroys the memory allocated previously in the initialization*/
void destroyFrontEnd(FrontData * front_data);

/*Function that's given as the callback to choose, depending on the mode, which function we are going to call*/
int gatherBackendData(struct Floor* floor, void* front_data);

/*Draws in screen the final time that the mode 1 simulation lasted*/
void drawFinalTime(FrontData *front_data);

void drawFunction(FrontData * front_data, int max_data);


/* Manage allegro event queue. floor: Used by MODE1 to draw. NULL otherwise */
void manageEvents(struct Floor* floor, FrontData* fd);
