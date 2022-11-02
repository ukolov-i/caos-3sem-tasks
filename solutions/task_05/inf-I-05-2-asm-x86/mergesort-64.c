#include <stdlib.h>
#include <stdio.h>


void insert_dispatch(int left, int right, const int* in, int* out) {
    for (int i = 0; i < right - left; ++i) {
        out[i] = in[left + i];
    }
}

void merge_helper(int left, int right, int* index, const int* in, int* out) {
    while (left < right) {
        out[*index] = in[left];
        ++(*index);
        ++left;
    }
}

void merge(int left, int middle, int right, int* out) {
    int size_left = middle - left;
    int size_right = right - middle;
 
    int copy_left[size_left];
    int copy_right[size_right];
    insert_dispatch(left, middle, out, copy_left);
    insert_dispatch(middle, right, out, copy_right);

 
    int i = 0;
    int j = 0;
    while (i < size_left && j < size_right) {
        if (copy_left[i] <= copy_right[j]) {
            out[left] = copy_left[i];
            ++i;
        } else {
            out[left] = copy_right[j];
            ++j;
        }
        ++left;
    }
 

    merge_helper(i, size_left, &left, copy_left, out);
    merge_helper(j, size_right, &left, copy_right, out);
}
 
void mergesort_impl(int left, int right, int* out) {
    if (left + 1 < right) {
        int middle = (right + left) / 2;
 
        mergesort_impl(left, middle, out);
        mergesort_impl(middle, right, out);

        merge(left, middle, right, out);
    }
}

void mergesort(int from, int to, const int* in, int* out) {
    for (int i = 0; i < to - from; ++i) {
        out[i] = in[from + i];
    }
    mergesort_impl(0, to - from, out);
}