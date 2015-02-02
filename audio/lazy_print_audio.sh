#!/bin/sh

make print_audio
cat 200hz.raw | head -c 64 | ./print_audio