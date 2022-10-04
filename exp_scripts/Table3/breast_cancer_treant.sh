#!/bin/bash

#generate the executable
g++ -O3 -o ../../src/compute_true_rob ../../src/compute_true_rob.cpp -I../../src/include

#generate reports
python3 ../../src/start_noise_analysis.py bc treant 7 3 0.15 1 0.05 7 100 ../../ ../../

python3 ../../src/start_noise_analysis.py bc treant 7 3 0.15 1 0.06 7 100 ../../ ../../

python3 ../../src/start_noise_analysis.py bc treant 7 3 0.15 1 0.07 7 100 ../../ ../../

python3 ../../src/start_noise_analysis.py bc treant 7 3 0.15 1 0.08 7 100 ../../ ../../

mkdir ../../exp/breast_cancer/exps_synthetic_logs/

#read reports
python3 ../../src/parse_noises_log.py bc treant 7 3 0.15 1 0.05 ../../

python3 ../../src/parse_noises_log.py bc treant 7 3 0.15 1 0.06 ../../

python3 ../../src/parse_noises_log.py bc treant 7 3 0.15 1 0.07 ../../

python3 ../../src/parse_noises_log.py bc treant 7 3 0.15 1 0.08 ../../