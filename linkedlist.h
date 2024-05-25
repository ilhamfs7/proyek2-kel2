#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node {
    char data;
    struct Node* next;
    struct Node* prev;
} Node;

Node* createNode(char data);
Node* insertEnd(Node* head, char data);
void swapNodeData(Node* node1, Node* node2);
void deleteNode(Node* prev, Node* curr);
Node* insertAfter(Node* node, char data);
void circularKanan(Node* head);
void circularKiri(Node* head);
void printList(Node* head);

#endif
