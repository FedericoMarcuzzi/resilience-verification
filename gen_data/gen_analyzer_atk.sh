#!/bin/sh

mkdir lib
mkdir tmp

# parameters: dataset n_estimators depth budget perturbation

# sensorless
python3 atk_gen.py sl 0.2 l


# diabetes
python3 atk_gen.py db 0.03 l


# cod-rna
python3 atk_gen.py cr 0.07 l


# breast-cancer
python3 atk_gen.py bc 0.15 l