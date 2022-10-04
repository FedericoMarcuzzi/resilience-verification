import numpy as np
import pandas as pd
import sys
import subprocess

def retrieve_data_from_logs(logname):
	log = open(logname, "r")
	accuracies = []
	robustnesss = []
	experiments = log.read().split("\n\n\n")
	for experiment in experiments[:-1]:
		experiment = experiment.split("\n")
		accuracy = experiment[1].split(" ")[1]
		accuracies += [float(accuracy)]
		robustness = experiment[2].split(" ")[2]
		robustnesss += [float(robustness)]

	return accuracies, robustnesss

model_filename_map = {"rf": "models/rf/rf_{}_{}_{}.json", "treant": "models/treant/treant_{}_{}_{}_sse_{}_{}_left.json"}
dataset_name_folder = {"bc": "exp/breast_cancer/", "db": "exp/diabetes/", "cr": "exp/cod_rna/", "sl" : "exp/sensorless/"}
atk_filename_base = "atk/atk_{}_{}_l.json"
test_set_filename_base = "test_set_{}"
synthetic_folder = "data_synth/"

dataset = sys.argv[1]
model = sys.argv[2]
n_estimators = sys.argv[3]
depth = sys.argv[4]
atk_magnitude = sys.argv[5]
budget = sys.argv[6]
noise = sys.argv[7]

prefix_to_reach_exp_folder = sys.argv[8]
prefix_to_reach_src_folder = sys.argv[9]

cmd = prefix_to_reach_src_folder + "src/main_min_rob"

dataset_folder_prefix = prefix_to_reach_exp_folder + dataset_name_folder[dataset]

if model == "rf":
	model_filename = dataset_folder_prefix + model_filename_map[model].format(dataset, n_estimators, depth)
else:
	model_filename = dataset_folder_prefix + model_filename_map[model].format(dataset, n_estimators, depth, budget, atk_magnitude)

attacker_filename = dataset_folder_prefix + atk_filename_base.format(dataset, atk_magnitude)

test_set_filename = test_set_filename_base.format(dataset)

print(subprocess.check_output([cmd, model_filename, attacker_filename, budget, noise, dataset_folder_prefix, synthetic_folder, test_set_filename]).decode())