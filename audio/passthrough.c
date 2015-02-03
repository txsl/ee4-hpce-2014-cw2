#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>

int main(int argc, char *argv[])
{	
	int n;
	if(argc < 2)
	{
		// Ie if the extra argument not passed in, default to 512
		n = 512;
	}
	else
	{
		n = atoi(argv[1]);
		
		// Check for any cheeky inputs
		if(n<1)
		{
			fprintf(stderr, "Cannot have a 0 or -ve number of samples. Exiting.\n");
			return -1;
		}
	}
	
	fprintf(stderr, "Processing %i stereo samples at a time\n", n);
	
	// Taking in to account the stereo channels
	n = 2*n;

	while(1){
		// Buffer containing n stereo samples (left and right, both 16 bit)
		int16_t samples[n];
		unsigned cbBuffer=sizeof(samples);	// size of bytes of n stereo samples (4 bytes per stereo sample set)
		
		// Read n samples from input
		// Currently there is a lot of overhead here, as we have all the overhead of a system function call,
		// but only get n*four bytes in return.
		int got=read(STDIN_FILENO, samples, cbBuffer);
		if(got<0){
			fprintf(stderr, "%s : Read from stdin failed, error=%s.", argv[0], strerror(errno));
			exit(1);
		}else if(got==0){
			break;	 // end of file
		}else if(got!=cbBuffer){
			fprintf(stderr, "%s : Did not receive expected number of bytes.\n", argv[0]);
			exit(1);
		}
		
		// Copy n samples to output
		// And again, a lot of overhead just to get n*four bytes.
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
