#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

#define N 128

void debug_print(char *stmt){
    fprintf(stderr, "%s\n", stmt);
}

int calc_pointer(int i, int j, int ptr, int right, int buf_len){
    int ret_ptr;
    ret_ptr = (j + ptr - i);
    
    if(ret_ptr < 0){
        ret_ptr = buf_len + ret_ptr; // - 1;
    }

    if(right){
        ret_ptr++;
    }
    return ret_ptr;
}

int main(int argc, char *argv[]){

    FILE* inp;
    int lines = 0;
    int i = 0;
    int ptr = 0;
    int ch, got, j, done;
    unsigned cbBuffer;
    // double calc[2*N];
    int buf_len;
    // int N;

    /*---  Check we acutally have enough args for this to work  ---*/

    if(argc<2){
        fprintf(stderr, "%s : Not enough arguments given. Filepath for FIR coefficients needed. Exiting\n", argv[0]);
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

    /*---  Read coefficients in  ---*/

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
    // N = ;

    /*---  Set up buffer based on number of coefficients and N  ---*/

    // The extra addition is to ensure the buffer length is a multiple of N 

    buf_len = (2*lines) + (lines % N) + 1;
    debug_print("afer buf len");
    if(buf_len < 2*N)
    {
        buf_len = 2*N;
    }

    int16_t buffer[buf_len];
    memset(buffer, 0, 2 * N * sizeof(buffer[0])); // SO: http://stackoverflow.com/questions/4066522/setting-an-array-to-one-value

    // Tells fprintf how many bytes to read
    cbBuffer = 2*N; // *sizeof(int16_t);

    fprintf(stderr, "Buffer length: %i, cbBuffer: %i\n", buf_len, cbBuffer);

    while(1){
        
        // temporary crap
        // int16_t buffer[N*2];
        // unsigned cbBuffer=sizeof(buffer);  // size of bytes of n stereo samples (4 bytes per stereo sample set)
        // fprintf(stderr, "hello\n");


        got=read(STDIN_FILENO, &buffer[ptr], cbBuffer);
        if(got<0){
            fprintf(stderr, "%s : Read from stdin failed, error=%s.", argv[0], strerror(errno));
            exit(1);
        }else if(got==0){
            break;   // end of file
        }else if(got!=cbBuffer){
            fprintf(stderr, "%s : Did not receive expected number of bytes.\nInstead got %i, but expected %i...\n", argv[0], got, cbBuffer);
            exit(1);
        }

        // fprintf(stderr, "hello2\n");

        /*---  Set some variables to be used during calculation and output  ---*/
        double calc[2*N];
        memset(calc, 0, 2 * N * sizeof(calc[0]));
        int16_t output[2*N];


        int point0, point1;
        int man_idx = 0;
        for(j=0; j<2*N; j=j+2){
            for(i=0; i<2*lines; i=i+2){
                // fprintf(stderr, "\nhello2\n");
                // point0=man_idx+ptr;
                // point1=man_idx+1+ptr;
                // point0=buf_len % (j/2+ ptr - i/2);
                // point1=buf_len % (j/2+ ptr + 1 - i/2);
                // fprintf
                point0 = calc_pointer(i, j, ptr, 0, buf_len);
                point1 = calc_pointer(i, j, ptr, 1, buf_len);
                // fprintf(stderr, "%s : referencing %i and %i. i=%i, j=%i, ptr=%i\n", argv[0], point0, point1, i/2, j, ptr);
                calc[j] = buffer[point0]*coeffs[i/2] + calc[j];
                calc[j+1] = buffer[point1]*coeffs[i/2] + calc[j+1];
                // fprintf(stderr, "calc value l: %f, r: %f \n", calc[j], calc[j+1]);
                // debug_print("\nhello\n");
                // fprintf(stderr, "here \n");
                man_idx = man_idx + 2;
            }
            
            output[j] = (int16_t) calc[j];
            output[j+1] = (int16_t) calc[j+1];
            // fprintf(stderr, "output value l: %f, r: %f \n", output[j], output[j+1]);
            // fprintf(stderr, "--\n");
        }
        // fprintf(stderr, "-----\n");
        // fprintf(stderr, "%lf in, %lf out", buffer[0], calc[0]);
        // for(i=0; i<N; )
        // calc[0] = buffer[0];
        // calc[1] = buffer[1];
        ptr = ptr + 2*N;
        // debug_print("before pointer modulo");
        ptr = ptr % buf_len;
        // debug_print("after pointer modulo");
        // ptr = 0;

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
        // return 0;
    }

}