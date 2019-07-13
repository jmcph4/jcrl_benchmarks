/**
 * list_append
 *
 * Benchmarks performance of appending random 64-bit unsigned integers to the
 * end of a JCRL List.
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <jcrl/constants.h>
#include <jcrl/list.h>
#include <jcrl/macros.h>

int main(int argc, char** argv) {
    if(argc != 2) { /* usage */
        fprintf(stderr, "usage: jcrl_benchmark_list_append n\n");
        return EXIT_FAILURE;
    }

    unsigned int n = atoi(argv[1]);
    unsigned int jcrl_result = JCRL_ERR_OK;

    struct timespec start_time;
    struct timespec end_time;

    if(n == 0) {
        return EXIT_SUCCESS;
    }

    srand(time(0));

    List list;

    /* initialise list */
    jcrl_result = list_init(&list);

    if(jcrl_result != JCRL_ERR_OK) {
        fprintf(stderr, "Failed to initialise list. JCRL returned %u\n",
                jcrl_result);
        return EXIT_FAILURE; 
    }

    /* allocate space for seed data */
    uint64_t* data = calloc(n, sizeof(uint64_t));

    if(data == NULL) {
       perror("calloc");
       return EXIT_FAILURE;
    }

    /* populate seed data */
    for(unsigned int i=0;i<n;i++) {
       data[i] = rand();
    }

    /**************************************************************************/
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

    /* insert each element into list */
    for(unsigned int i=0;i<n;i++) {
        jcrl_result = list_append(G_INT(data[i]), &list);

        if(jcrl_result != JCRL_ERR_OK) {
            fprintf(stderr, "Failed to append to list. JCRL returned %u\n",
                    jcrl_result);
            free(data);
            list_free(NULL, &list);
            return EXIT_FAILURE;
        }
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    /**************************************************************************/

    /* report results */
    long int s_duration = end_time.tv_sec - start_time.tv_sec;
    long int ns_duration = end_time.tv_nsec - start_time.tv_nsec;
    printf("%ld %ld\n", s_duration, ns_duration);

    /* free list */
    jcrl_result = list_free(NULL, &list);
    
    if(jcrl_result != JCRL_ERR_OK) {
        fprintf(stderr, "Failed to free list. JCRL returned %u\n", jcrl_result);
        return EXIT_FAILURE; 
    }

    free(data);

    return EXIT_SUCCESS;
}

