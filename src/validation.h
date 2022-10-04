#include "certification.h"

#include <vector>
#include <set>
#include <set>
#include <map>
#include <algorithm>
#include <random>
#include <cstddef>

/* verifica che la maggioranze della foresta concorda su u*/
bool validate_u(const SymbolicAttack& u, Ensemble& T, std::vector<Tree*>* uT=std::nullptr_t(), bool early_stopping=false) {
    std::map<float, size_t> counter;
    Counter count_pre, count_post;
    size_t count_err = 0;
    size_t e_size = T.size();

    /* initializes a counter map with labels as key and 0 as vlaue. */
    for (float l : T.get_labels())
        counter[l] = 0;

    /* for each tree inside the ensemble computes its agreement wrt pre and post images. */
    for (Tree* t : T.get_trees()) {
        bool robust = false;
        count_post = (*t)(u.get_post(), T.get_n_labels());

        if (count_post.size() == 1) {
            count_pre = (*t)(u.get_pre(), T.get_n_labels());
            robust = (count_pre.size() == 1 && (count_pre == count_post));
        }

        /* if the tree is robust wrt u, it increments the map counter. */
        if (robust) {
            auto first = count_pre.get_set().begin();
            counter[*first]++;

            /* if the majority of the forest agree on the label return true (robust). */
            bool res = (max_map(counter) > int(e_size / 2));
            if (res)
                return true;
            
        } else {
            count_err++;
            /* adds undecided tree */
            if (uT!=std::nullptr_t())
                uT->push_back(t);
        }

        /* if the majority of the forest are undecided return false (unsafe). */
        if (early_stopping && count_err > int(e_size / 2))
            return false;
    }

    /* if the majority of the forest agree on the label return true (robust), false otherwise. */
    return (max_map(counter) > int(e_size / 2));
}

/* checks type of a SA `u` passed as argument. Returns throught `uT` a vector of undecided tree of `u`. */
size_t validate_s2(const SymbolicAttack& u, Ensemble& T, std::vector<Tree*>* uT=std::nullptr_t(), float* l=nullptr) {
    float pre_label = 0;
    float post_label = 0;

    bool pre_check = T.predict_ensemble(u.get_pre(), pre_label);
    bool post_check = T.predict_ensemble(u.get_post(), post_label);
    size_t type = 3;

    /* pre e post hanno una sola label ed è la stessa (SAFE) */
    if (pre_check==true && post_check==true)
        /* TODO nel caso di multi labels (> 2) occorre controllare se una delle pre è quella della post ANCHE QUI */
        return (pre_label == post_label) ? 0 : 1; /* pre e post hanno una sola label ma diversa. Questo SA non può essere raffinato ulteriormente (ESAUSTO) */

    /* pre con più label, post con una sola TODO nel caso di multi labels (> 2) occorre controllare se una delle pre è quella della post */
    if (pre_check==false && post_check==true) {
        if (l!=nullptr)
            *l = post_label;
        type = 2;
    }
    
    /* finds undecided */
    validate_u(u, T, uT);
    return type;
}

/* TRUE ROBUSTNESS */
bool atk_rec(Ensemble& T, Attacker& A, VS_f& ths, V_f& x, float y, size_t f, size_t b) {
    if (b <= 0 || f >= A.get_dim()) {
        return (T.predict(x) == y);
    }
    
    bool out = atk_rec(T, A, ths, x, y, f + 1, b);
    if (out == false)
        return false;

    float orig = x[f];
    float c = A.get_cost()[f];
    /* bisogna aprire l'estremo perché se orig + atk = v allora non deve saltare */
    Interval intv(A[f].get_left(), A[f].get_right(), 0, 1);
    for (float v : ths[f]) {
        if (v > (orig + intv)) {
            float val = (orig <= v)? std::nextafterf(v, INF) : v;
            x[f] = val;
            out = atk_rec(T, A, ths, x, y, f + 1, b - c);
            x[f] = orig;
            if (!out)
                return false;
        }
    }

    x[f] = orig;
    return true;
}

float atk(Ensemble& T, Attacker& A, VS_f& ths, VV_f& X, V_f& Y, size_t b) {
    float count = 0;
    for (size_t i = 0; i < Y.size(); ++i) {
        if(T.predict(X[i]) == Y[i] && atk_rec(T, A, ths, X[i], Y[i], 0, b))
            count++;
    }
    return count / X.size();
}

float atk(Ensemble& T, Attacker& A, VS_f& ths, VV_f& X, V_f& Y, size_t b, vector<bool>& rob_instances_index) {
    float count = 0;
    for (size_t i = 0; i < Y.size(); ++i) {
        if(T.predict(X[i]) == Y[i] && atk_rec(T, A, ths, X[i], Y[i], 0, b)) {
            count++;
        }
        else
            rob_instances_index[i] = false;
    }
    return count / X.size();
}

float true_robustness(Ensemble& T, Attacker& A, VV_f& X, V_f& Y, size_t b) {
    VS_f ths = T.get_ths();
    return atk(T, A, ths, X, Y, b);
}

float true_robustness(Ensemble& T, Attacker& A, VV_f& X, V_f& Y, size_t b, vector<bool>& rob_instances_index) {
    VS_f ths = T.get_ths();
    return atk(T, A, ths, X, Y, b, rob_instances_index);
}

float robustness(VV_f& data, V_f& labels, std::set<SymbolicAttack>& U, Ensemble &T) {
    float count = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        float pred = T.predict(data[i]);
        if (pred == labels[i]) {
            bool robust = !check_in2(data[i], U, T, labels[i]);
            if (robust)
                count++;
        }
    }

    return count / data.size();
}

float resilience(VV_f& data, V_f& labels, std::set<SymbolicAttack>& U, HyperRectangle& N, Ensemble &T) {
    float count = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        float pred = T.predict(data[i]);
        if (pred == labels[i]) {
            HyperRectangle N_data = data[i] + N;
            bool resilient = check_inter(N_data, U);
            if (resilient)
                count++;
        }
    }

    return count / data.size();
}

void fast_statistics(VV_f& data, V_f& labels, Attacker& A, std::set<SymbolicAttack>& U, HyperRectangle& N, Ensemble &T, pair<float, float> res) {
    float robust_count = 0;
    float resilient_count = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        float pred = T.predict(data[i]);
        if (pred == labels[i]) {
            /* check robustness */
            bool robust = !check_in2(data[i], U, T, labels[i]);
            if (robust) {
                robust_count++;

                /* check resilience */
                HyperRectangle N_data = data[i] + N;
                bool resilient = check_inter(N_data, U);
                if (resilient)
                    resilient_count++;
            }
        }
    }

    res.first = robust_count/(float)data.size();
    res.second = resilient_count/(float)data.size();
}