from sklearn.datasets import load_breast_cancer, load_svmlight_file
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.datasets import fetch_openml

import os
import dill
import numpy as np
import pandas as pd
import json
import sys

SEED = 7


def normalize(X):
    X = (X - X.min(0)) / (X.max(0) - X.min(0))
    return np.nan_to_num(X)

######### DATASET #########

def load_diabetes():
    dataset = pd.read_csv("../datasets/diabetes.csv")
    data = dataset.to_numpy()
    X = data[:, : -1]
    y = data[:, -1]
    
    y[y == 0] = -1
    return X, y

def load_bc():
    dataset = load_svmlight_file("../datasets/breast-cancer")
    X = dataset[0].toarray()
    y = dataset[1]

    y[y==2] = 1
    y[y==4] = -1
    return X, y

def load_codrna(subset_dim):
    dataset_train = load_svmlight_file("../datasets/cod-rna")
    X = dataset_train[0].toarray()
    y = dataset_train[1]
    X, X_rem, y, y_rem = train_test_split(X, y, train_size=subset_dim, random_state=SEED)
    return X, y

def load_sensorless(lab=[1, 11]):
    dataset_train = pd.read_csv("../datasets/sensorless.csv", sep=' ', header=None)
    dataset_train.drop(index=dataset_train[~dataset_train[48].isin(lab)].index, inplace=True)
    dataset_train[48].replace(lab, [-1, 1], inplace=True)
    data = dataset_train.to_numpy()

    X = data[:, : -1]
    y = data[:, -1]
    return X, y

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

### DATASET TO JSON ###
def dataset_json(data, labels, filename):
    lbs = labels.tolist()
    dt = data.tolist()
    l = [lbs, dt]
    with open(filename, 'w') as f:
        f.write(json.dumps(l))



######### MODELS #########

### SAVE ENSEMBLE TO JSON ###
def str_to_json(ensamble, dim, lab=[-1,1]):
    n_trees = len(ensamble)
    i = 0
    str_tree = "\t" + str(dim) + ",\n\t["
    for l in lab:
        str_tree += str(l) + ", "
    
    str_tree = str_tree[:-2] + "],\n\t[\n"
    
    for dict_tree in ensamble:
        app = np.array(dict_tree['feature'])
        app+=1
        app[app<0] = 0
        dict_tree['feature'] = app
        
        app = np.array(dict_tree['children_left'])
        app+=1
        app[app<0] = 0
        dict_tree['children_left'] = app
        
        app = np.array(dict_tree['children_right'])
        app+=1
        app[app<0] = 0
        dict_tree['children_right'] = app
        
        str_f = '\t\t\t[' + ', '.join([str(v) for v in dict_tree['feature']]) + '],\n'
        str_t = '\t\t\t[' + ', '.join([str(v) for v in dict_tree['threshold']]) + '],\n'
        str_l = '\t\t\t[' + ', '.join([str(v) for v in dict_tree['children_left']]) + '],\n'
        str_r = '\t\t\t[' + ', '.join([str(v) for v in dict_tree['children_right']]) + '],\n'
        str_p = '\t\t\t[' + ', '.join([str(v) for v in dict_tree['prediction']]) + ']\n'
        
        i+=1
        if i < n_trees:
            str_tree += '\t\t[\n' + str_f + str_t + str_l + str_r + str_p + '\t\t],\n'
        else:
            str_tree += '\t\t[\n' + str_f + str_t + str_l + str_r + str_p + '\t\t]'
        
    str_ensamble = '[\n' + str_tree + '\n\t]\n]'
    print(str_ensamble)
    return str_ensamble



######### SKLEARN #########

### SKLEARN MODEL TO JSON ###
def sklearn_to_json(trees, dim, filename):
    ensamble = []
    classes = np.asarray([-1, 1])
    n_trees = len(trees)
    i = 0
    str_tree = ""
    for tr in trees:
        dict_tree = {}
        n_nodes = len(tr.tree_.value)
        n_labels = len(tr.tree_.value[0][0])   
        value = tr.tree_.value.reshape(n_nodes, n_labels)
        prediction = classes[np.argmax(value, axis=1)]
        
        dict_tree['feature'] = tr.tree_.feature
        dict_tree['threshold'] = tr.tree_.threshold
        dict_tree['children_left'] = tr.tree_.children_left
        dict_tree['children_right'] = tr.tree_.children_right
        dict_tree['prediction'] = prediction
        ensamble.append(dict_tree)

    with open(filename, 'w') as f:
        f.write(str_to_json(ensamble, dim))





### TREANT MODEL TO JSON ###
import re

child = '\[([0-9]*),([0-9]*)\]'
feature = 'Feature_ID:([0-9]*)'
threshold = 'Threshold:(.*),Num_instances'
score = 'Score:(.*),Num_instances'

def treant_to_json_rec(tree, dict_t, next, next_chld):
    line = tree[next]

    lr_child = re.findall(child, line)
    f = re.findall(feature, line)
    v = re.findall(threshold, line)
    s = re.findall(score, line)

    if lr_child == []:
    
        dict_t['feature'] += [-1]
        dict_t['threshold'] += [-1]
        dict_t['children_left'] += [-1]
        dict_t['children_right'] += [-1]

        f_s = float(s[0])
        ss = 0
        if f_s < 0:
            ss = -1
        else:
            ss = 1
        dict_t['prediction'] += [ss]

        return next_chld;

    else:
        dict_t['feature'] += [int(f[0])]
        dict_t['threshold'] += [float(v[0])]
        dict_t['children_left'] += [next_chld]

        pos_right_cld = len(dict_t['children_right'])
        dict_t['children_right'] += [-1]
        dict_t['prediction'] += [0]
         
        next_chld = treant_to_json_rec(tree, dict_t, int(lr_child[0][0]), next_chld + 1)
        dict_t['children_right'][pos_right_cld] = next_chld
        return treant_to_json_rec(tree, dict_t, int(lr_child[0][1]), next_chld + 1)


### TREANT MODEL TO JSON ###
def treant_to_json(filename):
    with open(filename, "r") as f:
        f.readline()
        
        n_trees = int(f.readline())
        tree = 0
        dict_tree = {n : [] for n in np.arange(n_trees)}
        for line in f:
            if line == '\n':
                tree += 1
            else:
                dict_tree[tree] = dict_tree[tree] + [line]

    ensemble = []
    for i in np.arange(n_trees):
        print(i)
        dict_t = {}

        dict_t['feature'] = []
        dict_t['threshold'] = []
        dict_t['children_left'] = []
        dict_t['children_right'] = []
        dict_t['prediction'] = []

        treant_to_json_rec(dict_tree[i], dict_t, len(dict_tree[i])-1, 1)
        ensemble.append(dict_t)

    return ensemble



######### TREANT ATKS AND DATASETS #########
import itertools
import string

def gen_name(dim):
    size = 1
    while dim > 0:
        for i in itertools.product(string.ascii_letters, repeat = size):
            word = ''.join(i)
            yield word
            dim -=1
            if dim == 0:
                break

        size += 1


### GEN ATK FILES ###
def gen_atk_str(dim):
    start = '''
    {
        "attacks": [
    '''

    end = '''
        ]
    }'''

    atk_f = '''
            {{
                "{}": [
                    {{
                        "pre": "(0, 1)",
                        "post": -{},
                        "cost": 1,
                        "is_numerical": true
                    }}
                ]
            }},'''

    for i in gen_name(dim):
        start += atk_f.format(i, 0.03)

    return start[:-1] + end



######### SYNTHETIC DATASETS #########
NUM_DATA = 100
NUM_TRY = 100

np.random.seed(SEED)

# compute the norm-2 of x with respect to the instances in X
def one_vs_all(x, X):
    n_ist, _ = X.shape
    out = np.zeros((n_ist, 1))

    for i in np.arange(n_ist):
        out[i] = np.linalg.norm(x - X[i])
    
    return out

def gen_synth_data(X_test, y_test, n, X, y, k):
    n_ist, _ = X_test.shape
    new_X = np.empty_like(X_test, dtype=float)
    X_idx = []

    new_x = []
    for i in np.arange(n_ist):
        print(n, ': ', i, '/', X_test.shape[0])
        j = 0
        flag = True

        while flag and j < k:
            x = X_test[i]
            low_l = x - n
            low_r = low_l + 0.00009
            up_r = x + n
            up_l = up_r - 0.00009

            out_l = np.zeros_like(low_l)
            out_r = np.zeros_like(up_r)

            f = 0
            for lor in np.random.randint(0, 2, len(x)):
                if lor == 0:
                    out_l[f] = low_l[f]
                    out_r[f] = low_r[f]
                else:
                    out_l[f] = up_l[f]
                    out_r[f] = up_r[f]
                f += 1

            new_x = np.random.uniform(out_l, out_r)

            # comute the norm-2 of new_x and the instances in X
            dst = one_vs_all(new_x, X)
            # assign to new_x the label of the closest instance in X
            l = y[np.argmin(dst)]

            # keep instance new_x only if the label l is the same of the original instance x
            if y_test[i] == l:
                X_idx.append(i)
                new_X[i] = new_x
                flag = False
            j += 1

        if flag:
            print('NOT FOUND!')
            new_X[i] = new_x

    X_idx = np.asarray(X_idx, dtype=int)
    return new_X, X_idx