#!/bin/bash

#compile the exec
g++ -O3 -o ../../src/robustness_varying_n_synthetic_datasets ../../src/robustness_varying_n_synthetic_datasets.cpp -I../../src/include

#generate rob value for each sythetic dataset
../../src/robustness_varying_n_synthetic_datasets ../../exp/cod_rna/models/treant/treant_cr_5_3_sse_1_0.07_left.json ../../exp/cod_rna/atk/atk_cr_0.07_l.json 1 ../../exp/cod_rna/ test_set_cr 0.01