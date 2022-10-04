#!/bin/bash 
#before running these experiments, be sure that the flag in config_exp_paper.h is equal to 1
g++ -O3 -o ../../src/analyze_exp_paper ../../src/analyze.cpp -I../../src/include -lpthread

.../../src/analyze_exp_paper ../../exp/cod_rna/models/treant/treant_cr_1_3_sse_1_0.07_left.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

.../../src/analyze_exp_paper ../../exp/cod_rna/models/treant/treant_cr_1_5_sse_1_0.07_left.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

.../../src/analyze_exp_paper ../../exp/cod_rna/models/treant/treant_cr_1_7_sse_1_0.07_left.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

.../../src/analyze_exp_paper ../../exp/cod_rna/models/treant/treant_cr_5_3_sse_1_0.07_left.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

.../../src/analyze_exp_paper ../../exp/cod_rna/models/treant/treant_cr_7_3_sse_1_0.07_left.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

.../../src/analyze_exp_paper ../../exp/cod_rna/models/treant/treant_cr_9_3_sse_1_0.07_left.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/test_set_cr.json 400 20 400 0.01 1 7

#from these tests it's possible to retrieve the min robustness

g++ -O3 -o ../../src/main_min_rob ../../src/main_min_rob.cpp -I../../src/include

python3 ../../src/compute_min_rob.py cr treant 1 3 0.07 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py cr treant 1 5 0.07 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py cr treant 1 7 0.07 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py cr treant 5 3 0.07 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py cr treant 7 3 0.07 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py cr treant 9 3 0.07 1 0.01 ../../ ../../