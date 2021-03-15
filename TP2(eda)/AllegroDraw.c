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
static int drawFloor(struct Floor* floor, void* front_data);

/*Uses allegro to draw the final function given all the data*/
static int drawFunction(struct Floor* floor, void* front_data);

/* Given an angle and modulus, get a point relative to the one called center. */
static coords_t directionVectorPosition(coords_t center, double angle, double modulus);
/* Draw a single robot over the floor */
static void  drawRobot(coords_t center, double angle, float tile_width, float tile_height);

/*******************************************************************************
 *******************************************************************************
							FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

int initFrontEnd(void* front_data){

	int error = 0;

	FrontData* p2front_data = (FrontData*) front_data;

	p2front_data->times_recorded = (long double*)malloc(50 * sizeof(long double));	//Here is where I'm going to save the times that are entered

	if (p2front_data->times_recorded == NULL) {

		printf("(Chuckles) I'm in danger\n");
		printf("Error allocating memory for front_data\n");
		error = 1;
	}

	p2front_data->times_count = 0;
	return 0;
}

int initAllegro(ALLEGRO_DISPLAY** disp, ALLEGRO_FONT** font1, ALLEGRO_FONT** font2) {

	int error = 0;

	al_init();

	if (!al_init_primitives_addon()) {

		printf("Error intializing primitives addon\n");
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

void destroyFrontEnd(void * front_data){

	FrontData* p2front_data = (FrontData*)front_data;

	al_destroy_font(p2front_data->small_font);
	al_destroy_font(p2front_data->big_font);
	free(p2front_data->times_recorded);
}

int publishStatus(struct Floor* floor, void* front_data) {

	if (floor->game_mode == 1) {

		drawFloor(floor, front_data);
	}
	else if (floor->game_mode == 2) {

		drawFunction(floor, front_data);
	}
	return 0;
}

int drawFloor(struct Floor* floor, void* front_data) {

	((FrontData*)front_data)->game_mode = floor->game_mode;	//I initialize data that has to be given to the main
	((FrontData*)front_data)->times_count = (int)floor->time_to_clean;

	int w = floor->width;
	int h = floor->height;

	int horizontal_gap = SCREENWIDHT / w;	//Gap between columns
	int vertical_gap = SCREENHEIGHT / h;		//Gap between rows

	int dist_lat_border = (SCREENWIDHT - horizontal_gap * w) / 2 + BORDE_WIDTH;	//I define this to center the floor in the display
	int dist_vert_border = (SCREENHEIGHT - vertical_gap * h) / 2 + BORDE_WIDTH;

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(dist_lat_border,
		dist_vert_border,
		dist_lat_border + horizontal_gap * w,
		dist_vert_border + vertical_gap * h,
		al_map_rgb(255, 255, 255)
	);

	for (int i = 0; i < h; i++) {

		for (int j = 0; j < w; j++) {

			if ((floor->clean)[i * w + j] == TILE_DIRTY) {	//If the tile is dirty I print a brown rectangle

				al_draw_filled_rectangle(dist_lat_border + j * horizontal_gap, dist_vert_border + i * vertical_gap, dist_lat_border + (j + 1) * horizontal_gap, dist_vert_border + (i + 1) * vertical_gap, al_map_rgb(131, 86, 86));
			}
			else if ((floor->clean)[i * w + j] == TILE_CLEAN) {	//If the tile is clean I print a white rectangle

				al_draw_filled_rectangle(dist_lat_border + j * horizontal_gap, dist_vert_border + i * vertical_gap, dist_lat_border + (j + 1) * horizontal_gap, dist_vert_border + (i + 1) * vertical_gap, al_map_rgb(255, 255, 255));
			}
		}
	}

	for (int i = 0; i < h; i++) {	//I draw the horizontal lines of the tiles

		al_draw_line(dist_lat_border, dist_vert_border + i * vertical_gap, dist_lat_border + SCREENWIDHT, dist_vert_border + i * vertical_gap, al_map_rgb(0, 0, 0), -1);
	}
	for (int i = 0; i < w; i++) {	//I draw the vertical lines of the tiles

		al_draw_line(dist_lat_border + i * horizontal_gap, dist_vert_border, dist_lat_border + i * horizontal_gap, dist_vert_border + SCREENWIDHT, al_map_rgb(0, 0, 0), -1);
	}

	for (int i = 0; i < floor->robots.robots_number; i++) {
		coords_t center = {
			.x = floor->robots.robots[i].coordinates.x * horizontal_gap + (SCREENWIDHT - horizontal_gap * w) / 2 + BORDE_WIDTH,
			.y = floor->robots.robots[i].coordinates.y * vertical_gap + (SCREENHEIGHT - vertical_gap * h) / 2 + BORDE_WIDTH
		};
		drawRobot(center, floor->robots.robots[i].angle, horizontal_gap, vertical_gap);
	}

	al_flip_display();

	Sleep(200);

	return 0;
}

int drawFunction(struct Floor* floor, void* front_data) {

	int error = 0;
	FrontData* p2front_data = (FrontData*)front_data;	//This variables are defined to make the function more readable
	long double* times_recorded = p2front_data->times_recorded;

	p2front_data->game_mode = floor->game_mode;

	printf("%f\n", floor->time_to_clean);

	times_recorded[(p2front_data->times_count)] = floor->time_to_clean;
	(p2front_data->times_count)++;

	al_clear_to_color(al_map_rgb((p2front_data->times_count) * 13, (p2front_data->times_count) * 7, (p2front_data->times_count) * 21));

	al_draw_text(p2front_data->big_font, al_map_rgb(0,0,0), SCREENWIDHT/2-200, SCREENHEIGHT/2-50, 0, "SIMULANDO");

	al_flip_display();

	return error;
}

int drawFunction2(FrontData * front_data){

	char aux_str[20];	//String that's being used to write letters and numbers

	FrontData* p2front_data = (FrontData*)front_data;	//This variables are defined to make the function more readable
	long double* times_recorded = p2front_data->times_recorded;

	(p2front_data->times_count)++;						//Here is the total number of "robots_number" that has been used

	//Here comes the drawing of the function

	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_line(BORDE_WIDTH, BORDE_WIDTH, BORDE_WIDTH, BORDE_WIDTH + SCREENHEIGHT, al_map_rgb(0, 0, 0), 4);
	al_draw_line(BORDE_WIDTH, BORDE_WIDTH + SCREENHEIGHT, BORDE_WIDTH + SCREENWIDHT, BORDE_WIDTH + SCREENHEIGHT, al_map_rgb(0, 0, 0), 4);

	int x_axis_gap = SCREENWIDHT / ((p2front_data->times_count) + 1);
	int y_axis_gap = SCREENHEIGHT / times_recorded[0];	//I get the y_axis gap using the highest time.

	for (int i = 1; i < ((p2front_data->times_count) + 1); i++) {

		al_draw_line(BORDE_WIDTH + i * x_axis_gap, +SCREENHEIGHT + BORDE_WIDTH + 10,
			BORDE_WIDTH + i * x_axis_gap, SCREENHEIGHT + BORDE_WIDTH - 10, al_map_rgb(0, 0, 0), 2);
	}
	for (int i = 1; i < ((p2front_data->times_count) + 1); i++) {

		al_draw_line(BORDE_WIDTH + 10, BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap,
			BORDE_WIDTH - 10, BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap, al_map_rgb(0, 0, 0), 2);
	}

	for (int i = 1; i < ((p2front_data->times_count) + 1); i++) {	//I run over every time recorded to draw the point

		al_draw_filled_circle(BORDE_WIDTH + i * x_axis_gap, BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap,
			4, al_map_rgb(255, 0, 0));

		_gcvt_s(aux_str, 20 * sizeof(char), times_recorded[i - 1], 6);

		al_draw_text(p2front_data->small_font, al_map_rgb(0, 0, 0), BORDE_WIDTH + i * x_axis_gap,
			BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap - 15, ALLEGRO_ALIGN_LEFT, aux_str);
	}

	al_flip_display();

	Sleep(50000);
}

void drawFinalTime(FrontData* front_data) {
	char str_size = 20;
	char * str = malloc(str_size * sizeof(char));
	if (str == NULL)
		return;

	al_clear_to_color(al_map_rgb(255, 255, 255));

	_itoa_s(front_data->times_count, str, str_size, 10);

	al_draw_text(front_data->big_font, al_map_rgb(0, 0, 0), SCREENWIDHT / 2 - 250, SCREENHEIGHT / 2 - 50, 0, "TICKS TOTALES");
	al_draw_text(front_data->big_font, al_map_rgb(0, 0, 0), SCREENWIDHT / 2 - 150, SCREENHEIGHT / 2 + 50, 0, str);

	al_flip_display();

	free(str);

	Sleep(5000);
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