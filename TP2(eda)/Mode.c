#include <stdio.h>
#include <math.h> /* islessequal */
#include <time.h> /* clock_t, clock(), CLOCKS_PER_SEC */
#include "Backend.h"
#include "Mode.h"
#include "Robots.h"

static void* createFloor(struct Floor*, int height, int widht, int robots_amount);			// this funtion returns NULL in case it fails to allocate the memory segment
static void destroyFloor(struct Floor*);
static int cleanFloor(struct Floor* floor_p);
static int isFloorClean(struct Floor* floor_p);

int runModeOne(int robots_number, int width, int height, statusCallback publishStatus, void * front_data) {
    struct Floor floor = {0};

    void*status,*moverobots_p;

    int is_clean,is_all_clean=0, fail=0;

    status = createFloor(&floor, height, width, robots_number);

    if (status != NULL) {						//if he could assign it, I'll go for a loop
        while ((is_all_clean == TILE_CLEAN) || fail==0) {	//the loop will repeat until it is all clean or until some function fails
            publishStatus(&floor, front_data);
            coords_t max = { floor.width, floor.height };
            moverobots_p = moveRobots(&floor, max);
            if (moverobots_p != NULL) {

                is_clean = cleanFloor(&floor);
                if (is_clean == SUCCESS) {

                    is_all_clean = isFloorClean(&floor);
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

int runModeTwo(int width, int height, statusCallback publishStatus, void* front_data) {
    int needed_robots = 0;

    double times_sum = 0.0; // Sum of all times elapsed until now to clean the floor
    double prev_simu_time = 0.0;

    struct Floor current_floor = { 0 };

    clock_t initial_time = 0, final_time = 0; // For timing

    if (createFloor(&current_floor, height, width, needed_robots) == NULL) {
        return FAILURE;
    }

    do {
        // The first time this loop is running, thhis three lines 
        // do not do anything really useful.
        times_sum = 0.0;
        prev_simu_time = current_floor.time_to_clean;
        destroyRobots(&(current_floor.robots));

        needed_robots += 1;
        generateRobots(&(current_floor.robots), needed_robots,
                        current_floor.width, current_floor.height);

        for (int i = 0; i < SIMULATION_ITERATIONS; i++) {
            floorSoftReset(&current_floor);
            final_time = 0;
            initial_time = clock();

            do {
                coords_t max = {(double)current_floor.width, 
                                (double) current_floor.height};
                moveRobots(&(current_floor.robots), max);
                cleanFloor(&current_floor);
            } while (isFloorClean(&current_floor) != TILE_CLEAN);

            final_time = clock();
            times_sum += elapsedTime(final_time, initial_time);
        }

        current_floor.time_to_clean = times_sum / SIMULATION_ITERATIONS;
        publishStatus(&current_floor, front_data);

    } while (islessequal(current_floor.time_to_clean - prev_simu_time,
                         SIMULATIONS_DELTA)
            );

    destroyFloor(&current_floor);
    return SUCCESS;
}

/*
 * Difference between two times measured using clock_t.
 *
 * Arguments:
 *  final: Last measured time.
 *  initial: FIrst measured time.
 * 
 * Returns:
 *  The difference between those times as double.
 */
static double elapsedTime(clock_t final, clock_t initial) {
    return ( ( (double)(final - initial) ) / CLOCKS_PER_SEC );
}


/*
 * Instead of recreating a whole floor, let's make it diry again!
 * 
 * Makes all tiles dirty and resets time_to_clean to 0.
 * Robot related variables and structures are ignoread, as well as
 * width and height properties.
 * 
 * Arguments:
 *  floor_p: The floor to soft-reset.
 * 
 * Returns:
 *  Nothing
 */
static void floorSoftReset(struct Floor* floor_p) {
    for (int i = 0; i < floor_p->clean_size; i++) {
        floor_p->clean[i] = TILE_DIRTY;
    }
    floor_p->time_to_clean = 0.0;
}

/*
 * Create a floor where robots will work.
 * 
 * Arguments:
 *  floor_p: Structure where the new floor will be stored.
 *  height: Floor's desired height.
 *  width: Floor's desired width.
 *  robots_ammount: How many robots should be initialized over this floor.
 * 
 * Returns:
 *  Nothing
 */
static void* createFloor(struct Floor* floor_p, int height, int widht, int robots_amount) {
    void* pointer;
    srand(time(NULL)); // For robots :)

    floor_p->clean = malloc((height * widht) * sizeof(int));				//an order is placed for a memory segment
    if (floor_p->clean != NULL) { 							//if returns NULL then the memory segment could not be allocated,otherwise it will contain the segment address
        floor_p->height = height;
        floor_p->width = widht;
        floor_p->clean_size = (height*widht);
        floor_p->time_to_clean = 0;							//initially since the time was not calculated, it is zero
        pointer = generateRobots(&(floor_p->robots), robots_amount,
                                 floor_p->width, floor_p->height);
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

/*
 * Destroy a floor.
 * 
 * Arguments:
 *  None
 * 
 * Returns:
 *  Nothing
 */
static void destroyFloor(struct Floor* floor_p){
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
int cleanFloor(struct Floor* floor_p) {
    if (floor_p == NULL)
        return FAILURE;

    int i = 0;
    while (i < floor_p->robots.robots_number) {
        int x = (int) floor(floor_p->robots.robots[i].coordinates.x);
        int y = (int) floor(floor_p->robots.robots[i].coordinates.y);

        // floor->clean is a 2D dynamic array
        floor_p->clean[x + floor_p->width * y] = TILE_CLEAN;
        i++;
    }
    return SUCCESS;
}

/*
 * Verify if the whole floor has been cleaned.
 * 
 * Arguments:
 *  floor_p: Floor to verify.
 * 
 * Returns:
 *  All tiles are clean: TILE_CLEAN definition
 *  At least one tile is dirty: TILE_DIRTY definition
 */
int isFloorClean(struct Floor* floor) {
    if (floor == NULL)
        return FAILURE;

    int tile_status = TILE_CLEAN;
    int i = 0;
    while (i < floor->clean_size && tile_status != TILE_DIRTY) {
        tile_status = floor->clean[i];
        i++;
    }
    return tile_status;
}

