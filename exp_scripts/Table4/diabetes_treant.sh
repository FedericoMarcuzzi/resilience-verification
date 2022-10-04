#!/bin/bash 

#before running these experiments, be sure that the flag in config_exp_paper.h is equal to 1
g++ -O3 -o ../../src/analyze_exp_paper ../../src/analyze.cpp -I../../src/include -lpthread

#from these tests it's possible to retrieve accuracy, true robustness, approximated robustness and resilience
../../src/analyze_exp_paper ../../exp/diabetes/models/treant/treant_db_1_3_sse_1_0.03_left.json ../../exp/diabetes/atk/atk_db_0.03_l.json 1 ../../exp/diabetes/test_set_db.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/diabetes/models/treant/treant_db_1_5_sse_1_0.03_left.json ../../exp/diabetes/atk/atk_db_0.03_l.json 1 ../../exp/diabetes/test_set_db.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/diabetes/models/treant/treant_db_1_7_sse_1_0.03_left.json ../../exp/diabetes/atk/atk_db_0.03_l.json 1 ../../exp/diabetes/test_set_db.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/diabetes/models/treant/treant_db_5_3_sse_1_0.03_left.json ../../exp/diabetes/atk/atk_db_0.03_l.json 1 ../../exp/diabetes/test_set_db.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/diabetes/models/treant/treant_db_7_3_sse_1_0.03_left.json ../../exp/diabetes/atk/atk_db_0.03_l.json 1 ../../exp/diabetes/test_set_db.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/diabetes/models/treant/treant_db_9_3_sse_1_0.03_left.json ../../exp/diabetes/atk/atk_db_0.03_l.json 1 ../../exp/diabetes/test_set_db.json 400 20 400 0.01 1 7

#from these tests it's possible to retrieve the min robustness

g++ -O3 -o ../../src/main_min_rob ../../src/main_min_rob.cpp -I../../src/include

python3 ../../src/compute_min_rob.py db treant 1 3 0.03 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py db treant 1 5 0.03 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py db treant 1 7 0.03 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py db treant 5 3 0.03 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py db treant 7 3 0.03 1 0.01 ../../ ../../

python3 ../../src/compute_min_rob.py db treant 9 3 0.03 1 0.01 ../../ ../../