#!/bin/sh

# parameters: dataset n_estimators depth budget perturbation

# sensorless
python3 treant.py sl 1 3 1 0.2
python3 treant.py sl 1 5 1 0.2
python3 treant.py sl 1 7 1 0.2

python3 treant.py sl 5 3 1 0.2
python3 treant.py sl 7 3 1 0.2
python3 treant.py sl 9 3 1 0.2


# diabetes
python3 treant.py db 1 3 1 0.03
python3 treant.py db 1 5 1 0.03
python3 treant.py db 1 7 1 0.03

python3 treant.py db 5 3 1 0.03
python3 treant.py db 7 3 1 0.03
python3 treant.py db 9 3 1 0.03


# cod-rna
python3 treant.py cr 1 3 1 0.07
python3 treant.py cr 1 5 1 0.07
python3 treant.py cr 1 7 1 0.07

python3 treant.py cr 5 3 1 0.07
python3 treant.py cr 7 3 1 0.07
python3 treant.py cr 9 3 1 0.07


# breast-cancer
python3 treant.py bc 1 3 1 0.15
python3 treant.py bc 1 5 1 0.15
python3 treant.py bc 1 7 1 0.15

python3 treant.py bc 5 3 1 0.15
python3 treant.py bc 7 3 1 0.15
python3 treant.py bc 9 3 1 0.15