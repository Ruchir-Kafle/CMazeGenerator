#pragma once

#include <stdio.h>
#include <stdlib.h>

// forward declaration
typedef struct Node Node;

// linked list node
struct Node {
    Node *previousNode;
    Node *nextNode;

    void *value;
};

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
