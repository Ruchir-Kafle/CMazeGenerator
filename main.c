#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// linked list implementation
// linked list node
// maze implemetation:
//      maze row -> maze row -> maze row
//      maze tile -> maze tile -> maze tile
#include "linkedList.h"

// macros
#define MAZE_ROWS 10
#define MAZE_COLUMNS 10

// forward declaration
typedef struct MazeTile MazeTile;

// MazeTile properties, value of a single node of a row
struct MazeTile {
    int value;
    MazeTile *right;
    MazeTile *left;
    MazeTile *up;
    MazeTile *down;
};

// config structs

// maze to allow optional inputs for pathing
typedef struct {
    int maze_entrance;
    int maze_exit;
} MazeConfig;

// generate maze template
void generateMazeTemplate(int maze[][MAZE_COLUMNS]) {

    // init array w/ MAZE_ROWS rows and MAZE_COLUMNS columns
    for (int i = 0; i < MAZE_ROWS; i++) {
        for (int j = 0; j < MAZE_COLUMNS; j++) {
            // instantiate new MazeTile for each tile
            MazeTile *tile = malloc(sizeof(MazeTile));

            if (tile == NULL) {
                printf("Memory allocation failed\n");
                exit(1);
            }

            // if edge, place wall; else empty
            if (i == MAZE_ROWS - 1 || j == MAZE_COLUMNS - 1 || i == 0 ||
                j == 0) {
                tile->value = 1;
            } else {
                tile->value = 0;
            }
        }
    }
}

// generate maze paths from the entrance out
void generatePaths(int maze[][MAZE_COLUMNS], MazeConfig *config) {

    // checking if the user provided the optional params: maze entrance and exit
    int maze_entrance_tile = config->maze_entrance ? config->maze_entrance : -1;
    int maze_exit_tile = config->maze_exit ? config->maze_exit : -1;

    // if maze_entrance_tile was provided, set that tile to entrance; else
    // mid-tile is entrance
    if (maze_entrance_tile != -1) {
        maze[MAZE_ROWS - 1][maze_entrance_tile] = 3;
    } else {
        // approximately in the middle, for odd sized mazes, take half then
        // round down
        maze[MAZE_ROWS - 1][(int)((MAZE_COLUMNS - 1) / 2)] = 3;
    }

    // same for maze_exit_tile as maze_entrance_tile
    if (maze_exit_tile != -1) {
        maze[MAZE_ROWS - 1][maze_exit_tile] = 3;
    } else {
        maze[MAZE_ROWS - 1][(int)((MAZE_COLUMNS - 1) / 2)] = 3;
    }
}

void printMaze(int maze[][MAZE_COLUMNS]) {
    for (int i = 0; i < MAZE_ROWS; i++) {
        printf("[");
        for (int j = 0; j < MAZE_COLUMNS; j++) {
            printf(" %d", maze[i][j]);
        }
        printf(" ]\n");
    }
}

// temp main for debugging maze logic
int main() {

    int maze[MAZE_ROWS][MAZE_COLUMNS];
    generateMazeTemplate(maze);

    printMaze(maze);

    return 0;
}

// raylib code for displaying stuff
int raylib_main() {

    // initialize window
    InitWindow(600, 400, "Maze");

    // main game loop
    while (!WindowShouldClose()) {
        // begin loop, start drawing
        BeginDrawing();

        // all drawing should happen below

        ClearBackground(SKYBLUE);

        // end loop, stop drawing
        EndDrawing();
    }

    // close the window
    CloseWindow();

    return 0;
}
