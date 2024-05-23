#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memori penuh!");
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
        Node* tail = head->prev;
        tail->next = newNode;
        newNode->prev = tail;
        newNode->next = head;
        head->prev = newNode;
        tail = NULL;
        newNode = NULL;
        return head;
    }
}

void swapNodeData(Node* node1, Node* node2) {
    char temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
}

void deleteNode(Node* prev, Node* curr) {
	prev->next = curr->next;
    curr->next->prev = prev;
    free(curr);
}

Node* insertAfter(Node* node, char data) {
    Node* newNode = createNode(data);
    Node* temp = node->next;
    temp->prev = newNode;
    node->next = newNode;
    newNode->next = temp;
    newNode->prev = node;
    temp = NULL;
    return newNode;
}

void circularKanan(Node* head) {
	Node* current = head;
	char temp = '\0', temp1;
	do {
		if (temp == '\0') {
			temp = current->prev->data;
			temp1 = current->next->data;
			current->next->data = current->data;
			current->data = temp;
		} else {
			temp = current->next->data;
			current->next->data = temp1;
			temp1 = temp;
		}
		current = current->next;
	}while (current != head);
	current->data = temp;
}

void circularKiri(Node* head) {
    if (head == NULL) return;

    Node* current = head;
    char temp = current->data;
    while (current->next != head) {
        current->data = current->next->data;
        current = current->next;
    }
    current->data = temp;
}

void printList(Node* head) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }
    Node* current = head;
    do {
        printf("%c ", current->data);
        current = current->next;
    } while (current != head);
    printf("\n");
}
