#include "Backend.h"
#include <stdlib.h>				//this library includes malloc

/**********************PROTOTYPES******************/
void* createFloor(struct Floor*, int height, int widht, int robots_amount);			// this funtion returns NULL in case it fails to allocate the memory segment

void destroyFloor(struct Floor*);

void generateRobots(struct Floor*, int ammount);




void* createFloor(struct Floor* floor_p, int height, int widht, int robots_amount) {
	floor_p->clean = malloc((height * widht) * sizeof(int));				//an order is placed for a memory segment
	if (floor_p->clean != NULL) { 							//if returns NULL then the memory segment could not be allocated,otherwise it will contain the segment address
		floor_p->height = height;
		floor_p->width = widht;
		floor_p->time_to_clean = 0;							//initially since the time was not calculated, it is zero
		generateRobots(floor_p, robots_amount);
	}
	else{												// the main is informed that it failed
		return NULL;
	}
}


void destroyFloor(struct Floor* floor_p){
	free(floor_p->clean);
}