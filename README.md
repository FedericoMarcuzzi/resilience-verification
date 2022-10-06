# Resilience Verification

This repository contains the implementation of the data-independent stability analysis for decision-tree ensembles proposed by **Calzavara *et. al.*** in their research paper titled [<em>Beyond Robustness: Resilience Verification of Tree-Based Classifiers</em>](https://www.sciencedirect.com/science/article/pii/S0167404822002371). This repository also contains the code and the scripts to reproduce the experiments described in the paper.

## Installation
Download the repository. Remember to compile using the flags <em>-Iinclude</em> and <em>-lpthread</em> to use the analyzer.

### Requirments
The following requirements are necessary to use the analyzer. For reproducibility reasons, specific versions of the libraries are required.
- Python3
- C++14 compiler
- sklearn (version 0.23.2 used for the experiments)
- pandas (version 1.1.3 used for the experiments)
- numpy (version 1.19.1 used for the experiments)
- <em>treant</em> and <em>libtreantLib.so</em> in /gen_data/lib/ (see the section <em>Treant</em>)
  
### Treant
The TREANT repository at [https://github.com/FedericoMarcuzzi/TREANT](https://github.com/FedericoMarcuzzi/TREANT) contains the instructions to compile the <em>treant</em> executable and the <em>libtreantLib.so</em> library. Move <em>treant</em> and <em>libtreantLib.so</em> in gen_data/lib/. 

## Usage

### Use analyzer:
In order to use the analyzer on its own, in config_exp_paper.h set the CONFIG_EXPERIMENTS_PAPER flag to 0. With this configuration, the analyzer won't display the execution time and statistics.

In <em>analyze_tree.h</em> and <em>analyze_ensemble.h</em> files there are the functions to perform an analysis of a decision tree or a decision tree ensemble. In <em>src</em> folder there is <em>analyze.cpp</em>, an example of how to use the analyzer.
In the same file, there is also an example of how to compute the robustness and resilience given the output of the analyzer.

The example can be compiled with the following string:
```bash
g++ -O3 -o analyze analyze.cpp -Iinclude -lpthread
```
The program requires these arguments:
```bash
./analyze <ensemble_filename.json> <attacker_filename.json> <attacker_budget >= 0> <test_set_filename.json> <num iter > 0> <n_threads > 1> <iterations_x_dump > 0> <noises (divided by -)> <closed_interval_noise> <seed>
```
The specific parameters are:
- <em>ensemble_filename.json</em>: the JSON file containing the structure of the tree-based classifier. It's possible to train a specific tree-based ensemble and obtain the classifier in the JSON format by using the scripts random_forest.py and treant.py in the gen_data folder as follows:
  ```bash
  ./python3 random_forest.py <dataset_abbreviation> <train_size> <n_trees> <max_depth>

  ./python3 treant.py <dataset_abbreviation> <n_trees> <max_depth> <budget> <perturbation>
  ```
  See some examples in gen_data/train_random_forest.sh and gen_data/train_treant.sh. Moreover, see the available dataset abbreviations in the two scripts random_forest.py and treant.py.
- <em>attacker_filename.json</em>: the JSON file specifying the attack interval per feature. It's possible to generate an attack file by using the script atk_gen.py in the gen_data folder as follows:
  ```bash
  python3 atk_gen.py <dataset_abbreviation> <atk_magnitude> <atk only left direction (l), only right direction (r) or both (b)>
  ```
  See some examples in gen_data/gen\_analyzer\_atk.sh
- <em>attacker_budget</em>: a natural number that specifies the attacker's budget.
- <em>test_set_filename.json</em>: test set file in JSON format. The test set is automatically generated from the original dataset when you train a random forest on the dataset. In particular, the command:
  ```bash
    ./python3 random_forest.py <dataset_abbreviation> <train_size> <n_trees> <max_depth>
  ```
  generate the test set of the dataset corresponding to the dataset_abbreviation that contains a fraction 1-train_size of instances of the original dataset.
- <em>num iter > 0</em>: number of iterations performed for the analysis.
- <em>n_threads > 1</em>: number of used threads.
- <em>iterations_x_dump > 0</em>: number of iterations after which the analyzer dumps information about the content of the sets <em>C</em> and <em>E</em>.
- <em>noises (divided by -)</em>: list of used noises to compute the resilience. Each value of noise must be separated from the others using the character -. The specified value n of the noise corresponds to a noise interval with -n and n as extreme values. An example is 0.05-0.07-0.1.
- <em>closed_interval_noise</em>: value 1 or 0 that specifies if the noise interval is open or close.
- <em>seed</em>: seed of the random function.

##### Example
The following is an input example:
```bash
./analyze ../exp/breast_cancer/models/rf/rf_bc_5_3.json ../exp/breast_cancer/atk/atk_bc_0.15_l.json 1 ../exp/breast_cancer/test_set_bc.json 400 20 100 0.05 1 7
```
In detail, the analyzer takes as input a random forest of size 5 with trees of depth 3, trained with the <em>breast_cancer</em> dataset.
The attacker can perturb at most one feature by adding a value in the interval [-0.15, 0.15]. The analyzer performs 400 iterations using 20 threads and dumping information about the content of the sets <em>C</em> and <em>E</em> every 100 iterations. Finally, we consider a noise of magnitude 0.05 and the noise interval is closed.

### Reproduce the experiments

In order to reproduce our experiments, in config_exp_paper.h set the CONFIG_EXPERIMENTS_PAPER flag to 1.

In the <em>exp_scripts</em> folder there are the bash scripts to execute the experiments to retrieve the data for each Table/Figure presented in the paper. 

Before executing the scripts in <em>exp_scripts</em> folder, you must execute the bash scripts:
- <em>gen_analyzer_atk.sh</em>
- <em>train_random_forest.sh</em>
- <em>train_treant.sh</em>

These scripts generate the test sets, the random forests, the treant models, and the attacker files used in our experimental evaluation.

To perform the experiments on the synthetic datasets, you must execute the bash scripts:
- <em>gen_synthetic_data.sh</em>

## Credit

if you use this implementation in your work, please add a reference/citation to our paper. You can use the following BibTeX entry:

```bash
@article{CalzavaraCLMO22,
  author    = {Stefano Calzavara and
               Lorenzo Cazzaro and
               Claudio Lucchese and
               Federico Marcuzzi and
               Salvatore Orlando},
  title     = {Beyond robustness: Resilience verification of tree-based classifiers},
  journal   = {Comput. Secur.},
  volume    = {121},
  pages     = {102843},
  year      = {2022},
  url       = {https://doi.org/10.1016/j.cose.2022.102843},
  doi       = {10.1016/j.cose.2022.102843}
}
```

## Support

If you want to ask questions about the code and how to use it, feel free to contact us by sending an email to lorenzo.cazzaro@unive.it or federico.marcuzzi@unive.it.
