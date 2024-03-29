#!/bin/bash

#generate the executable
g++ -O3 -o ../../src/compute_true_rob ../../src/compute_true_rob.cpp -I../../src/include

python3 ../../src/start_noise_analysis.py cr rf 7 3 0.07 1 0.01 7 100 ../../ ../../

python3 ../../src/start_noise_analysis.py cr rf 7 3 0.07 1 0.02 7 100 ../../ ../../

python3 ../../src/start_noise_analysis.py cr rf 7 3 0.07 1 0.03 7 100 ../../ ../../

python3 ../../src/start_noise_analysis.py cr rf 7 3 0.07 1 0.04 7 100 ../../ ../../

mkdir ../../exp/cod_rna/exps_synthetic_logs/

#read reports
python3 ../../src/parse_noises_log.py cr rf 7 3 0.07 1 0.01 ../../ ../../

python3 ../../src/parse_noises_log.py cr rf 7 3 0.07 1 0.02 ../../ ../../

python3 ../../src/parse_noises_log.py cr rf 7 3 0.07 1 0.03 ../../ ../../

python3 ../../src/parse_noises_log.py cr rf 7 3 0.07 1 0.04 ../../ ../../