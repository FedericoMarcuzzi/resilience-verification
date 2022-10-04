#include "data_independent.h"
#include <fstream>

int main(int argc, char* argv[]) {

    bool flag_print = false;
       
    /* check input parameters */
    if (argc != 8) {
        std::cout << "./executable <ensemble_filename.json> <attacker_filename.json> <attacker_budget >= 0> <noises (divided by -)> <test_set_path> <test_set_filename_base.json> <synthetics_folder>" << std::endl;
        return -1;
    }

    /* budget */
    size_t b = 0;
    sscanf(argv[3], "%zu", &b);
    
    std::cout << std::endl;

    std::cout << "LOAD ENSEMBLE: " << argv[1] << std::endl;
    Ensemble* T = load_model(argv[1], true);

    std::cout << "LOAD ATTACKER: " << argv[2] << std::endl;
    Attacker A = Attacker(argv[2], b, T->get_f_idx());
    std::cout << "Attacker budget: " << A.get_budget() << std::endl;
    std::cout << "Attacker atk vector: ";
    A.get_atk().print();
    std::cout << "Attacker cost vector: ";
    print_ist(A.get_cost());

    /*estrae dal nome del modello alcune info utili*/
    char* token = strtok(argv[1], "/");
	char* prev_token;

    while(token != NULL){ //rimuovi il path dal nome
        prev_token = token;
        token = strtok(NULL, "/");
    }

    vector<string> tokens;
    token = strtok(prev_token, "_");

    while(token != NULL){ //estrai i diversi campi
        tokens.push_back(token);
        token = strtok(NULL, "_");
    }

    string last_token = tokens[tokens.size()-1];
    tokens[tokens.size()-1] = last_token.substr(0, last_token.find("."));

    string model = tokens[0];
    string dataset = tokens[1];
    string n_estimators = tokens[2];
    string depth = tokens[3];

	vector<string> noise_perts;
    token = strtok(argv[4], "-");
    while(token != NULL){ //rimuovi il path dal nome
        noise_perts.push_back(token);
        token = strtok(NULL, "-");
    }

    std::cout << "LOAD DATASET: ";
    std::vector<std::vector<float>> test_set;
    std::vector<float> test_labels;
    string dataset_path = string(argv[5]);
    string dataset_base_name = string(argv[7]);
    string dataset_orig_name = dataset_path + dataset_base_name + ".json";
    cout << dataset_orig_name << endl;
    load_dataset(dataset_orig_name, test_set, test_labels);
    test_set = prune_dataset(test_set, T->get_f_idx());

    char* buffer = new char[200];
    vector<bool> robustness_x_instance(test_set.size(), true);

    cout << dataset << endl;
    cout << model << endl;
    sprintf(buffer, "Dataset: %s\tTest_set: %s\tModel: %s\tDepth: %s\tn_estimators: %s\t", dataset.c_str(), argv[4], model.c_str(), depth.c_str(), n_estimators.c_str());
    cout << buffer << endl;

	sprintf(buffer, "Accuracy: %f\n", T->score(test_set, test_labels));
    cout << buffer << endl;

    sprintf(buffer, "True robustness orig: %f\n", true_robustness(*T, A, test_set, test_labels, A.get_budget(), robustness_x_instance));
    cout << buffer << endl;

    string synthetics_folder = string(argv[6]);

	for(string noise : noise_perts){
		for(size_t i = 0; i < 100; i++) {
			string dataset_orig_name = dataset_path + synthetics_folder + dataset_base_name + "_" + noise + "_" + to_string(i) + ".json";
    		load_dataset(dataset_orig_name, test_set, test_labels);
            test_set = prune_dataset(test_set, T->get_f_idx());
			true_robustness(*T, A, test_set, test_labels, A.get_budget(), robustness_x_instance);
		}
		float rob_instances = 0;
		for(auto iter = robustness_x_instance.begin(); iter != robustness_x_instance.end(); iter++)
			rob_instances += (*iter) ? 1 : 0;
		sprintf(buffer, "Min robustness noise %s: %f\n", noise.c_str(), rob_instances/test_set.size());
    	cout << buffer << endl;
	}
}
