#include <stdio.h>
#include "linkedlist.h"

Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory full!");
        exit(1);
    }
    newNode->data = data;
    newNode->next = newNode;
    newNode->prev = newNode;
    return newNode;
}

Node* insertEnd(Node* head, char data) {
    if (head == NULL) {
        return createNode(data);
    } else {
        Node* newNode = createNode(data);
        Node* tail = head->prev; // Get the tail node
        tail->next = newNode;
        newNode->prev = tail;
        newNode->next = head;
        head->prev = newNode;
        return head;
    }
}

void swapNodeData(Node* node1, Node* node2) {
    char temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
}

void deleteNode(Node* head, Node* curr) {
    if (head == curr && head->next == head) {
        // Only one node in the list
        free(curr);
        return;
    }
    
    Node* prev = curr->prev;
    Node* next = curr->next;
    prev->next = next;
    next->prev = prev;
    
    if (curr == head) {
        head = next;
    }
    
    free(curr);
}

Node* insertAfter(Node* node, char data) {
    Node* newNode = createNode(data);
    Node* nextNode = node->next;
    
    node->next = newNode;
    newNode->prev = node;
    newNode->next = nextNode;
    nextNode->prev = newNode;
    
    return newNode;
}
