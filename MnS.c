#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int size;
    int mark;
    struct node* link1;
    struct node* link2;
    struct node* link3;
} node;

typedef struct root {
    struct node* next;
} root;

node* heap[8];
root* root1 = NULL;
root* root2 = NULL;

node* getnewnode(int size) {
    node* temp = (node*)malloc(sizeof(node));
    temp->size = size;
    temp->link1 = NULL;
    temp->link2 = NULL;
    temp->link3 = NULL;
    temp->mark = 0;
    return temp;
}

void initialize() {
    node* temp = getnewnode(5);
    heap[0] = temp;
    temp = getnewnode(1);
    heap[1] = temp;
    temp = getnewnode(2);
    heap[2] = temp;
    temp = getnewnode(9);
    heap[3] = temp;
    temp = getnewnode(10);
    heap[4] = temp;
    temp = getnewnode(7);
    heap[5] = temp;
    temp = getnewnode(8);
    heap[6] = temp;
    temp = getnewnode(3);
    heap[7] = temp;
    
    heap[0]->link1=heap[1];
    heap[1]->link1=heap[2];
    heap[1]->link2=heap[3];
    heap[1]->link3=heap[4];
    heap[5]->link1=heap[1];
    heap[5]->link2=heap[6];
    heap[6]->link1=heap[3];
    heap[7]->link1=heap[4];
    heap[7]->link2=heap[6];

    root1 = (root*)malloc(sizeof(root));
    root2 = (root*)malloc(sizeof(root));
    root1->next = heap[0];
    
    
    root2->next = heap[1];
}

void print_node(node* node) {
    if (node == NULL) {
        
        return;
    }
    printf("[Node size = %d]->", node->size);
    if (node->link1 != NULL || node->link2 != NULL || node->link3 != NULL) {
        
        print_node(node->link1);
        print_node(node->link2);
        print_node(node->link3);
        
    }
}

void print_heap(node* arr) {
    for (int i = 0; i < 8; i++) {
        if (heap[i] != NULL) {
            print_node(heap[i]);
            printf("\n");
        }
    }
}

void print_useful_heap(root* value) {
    printf("root->");
    print_node(value->next);
    printf("\n");
}

void mark_node(node* ptr) {
    node* head = ptr;
    node* tail = NULL;
    node* middle = NULL;

    while (head != NULL) {
        if (!(head->mark == 1)) {
            head->mark = 1;
        }
        if (head->link1 != NULL && !(head->link1->mark == 1)) {
            tail = middle;
            middle = head;
            head = head->link1;
        }
        else if (head->link2 != NULL && !(head->link2->mark == 1)) {
            tail = middle;
            middle = head;
            head = head->link2;
        }
        else if (head->link3 != NULL && !(head->link3->mark == 1)) {
            tail = middle;
            middle = head;
            head = head->link3;
        }
        else {
            head = middle;
            middle = tail;
            tail = NULL;
        }
    }
}

void mark(root* value) {
    mark_node(value->next);
}

void sweep(node* arr) {
    for (int i = 0; i < 8; i++) {
        if (heap[i] != NULL) {
            if (!heap[i]->mark) {
                heap[i]->link1 = NULL;
                heap[i]->link2 = NULL;
                heap[i]->link3 = NULL;
                free(heap[i]);
                heap[i] = NULL;
            }
        }
    }
}

void garbage_collector(root* r1, root* r2, node* hp) {
    printf("Mark phase started.......\n");
    mark(r1);
    mark(r2);
    printf("Marking done\n");
    printf("Sweep phase started.......\n");
    sweep(hp);
}

int main() {
    printf("This is the Simulation for mark sweep garbage collector\n");
    //printf("Key to navigate the printed table: - indicates connection and [] indicate all the elements connected to the element\n");
    printf("the full heap is: \n");
    initialize();
    print_heap(heap);
    printf("-----------------\n");
    printf("Heap connected to the roots is: \n");
    print_useful_heap(root1);
    print_useful_heap(root2);
    printf("-----------------\n");
    garbage_collector(root1, root2, heap);
    printf("the garbage collector was called, let us see the full heap now\n");
    printf("---------------\ncurrent heap:\n");
    print_heap(heap);
    printf("let us see if it matches heap connected to roots printed below\n");
    print_useful_heap(root1);
    print_useful_heap(root2);

    return 0;
}
