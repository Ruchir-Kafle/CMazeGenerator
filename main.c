#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// macros
#define MAZE_ROWS 10
#define MAZE_COLUMNS 10

// forward declaration
typedef struct MazeTile MazeTile;
typedef struct Node Node;

// linked list implementation
// linked list node
// maze implemetation:
//      maze row -> maze row -> maze row
//      maze tile -> maze tile -> maze tile
struct Node {
    Node *previousNode;
    Node *nextNode;

    void *value;
};

// single maze tile
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

// linked list options
typedef struct {
    int size;
} LinkedListConfig;

// optional value to pass in to a node during instantiation
typedef struct {
    void *value;
} NodeConfig;

Node *initSingleNode(NodeConfig *config) {
    void *configValue;

    // check if given a config value for the value the node should be given
    // during instantiation
    if (config != NULL) {
        configValue = config->value ? config->value : NULL;
    } else {
        configValue = NULL;
    }

    // create new node
    Node *newNode = (Node *)malloc(sizeof(Node));

    // check for invalid mem allocation
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    newNode->value = configValue;
    return newNode;
}

Node *initDoublyLinkedList(LinkedListConfig *config) {
    int size;

    // check if size is provided
    if (config != NULL) {
        size = config->size ? config->size : -1;
    } else {
        size = -1;
    }

    // if no size was provided/too small, set to default 2, as 2 is the minimum
    // size
    if (size <= 1) {
        size = 2;
    }

    // head node for when created as a start point for linked list
    Node *head;

    // create a new node for each node wished for
    for (int i = 0; i < size; i++) {
        Node *element = initSingleNode(NULL);

        // if first iteration, set node to head, which always has previousNode
        // to NULL
        if (i == 0) {
            head = element;
            head->previousNode = NULL;
            head->nextNode = NULL;
        } else {
            // setting up a var outside of the while loop for list traversal
            Node *next = head;

            // if the current node's next node is not NULL, that means we have
            // not yet reached the tail, so we must keep setting the current
            // node to the next node until we have found the last node
            while (next->nextNode != NULL) {
                next = next->nextNode;
            }

            // when we have found the tail, we set that node to have a nextNode
            // and our new node to have a previousNode, meaning it is no longer
            // the tail
            element->previousNode = next;
            next->nextNode = element;
        }
    }

    return head;
}

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

    int maze[MAZE_ROWS][MAZE_COLUMNS] = {{0, 0, 0, 0, 0}};
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
