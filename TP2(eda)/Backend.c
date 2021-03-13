#include <stdlib.h>				//this library includes malloc
#include <time.h>

#include "Backend.h"
#include "Robots.h"
/**********************PROTOTYPES******************/
void* createFloor(struct Floor*, int height, int widht, int robots_amount);			// this funtion returns NULL in case it fails to allocate the memory segment

void destroyFloor(struct Floor*);






void* createFloor(struct Floor* floor_p, int height, int widht, int robots_amount) {
	void* pointer;
    srand(time(NULL)); // For robots :)

	floor_p->clean = malloc((height * widht) * sizeof(int));				//an order is placed for a memory segment
	if (floor_p->clean != NULL) { 							//if returns NULL then the memory segment could not be allocated,otherwise it will contain the segment address
		floor_p->height = height;
		floor_p->width = widht;
		floor_p->clean_size = (height*widht);
		floor_p->time_to_clean = 0;							//initially since the time was not calculated, it is zero
		pointer = generateRobots(floor_p, robots_amount);
		if (pointer == NULL) {							//if it points to null then the robots could not be created
			return NULL;
		}
		else {
			return floor_p->clean;
		}
	}
	else{												// the main is informed that it failed

		return NULL;
	}
}


void destroyFloor(struct Floor* floor_p){
	floor_p->robots.robots_number = 0;
	floor_p->robots.robots->coordinates.x = 0;
	floor_p->robots.robots->coordinates.y = 0;
	floor_p->robots.robots->angle = 0;
	floor_p->height = 0;
	floor_p->width = 0;
	floor_p->clean_size = (0);
	floor_p->time_to_clean = 0;
	free(floor_p->robots.robots);
	free(floor_p->clean);
}