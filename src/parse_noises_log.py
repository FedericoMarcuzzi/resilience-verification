import numpy as np
import pandas as pd
import sys

def retrieve_data_from_logs(logname):
	log = open(logname, "r")
	accuracies = []
	robustnesss = []
	experiments = log.read().split("\n\n")
	print(experiments)
	for experiment in experiments[:-1]:
		experiment = experiment.split("\n")
		print(experiment)
		accuracy = experiment[0].split(" ")[1]
		accuracies += [float(accuracy)]
		robustness = experiment[1].split(" ")[2]
		robustnesss += [float(robustness)]

	return accuracies, robustnesss


dataset_name_folder = {"bc": "exp/breast_cancer/", "db": "exp/diabetes/", "cr": "exp/cod_rna/", "sl" : "exp/sensorless/"}

dataset = sys.argv[1]
model = sys.argv[2]
n_estimators = sys.argv[3]
depth = sys.argv[4]
atk_magnitude = sys.argv[5]
budget = sys.argv[6]
noise = sys.argv[7]
prefix_to_reach_exp_folder = sys.argv[8]#"../../"
log_filename_base = "exps_synthetic_logs/testmodel{}dataset{}noise{}depth{}nestim{}atk{}budget{}.txt"

print("\n======================================")
print("DATASET: ", dataset)
log_filename_base_dataset = prefix_to_reach_exp_folder + dataset_name_folder[dataset] + log_filename_base

print("MODEL: ", model)
print("NOISE: ", noise)
log_filename = log_filename_base_dataset.format(model, dataset, noise, depth, n_estimators, atk_magnitude, budget)
accuracies, robustnesss = retrieve_data_from_logs(log_filename)

indexes = np.argsort(robustnesss)
max_indexes = indexes[-5:]
min_indexes = indexes[:5]

max_list = []
for index in max_indexes:
	max_list += [(accuracies[index], robustnesss[index])]

min_list = []
for index in min_indexes:
	min_list += [(accuracies[index], robustnesss[index])]

print("TUPLES IN THE FORM (ACCURACY, ROBUSTNESS)")
print("5 TUPLES MIN ROBUSTNESS ", min_list)
print("5 TUPLES MAX ROBUSTNESS ", max_list)
print("======================================\n")
