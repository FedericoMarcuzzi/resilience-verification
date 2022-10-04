#!/bin/bash 

#before running these experiments, be sure that the flag in config_exp_paper.h is equal to 1
g++ -O3 -o ../../src/analyze_exp_paper ../../src/analyze.cpp -I../../src/include -lpthread

#from these tests it's possible to retrieve accuracy, true robustness, approximated robustness and resilience
../../src/analyze_exp_paper ../../exp/breast_cancer/models/treant/treant_bc_1_3_sse_1_0.15_left.json ../../exp/breast_cancer/atk/atk_bc_0.15_l.json 1 ../../exp/breast_cancer/test_set_bc.json 400 20 400 0.05 1 7

../../src/analyze_exp_paper ../../exp/breast_cancer/models/treant/treant_bc_1_5_sse_1_0.15_left.json ../../exp/breast_cancer/atk/atk_bc_0.15_l.json 1 ../../exp/breast_cancer/test_set_bc.json 400 20 400 0.05 1 7

../../src/analyze_exp_paper ../../exp/breast_cancer/models/treant/treant_bc_1_7_sse_1_0.15_left.json ../../exp/breast_cancer/atk/atk_bc_0.15_l.json 1 ../../exp/breast_cancer/test_set_bc.json 400 20 400 0.05 1 7

../../src/analyze_exp_paper ../../exp/breast_cancer/models/treant/treant_bc_5_3_sse_1_0.15_left.json ../../exp/breast_cancer/atk/atk_bc_0.15_l.json 1 ../../exp/breast_cancer/test_set_bc.json 400 20 400 0.05 1 7

../../src/analyze_exp_paper ../../exp/breast_cancer/models/treant/treant_bc_7_3_sse_1_0.15_left.json ../../exp/breast_cancer/atk/atk_bc_0.15_l.json 1 ../../exp/breast_cancer/test_set_bc.json 400 20 400 0.05 1 7

../../src/analyze_exp_paper ../../exp/breast_cancer/models/treant/treant_bc_9_3_sse_1_0.15_left.json ../../exp/breast_cancer/atk/atk_bc_0.15_l.json 1 ../../exp/breast_cancer/test_set_bc.json 400 20 400 0.05 1 7

#from these tests it's possible to retrieve the min robustness

g++ -O3 -o ../../src/main_min_rob ../../src/main_min_rob.cpp -I../../src/include

python3 ../../src/compute_min_rob.py bc treant 1 3 0.15 1 0.05 ../../ ../../

python3 ../../src/compute_min_rob.py bc treant 1 5 0.15 1 0.05 ../../ ../../

python3 ../../src/compute_min_rob.py bc treant 1 7 0.15 1 0.05 ../../ ../../

python3 ../../src/compute_min_rob.py bc treant 5 3 0.15 1 0.05 ../../ ../../

python3 ../../src/compute_min_rob.py bc treant 7 3 0.15 1 0.05 ../../ ../../

python3 ../../src/compute_min_rob.py bc treant 9 3 0.15 1 0.05 ../../ ../../