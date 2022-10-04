#!/bin/bash 

#before running these experiments, be sure that the flag in config_exp_paper.h is equal to 1
g++ -O3 -o ../../src/analyze_exp_paper ../../src/analyze.cpp -I../../src/include -lpthread

#from these tests it's possible to retrieve accuracy, true robustness, approximated robustness and resilience
../../src/analyze_exp_paper ../../exp/sensorless/models/treant/treant_sl_1_3_sse_1_0.2_left.json ../../exp/sensorless/atk/atk_sl_0.2_l.json 1 ../../exp/sensorless/test_set_sl.json 400 20 400 0.03 1 7

../../src/analyze_exp_paper ../../exp/sensorless/models/treant/treant_sl_1_5_sse_1_0.2_left.json ../../exp/sensorless/atk/atk_sl_0.2_l.json 1 ../../exp/sensorless/test_set_sl.json 400 20 400 0.03 1 7

../../src/analyze_exp_paper ../../exp/sensorless/models/treant/treant_sl_1_7_sse_1_0.2_left.json ../../exp/sensorless/atk/atk_sl_0.2_l.json 1 ../../exp/sensorless/test_set_sl.json 400 20 400 0.03 1 7

../../src/analyze_exp_paper ../../exp/sensorless/models/treant/treant_sl_5_3_sse_1_0.2_left.json ../../exp/sensorless/atk/atk_sl_0.2_l.json 1 ../../exp/sensorless/test_set_sl.json 400 20 400 0.03 1 7

../../src/analyze_exp_paper ../../exp/sensorless/models/treant/treant_sl_7_3_sse_1_0.2_left.json ../../exp/sensorless/atk/atk_sl_0.2_l.json 1 ../../exp/sensorless/test_set_sl.json 400 20 400 0.03 1 7

../../src/analyze_exp_paper ../../exp/sensorless/models/treant/treant_sl_9_3_sse_1_0.2_left.json ../../exp/sensorless/atk/atk_sl_0.2_l.json 1 ../../exp/sensorless/test_set_sl.json 400 20 400 0.03 1 7

#from these tests it's possible to retrieve the min robustness

g++ -O3 -o ../../src/main_min_rob ../../src/main_min_rob.cpp -I../../src/include

python3 ../../src/compute_min_rob.py sl treant 1 3 0.2 1 0.03 ../../ ../../

python3 ../../src/compute_min_rob.py sl treant 1 5 0.2 1 0.03 ../../ ../../

python3 ../../src/compute_min_rob.py sl treant 1 7 0.2 1 0.03 ../../ ../../

python3 ../../src/compute_min_rob.py sl treant 5 3 0.2 1 0.03 ../../ ../../

python3 ../../src/compute_min_rob.py sl treant 7 3 0.2 1 0.03 ../../ ../../

python3 ../../src/compute_min_rob.py sl treant 9 3 0.2 1 0.03 ../../ ../../