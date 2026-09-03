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
    int data;
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
Node *generateMazeTemplate() {
    // create linked list for maze
    // creates the config, indicating that this linked list should contain the
    // number of rows dictated by MAZE_ROWS
    LinkedListConfig rowsConfig = {.size = MAZE_ROWS};
    // create the linked list of rows, with the first row, the head of this
    // doubly linked list being returned
    Node *firstRow = initDoublyLinkedList(&rowsConfig);

    // setting up columns w/ iteration and config
    Node *rowNode = firstRow;
    LinkedListConfig columnsConfig = {.size = MAZE_COLUMNS};

    // this bool lets us set a bunch of walls if the first or last row is set
    bool shouldBeWall;
    // for each node in the linked list, set its data to another linked list
    // outer linked list = rows
    // each inner linked list = columns
    while (rowNode->nextNode != NULL) {
        rowNode->data = initDoublyLinkedList(&columnsConfig);
        // check if the row is the first or last, in which case all created
        // tiles should be walls
        if (isHead(rowNode) || isTail(rowNode)) {
            shouldBeWall = true;
        } else {
            shouldBeWall = false;
        }

        // second loop to loop through inner linked lists and assign each data
        // attribute a tile struct
        Node *columnNode = rowNode->data;
        while (columnNode->nextNode != NULL) {
            // instantiate new MazeTile for each tile
            MazeTile *tile = malloc(sizeof(MazeTile));

            if (tile == NULL) {
                printf("Memory allocation failed\n");
                exit(1);
            }

            // check if the tile should be a wall and set its value
            // plus check if the current node in the iteration is the first or
            // last, in which case it should be a wall
            if (shouldBeWall || isTail(columnNode) || isHead(columnNode)) {
                tile->data = 1;
            } else {
                tile->data = 0;
            }

            // set the inner list's data to be a MazeTile
            columnNode->data = tile;

            // go to the next node
            columnNode = columnNode->nextNode;
        }

        // go to the next row
        rowNode = rowNode->nextNode;
    }

    return firstRow;
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
    Node *maze = generateMazeTemplate();
    printLinkedList(maze);

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
