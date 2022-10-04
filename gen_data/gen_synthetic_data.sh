#!/bin/sh

# parameters: dataset n_estimators depth budget perturbation

# sensorless
python3 synthetic_dataset.py sl 0.03 0.8
python3 synthetic_dataset.py sl 0.04 0.8
python3 synthetic_dataset.py sl 0.05 0.8
python3 synthetic_dataset.py sl 0.06 0.8

# diabetes
python3 synthetic_dataset.py db 0.01 0.8
python3 synthetic_dataset.py db 0.02 0.8
python3 synthetic_dataset.py db 0.03 0.8
python3 synthetic_dataset.py db 0.04 0.8

# cod-rna
python3 synthetic_dataset.py cr 0.01 0.8
python3 synthetic_dataset.py cr 0.02 0.8
python3 synthetic_dataset.py cr 0.03 0.8
python3 synthetic_dataset.py cr 0.04 0.8

# breast-cancer
python3 synthetic_dataset.py bc 0.05 0.8
python3 synthetic_dataset.py bc 0.06 0.8
python3 synthetic_dataset.py bc 0.07 0.8
python3 synthetic_dataset.py bc 0.08 0.8