from misc import *
import sys
import pandas as pd
from pathlib import Path
from sklearn.ensemble import RandomForestClassifier

'''
Parametri dello script:
    - dataset: nome del dataset identificato da 2 lettere + UNIQUE se si vuole utilizzare il filtro (vedi lista sopra);
    - train_size: dimensione del training set per il modello da allenare. Dimensione porzione.
    - n_estimators: parametro opzionale per le foreste, numero di alberi.
    - max_depth: max_depth per i trees;
Esempio invocazione: python3 data_gen_with_options.py wn 1 3000 7 rf entropy 20 1 
'''

if(len(sys.argv) < 4):
    sys.exit("python3 executable <dataset> <train_size> <n_estimators> <max_depth>")

dataset_name = sys.argv[1]
train_size = float(sys.argv[2])
n_estimators = int(sys.argv[3])
max_depth = int(sys.argv[4])


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
path_models = path_dataset + "models/rf/"

Path(path_exp).mkdir(parents=True, exist_ok=True)
Path(path_dataset).mkdir(parents=True, exist_ok=True)
Path(path_models).mkdir(parents=True, exist_ok=True)


# remove duplicates
X, idx = np.unique(X, axis=0, return_index=True)
y = y[idx]

# normalize in [0, 1]
X = normalize(X) 

# split in train and test set
X_train, X_test, y_train, y_test = train_test_split(X, y, train_size=train_size, random_state=SEED)

print("#####################################################################################")
print("[info]: dataset size: ", len(X))
print("[info]: training set size: ", len(X_train))
print("[info]: test set size: ", len(X_test))

# save cleaned train set for treant
filename = path_dataset + "train_set_" + dataset_name + "_cleaned.txt"

if not Path(filename).is_file():
    data = pd.DataFrame(X_train, columns=[i for i in gen_name(dim)])
    data["LABELS"] = y_train
    data.to_csv(filename, index=None, sep=' ')

# save test set in json
filename = path_dataset + "test_set_{}.json".format(dataset_name)

if not Path(filename).is_file():
    dataset_json(X_test, y_test, filename)

# model train
filename = path_models + "rf_{}_{}_{}.json".format(dataset_name, n_estimators, max_depth)
if not Path(filename).is_file():
    print("[info]: model training")
    clf = RandomForestClassifier(n_estimators=n_estimators, random_state=SEED, max_depth=max_depth)
    clf.fit(X_train, y_train)
    sklearn_to_json(clf.estimators_, dim, filename)
else:
    print("[info]: the model already exist")