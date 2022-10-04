#!/bin/sh

# parameters: dataset n_estimators depth budget perturbation

# sensorless
python3 random_forest.py sl 0.8 1 3
python3 random_forest.py sl 0.8 1 5
python3 random_forest.py sl 0.8 1 7

python3 random_forest.py sl 0.8 5 3
python3 random_forest.py sl 0.8 7 3
python3 random_forest.py sl 0.8 9 3


# diabetes
python3 random_forest.py db 0.8 1 3
python3 random_forest.py db 0.8 1 5
python3 random_forest.py db 0.8 1 7

python3 random_forest.py db 0.8 5 3
python3 random_forest.py db 0.8 7 3
python3 random_forest.py db 0.8 9 3

# cod-rna
python3 random_forest.py cr 0.8 1 3
python3 random_forest.py cr 0.8 1 5
python3 random_forest.py cr 0.8 1 7

python3 random_forest.py cr 0.8 5 3
python3 random_forest.py cr 0.8 7 3
python3 random_forest.py cr 0.8 9 3


# breast-cancer
python3 random_forest.py bc 0.8 1 3
python3 random_forest.py bc 0.8 1 5
python3 random_forest.py bc 0.8 1 7

python3 random_forest.py bc 0.8 5 3
python3 random_forest.py bc 0.8 7 3
python3 random_forest.py bc 0.8 9 3