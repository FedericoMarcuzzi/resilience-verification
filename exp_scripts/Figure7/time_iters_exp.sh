#!/bin/bash 
#before running these experiments, be sure that the flag in config_exp_paper.h is equal to 1

g++ -O3 -o ../../src/analyze_exp_paper ../../src/analyze.cpp -I../../src/include -lpthread

../../src/analyze_exp_paper ../../exp/diabetes/models/treant/treant_db_11_3_sse_1_0.03_left.json ../../exp/diabetes/atk/atk_db_0.03_l.json 1 ../../exp/diabetes/test_set_db.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/diabetes/models/treant/treant_db_11_3_sse_1_0.03_left.json ../../exp/diabetes/atk/atk_db_0.03_l.json 2 ../../exp/diabetes/test_set_db.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/diabetes/models/treant/treant_db_11_3_sse_1_0.03_left.json ../../exp/diabetes/atk/atk_db_0.03_l.json 3 ../../exp/diabetes/test_set_db.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/diabetes/models/treant/treant_db_11_3_sse_1_0.03_left.json ../../exp/diabetes/atk/atk_db_0.03_l.json 4 ../../exp/diabetes/test_set_db.json 400 20 400 0.01 1 7

../../src/analyze_exp_paper ../../exp/diabetes/models/treant/treant_db_11_3_sse_1_0.03_left.json ../../exp/diabetes/atk/atk_db_0.03_l.json 5 ../../exp/diabetes/test_set_db.json 400 20 400 0.01 1 7