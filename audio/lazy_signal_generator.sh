#!/bin/sh

make signal_generator
./signal_generator $1 | ./audio_sink.sh