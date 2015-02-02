#!/bin/bash
../local/bin/sox --type raw  --bits 16 --rate 44.1k --channels 2 --endian little --encoding signed-integer - -d