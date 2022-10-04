#include <cstdlib>
#include <algorithm>
#include "analyze_tree.h"
#define SYMATKCOMPARATOR priority_pairs_cut_ind
#define MIN_SYMATK_TO_BE_SPLITTED 10

void sa_from_intvs(const SymbolicAttack& u, std::set<SymbolicAttack>& U, Attacker& A, std::vector<Interval>& intvs, size_t f, bool mode) {
    const std::vector<Interval>& u_pre = u.get_pre().get_intvs_ref();
    const std::vector<Interval>& u_post = u.get_post().get_intvs_ref();

    SymbolicAttack s_u;
    for (Interval& i : intvs) {
        s_u = u;
        s_u.get_pre_noconst()[f] = i;

        if (mode) {
            if (u_pre[f] == u_post[f])
                s_u.get_post_noconst()[f] = i;
            else
                s_u.get_post_noconst()[f] = u_post[f].intersection(i + A[f]);
        } else
            s_u.get_post_noconst()[f] = u_post[f].intersection(i + A[f]);

        s_u.split_increment();

        if (!s_u.is_empty())
            U.insert(s_u);
    }
}

void split_interval_in_4(const Interval& i, std::vector<Interval>& intvs, float v, const Interval atk) {
    float atk_left = atk.get_left();
    float atk_right = atk.get_right();

    if (v > i) {
        if (v + atk_left > i) {
            intvs.emplace_back(i.get_left(), v + atk_left, i.get_left_type(), 0);
            intvs.emplace_back(v + atk_left, v, 1, 0);
        } else
            intvs.emplace_back(i.get_left(), v, i.get_left_type(), 0);

        if (v + atk_right > i) {
            intvs.emplace_back(v, v + atk_right, 1, 0);
            intvs.emplace_back(v + atk_right, i.get_right(), 1, i.get_right_type());    
        } else
            intvs.emplace_back(v, i.get_right(), 1, i.get_right_type());
    } else {
        if (v + atk_left > i) {
            intvs.emplace_back(v + atk_left, i.get_right(), 0, i.get_right_type());
            intvs.emplace_back(i.get_left(), v + atk_left, i.get_left_type(), 1);
        }

        if (v + atk_right > i) {
            intvs.emplace_back(i.get_left(), v + atk_right, i.get_left_type(), 0);
            intvs.emplace_back(v + atk_right, i.get_right(), 1, i.get_right_type());    
        }
    }
}


VM_fst get_ths_occ(std::vector<Tree*>& T, size_t dim) {
    VM_fst ths_occ = VM_fst(dim);
   
    for (Tree* t : T) {
        VS_f ths_p = VS_f(dim);
        t->fill_ths(ths_p);

        for (size_t f = 0; f < dim; ++f) {
            for (float v : ths_p[f]) {
                if (ths_occ[f].find(v) == ths_occ[f].end())
                    ths_occ[f][v] = 1;
                else
                    ths_occ[f][v]++;
            }   
        }
    }

    return ths_occ;
}

/* seleziona la threshold con l'occorrenza più alta tra gli laberi indecisi */
bool split_max_occ(const SymbolicAttack& u, std::vector<Tree*>& T, Attacker& A, size_t& out_f, float& out_v, double p = 0.7) {
    size_t dim = A.get_dim();
    const std::vector<Interval>& u_pre = u.get_pre().get_intvs_ref();

    /* cerca le threshold degli alberi indecisi */
    VM_fst ths = get_ths_occ(T, dim);

    size_t f_max = 0;
    float v_max = 0;
    size_t max_occ = 0;
    bool flag = false;
    for (size_t f = 0; f < dim; ++f) {
        Interval I = Interval(u_pre[f].get_left(), u_pre[f].get_right(), 1, 1);
        for (P_fst p : ths[f]) {
            if (p.second > max_occ) {
                float v = p.first;
                if ( v > I || (v + A[f].get_left()) > I || (v + A[f].get_right()) > I ) {
                    f_max = f;
                    v_max = p.first;
                    max_occ = p.second;
                    flag = true;
                }
            }
        }
    }

    out_f = f_max;
    out_v = v_max;
    return flag;
}

/* funzione utile se cambiamo split (da rimuovere a lavoro finito) */
void split_sa(const SymbolicAttack& u, std::set<SymbolicAttack>& U, std::vector<Tree*>& T, Attacker& A) {
    size_t f;
    float v;
    bool res = split_max_occ(u, T, A, f, v, 0);
    if (res) {
        const std::vector<Interval>& u_pre = u.get_pre().get_intvs_ref();
        std::vector<Interval> intvs;
        split_interval_in_4(u_pre[f], intvs, v, A[f]);
        sa_from_intvs(u, U, A, intvs, f, u.get_paid() == A.get_budget());
    }
}

Ensemble ens;

struct priority_pairs_cut_ind {
  bool operator() (const SymbolicAttack& a, const SymbolicAttack& b) {
    bool less = a < b;
    if (!less && !(b < a)) //for removing sym atk duplicates
        return a < b;
    else {
        size_t a_split_counter = a.get_split_counter(); 
        size_t b_split_counter = b.get_split_counter();
        if(a_split_counter != b_split_counter) 
            return a_split_counter < b_split_counter;
        else {
            std::vector<Tree*> uT1;
            validate_s2(a, ens, &uT1);
            std::vector<Tree*> uT2;
            validate_s2(b, ens, &uT2);
            size_t uTsizea = uT1.size();
            size_t uTsizeb = uT2.size();
            if (uTsizea != uTsizeb)
                return uTsizea < uTsizeb;
            else return less;
        }
    }
  }
};