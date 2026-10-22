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
// maze rows and columns should be odd
#define MAZE_ROWS 81
#define MAZE_COLUMNS 81

#define HORIZONTAL_PADDING 200
#define VERTICAL_PADDING (HORIZONTAL_PADDING / 2)

#define TILE_DIMENSIONS 10

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

typedef enum {
    UNVISITED,
    EMPTY,
    WALL
} COLORS;

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
    while (rowNode != NULL) {
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
        while (columnNode != NULL) {
            // instantiate new MazeTile for each tile
            MazeTile *tile = malloc(sizeof(MazeTile));

            if (tile == NULL) {
                printf("Memory allocation failed\n");
                exit(1);
            }

            // check if the tile should be a wall and set its value
            // plus check if the current node in the iteration is the first or
            // last, in which case it should be a wall
            // or, for recursiveBacktracking, if the node is at an even row and column, it should be a wall
            // this creates a grid where every other block is a wall
            if (shouldBeWall || isTail(columnNode) || isHead(columnNode) 
                || (rowNode->index % 2 == 0 || columnNode->index % 2 == 0)) {
                tile->data = WALL;
            } else {
                tile->data = EMPTY;
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
void linkMazeTiles(Node *mazeHead) {
    // check if maze head is not provided
    if (mazeHead == NULL || !isHead(mazeHead)) {
        printf("Maze head not provided");
        return;
    }

    // loop through rows and loop through columns
    Node *row = mazeHead;

    while (row != NULL) {
        Node *column = (Node *)row->data;

        while (column != NULL) {
            MazeTile *workingMazeTile = (MazeTile *)column->data;

            workingMazeTile->left = column->previousNode ? (MazeTile *)column->previousNode->data : NULL;
            workingMazeTile->right = column->nextNode ? (MazeTile *)column->nextNode->data : NULL;

            workingMazeTile->up = row->previousNode ? (MazeTile *)correspondingElementInPreviousRow(row, column)->data : NULL;
            workingMazeTile->down = row->nextNode ? (MazeTile *)correspondingElementInNextRow(row, column)->data : NULL;

            column = column->nextNode;
        }

        row = row->nextNode;
    }
}          

// generate maze paths from the entrance out
void generatePaths(Node *mazeHead, MazeConfig *config) {

    // checking if the user provided the optional params: maze entrance and exit
    int maze_entrance_tile = config->maze_entrance ? config->maze_entrance : -1;
    int maze_exit_tile = config->maze_exit ? config->maze_exit : -1;

    // if maze_entrance_tile was provided, set that tile to entrance; else
    // mid-tile is entrance
    if (maze_entrance_tile != -1) {
    } else {                 
  			
        // approximately in the middle, for odd sized mazes, take half then
        // round down
    }
    // same for maze_exit_tile as maze_entrance_tile
    if (maze_exit_tile != -1) {
    } else {
    }
}

void recursiveBacktracking(Node *mazeHead) {
    // perfect maze generation algorithm
    // simple explanation:
    //      pick a random place to begin and set its state to visited
    //      choose an adjacent unvisited cell, which in this case will be two blocks away
    //      this is necessary in order to allow walls to have space on the board.
    //      When two cells have been picked, by going from one to the next,
    //      get rid of the wall in between them so they are merged if it has not yet been visited.
    //      The next cell should have its state set to visited.
    //      Continue until there are no more adjacent cells.
    //      Backtrack until you can visit a new cell, and when you return to start through backtracking, finish.
    
    // ensure user input validity
    if (mazeHead == NULL) {                                                                                                                                                                                                                                                                                                                                                            
        fprintf(stderr, "Invalid maze head provided");
        exit(EXIT_FAILURE);
    }

    // pick random tile to begin
    int randomRow = GetRandomValue(0, MAZE_ROWS-1);
    int randomCol = GetRandomValue(0, MAZE_COLUMNS-1);
    
    Node *randomTileRow = searchByIndex(mazeHead, randomRow);
    Node *randomTile = searchByIndex(randomTileRow->data, randomCol);
    MazeTile *randomMazeTile = randomTile->data; 
   
    // memory
    LinkedListConfig visitedConfig = {.size = 1};
    LinkedListConfig nestedArrayConfig = {.size = 2};
    Node *visitedNodes = initDoublyLinkedList(&visitedConfig);
    visitedNodes->data = initDoublyLinkedList(&nestedArrayConfig);

    // setting the memory to the row and column which the maze tile is in
    Node *nestedNode = visitedNodes->data;
    nestedNode->data = &randomRow;
    nestedNode->nextNode->data = &randomCol;

    int dx = 0, dy = 0;
    // instantiating the choices array with nothing
    // choices are left, right, up, down; each is x, y
    int choices[4][2] = {{0,0}, {0,0}, {0,0}, {0,0}};

    if (randomMazeTile->left != NULL) {
        choices[0][0] = -1;
    }
    if (randomMazeTile->right != NULL) {
        choices[1][0] = 1;
    }
    if (randomMazeTile->up != NULL) {
        choices[2][1] = 1;
    }
    if (randomMazeTile->down != NULL) {
        choices[3][1] = -1;
    }

    // choosing which adjacent tile in the choices array we should go to
    while (dx + dy != 1) {
        int choose = GetRandomValue(0, 3);
        dx = choices[choose][0];
        dy = choices[choose][1];
    }

}


void printMaze(Node *head) {
    // printing the maze requires the head of the linked list, otherwise adds
    // more complexity
    if (!isHead(head)) {
        printf("Not linked list head.");
        return;
    }

    // creating an iteration variable for the rows
    Node *row = head;
    while (row != NULL) {
        Node *column = (Node *)row->data;

        while (column != NULL) {
            printf(" %d", ((MazeTile *)column->data)->data);
            column = column->nextNode;
        }

        printf("\n");
        row = row->nextNode;
    }
}

// raylib code for displaying stuff
void raylib_main(Node *mazeHead) {

    // initialize window
    InitWindow(MAZE_COLUMNS * TILE_DIMENSIONS + HORIZONTAL_PADDING, MAZE_ROWS * TILE_DIMENSIONS + VERTICAL_PADDING, "Maze");

    // main game loop
    while (!WindowShouldClose()) {
        // begin loop, start drawing
        BeginDrawing();

            // all drawing should happen below
            ClearBackground(SKYBLUE);
            
            int halfXPadding = HORIZONTAL_PADDING / 2;
            int halfYPadding = VERTICAL_PADDING / 2;

            for (int i = 0; i < MAZE_ROWS; i++) {
                for (int j = 0; j < MAZE_COLUMNS; j++) {
                    Color color;

                    Node *row = searchByIndex(mazeHead, i);
                    Node *tileNode = searchByIndex(row->data, j);
                    MazeTile *correspondingTile = tileNode->data;
                    switch (correspondingTile->data) {
                        case (EMPTY):
                            color = WHITE;
                            break;
                        case (WALL):
                            color = GRAY;
                            break;
                    }

                    DrawRectangle(halfXPadding + i * TILE_DIMENSIONS, 
                                  halfYPadding + j * TILE_DIMENSIONS, 
                                  TILE_DIMENSIONS, 
                                  TILE_DIMENSIONS, 
                                  color);
                }
            }
 


        // end loop, stop drawing
        EndDrawing();
    }

    // close the window
    CloseWindow();
}

// temp main for deb;ugging maze logic
int main() {
    Node *maze = generateMazeTemplate();
    linkMazeTiles(maze);

    printMaze(maze);

    raylib_main(maze);

    return 0;
}

