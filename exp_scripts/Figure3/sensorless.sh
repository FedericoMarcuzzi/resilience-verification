#!/bin/bash

#compile the exec
g++ -O3 -o ../../src/robustness_varying_n_synthetic_datasets ../../src/robustness_varying_n_synthetic_datasets.cpp -I../../src/include

#generate rob value for each sythetic dataset
../../src/robustness_varying_n_synthetic_datasets ../../exp/sensorless/models/treant/treant_sl_5_3_sse_1_0.2_left.json ../../exp/sensorless/atk/atk_sl_0.2_l.json 1 ../../exp/sensorless/ test_set_sl 0.03