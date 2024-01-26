#include "data_independent.h"
#include <fstream>

vector<bool> merge_vect(vector<bool> v1, vector<bool> v2) {
    vector<bool> out;
    for (size_t i = 0; i < v1.size(); ++i)
        out.push_back(v1[i] * v2[i]);

    return out;
}

size_t counts_true(vector<bool> v) {
    size_t out = 0;
    for (auto b : v)
        if (b)
            out++;

    return out;
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cout << "./executable <ensemble_filename.json> <attacker_filename.json> <attacker_budget >= 0> <test_set_path> <test_set_filename.json> <noise>" << std::endl;
        return -1;
    }

    size_t b = 0;
    sscanf(argv[3], "%zu", &b);

    std::cout << "LOAD ENSEMBLE: " << argv[1] << std::endl;
    Ensemble* T = load_model(argv[1], true);

    std::cout << "LOAD ATTACKER: " << argv[2] << std::endl;
    Attacker A = Attacker(argv[2], b, T->get_f_idx());
    std::cout << "Attacker budget: " << A.get_budget() << std::endl;
    std::cout << "Attacker atk vector: ";
    A.get_atk().print();
    std::cout << "Attacker cost vector: ";
    print_ist(A.get_cost());

    char* token = strtok(argv[1], "/");
	char* prev_token;

    while(token != NULL){
        prev_token = token;
        token = strtok(NULL, "/");
    }

    vector<string> tokens;
    token = strtok(prev_token, "_");

    while(token != NULL){
        tokens.push_back(token);
        token = strtok(NULL, "_");
    }

    string last_token = tokens[tokens.size()-1];
    tokens[tokens.size()-1] = last_token.substr(0, last_token.find("."));

    string model = tokens[0];
    string dataset = tokens[1];
    string n_estimators = tokens[2];
    string depth = tokens[3];

	string noise = argv[6];

    std::cout << "LOAD DATASET: " << argv[4] << std::endl;
    std::vector<std::vector<float>> test_set;
    std::vector<float> test_labels;
    string dataset_path = string(argv[4]);
    string dataset_base_name = string(argv[5]);
    string dataset_orig_name = dataset_path + dataset_base_name + ".json";

    cout << "LOAD TEST SET: " << test_labels.size() << endl;
    load_dataset(dataset_orig_name, test_set, test_labels);
    test_set = prune_dataset(test_set, T->get_f_idx());

    cout << "TEST SET DIM: " << test_labels.size() << endl;

    char* buffer = new char[20000];

    sprintf(buffer, "Dataset: %s\tTest_set: %s\tModel: %s\tDepth: %s\tn_estimators: %s\t", dataset.c_str(), argv[4], model.c_str(), depth.c_str(), n_estimators.c_str());
    cout << buffer << endl;

	sprintf(buffer, "Accuracy: %f\n", T->score(test_set, test_labels));
    cout << buffer << endl;

    vector<bool> robustness_x_instance(test_set.size(), true);
    sprintf(buffer, "Robustness from 0 to 100 synthetic datasets:\n[%f-, ", true_robustness(*T, A, test_set, test_labels, A.get_budget(), robustness_x_instance));
    cout << buffer << endl;

    vector<vector<bool>> matt;
    for (size_t i = 0; i < 100; i++)
        matt.push_back(robustness_x_instance);

    for(size_t i = 0; i < 100; i++) {
        string dataset_orig_name = dataset_path + "data_synth/" + dataset_base_name + "_" + noise + "_" + to_string(i) + ".json";
        load_dataset(dataset_orig_name, test_set, test_labels);
        test_set = prune_dataset(test_set, T->get_f_idx());
        true_robustness(*T, A, test_set, test_labels, A.get_budget(), matt[i]);
    }

    vector<bool> final = robustness_x_instance;
    vector<bool> out_best;
    size_t min_true = 0, remove_idx = 0;

    for(size_t i = 0; i < 100; ++i) {
        remove_idx = 0;
        for(size_t j = 0; j < matt.size(); ++j) {
            vector<bool> out = merge_vect(final, matt[j]);
            size_t val = counts_true(out);
            if (val > min_true) {
                min_true = val;
                remove_idx = j;
            }
        }

        final = merge_vect(final, matt[remove_idx]);
        matt.erase(matt.begin() + remove_idx);

        float rob_instances = 0;
        for(auto iter = final.begin(); iter != final.end(); iter++)
            rob_instances += (*iter) ? 1 : 0;

        sprintf(buffer, "%f, ", rob_instances/test_set.size());
        cout << buffer << endl;
    }

    std::cout << "Done!" << std::endl;
}
