/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#include "AllegroDraw.h"

 /*******************************************************************************
  * DEFINITIONS AND MACROS
  ******************************************************************************/

// We guess that 50 times will be recorded in mode 2. If there're more, the list will be expanded
#define INITIAL_GUESS_FOR_TIMES_RECORDED	(50)

#define COLOR_MODE1_BACKGROUND	(al_map_rgb(255, 255, 255))	// White
#define COLOR_TILE_DIRTY		(al_map_rgb(131, 86, 86))	// Brown
#define COLOR_TILE_CLEAN		(al_map_rgb(255, 255, 255))	// White
#define COLOR_TILE_SEPARETOR	(al_map_rgb(0, 0, 0))		// Black

 /* In case M_PI is not defined (copied from definition in glibc's math.h file)
  * It seems like some non-POSIX implementations could not have it defined.
  * See:
  * https://stackoverflow.com/questions/9912151/math-constant-pi-value-in-c#comment12648210_9912169
  */
#ifndef M_PI
#define M_PI            3.14159265358979323846 /* pi */
#endif
  // Convert Degrees to Radians
#define DEG2RAD(d)				((d)*(M_PI/180))

 /*******************************************************************************
  * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
  ******************************************************************************/

/*Uses allegro to draw the tiles and the robots given which are dirty and which are clean
	and also the position of every robot*/
static void drawFloor(struct Floor* floor, void* front_data);

/*Records all the data needed to draw the final function*/
static void recordDataFunction(struct Floor* floor, void* front_data);

/* Given an angle and modulus, get a point relative to the one called center. */
static coords_t directionVectorPosition(coords_t center, double angle, double modulus);
/* Draw a single robot over the floor */
static void  drawRobot(coords_t center, double angle, float tile_width, float tile_height);

static void checkKeys(FrontData* fd);



/*******************************************************************************
 *******************************************************************************
							FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int initFrontEnd(void* front_data){

	int error = SUCCESS;

	FrontData* p2front_data = (FrontData*) front_data;

	p2front_data->times_count = 0;
	p2front_data->robots_used = 0;

	p2front_data->game_mode = MODE_UNSET;
	p2front_data->evqueue = NULL;
	p2front_data->timer.main = NULL;
	p2front_data->timer.animations = NULL;
	p2front_data->small_font = NULL;
	p2front_data->big_font = NULL;
	p2front_data->disp = NULL;

	p2front_data->request.exit = 0;
	p2front_data->request.restart = 0;
	p2front_data->request.lock_front = 0; // Managed in gatherBackendData

	p2front_data->times_recorded = (long double*) calloc(INITIAL_GUESS_FOR_TIMES_RECORDED, sizeof(long double));	//Here is where I'm going to save the times that are entered

	if (p2front_data->times_recorded == NULL) {

		printf("(Chuckles) I'm in danger\n");
		printf("Error allocating memory for front_data\n");
		error = FAILURE;
	}
	
	p2front_data->keyboard_keys = (unsigned char*) calloc(ALLEGRO_KEY_MAX, sizeof(unsigned char));
	if (p2front_data->keyboard_keys == NULL) {
		printf("(Chuckles) I'm in danger\n");
		printf("Error allocating memory for front_data\n");
		error = FAILURE;
	}

	return error;
}

int restartFrontEnd(FrontData* fd) {
	al_stop_timer(fd->timer.main);
	al_stop_timer(fd->timer.animations);

	fd->game_mode = MODE_UNSET;
	fd->robots_used = 0;

	fd->request.exit = 0;
	fd->request.restart = 0;
	fd->request.lock_front = 0; // Managed in gatherBackendData

	for (int i = 0; i < fd->times_count; i++)
	{
		fd->times_recorded[i] = 0.0;
	}
	fd->times_count = 0;
	
	for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
	{
		fd->keyboard_keys[i] = 0;
	}

	return 0;
}


int initAllegro(ALLEGRO_DISPLAY** disp, ALLEGRO_FONT** font1, ALLEGRO_FONT** font2) {

	int error = 0;

	al_init();

	if (!al_init_primitives_addon()) {

		printf("Error intializing primitives addon\n");
		error = 1;
	}
	if (!al_install_keyboard()) {
		printf("Error initializing keyboard.\n");
		error = 1;
	}
	if (! al_init_font_addon()) {

		printf("Error initializing font addon\n");
		error = 1;
	}
	if (! al_init_ttf_addon()) {

		printf("Error initializing font addon\n");
		error = 1;
	}
	if (!((*disp) = al_create_display(SCREENWIDHT + 2 * BORDE_WIDTH, SCREENHEIGHT + 2 * BORDE_WIDTH))) {

		printf("Error loading display\n");
		error = 1;
	}

	if (!(*font1 = al_load_ttf_font("..\\TP2(eda)\\IBMPlexSerif-Regular.ttf", 10, 0) ) ) {

		printf("Error loading font1\n");
		error = 1;
	}
	if (!(*font2 = al_load_ttf_font("..\\TP2(eda)\\IBMPlexSerif-Regular.ttf", 80, 0))) {

		printf("Error loading font2\n");
		error = 1;
	}

	return error;
}

void destroyFrontEnd(FrontData * front_data){

	if (front_data == NULL)
		return;

	if (front_data->disp != NULL) al_destroy_display(front_data->disp);

	if (front_data->small_font != NULL) al_destroy_font(front_data->small_font);
	if (front_data->big_font != NULL) al_destroy_font(front_data->big_font);

	al_uninstall_keyboard();
	if (front_data->evqueue != NULL) al_destroy_event_queue(front_data->evqueue);
	if (front_data->timer.main != NULL) al_destroy_timer(front_data->timer.main);

	if(front_data->times_recorded != NULL) free(front_data->times_recorded);
	if(front_data->keyboard_keys != NULL) free(front_data->keyboard_keys);
}

int gatherBackendData(struct Floor* floor, void* front_data) {
	if (floor == NULL || front_data == NULL)
		return 1;

	FrontData* fd = (FrontData*)front_data;
	fd->game_mode = floor->game_mode;

	if (floor->game_mode == MODE1) {
		fd->robots_used = floor->robots.robots_number;
		fd->times_count = (int)floor->time_to_clean;
		fd->request.lock_front = 1;
	}
	else if (floor->game_mode == MODE2) {

		recordDataFunction(floor, front_data);
	}

	do {
		manageEvents(floor, (FrontData*)front_data);
	} while (fd->request.lock_front > 0);

	if (fd->request.exit || fd->request.restart)
	{
		return 1;
	}

	return 0;
}

static void drawFloor(struct Floor* floor, void* front_data) {
	if (front_data == NULL)
		return;
	if (floor == NULL)
		return; 

	if (((FrontData *) front_data)->request.exit)
		return;


	int w = floor->width;
	int h = floor->height;

	int horizontal_gap = SCREENWIDHT / w;	//Gap between columns
	int vertical_gap = SCREENHEIGHT / h;		//Gap between rows

	int dist_lat_border = (SCREENWIDHT - horizontal_gap * w) / 2 + BORDE_WIDTH;	//I define this to center the floor in the display
	int dist_vert_border = (SCREENHEIGHT - vertical_gap * h) / 2 + BORDE_WIDTH;

	// Background
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(dist_lat_border,
		dist_vert_border,
		dist_lat_border + horizontal_gap * w,
		dist_vert_border + vertical_gap * h,
		COLOR_MODE1_BACKGROUND
	);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			ALLEGRO_COLOR tile_color = al_map_rgb(0,0,0);
			if ((floor->clean)[i * w + j] == TILE_DIRTY) {	//If the tile is dirty I print a brown rectangle
				tile_color = COLOR_TILE_DIRTY;
			}
			else if ((floor->clean)[i * w + j] == TILE_CLEAN) {	//If the tile is clean I print a white rectangle
				tile_color = COLOR_TILE_CLEAN;
			}
			al_draw_filled_rectangle(dist_lat_border + j * horizontal_gap, dist_vert_border + i * vertical_gap, 
									dist_lat_border + (j + 1) * horizontal_gap, dist_vert_border + (i + 1) * vertical_gap, 
									tile_color);
		}
	}
	for (int i = 0; i <= h; i++) {	//I draw the horizontal lines of the tiles

		al_draw_line(dist_lat_border, dist_vert_border + i * vertical_gap,
			dist_lat_border + SCREENWIDHT, dist_vert_border + i * vertical_gap,
			COLOR_TILE_SEPARETOR, -1);
	}
	for (int i = 0; i <= w; i++) {	//I draw the vertical lines of the tiles

		al_draw_line(dist_lat_border + i * horizontal_gap, dist_vert_border, 
			dist_lat_border + i * horizontal_gap, dist_vert_border + SCREENHEIGHT, 
			COLOR_TILE_SEPARETOR, -1);
	}

	for (int i = 0; i < floor->robots.robots_number; i++) {
		coords_t center = {
			.x = floor->robots.robots[i].coordinates.x * horizontal_gap + (SCREENWIDHT - horizontal_gap * w) / 2 + BORDE_WIDTH,
			.y = floor->robots.robots[i].coordinates.y * vertical_gap + (SCREENHEIGHT - vertical_gap * h) / 2 + BORDE_WIDTH
		};
		drawRobot(center, floor->robots.robots[i].angle, horizontal_gap, vertical_gap);
	}

	al_flip_display();
	//Sleep(200);
}

static void recordDataFunction(struct Floor* floor, void* front_data) {
	if (front_data == NULL)
		return;
	if (floor == NULL)
		return; 

	if (((FrontData *) front_data)->request.exit)
		return;

	FrontData* p2front_data = (FrontData*)front_data;	//This variables are defined to make the function more readable

	if (p2front_data->times_count >= INITIAL_GUESS_FOR_TIMES_RECORDED - 1) {
		long double* tmp = (long double*)realloc(p2front_data->times_recorded, sizeof(long double)*(p2front_data->times_count + 2) );
		if (tmp == NULL) {
			p2front_data->request.exit = 1;
			printf("Error reallocating memory :(\n");
			return;
		}
		p2front_data->times_recorded = tmp;
		//(p2front_data->times_recorded)[ (p2front_data->times_count)+1] = 0.0;
	}
	long double* times_recorded = p2front_data->times_recorded;

	p2front_data->game_mode = floor->game_mode;

	(p2front_data->times_recorded)[p2front_data->times_count] = floor->time_to_clean;
	(p2front_data->times_count)++;

	al_clear_to_color(al_map_rgb((p2front_data->times_count) * 13, (p2front_data->times_count) * 7, (p2front_data->times_count) * 21));

	al_draw_text(p2front_data->big_font, al_map_rgb(0,0,0), SCREENWIDHT/2 + BORDE_WIDTH, SCREENHEIGHT/2 - 50, ALLEGRO_ALIGN_CENTRE, "SIMULATING");

	al_flip_display();

}

void drawFunction(FrontData * front_data, int max_data){
	if (front_data == NULL)
		return;

	if (((FrontData *) front_data)->request.exit)
		return;

	char aux_str[20];	//String that's being used to write letters and numbers

	FrontData* p2front_data = (FrontData*)front_data;	//This variables are defined to make the function more readable
	long double* times_recorded = p2front_data->times_recorded;

	//Here comes the drawing of the function

	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_draw_line(BORDE_WIDTH, BORDE_WIDTH, BORDE_WIDTH, BORDE_WIDTH + SCREENHEIGHT, al_map_rgb(0, 0, 0), 4);	//I draw the y axis
	al_draw_line(BORDE_WIDTH, BORDE_WIDTH + SCREENHEIGHT, BORDE_WIDTH + SCREENWIDHT, BORDE_WIDTH + SCREENHEIGHT, al_map_rgb(0, 0, 0), 4); //I draw the x axis

	double x_axis_gap = SCREENWIDHT / max_data;
	double y_axis_gap = SCREENHEIGHT / times_recorded[ (p2front_data->times_count) - max_data ];	//I get the y_axis gap using the highest time.

	for (int i = 1 + ((p2front_data->times_count)-max_data); i < (p2front_data->times_count); i++) {

		al_draw_line(BORDE_WIDTH + i * x_axis_gap, SCREENHEIGHT + BORDE_WIDTH + 10,
			BORDE_WIDTH + i * x_axis_gap, SCREENHEIGHT + BORDE_WIDTH - 10, al_map_rgb(0, 0, 0), 2);

		if (p2front_data->times_count <= 50) {

			if (i == 1 || (i % 2 == 0 && i != ((p2front_data->times_count) - 2)) || i == (p2front_data->times_count) - 1) {
				al_draw_textf(p2front_data->small_font, al_map_rgb(0, 0, 0), BORDE_WIDTH + i * x_axis_gap, SCREENHEIGHT + BORDE_WIDTH + 12
					, ALLEGRO_ALIGN_CENTRE, "%d", i);
			}
		}
		else {

			if (i == 1 || (i % 5 == 0 && i != ((p2front_data->times_count) - 2)) || i == (p2front_data->times_count) - 1) {
				al_draw_textf(p2front_data->small_font, al_map_rgb(0, 0, 0), BORDE_WIDTH + i * x_axis_gap, SCREENHEIGHT + BORDE_WIDTH + 12
					, ALLEGRO_ALIGN_CENTRE, "%d", i);
			}
		}
	}
	for (int i = 1 + ((p2front_data->times_count)-max_data); i < (p2front_data->times_count); i++) {

		al_draw_line(BORDE_WIDTH + 10, BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap,
			BORDE_WIDTH - 10, BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap, al_map_rgb(0, 0, 0), 2);
	}

	for (int i = 1 + ((p2front_data->times_count) - max_data); i < (p2front_data->times_count); i++) {	//I run over every time recorded to draw the point

		al_draw_filled_circle(BORDE_WIDTH + i * x_axis_gap, BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap,
			4, al_map_rgb(255, 0, 0));

		_gcvt_s(aux_str, 20 * sizeof(char), times_recorded[i - 1], 3);

		if (p2front_data->times_count <= 50) {

			if (i == 1 || (i % 2 == 0 && i != ((p2front_data->times_count) - 2)) || i == (p2front_data->times_count) - 1) { //I only print the first one, the even ones and the last one
				al_draw_text(p2front_data->small_font, al_map_rgb(0, 0, 0), BORDE_WIDTH + i * x_axis_gap,					//I don't print the time previous to last one
					BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap - 15, ALLEGRO_ALIGN_CENTRE, aux_str);
			}
		}
		else {

			if (i == 1 || (i %5 == 0 && i != ((p2front_data->times_count) - 2)) || i == (p2front_data->times_count) - 1) { //I only print the first one, the multiplos de 4 and the last one
				al_draw_text(p2front_data->small_font, al_map_rgb(0, 0, 0), BORDE_WIDTH + i * x_axis_gap,					//I don't print the time previous to last one
					BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap - 15, ALLEGRO_ALIGN_CENTRE, aux_str);
			}
		}
	}

	al_flip_display();
}

void drawFinalTime(FrontData* front_data) {
	al_clear_to_color(al_map_rgb(255, 255, 255));


	al_draw_multiline_textf(front_data->big_font, al_map_rgb(0, 0, 0),
		SCREENWIDHT / 2 + BORDE_WIDTH , SCREENHEIGHT / 4 , 
		SCREENWIDHT - BORDE_WIDTH*2, 0,
		ALLEGRO_ALIGN_CENTRE, 
		"%d TICKS WERE NEEDED TO CLEAN THIS FLOOR...\nUSING JUST %d ROBOTS!",
		front_data->times_count, front_data->robots_used);

	al_flip_display();

}

static void drawRobot(coords_t center, double angle, float tile_width, float tile_height) {
	// Maximum radius of a robot
	float outer_radius = floor(min(tile_width / 2, tile_height / 2)) * 3 / 4;

	double eyes_offset = outer_radius / 3;
	double eyes_radius = outer_radius / 5;

	// Border
	al_draw_filled_circle(center.x, center.y, outer_radius, al_map_rgb(0, 0, 0));
	// Inner
	al_draw_filled_circle(center.x, center.y, outer_radius - 1, al_map_rgb(128, 128, 128));

	// Eyes
	coords_t eyes_middle_point = directionVectorPosition(center, angle, outer_radius / 3);

	al_draw_filled_circle(eyes_middle_point.x - eyes_offset,
		eyes_middle_point.y - eyes_offset,
		eyes_radius, al_map_rgb(200, 0, 0));
	al_draw_filled_circle(eyes_middle_point.x + eyes_offset,
		eyes_middle_point.y + eyes_offset,
		eyes_radius, al_map_rgb(200, 0, 0));
}

static coords_t directionVectorPosition(coords_t center, double angle, double modulus) {
	coords_t new_coords = { .x = center.x, .y = center.y };

	if (isless(angle, 0.0)) {
		angle = 360 - angle;
	}

	// 0 <= angle < 90
	if (isless(angle, 90.0)) {
		new_coords.x += modulus * sin(DEG2RAD(angle));
		new_coords.y -= modulus * cos(DEG2RAD(angle));
	}
	// 90 <= angle < 180
	else if (isless(angle, 180.0)) {
		new_coords.x += modulus * sin(DEG2RAD(angle - 90));
		new_coords.y += modulus * cos(DEG2RAD(angle - 90));
	}
	// 180 <= angle < 270
	else if (isless(angle, 270.0)) {
		new_coords.x -= modulus * sin(DEG2RAD(angle - 180));
		new_coords.y += modulus * cos(DEG2RAD(angle - 180));
	}
	// 270 <= angle < 360
	else if (isless(angle, 360.0)) {
		new_coords.x -= modulus * sin(DEG2RAD(angle - 270));
		new_coords.y -= modulus * cos(DEG2RAD(angle - 270));
	}
	return new_coords;
}

void
manageEvents(struct Floor* floor, FrontData* fd) {
	ALLEGRO_EVENT current_ev;

	al_wait_for_event(fd->evqueue, &current_ev);

	//if (al_get_next_event(fd->evqueue, &current_ev)) {
		switch (current_ev.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				fd->request.exit = 1;
				break;

			case ALLEGRO_EVENT_KEY_DOWN:
				fd->keyboard_keys[current_ev.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_KEY_UP:
				fd->keyboard_keys[current_ev.keyboard.keycode] &= KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_TIMER:
				if (current_ev.timer.source == fd->timer.main) {
					checkKeys(fd);
				}
				if (current_ev.timer.source == fd->timer.animations) {
					switch (fd->game_mode)
					{
						case MODE1:
							if (floor != NULL) drawFloor(floor, fd);
							break;
						default:
							break;
					}
					fd->request.lock_front = !fd->request.lock_front; // Unlock backend
				}
				// Mode2 MUST NOT be locked. This is verified just in case
				if (fd->game_mode == MODE2) {
					fd->request.lock_front = fd->request.lock_front = 0;
				}

				break;
			default:
				break;
		}
//	}
}


static void
checkKeys(FrontData* fd) {
	if (fd == NULL) return;

	if (fd->keyboard_keys[ALLEGRO_KEY_Q] == KEY_READY) {
		fd->keyboard_keys[ALLEGRO_KEY_Q] &= KEY_SEEN; // Clear key

		fd->request.exit = 1;
	}
	else if (fd->keyboard_keys[ALLEGRO_KEY_R] == KEY_READY) {
		fd->keyboard_keys[ALLEGRO_KEY_R] &= KEY_SEEN; // Clear key

		fd->request.restart = 1;
	}
}