#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

int main(int argc, char *argv[])
{

    int i = 0;
    int lines = 0;
    int ch;
    FILE* inp;

    if(argc < 2)
    {
        fprintf(stderr, "%s: No Coefficient file supplied. Exiting.\n", argv[0]);

        return -1;
    }
    
    inp = fopen(argv[1], "r");

    /*---  Work out number of coefficients  ---*/

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
    
    int16_t buffer[2*lines];

    for(i=0; i<2*lines; i++){
        buffer[i] = 0;
    }

    // for(i=0; i<lines; i++){
    //     fprintf(stderr, "%lf\n", coeffs[i]);
    // }
    // return 0;

    while(1){
        // Buffer containing n stereo samples (left and right, both 16 bit)
        int16_t samples[2];
        int16_t output[2];
        double calc[2];
        unsigned cbBuffer=sizeof(samples);  // size of bytes of n stereo samples (4 bytes per stereo sample set)

        calc[0] = 0;
        calc[1] = 0;
        

        int got=read(STDIN_FILENO, samples, cbBuffer);
        if(got<0){
            fprintf(stderr, "%s : Read from stdin failed, error=%s.", argv[0], strerror(errno));
            exit(1);
        }else if(got==0){
            break;   // end of file
        }else if(got!=cbBuffer){
            fprintf(stderr, "%s : Did not receive expected number of bytes.\nInstead got %i, but expected %i...\n", argv[0], got, cbBuffer);
            exit(1);
        }

        buffer[0] = (double) samples[0];
        buffer[1] = (double) samples[1];
        
        for(i=0; i<2*lines; i=i+2){
            calc[0] = coeffs[i/2]*buffer[i] + calc[0];
            calc[1] = coeffs[i/2]*buffer[i+1] + calc[1];
            // fprintf(stderr, "coeff=%lf, calc1=%lf, calc2=%lf\n", coeffs[i/2], calc[0], calc[1]);
            // calc
        }
        // return 0;

        output[0] = (int16_t) calc[0];
        output[1] = (int16_t) calc[1];

        fprintf(stderr, "buf3=%i, buf4=%i \n", buffer[3], buffer[4]);

        for(i=0; i<((2*lines)); i=i+2){
            buffer[i+2] = buffer[i];
            buffer[i+3] = buffer[i+1];
        }
        fprintf(stderr, "buf3=%i, buf4=%i \n", buffer[3], buffer[4]);
        fprintf(stderr, "----\n"q   );

        // Copy n samples to output
        // And again, a lot of overhead just to get n*four bytes.
        int done=write(STDOUT_FILENO, output, cbBuffer);
        if(done<0){
            fprintf(stderr, "%s : Write to stdout failed, error=%s.", argv[0], strerror(errno));
            exit(1);
        }else if(done!=cbBuffer){
            fprintf(stderr, "%s : Could not read requested number of bytes from stream.\n", argv[0]);
        }
    }

    return 0;
}
