#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

#define N 1

int main(int argc, char *argv[]){

    FILE* inp;
    int lines = 0;
    int i = 0;
    int ptr = 0;
    int ch, got, j, done;
    unsigned cbBuffer;
    // double calc[2*N];
    int buf_len;


    if(argc<2){
        fprintf(stderr, "%s : Not enough arguments given. Filepath for FIR coefficients needed. Exiting\n", argv[0]);
        return -1;
    }

    inp = fopen(argv[1], "r");

    // Thanks to stackoverflow: http://stackoverflow.com/a/12733630/1397412
    while(!feof(inp))
    {
      ch = fgetc(inp);
      if(ch == '\n')
      {
        lines++;
      }
    }

    rewind(inp);

    fprintf(stderr, "%s : %i coefficients to be used\n", argv[0], lines);

    double coeffs[lines];

    while (!feof(inp)){
        fscanf(inp, "%lf", &coeffs[i]);
        i++;
    }

    // for(i=0; i<lines; i++){
    //     fprintf(stderr, "in a loop %i. Value is %lf\n", i, coeffs[i]);
    // }

    // buf_len = 2*lines;
    // int16_t buffer[buf_len];
    // cbBuffer = 2*N*sizeof(int16_t);

    while(1){
        
        // temporary crap
        int16_t buffer[N*2];
        unsigned cbBuffer=sizeof(buffer);  // size of bytes of n stereo samples (4 bytes per stereo sample set)
        
        got=read(STDIN_FILENO, buffer, cbBuffer);
        if(got<0){
            fprintf(stderr, "%s : Read from stdin failed, error=%s.", argv[0], strerror(errno));
            exit(1);
        }else if(got==0){
            break;   // end of file
        }else if(got!=cbBuffer){
            fprintf(stderr, "%s : Did not receive expected number of bytes.\nInstead got %i, but expected %i...\n", argv[0], got, cbBuffer);
            exit(1);
        }


        // calc_1 = 0;
        // calc_2 = 0;

        // Another SO: http://stackoverflow.com/questions/4066522/setting-an-array-to-one-value
        // memset(calc, 0, 2 * N * sizeof(calc[0]));
        double calc[2];
        int16_t output[2];
        // calc[0] = 0;
        // calc[1] = 0;
        // calc[2] = 0;
        // calc[3] = 0;
        // calc[4] = 0;
        // // calc[20] = 0;
        // int s = sizeof(calc)/sizeof(double);
        // fprintf(stderr, "%i\n", sizeof(calc[0]));

        for(j=0; j<N; j++){         
            for(i=0; i<lines; i=i+2){
                // fprintf(stderr, "%s : ptr referencing %i and %i\n", argv[0], (2*j)+i+ptr, (2*j)+1+i+ptr);
                // calc[j] = buffer[(2*j)+i+ptr]*coeffs[i/2] + calc[j];
                calc[j] = (double) buffer[0];
                calc[j+1] = (double) buffer[1];
                // calc[j+1] = buffer[(2*j)+1+i+ptr]*coeffs[i] + calc[j+1];
            }
            output[j] = (int16_t) calc[j];
            output[j+1] = (int16_t) calc[j+1];
        }
        // fprintf(stderr, "%lf in, %lf out", buffer[0], calc[0]);
        // for(i=0; i<N; )
        // calc[0] = buffer[0];
        // calc[1] = buffer[1];
        ptr = ptr + 2*N;
        ptr = ptr % buf_len;

        // fprintf(stderr, "%s : ptr = %i \n", argv[0], ptr);

        // Write samples to stdout
        
        // output[0] = (int16_t) calc[0];
        // output[1] = (int16_t) calc[1];

        done=write(STDOUT_FILENO, output, cbBuffer);
        if(done<0){
            fprintf(stderr, "%s : Write to stdout failed, error=%s.", argv[0], strerror(errno));
            exit(1);
        }else if(done!=cbBuffer){
            fprintf(stderr, "%s : Could not read requested number of bytes from stream.\n", argv[0]);
        }

    }

}