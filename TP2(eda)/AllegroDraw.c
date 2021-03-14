#include "AllegroDraw.h"

#include <stdio.h>
#include <Windows.h>

static int drawFloor(struct Floor* floor, void* front_data);

static int drawFunction(struct Floor* floor, void* front_data);

int initFrontEnd(void* front_data){

	int error = 0;

	FrontData* p2front_data = (FrontData*) front_data;

	p2front_data->times_recorded = (double*)malloc(50 * sizeof(double));	//Here is where I'm going to save the times that are entered

	if (p2front_data->times_recorded == NULL) {

		printf("(Chuckles) I'm in danger\n");
		printf("Error allocating memory for front_data\n");
		error = 1;
	}

	p2front_data->times_count = 0;
	return 0;
}

int initAllegro(ALLEGRO_DISPLAY** disp, ALLEGRO_FONT** font) {

	int error = 0;

	al_init();

	if (!al_init_primitives_addon()) {

		printf("Error intializing primitives addon\n");
		error = 1;
	}
	if (!al_init_font_addon()) {

		printf("Error initializing font addon\n");
		error = 1;
	}
	if (!((*disp) = al_create_display(SCREENWIDHT + 2 * BORDE_WIDTH, SCREENHEIGHT + 2 * BORDE_WIDTH))) {

		printf("Error loading display\n");
		error = 1;
	}
	if (!(*font = al_load_font("IBMPlexSerif-Regular.ttf", 50, 0))) {

		printf("Error loading font\n");
		error = 1;
	}

	return error;
}

void destroyFrontEnd(ALLEGRO_FONT** font){

	al_destroy_font(*font);
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

	int w = floor->width;
	int h = floor->height;

	int horizontal_gap = SCREENWIDHT / w;	//Gap between columns
	int vertical_gap = SCREENHEIGHT / h;		//Gap between rows

	double x;	//Variables that will be used in the drawing of the robots
	double y;

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

	
	for (int i = 0; i<floor->robots.robots_number; i++){

		x = (floor->robots.robots[i]).coordinates.x;
		y = (floor->robots.robots[i]).coordinates.y;

		al_draw_filled_circle(y*vertical_gap + (SCREENWIDHT - horizontal_gap * w) / 2 + BORDE_WIDTH,
			y*horizontal_gap + (SCREENHEIGHT - vertical_gap * h) / 2 + BORDE_WIDTH ,
			6, al_map_rgb(0,230,255) );
	}

	al_flip_display();

	Sleep(500);

	return 0;
}

int drawFunction(struct Floor* floor, void* front_data) {

	int error = 0;
	FrontData* p2front_data = (FrontData*)front_data;	//This variables are defined to make the function more readable
	int count = p2front_data->times_count;
	double* times_recorded = p2front_data->times_recorded;

	if (floor->time_to_clean > 0.1) {	//I won't draw anything until the time is beaten

		times_recorded[count] = floor->time_to_clean;
		count++;

		al_clear_to_color(al_map_rgb(255, 255, 255));

		//al_draw_text(font, al_map_rgb(0, 0, 0), SCREENWIDHT / 2, SCREENHEIGHT / 2, ALLEGRO_ALIGN_LEFT, "SIMULANDO CON");
		//al_draw_text(font, al_map_rgb(0, 0, 0), SCREENWIDHT / 2, SCREENHEIGHT / 2 - 100, ALLEGRO_ALIGN_LEFT, (conteo+1) );

		al_flip_display();

	}
	else {
		times_recorded[count] = floor->time_to_clean;
		count++;						//Here is the total number of "robots_number" that has been used

		//Here comes the drawing of the function

		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_draw_line(BORDE_WIDTH, BORDE_WIDTH, BORDE_WIDTH, BORDE_WIDTH + SCREENHEIGHT, al_map_rgb(0, 0, 0), 8);
		al_draw_line(BORDE_WIDTH, BORDE_WIDTH + SCREENHEIGHT, BORDE_WIDTH + SCREENWIDHT, BORDE_WIDTH + SCREENHEIGHT, al_map_rgb(0, 0, 0), 8);

		int x_axis_gap = SCREENWIDHT / (count + 1);
		int y_axis_gap = SCREENHEIGHT / times_recorded[0];	//I get the y_axis gap using the highest time.

		for (int i = 1; i < (count + 1); i++) {

			al_draw_line(BORDE_WIDTH + i * x_axis_gap, +SCREENHEIGHT + BORDE_WIDTH + 10,
				BORDE_WIDTH + i * x_axis_gap, SCREENHEIGHT + BORDE_WIDTH - 10, al_map_rgb(0, 0, 0), 4);
		}
		for (int i = 1; i < (count + 1); i++) {

			al_draw_line(BORDE_WIDTH + 10, BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap,
				BORDE_WIDTH - 10, BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap, al_map_rgb(0, 0, 0), 4);
		}

		for (int i = 1; i < (count + 1); i++) {	//I run over every time recorded to draw the point

			al_draw_filled_circle(BORDE_WIDTH + i * x_axis_gap, BORDE_WIDTH + SCREENHEIGHT - times_recorded[i - 1] * y_axis_gap,
				12, al_map_rgb(255, 0, 0));
		}

		//al_draw_text(font, al_map_rgb(0, 0, 0), SCREENWIDHT / 4, SCREENHEIGHT / 4, ALLEGRO_ALIGN_LEFT, "01235");

		al_flip_display();

		Sleep(500);
	}

	return error;
}