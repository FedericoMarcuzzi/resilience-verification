#!/bin/bash

#before running these experiments, be sure that the flag in config_exp_paper.h is equal to 1
g++ -O3 -o ../../src/analyze_exp_paper ../../src/analyze.cpp -I../../src/include -lpthread

#from these tests it's possible to retrieve accuracy, true robustness, approximated robustness and resilience
../../src/analyze_exp_paper ../../exp/cod_rna/models/rf/rf_cr_1_3.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/cod_rna/models/rf/rf_cr_1_5.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/cod_rna/models/rf/rf_cr_1_7.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/cod_rna/models/rf/rf_cr_5_3.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/cod_rna/models/rf/rf_cr_7_3.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/cod_rna/models/rf/rf_cr_9_3.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

#from these tests it's possible to retrieve the min robustness

g++ -O3 -o ../../src/main_min_rob ../../src/main_min_rob.cpp -I../../src/include

python3 ../../src/compute_min_rob.py cr rf 1 3 0.07 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py cr rf 1 5 0.07 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py cr rf 1 7 0.07 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py cr rf 5 3 0.07 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py cr rf 7 3 0.07 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py cr rf 9 3 0.07 1 0.01 ../../ ../../