from fileinput import filename
import numpy as np
import sys
from pathlib import Path

if(len(sys.argv) < 4):
    sys.exit("python3 executable <dataset> <train_size> <n_estimators> <max_depth>")

### GET DATASET BASIC INFO ###
def get_data_info(dataset):
    if dataset == 'bc':
        return 'breast_cancer', 10

    if dataset == 'db':
        return 'diabetes', 8

    if dataset == 'cr':
        return 'cod_rna', 8

    if dataset == 'sl':
        return 'sensorless', 48


dataset_name = sys.argv[1]
e = sys.argv[2]
side = sys.argv[3]

full_name, dim = get_data_info(dataset_name)


# main folders path
path_exp = "../exp/"
path_dataset = path_exp + full_name + "/"
path_atks = path_dataset + "atk/"

Path(path_exp).mkdir(parents=True, exist_ok=True)
Path(path_dataset).mkdir(parents=True, exist_ok=True)
Path(path_atks).mkdir(parents=True, exist_ok=True)


start = "[\n"
mid = "\t[{}, {}, 0, 0, 1],\n"
end = "]"

left = 0
right = 0

if side == 'l':
    left = "-" + e

if side == 'r':
    right = "-" + e

if side == 'b':
    left = e
    right = e

for i in np.arange(dim):
    start += mid.format(left, right)

start = start[:-2] + "\n" + end

filename = path_atks + "atk_{}_{}_{}.json".format(dataset_name, e, side)

if not Path(filename).is_file():
    with open(filename, "w") as f:
        f.write(start)

print("[info]: {}, {}, {} done!".format(dataset_name, e, side))