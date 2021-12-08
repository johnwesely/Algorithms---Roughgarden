/* counts number over inversions in a file containing integers seperated by '/n'
** takes one command line argument a file containing the list of numbers */
#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>

unsigned long long count_inversions(int* arr, size_t len);
unsigned long long count_inversions_wrapper(int* arr, size_t len);
unsigned long long merge_count(int* arr1, size_t len1, int* arr2, size_t len2);
size_t line_count(FILE* f); 
int* f_to_int_arr(FILE* f, size_t len);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("inversion takes one command line argument:\n a file containing a list of numbers seperated by '\\n'\n");
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

    printf("number of inversions = %lld\n", count_inversions(arr, len));

    return EXIT_SUCCESS;
}

// copy arr and call count_inversions wrapper with copy to preserve original array
unsigned long long count_inversions(int* arr, size_t len) {
    int arrcpy[len];

    for (size_t i = 0; i < len; ++i) {
        arrcpy[i] = arr[i];
    }

    return count_inversions_wrapper(arrcpy, len);
}

// standard merge sort with addition of ret variable tracking split inversions
unsigned long long count_inversions_wrapper(int* arr, size_t len) {
    if (len < 2) return 0;
    unsigned long long ret = 0;
    size_t m = len / 2;

    ret += count_inversions_wrapper(arr, m);
    ret += count_inversions_wrapper(arr + m, len - m);

    ret += merge_count(arr, m, arr + m, len - m);

    return ret;
}

unsigned long long merge_count(int* arr1, size_t len1, int* arr2, size_t len2) {
    unsigned long long ret = 0;
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


