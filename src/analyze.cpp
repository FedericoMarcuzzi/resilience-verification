#include "data_independent.h"
#include "analyze_ensemble.h"

int main(int argc, char* argv[]) {

    bool flag_print = false;
       
    /* check input parameters */
    if (argc < 11) {
        std::cout << "./executable <ensemble_filename.json> <attacker_filename.json> <attacker_budget >= 0> <test_set_filename.json> <num iter > 0> <n_threads> <iterations_x_dump> <noises (divided by -)> <closed_interval_noise> <seed>" << std::endl;
        return -1;
    }

    cout << "###############" << endl;
    cout << "START" << endl;

    /* budget */
    size_t b = 0;
    sscanf(argv[3], "%zu", &b);

    /* num iterations */
    size_t iter;
    sscanf(argv[5], "%zu", &iter);

    size_t n_threads;
    sscanf(argv[6], "%zu", &n_threads);

    size_t iterations_x_dump;
    sscanf(argv[7], "%zu", &iterations_x_dump);

    vector<float> noise_perts;
    char* token;
    char* prev_token;
    token = strtok(argv[8], "-");
    float temporary_noise_pert;
    while(token != NULL){ //rimuovi il path dal nome
        sscanf(token, "%f", &temporary_noise_pert);
        noise_perts.push_back(temporary_noise_pert);
        token = strtok(NULL, "-");
    }

    size_t closed_interval_noise;
    sscanf(argv[9], "%zu", &closed_interval_noise);

    size_t seed;
    sscanf(argv[10], "%zu", &seed);
    
    srand(seed);
    
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

    std::cout << "LOAD DATASET: " << argv[4] << std::endl;
    std::vector<std::vector<float>> test_set;
    std::vector<float> test_labels;
    load_dataset(argv[4], test_set, test_labels);
    test_set = prune_dataset(test_set, T->get_f_idx());

    cout << "TEST SET DIM: " << test_labels.size() << endl;

    /*estrae dal nome del modello alcune info utili*/
    token = strtok(argv[1], "/");

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

    char* buffer = new char[2000];

    sprintf(buffer, "Dataset: %s\tTest_set: %s\tModel: %s\tDepth: %s\tN_estimators: %s\tIter: %zu\tBudget: %zu\tN_threads: %zu\tSeed_exp: %zu\n", dataset.c_str(), argv[4], model.c_str(), depth.c_str(), n_estimators.c_str(), iter, b, n_threads, seed);
    cout << buffer << endl;

    sprintf(buffer, "Accuracy: %f\n", T->score(test_set, test_labels));
    cout << buffer << endl;

    sprintf(buffer, "True robustness: %f\n", true_robustness(*T, A, test_set, test_labels, A.get_budget()));
    cout << buffer << endl;

    std::vector<HyperRectangle> noises;

    for(size_t i = 0; i < noise_perts.size(); i++)
        noises.emplace_back(A.get_dim(), noise_perts[i], closed_interval_noise);

    #if CONFIG_EXPERIMENTS_PAPER == 0

        set<SymbolicAttack> U;
        analyze_ensemble(*T, A, n_threads, iter, iterations_x_dump, U);

        sprintf(buffer, "END\n\n");

        sprintf(buffer, "Accuracy: %f\n", T->score(test_set, test_labels));
        cout << buffer << endl;

        sprintf(buffer, "True robustness: %f\n", true_robustness(*T, A, test_set, test_labels, A.get_budget()));
        cout << buffer << endl;

        sprintf(buffer, "Robustness: %f\n", robustness(test_set, test_labels, U, *T));
        cout << buffer << endl;

        for(size_t i = 0; i < noise_perts.size(); i++) {
            sprintf(buffer, "Resilience for noise %f: %f\n", noise_perts[i], resilience(test_set, test_labels, U, noises[i], *T));
            cout << buffer << endl;
        }
        cout << "###############" << endl;

    #else

        analyze_ensemble(*T, A, n_threads, iter, iterations_x_dump, test_set, test_labels, noises);

    #endif
}
