from misc import *
import os
import sys
from pathlib import Path

if(len(sys.argv) < 5):
    sys.exit("python3 executable <dataset> <n_estimators> <depth> <budget> <epsilon>")

njobs = 5

dataset_name = sys.argv[1] # dataset
n_est = sys.argv[2] # ensemble size
depth = sys.argv[3] # tree depth
b = sys.argv[4] # budget
e = sys.argv[5] # epsilon

full_name, dim = get_data_info(dataset_name)


# main folders path
path_exp = "../exp/"
path_dataset = path_exp + full_name + "/"
path_models = path_dataset + "models/treant/"

Path(path_exp).mkdir(parents=True, exist_ok=True)
Path(path_dataset).mkdir(parents=True, exist_ok=True)
Path(path_models).mkdir(parents=True, exist_ok=True)
Path("tmp").mkdir(parents=True, exist_ok=True)


# treant train set
train_set = path_dataset + "train_set_" + dataset_name + "_cleaned.txt"
if not Path(train_set).is_file():
    print("run <data_gen.py> before this file: <treant.py>")
    exit(0)

# create atk file
atk_filename = "tmp/attacks_{}_{}_left.json".format(dataset_name, e)
if not Path(atk_filename).is_file():
    with open(atk_filename, "w") as f:
        f.write(gen_atk_str(dim))

filename = path_models + "treant_{}_{}_{}_sse_{}_{}_left.json".format(dataset_name, n_est, depth, b, e)
print("\n#####################################################################################\n\n\t", filename, "\n\n#####################################################################################\n")

# train treant model
if Path(filename).is_file():
    print("EXISTS: ", filename)
else:
    print("NOT EXISTS: ", filename)
    treant_tmp_filename = "./tmp/treant_{}_{}_{}_{}_left".format(dataset_name, n_est, depth, e)

    # run treant
    cmd = "LD_PRELOAD=" + os.getcwd() + "/lib/libtreantLib.so ./lib/treant -a {} -b {} -d {} -f {} -e {} -i {} -o {} -j {}".format(atk_filename, b, depth, train_set, n_est, 0, treant_tmp_filename, njobs) # 16 > /dev/null
    print(cmd)
    os.system(cmd)

    ensemble = treant_to_json(treant_tmp_filename)

    with open(filename, 'w') as f:
        f.write(str_to_json(ensemble, dim))
