#!/bin/bash

#generate the executable
g++ -O3 -o ../../src/compute_true_rob ../../src/compute_true_rob.cpp -I../../src/include

python3 ../../src/start_noise_analysis.py sl rf 7 3 0.2 1 0.03 7 100 ../../ ../../

python3 ../../src/start_noise_analysis.py sl rf 7 3 0.2 1 0.04 7 100 ../../ ../../

python3 ../../src/start_noise_analysis.py sl rf 7 3 0.2 1 0.05 7 100 ../../ ../../

python3 ../../src/start_noise_analysis.py sl rf 7 3 0.2 1 0.06 7 100 ../../ ../../

#read reports
python3 ../../src/parse_noises_log.py sl rf 7 3 0.2 1 0.03 ../../ ../../

python3 ../../src/parse_noises_log.py sl rf 7 3 0.2 1 0.04 ../../ ../../

python3 ../../src/parse_noises_log.py sl rf 7 3 0.2 1 0.05 ../../ ../../

python3 ../../src/parse_noises_log.py sl rf 7 3 0.2 1 0.06 ../../ ../../