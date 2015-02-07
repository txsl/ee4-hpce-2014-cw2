#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

#define N 512

int main(int argc, char *argv[]){

    int got, done;
    FILE* inp_1;
    FILE* inp_2;
    int i;

    if(argc<3){
        fprintf(stderr, "%s : Not enough arguments given. 2 Filepaths needed. Exiting", argv[0]);
        return -1;
    }

    inp_1 = fopen(argv[1], "rb");
    inp_2 = fopen(argv[2], "rb");

    if(!inp_1){
        fprintf(stderr, "%s : Could not read %s. Exiting.\n", argv[0], argv[1]);
        return -1;
    }

    else if(!inp_2){
        fprintf(stderr, "%s : Could not read %s. Exiting.\n", argv[0], argv[2]);
        return -1;
    }

    while(1){
        int16_t samples_1[N];
        int16_t samples_2[N];
        unsigned cbBuffer=sizeof(samples_1);

        // Thanks http://stackoverflow.com/questions/24716250/c-store-read-binary-file-into-buffer
        // for the inspiration to use fread

        // read samples from both file descriptors
        got=fread(samples_1, sizeof(int16_t), N, inp_1);
        if(got<0){
            fprintf(stderr, "%s : Read from %s failed, error=%s.", argv[0], argv[1], strerror(errno));
            exit(1);
        }else if(got==0){
            break;   // end of file
        }else if(got!=N){
            fprintf(stderr, "%s : Did not receive expected number of bytes.\n", argv[0]);
            exit(1);
        }

        got=fread(samples_2, sizeof(int16_t), N, inp_2);
        if(got<0){
            fprintf(stderr, "%s : Read from %s failed, error=%s.", argv[0], argv[1], strerror(errno));
            exit(1);
        }else if(got==0){
            break;   // end of file
        }else if(got!=N){
            fprintf(stderr, "%s : Did not receive expected number of bytes.\n", argv[0]);
            exit(1);
        }


        // Take the mean of both signals. Save back to one of the arrays to save memory
        for(i=0 ;i<N; i++){
            samples_1[i] = (samples_1[i] + samples_2[i])/2;
        }

        // Write samples to stdout
        done=write(STDOUT_FILENO, samples_1, cbBuffer);
        if(done<0){
            fprintf(stderr, "%s : Write to stdout failed, error=%s.", argv[0], strerror(errno));
            exit(1);
        }else if(done!=cbBuffer){
            fprintf(stderr, "%s : Could not read requested number of bytes from stream.\n", argv[0]);
        }


    }        

}