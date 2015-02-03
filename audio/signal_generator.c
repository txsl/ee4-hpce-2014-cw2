#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

int main(int argc, char *argv[]){

    // Cast all as doubles to ensure max accuracy (little time difference)
    double f;
    double t = 0;
    double time_gap = 1/44100.0;
    double calc;
    int N = 512;
    int i = 0;

    if(argc < 2)
    {
        // Ie if the extra argument not passed in, default to 512
        f = 200;
    }
    else
    {
        // According to http://www.cplusplus.com/reference/cstdlib/atof/ ,
        // it returns a double (hence f cast as double)
        f = atof(argv[1]);
    }

    while(1){
        
        int16_t samples[N];
        unsigned cbBuffer=sizeof(samples);
        
        for(i=0; i<N; i=i+2){
            
            calc = 30000*sin(t*2*M_PI*f);
            samples[i] = calc;
            samples[i+1] = calc;

            t = t + time_gap;
        }
        
        int done=write(STDOUT_FILENO, samples, cbBuffer);
        if(done<0){
            fprintf(stderr, "%s : Write to stdout failed, error=%s.", argv[0], strerror(errno));
            exit(1);
        }else if(done!=cbBuffer){
            fprintf(stderr, "%s : Could not read requested number of bytes from stream.\n", argv[0]);
        }
    }

    return 0;
}