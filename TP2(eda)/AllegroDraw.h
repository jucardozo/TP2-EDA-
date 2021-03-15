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

#define SCREENWIDHT 1200
#define SCREENHEIGHT 600
#define BORDE_WIDTH	25

/*Structure given to both drawing functions, because I need some of the data that's given*/
typedef struct {

	ALLEGRO_FONT* small_font;
	ALLEGRO_FONT* big_font;
	long double* times_recorded;
	int times_count;
	int game_mode;
} FrontData;

/*Initializes the data that will be given to the drawing functions, that is time recorded and time count*/
int initFrontEnd(void* front_data);

/*Initializes all Allegro addons, the display and the font*/
int initAllegro(ALLEGRO_DISPLAY** disp, ALLEGRO_FONT** font1, ALLEGRO_FONT** font2);

/*Destroys the memory allocated previously in the initialization*/
void destroyFrontEnd(void * front_data);

/*Function that's given as the callback to choose, depending on the mode, which function we are going to call*/
int publishStatus(struct Floor* floor, void* front_data);

void drawFinalTime(FrontData *front_data);

int drawFunction2(FrontData * front_data);