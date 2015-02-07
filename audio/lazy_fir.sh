#!/bin/sh

make fir_filter

# thanks http://askubuntu.com/questions/29370/how-to-check-if-a-command-succeeded
if [ $? -eq 0 ]; then
    ./mp3_file_src.sh bn9th_m4.mp3 | ./fir_filter coeffs/f800.csv  | ./audio_sink.sh  #> /dev/null #| ./audio_sink.sh #  #
else
    echo FAIL
fi

