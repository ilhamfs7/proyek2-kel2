#ifndef encodeLSB_h
#define encodeLSB_h

typedef struct Node {
    char data;
    struct Node* next;
} Node;

Node* createNode(char data);
Node* insertEnd(Node* head, char data);
void freeList(Node* head);
void encode(const char* input_filename, const char* output_filename, const char* message);

#endif
