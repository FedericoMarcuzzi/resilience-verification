import subprocess  
import importlib
import sys
import numpy as np

model_filename_map = {"rf": "models/rf/rf_{}_{}_{}.json", "treant": "models/treant/treant_{}_{}_{}_sse_{}_{}_left.json"}
dataset_name_folder = {"bc": "exp/breast_cancer/", "db": "exp/diabetes/", "cr": "exp/cod_rna/", "sl" : "exp/sensorless/"}
atk_filename_base = "atk/atk_{}_{}_l.json"
log_filename_base = "exps_synthetic_logs/testmodel{}dataset{}noise{}depth{}nestim{}atk{}budget{}.txt"
test_set_orig_filename_base = "test_set_{}.json"
test_set_synthetic_filename_base = "data_synth/test_set_{}_{}_{}.json"

dataset = sys.argv[1]
model = sys.argv[2]
n_estimators = sys.argv[3]
depth = sys.argv[4]
atk_magnitude = sys.argv[5]
budget = sys.argv[6]
noise = sys.argv[7]
seed = sys.argv[8]
n_synthetic_datasets = sys.argv[9]
prefix_to_reach_exp_folder = sys.argv[10]
prefix_to_reach_src_folder = sys.argv[11]

cmd = prefix_to_reach_src_folder + "src/compute_true_rob"

dataset_folder_prefix = prefix_to_reach_exp_folder + dataset_name_folder[dataset]

if model == "rf":
	model_filename = dataset_folder_prefix + model_filename_map[model].format(dataset, n_estimators, depth)
else:
	model_filename = dataset_folder_prefix + model_filename_map[model].format(dataset, n_estimators, depth, budget, atk_magnitude)

attacker_filename = dataset_folder_prefix + atk_filename_base.format(dataset, atk_magnitude)

test_set_orig_filename = dataset_folder_prefix + test_set_orig_filename_base.format(dataset)

log_filename = dataset_folder_prefix + log_filename_base.format(model, dataset, noise, depth, n_estimators, atk_magnitude, budget)
temp = subprocess.run([cmd, model_filename, attacker_filename, budget, test_set_orig_filename, log_filename, seed])

for i in range(0, int(n_synthetic_datasets)):
	test_set_synthetic_filename = dataset_folder_prefix + test_set_synthetic_filename_base.format(dataset, noise, i)
	temp = subprocess.run([cmd, model_filename, attacker_filename, budget, test_set_synthetic_filename, log_filename, seed])