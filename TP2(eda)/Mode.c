#include "Backend.h"
#include "Mode.h"
#include "Robots.h"
#include <stdio.h>


int runModeOne(struct Floor* floor_p,int robots_number, int width, int height) {
	void*status,*moverobots_p;
	int is_clean,is_all_clean=0, fail=0;
	status = createFloor(floor_p, height, width, robots_number);
	if (status != NULL) {						//if he could assign it, I'll go for a loop
		while ((is_all_clean == TILE_CLEAN) || fail==0) {	//the loop will repeat until it is all clean or until some function fails
															
			publishStatus(floor_p->clean, frotdata);///no se que hago aca
			moverobots_p = moveRobots(floor_p);
			if (moverobots_p != NULL) {

				is_clean = cleanFloor(floor_p);
				if (is_clean == SUCCESS) {

					is_all_clean = isFloorClean(floor_p);
				}
				else {
					fail = 1;		//the robot could not clean its tile(cleanFloor)
				}
			}
			else {					//the robots could not move(moveRobots)
				fail = 1;
			}

		}
	}
	else {
		return FAILURE;
	}
	if (fail == 1) {
		return FAILURE;
	}
	else {
		return SUCCESS;
	}

}
