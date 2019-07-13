#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <jcrl/constants.h>
#include <jcrl/map.h>
#include <jcrl/macros.h>

int main(int argc, char** argv) {
    if(argc != 2) { /* usage */
        fprintf(stderr, "usage: jcrl_benchmark_map_set n\n");
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

    Map map;

    /* initialise map */
    jcrl_result = map_init(&map);

    if(jcrl_result != JCRL_ERR_OK) {
        fprintf(stderr, "Failed to initialise map. JCRL returned %u\n",
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

    /* insert each element into map */
    for(unsigned int i=0;i<n;i++) {
        jcrl_result = map_set(G_INT(data[i]), G_INT(data[i]), &map);

        if(jcrl_result != JCRL_ERR_OK) {
            fprintf(stderr, "Failed to insert into map. JCRL returned %u\n",
                    jcrl_result);
            free(data);
            map_free(NULL, &map);
            return EXIT_FAILURE;
        }
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    /**************************************************************************/

    /* report results */
    long int s_duration = end_time.tv_sec - start_time.tv_sec;
    long int ns_duration = end_time.tv_nsec - start_time.tv_nsec;
    printf("%ld %ld\n", s_duration, ns_duration);

    /* free map */
    jcrl_result = map_free(NULL, &map);
    
    if(jcrl_result != JCRL_ERR_OK) {
        fprintf(stderr, "Failed to free map. JCRL returned %u\n", jcrl_result);
        return EXIT_FAILURE; 
    }

    free(data);

    return EXIT_SUCCESS;
}

