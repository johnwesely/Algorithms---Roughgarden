#include <stdio.h>
#include <stdlib.h> 

int count_inversions(int* arr, size_t len);
int merge_count(int* arr1, size_t len1, int* arr2, size_t len2);

int main(int argc, char* arg_v[]) {
    int test[] = { 1, 5, 4, 3, 2, 1 };

    printf("number of inversions = %d\n", count_inversions(test, 6));

    return EXIT_SUCCESS;
}

int count_inversions(int* arr, size_t len) {
    if (len < 2) return 0;
    size_t ret = 0;
    size_t m = len / 2;

    ret += count_inversions(arr, m);
    ret += count_inversions(arr + m, len - m);

    ret += merge_count(arr, m, arr + m, len - m);

    return ret;
}

int merge_count(int* arr1, size_t len1, int* arr2, size_t len2) {
    int ret = 0;
    int tmp[len1+len2];
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    size_t rem_1 = len1;
    size_t rem_2 = len2;

    // sort arrays into tmp,counting number of split inversions
    while (rem_1 && rem_2) {
        if (arr1[j] <= arr2[k]) {
            tmp[i++] = arr1[j++];
            --rem_1;
        } else {
            tmp[i++] = arr2[k++];
            ret += rem_1;
            -- rem_2;
        }
    }

    // copy remainders into tmp
    while (rem_1--) {
        tmp[i++] = arr1[j++];
    }
    
    while (rem_2--) {
        tmp[i++] = arr2[k++];
    }
    
    // copy sorted array into original 
    for (i = 0; i < len1 + len2; ++i) {
        arr1[i] = tmp[i];
    }

    return ret;
}

int f_to_int_arr(FILE* f, int* arr) {
    return 1;
}
