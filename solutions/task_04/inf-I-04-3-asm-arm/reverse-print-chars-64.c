#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


int* init(int capacity) {
    int* vector = realloc(0, capacity * sizeof(int));
    if (vector == NULL) {
        exit(1);
    }
    return vector;
}

void expand(int** vector, int* capacity) {
    int new_capacity = 2 * (*capacity);
    int* tmp = realloc(*vector, new_capacity * sizeof(int));
    if (tmp == NULL) {
        free(vector);
        exit(1);
    }
    *vector = tmp;
    *capacity = new_capacity;
}

void push_back(int** vector, int* size, int* capacity, const int value) {
    if ((*capacity) == (*size)) {
        expand(vector, capacity);
    }
    (*vector)[(*size)] = value;
    ++(*size);
}

void print_reverse(const int* vector, int size) {
    --size;
    while(size >= 0) {
        putchar(vector[size]);
        --size;
    }
}


int main() {
    int capacity = 8;
    int size = 0;
    int* vector = init(capacity);
    int c;
    while((c = getchar()) != EOF) {
        push_back(&vector, &size, &capacity, c);
    }
    print_reverse(vector, size);
    free(vector);
}