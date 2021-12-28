#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __job {
    int  weight;
    int  length;
} job_t;

typedef struct __job_arr {
    int    len;
    job_t* arr;
} job_arr_t;

job_arr_t* read_file(FILE* f);
void destroy_job_arr(job_arr_t*);
void print_jobs(job_arr_t* jobs, int len);
long int weighted_finish_time(job_arr_t* jobs);
int compare_difference(const void* a, const void* b);
int compare_ratio(const void* a, const void* b);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Scheduler takes one argument, a file representing a job list");
    }

    FILE* f = fopen(argv[1], "r");

    if (f == 0) {
        perror("argv[1] failed to open ");
        return EXIT_FAILURE;
    }

    job_arr_t* jobs = read_file(f);
    fclose(f);

    qsort(jobs->arr, jobs->len, sizeof(job_t), compare_difference);
    printf("finish time for comparison by difference = %ld\n", weighted_finish_time(jobs));

    qsort(jobs->arr, jobs->len, sizeof(job_t), compare_ratio);
    printf("finish time for comparison by ratio = %ld\n", weighted_finish_time(jobs));

    destroy_job_arr(jobs);
    return EXIT_SUCCESS;
}

long int weighted_finish_time(job_arr_t* jobs) {
    long int ret = 0;
    long int finish_time = 0;

    for (size_t i = 0; i < jobs->len; ++i) {
        ret += jobs->arr[i].weight * (jobs->arr[i].length + finish_time);
        finish_time += jobs->arr[i].length;
    }

    return ret;
}

job_arr_t* create_job_arr(int len) {
    job_arr_t* ret = malloc(sizeof(job_arr_t));

    if (ret == 0) {
        perror("allocation for job array failed ");
        exit(0);
    }

    ret->len = len;
    ret->arr = malloc(sizeof(job_t) * len);
    
    if (ret->arr == 0) {
        perror("allocation for job array failed ");
        exit(0);
    }

    return ret;
}

void destroy_job_arr(job_arr_t* jobs) {
    free(jobs->arr);
    free(jobs);
}

job_arr_t* read_file(FILE* f) {
    fseek(f, 0, SEEK_SET);
    
    size_t buff_size = 256;
    char* buff = calloc(buff_size, 1);

    if (getline(&buff, &buff_size, f) == -1) {
        printf("empty file\n");
        exit(0);
    }

    job_arr_t* ret = create_job_arr(atoi(buff));

    size_t i = 0;
    while (getline(&buff, &buff_size, f) != -1) {
        ret->arr[i].weight = atoi(strtok(buff, " "));
        ret->arr[i++].length = atoi(strtok(0, " "));
    }

    free(buff);
    return ret;
}

int compare_difference(const void* a, const void* b) {
    job_t* aa = (job_t*) a;
    job_t* bb = (job_t*) b;

    int weighted_a = aa->weight - aa->length;
    int weighted_b = bb->weight - bb->length;

    if (weighted_a == weighted_b) {
        return bb->weight - aa->weight;
    }

    return weighted_b - weighted_a;
}

int compare_ratio(const void* a, const void* b) {
    job_t* aa = (job_t*) a;
    job_t* bb = (job_t*) b;

    float weighted_a = (float)aa->weight / (float)aa->length;
    float weighted_b = (float)bb->weight / (float)bb->length;

    if (weighted_a == weighted_b) {
        if (aa->weight > bb->weight) {
            return -1; 
        } else {
            return 1;
        }
    }

    if (weighted_a > weighted_b) {
        return -1;
    } else {
        return 1;
    }
}

void print_jobs(job_arr_t* jobs, int len) {
    for (int i = 0; i < len; ++i) {
        printf("arr[%d] weight = %d, length = %d, ratio = %f\n", i, jobs->arr[i].weight, jobs->arr[i].length, (float)jobs->arr[i].weight / (float)jobs->arr[i].length);
    }
}
