/**
 * queue_push
 *
 * Benchmarks performance of enqueuing random 64-bit unsigned integers to the
 * end of a JCRL queue.
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <jcrl/constants.h>
#include <jcrl/queue.h>
#include <jcrl/macros.h>

int main(int argc, char** argv) {
    if(argc != 2) { /* usage */
        fprintf(stderr, "usage: jcrl_benchmark_queue_push n\n");
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

    Queue queue;

    /* initialise queue */
    jcrl_result = queue_init(&queue);

    if(jcrl_result != JCRL_ERR_OK) {
        fprintf(stderr, "Failed to initialise queue. JCRL returned %u\n",
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

    /* insert each element into queue */
    for(unsigned int i=0;i<n;i++) {
        jcrl_result = queue_push(G_INT(data[i]), &queue);

        if(jcrl_result != JCRL_ERR_OK) {
            fprintf(stderr, "Failed to push to queue. JCRL returned %u\n",
                    jcrl_result);
            free(data);
            queue_free(NULL, &queue);
            return EXIT_FAILURE;
        }
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    /**************************************************************************/

    /* report results */
    long int s_duration = end_time.tv_sec - start_time.tv_sec;
    long int ns_duration = end_time.tv_nsec - start_time.tv_nsec;
    printf("%ld %ld\n", s_duration, ns_duration);

    /* free queue */
    jcrl_result = queue_free(NULL, &queue);
    
    if(jcrl_result != JCRL_ERR_OK) {
        fprintf(stderr, "Failed to free queue. JCRL returned %u\n", jcrl_result);
        return EXIT_FAILURE; 
    }

    free(data);

    return EXIT_SUCCESS;
}

