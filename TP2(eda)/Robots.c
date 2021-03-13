#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Robots.h"
double getRandomAngle(void);

void* generateRobots(struct Floor* floor_p, int ammount) {
	void* pointer;
	pointer = malloc(ammount * sizeof(floor_p->robots));
	if (pointer != NULL) {
		srand(time(0));
		floor_p->robots.robots_number = ammount;
		floor_p->robots.robots->coordinates.x = (0 + rand() % ((floor_p->width) + 1));			//a random number is assigned between zero and the maximum width value
		floor_p->robots.robots->coordinates.y = (0 + rand() % ((floor_p->height) + 1));			//a random number is assigned between zero and the maximum height value
		floor_p->robots.robots->angle = getRandomAngle();
		return pointer;
	}
	else{
		floor_p->robots.robots_number = 0;
		floor_p->robots.robots->coordinates.x = 0;			//a random number is assigned between zero and the maximum width value
		floor_p->robots.robots->coordinates.y = 0;			//a random number is assigned between zero and the maximum height value
		floor_p->robots.robots->angle = 0;
		return NULL;
	}
}