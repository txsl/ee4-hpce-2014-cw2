#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

int main(int argc, char *argv[]){

    int idx = 1;
    int i = 0;
    // quirky C (as ever) means need to leave .0 there to be treated as a float
    double time_gap = 1/44100.0;
    double time_idx = 0;

    int n = 8;

    while(1){
        // Buffer containing n stereo samples (left and right, both 16 bit).
        int16_t samples[n];
        unsigned cbBuffer=sizeof(samples);  // size in bytes of n stereo samples (n*4 bytes)
        
        // Read n samples from input
        // Currently there is a lot of overhead here, as we have all the overhead of a system function call,
        // but only get n*four bytes in return.
        int got=read(STDIN_FILENO, samples, cbBuffer);

        if(got<0){
            fprintf(stderr, "%s : Read from stdin failed, error=%s.", argv[0], strerror(errno));
            exit(1);
        }else if(got==0){
            break;   // end of file
        }else if(got!=cbBuffer){
            fprintf(stderr, "%s : Did not receive expected number of bytes.\n", argv[0]);
            exit(1);
        }

        // print results to stdout
        for(i=0; i<n; i = i + 2)
        {
            // format as per DT's spec
            fprintf(stdout, "%i, %.6lf, %d, %d\n", idx, time_idx, samples[i], samples[i+1]);
            
            // increment counter and timing counter
            idx++;
            time_idx = time_idx + time_gap;
        }        

    }

    return 0;
}