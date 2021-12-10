/* Implementation of Quicksort
** qs takes one argument, a file containing a list of integers sperated by '\n' */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIRST 1 
#define LAST 2
#define MEDIAN 3

int quick_sort(int* arr, int len);
int qs(int* arr, int l, int r);
int partition(int* arr, int pivot, int l, int r);
int choose_pivot(int* arr, int l, int r, int flag);
void swap(int* x, int* y);
size_t line_count(FILE* f); 
int* f_to_int_arr(FILE* f, size_t len);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("qs takes one command line argument:\n a file containing a list of integers seperated by '\\n'\n");
        return EXIT_FAILURE;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("unable to open file: ");
        return EXIT_FAILURE;
    }

    size_t len = line_count(f);
    if (len == -1) {
        printf("reading file to array failed");
    }

    int* arr = f_to_int_arr(f, len);

    quick_sort(arr, len);

    for (size_t i = 0; i < len; ++i) {
        if (i % 10) {
            printf("%d  ", arr[i]);
        } else {
            printf("%d\n", arr[i]);
        }
    }

    free(arr);
    fclose(f);
    return EXIT_SUCCESS;
}


int quick_sort(int* arr, int len) {
    comp_count += len - 1;
    qs(arr, 0, len - 1);
    return 1;
}

int qs(int* arr, int l, int r) {
    if (r - l < 1) return 1;    // base case, list of one or fewer elements

    int pivot = choose_pivot(arr, l, r, LAST);     // choose pivot and place in zero index

    int m = partition(arr, pivot, l, r);  // partiton arr around pivot

    if (m - l - 1 > 0) {
        comp_count += m - l - 1;
    }
    if (r - m - 1 > 0) {
        comp_count += r - m - 1;
    }   
    qs(arr, l, m - 1);          // sort left partition
    qs(arr, m + 1, r);          // sort right partition 

    return 1;
}

int choose_pivot(int* arr, int l, int r, int flag) {
    if (flag == LAST) {
        swap(&arr[l], &arr[r]);
    } else if (flag == MEDIAN) {
        int even = (r - l + 1) % 2;
        int m = l + ((r - l + even) / 2);
        if ((arr[m] < arr[l] && arr[m] > arr[r]) || (arr[m] > arr[l] && arr[m] <arr[r])) {
            swap(&arr[m], &arr[l]);
        } 
        if ((arr[r] < arr[m] && arr[r] > arr[l]) || (arr[r] > arr[m] && arr[r] < arr[l])) {
            swap(&arr[r], &arr[l]);
        }
    }

    return arr[l];
}

int partition(int* arr, int pivot, int l, int r) {
    int i = l + 1; int j = l + 1;

    while (j <= r) {
        if (arr[j] < pivot && j > i) {
            swap(&arr[i++], &arr[j]);
        } else if (arr[j] < pivot && j == i) {
            ++i; 
        }
        ++j;
    }

    swap(&arr[l], &arr[i-1]);
    return i - 1;
}

void swap(int* x, int* y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
    return;
}

size_t line_count(FILE* f) {
    fseek(f, 0, SEEK_SET);
    size_t buff_size = 256;
    char* buff = malloc(buff_size);
    if (!buff) {
        perror("allocation failed: ");
        return -1;
    }

    size_t line_count = 0;

    while (getline(&buff, &buff_size, f) > 0) {
        ++line_count;
    }

    free(buff);

    return line_count;
}

int* f_to_int_arr(FILE* f, size_t len) {
    fseek(f, 0, SEEK_SET);
    size_t buff_size = 256;
    int* ret = malloc(sizeof(int) * len);
    char* buff = malloc(buff_size);
    if (!buff || !ret) {
        perror("allocation failed: ");
    }

    size_t i = 0;
    while (getline(&buff, &buff_size, f) > 0) {
        ret[i++] = atoi(buff);
    }

    free(buff);
    return ret;
}




