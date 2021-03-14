#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "Backend.h"

#define SCREENWIDHT 1200
#define SCREENHEIGHT 600
#define BORDE_WIDTH	25

typedef struct {

	ALLEGRO_FONT* font;
	double* times_recorded;
	int times_count;
} FrontData;

int initFrontEnd(void* front_data);

int initAllegro(ALLEGRO_DISPLAY** disp, ALLEGRO_FONT** font);

void destroyFrontEnd(void * front_data);

int publishStatus(struct Floor* floor, void* front_data);