#!/bin/bash

 ./merge <( ./merge <( ./merge <(./signal_generator 500) <(./signal_generator 600) )  <( ./merge <(./signal_generator 800) <(./signal_generator 1000) ) ) <( ./merge <( ./merge <(./signal_generator 1200) <(./signal_generator 1400) ) <(cat -) )