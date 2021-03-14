#ifndef ROBOTS_H
#define ROBOTS_H	1

/******************** DEFINITIONS ********************/
/******************** MACROS ********************/
/******************** STRUCTURES AND TYPEDEF ********************/
typedef struct Coordinates {
    double x;
    double y;
} coords_t;

struct Robot {
    double angle;
    coords_t coordinates;
};

struct RobotCollection {
    int robots_number;
    struct Robot* robots;
};

/******************** PROTOTYPES ********************/
/* 
 * generates a specified number of robots, randomly placing them
 * 
 * Arguments:
 *  b1: Collection where robots will be stored.
 *  ammount: number of robots to generate.
 *  maximum: Maximum X and Y coordinates a robot can take.
 *          (Usually, the floor's width and height).
 * 
 * Returns:
 *  Success: allocated memory segment address
 *  Failure: NULL
 */
void* generateRobots(struct RobotCollection* b1, int ammount, coords_t maximum);

/*
 * Destroy all robots in a robot collection and sets its robots_number to 0.
 * 
 * Arguments:
 *  None
 * Returns:
 *  Nothing
 */
void destroyRobots(struct RobotCollection* b1);


/*
 * Perform robots movement around the board.
 * 
 * Arguments:
 *  b1: Robots to move around the board.
 *  maximum_coordinates: Max x and y where a robot can stand 
 *                       (usually, the floor's height and width).
 * 
 * Returns:
 *  Success: Pointer to the floor argument
 *  Failure: NULL
 */
void * moveRobots(struct RobotCollection* b1, coords_t maximum_coordinates);

#endif