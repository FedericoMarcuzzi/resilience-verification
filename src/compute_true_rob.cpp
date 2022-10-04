#include "data_independent.h"
#include <fstream>

int main(int argc, char* argv[]) {
       
    /* check input parameters */
    if (argc != 7) {
        std::cout << "./executable <ensemble_filename.json> <attacker_filename.json> <attacker_budget >= 0> <test_set_filename.json> <filename_log.txt> <seed>" << std::endl;
        return -1;
    }

    cout << "COMPUTE TRUE ROBUSTNESS" << endl;

    /* budget */
    size_t b = 0;
    sscanf(argv[3], "%zu", &b);

	size_t seed;
    sscanf(argv[6], "%zu", &seed);
    srand(seed);
    
    std::cout << "LOAD ENSEMBLE: " << argv[1] << std::endl;
    Ensemble* T = load_model(argv[1]);

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
    string seed_gen = "7";

    std::ofstream ofs; 
    ofs.open(argv[5], std::ifstream::app);
    if(ofs.fail()){
        ofs.open(argv[5], std::ifstream::in);
    }

    std::cout << "LOAD ATTACKER: " << argv[2] << std::endl;
    Attacker A = Attacker(argv[2], b, T->get_f_idx());
    std::cout << "Attacker budget: " << A.get_budget() << std::endl;
    std::cout << "Attacker atk vector: ";
    A.get_atk().print();
    std::cout << "Attacker cost vector: ";
    print_ist(A.get_cost());

    std::cout << "LOAD DATASET: " << argv[4] << std::endl;
    std::vector<std::vector<float>> test_set;
    std::vector<float> test_labels;
    string dataset_path = string(argv[4]);
    load_dataset(dataset_path, test_set, test_labels);
    test_set = prune_dataset(test_set, T->get_f_idx());

    char* buffer = new char[200];

	sprintf(buffer, "Accuracy: %f\n", T->score(test_set, test_labels));
    cout << buffer << endl;
    ofs.write(buffer, strlen(buffer));
	
    sprintf(buffer, "True robustness: %f\n\n", true_robustness(*T, A, test_set, test_labels, A.get_budget()));
    cout << buffer << endl;
    ofs.write(buffer, strlen(buffer));

    ofs.close();
}
