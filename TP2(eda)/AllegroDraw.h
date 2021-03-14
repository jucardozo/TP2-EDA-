#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "Backend.h"

#define SCREENWIDHT 1360
#define SCREENHEIGHT 768
#define BORDE_WIDTH	25

typedef struct {

	ALLEGRO_FONT* font;
	double* times_recorded;
	int times_count;
} FrontData;

int initFrontEnd(void* front_data);

int initAllegro(ALLEGRO_DISPLAY** disp, ALLEGRO_FONT** font);

void destroyFrontEnd(ALLEGRO_FONT** font);

int publishStatus(struct Floor* floor, void* front_data);