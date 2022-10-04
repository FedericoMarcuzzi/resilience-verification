from misc import *
import sys
import numpy as np
from pathlib import Path

if(len(sys.argv) < 3):
    sys.exit("python3 executable <dataset> <noise> <train_size>")

dataset_name = sys.argv[1]
noise = float(sys.argv[2])
train_size = float(sys.argv[3])

if dataset_name == 'bc':
    X, y = load_bc()

if dataset_name == 'db':
    X, y = load_diabetes()

if dataset_name == 'cr':
    X, y = load_codrna(10000)

if dataset_name == 'sl':
    X, y = load_sensorless()

full_name, dim = get_data_info(dataset_name)


# main folders path
path_exp = "../exp/"
path_dataset = path_exp + full_name + "/"
path_sidx = path_dataset + "idx_synth/"
path_sdata = path_dataset + "data_synth/"

Path(path_exp).mkdir(parents=True, exist_ok=True)
Path(path_dataset).mkdir(parents=True, exist_ok=True)
Path(path_sidx).mkdir(parents=True, exist_ok=True)
Path(path_sdata).mkdir(parents=True, exist_ok=True)


# remove duplicates
X, idx = np.unique(X, axis=0, return_index=True)
y = y[idx]

# normalize in [0, 1]
X = normalize(X) 

# split in train and test set
_, X_test, _, y_test = train_test_split(X, y, train_size=train_size, random_state=SEED)

# generate synthetic test set
for i in np.arange(NUM_DATA):
    X_new, X_idx = gen_synth_data(X_test, y_test, noise, X, y, NUM_TRY)

    print(noise, 'iteration', i)
    print('size prop: ', len(X_idx) / len(y_test))

    # save indices
    idx_filename = path_sidx + "test_idx_{}_{}_{}".format(dataset_name, noise, i)
    np.save(idx_filename, X_idx)

    # save synthetic dataset
    data_filename = path_sdata + "test_set_{}_{}_{}.json".format(dataset_name, noise, i)
    dataset_json(X_new, y_test, data_filename)