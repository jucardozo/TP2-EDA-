#ifndef ROBOTS_H
#define ROBOTS_H	1

#include "Backend.h"

/******************** DEFINITIONS ********************/
/******************** MACROS ********************/
/******************** STRUCTURES AND TYPEDEF ********************/

/******************** PROTOTYPES ********************/
/* TODO
 * 
 * 
 * Arguments:
 *  floor_p: 
 *  ammount: 
 * 
 * Returns:
 *  Success: 
 *  Failure: 
 */
void* generateRobots(struct Floor* floor_p, int ammount);

/*
 * Clean those tiles where a robot is standing.
 * 
 * Arguments:
 *  floor_p: Floor to clean.
 * 
 * Returns:
 *  Success: SUCCESS definition
 *  Failure: FAILURE definition
 */
int cleanFloor(struct Floor* floor_p);

/*
 * Verify if the whole floor has been cleaned.
 * 
 * Arguments:
 *  floor: Floor to verify.
 * 
 * Returns:
 *  All tiles are clean: TILE_CLEAN definition
 *  At least one tile is dirty: TILE_DIRTY definition
 */
int isFloorClean(struct Floor* floor);

/*
 * Perform robots movement around the board.
 * 
 * Arguments:
 *  floor: Floor and robots to move around.
 * 
 * Returns:
 *  Success: Pointer to the floor argument
 *  Failure: NULL
 */
void * moveRobots(struct Floor* floor);

#endif