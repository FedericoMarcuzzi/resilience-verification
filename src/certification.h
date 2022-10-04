#include "hyper_interval.h"
#include "mytype.h"
#include "misc.h"

#include <vector>
#include <set>
#include <set>
#include <map>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <jsoncons/json.hpp>

namespace jc = jsoncons;

#define LEFT 1
#define RIGHT 0

class Attacker {

public:
    Attacker() {
    }


    Attacker(HyperRectangle& atk, V_f& c, size_t b) {
        this->atk = atk;
        this->c = c;
        this->b = b;
    }

    Attacker(std::string file_name, size_t b) {
        this->b = b;
        std::string s = read_file(file_name, "Attacker");
        std::vector<Interval> intv;
        auto loaded = jc::decode_json<VV_f>(s);
        
        for (V_f& v : loaded) {
            intv.emplace_back(v[0], v[1], v[2], v[3]);
            c.emplace_back(v[4]);
        }

        atk = HyperRectangle(intv);
    }

    Attacker(std::string file_name, size_t b, V_st f_idx) {
        this->b = b;
        std::string s = read_file(file_name, "Attacker");
        std::vector<Interval> intv;
        auto loaded = jc::decode_json<VV_f>(s);

        size_t i = 0, j = 0;
        for (V_f& v : loaded) {
            if (i == f_idx[j]) {
                intv.emplace_back(v[0], v[1], v[2], v[3]);
                c.emplace_back(v[4]);
                ++j;
            }
            if (j == f_idx.size())
                break;
            ++i;
        }

        atk = HyperRectangle(intv);
    }

    const HyperRectangle& get_atk() const {
        return atk;
    }

    const std::vector<float>& get_cost() const {
        return c;
    }

    size_t get_budget() {
        return b;
    }

    size_t size() {
        return c.size();
    }

    const Interval& operator[](const size_t i) {
        return atk[i];
    }

    const size_t get_dim() const {
        return atk.get_dim();
    }

private:
    HyperRectangle atk;
    std::vector<float> c;
    size_t b;
};


class Counter {

public:
    Counter(size_t d=0) {
        this->d = d;
        state = (0==d);
    }

    bool add(float l) {
        v.insert(l);
        state = (v.size()==d);
        return state;
    }

    const std::set<float>& get_set() const {
        return v;
    }

    size_t size() {
        return v.size();
    }

    void print() {
        for (float l : v)
            std::cout << "  " << l;
        std::cout << std::endl;
    }

private:
    std::set<float> v;
    size_t d;
    bool state;
};

bool operator==(const Counter &l, const Counter &r) {
    return l.get_set() == r.get_set();
}


class SymbolicAttack {

    public:
        SymbolicAttack() {
            dim = 0;
            paid = 0;
            HyperRectangle pre = HyperRectangle();
            HyperRectangle post = HyperRectangle();
            atk_f = std::vector<float>(0);
            split_counter = 0;
        }

        SymbolicAttack(size_t dim, bool empty=false) {
            this->dim = dim;
            paid = 0;
            pre = HyperRectangle(dim, empty);
            post = HyperRectangle(dim, empty);
            atk_f = std::vector<float>(dim, 0);
            split_counter = 0;
        }

        SymbolicAttack(const HyperRectangle& pre, const  HyperRectangle& post, size_t k, std::vector<float> atk_f) {
            dim = pre.get_dim();
            if (dim != post.get_dim())
                throw std::runtime_error("pre and post images must have the same dimension!!!");
            paid = k;
            this->pre = pre;
            this->post = post;
            this->atk_f = atk_f;
            this->split_counter = 0;
        }

        void split_increment(){
            this->split_counter++;
        }

        size_t get_split_counter() const{
            return split_counter;
        }

        bool is_empty() const {
            return (pre.is_empty() || post.is_empty());
        }

        const HyperRectangle& get_pre() const {
            return pre;
        }

        const HyperRectangle& get_post() const {
            return post;
        }

        HyperRectangle& get_pre_noconst() {
            return pre;
        }

        HyperRectangle& get_post_noconst() {
            return post;
        }

        size_t get_paid() const {
            return paid;
        }

        size_t get_dim() const {
            return dim;
        }

        std::vector<float> get_atk_f() const {
            return atk_f;
        }

        void _print(const HyperRectangle& h) const {
            Interval base = Interval();
            size_t idx_f = 0;
            bool flag = false;
            for (const Interval& i : h.get_intvs_ref()) {
                if (i != base) {
                    if (flag)
                        std::cout << ", ";
                    flag = true;
                    i.print(false);
                    std::cout << " f" << idx_f;
                }
                idx_f++;
            }
        }

        void print(bool el=true) const {
            std::cout << "{ ";
            _print(pre);
            std::cout << " -> ";
            _print(post);
            std::cout << " } " << paid;

            if (el)
                std::cout << std::endl;
        }

    private:
        HyperRectangle pre, post;
        size_t paid, dim;
        std::vector<float> atk_f;
        size_t split_counter;
};

/* hash function for set */
namespace std
{
    template<> struct hash<Interval>
    {
        size_t operator() (const Interval &i) const {
            return std::hash<size_t>()(i.get_left()) ^ std::hash<size_t>()(i.get_left()) ^ std::hash<size_t>()(i.get_left()) ^ std::hash<size_t>()(i.get_left());
        }
    };

    template<> struct hash<HyperRectangle>
    {
        size_t operator() (const HyperRectangle &s) const {
            size_t hash_output = 0;
            for (const Interval& i : s.get_intvs_ref())
                hash_output = hash_output ^ std::hash<Interval>()(i);
            return hash_output;
        }
    };

    template<> struct hash<SymbolicAttack>
    {
        size_t operator() (const SymbolicAttack &s) const {
            const HyperRectangle& pre = s.get_pre();
            const HyperRectangle& post = s.get_post();

            return std::hash<HyperRectangle>()(pre) ^ std::hash<HyperRectangle>()(post);
        }
    };
}

bool operator==(const SymbolicAttack& l, const SymbolicAttack& r) {
    return (l.get_pre() == r.get_pre() && l.get_post() == r.get_post());
}

bool operator<(const SymbolicAttack& l, const SymbolicAttack& r) {
    if (l.get_pre() < r.get_pre())
        return true;
    
    if (r.get_pre() < l.get_pre())
        return false;
    
    if (l.get_post() < r.get_post())
        return true;
    
    return false;
}

std::set<SymbolicAttack>* refine_for(std::set<SymbolicAttack>* S, size_t f, float v, Attacker& A, bool type) {
    std::set<SymbolicAttack>* S_out = new std::set<SymbolicAttack>;
    float atk_left = A[f].get_left();
    float atk_right = A[f].get_right();
    float zero = 0;
    size_t cf = A.get_cost()[f];
    size_t b = A.get_budget();

    for (const SymbolicAttack& s : *S) {
        size_t k = s.get_paid();
        const std::vector<Interval>& pre = s.get_pre().get_intvs();
        const std::vector<Interval>& post = s.get_post().get_intvs();

        Interval na_post_f = post[f].intersection( (type) ? Interval(-INF, v, 1, 0) : Interval(v, INF, 1, 1) );

        if (!na_post_f.is_empty()) {
            std::vector<Interval> na_pre = pre;
            std::vector<Interval> na_post = post;
            na_post[f] = na_post_f;  

            if (pre[f]==post[f])
                na_pre[f] = na_post_f;
            else
                na_pre[f] = pre[f].intersection( (type) ? Interval(-INF, v - std::min(zero, atk_left), 1, 0) : Interval(v - std::max(zero, atk_right), INF, 1, 1) );
        
            S_out->emplace(HyperRectangle(na_pre), HyperRectangle(na_post), k, s.get_atk_f());
        }

        bool check = (type)? atk_left < 0 : atk_right > 0;
        if (check && k+cf<=b && pre[f]==post[f]) {
            Interval a_pre_f = pre[f].intersection( (type) ? Interval(v, v - atk_left, 1, 0) : Interval(v - atk_right, v, 1, 0) );
            if(!a_pre_f.is_empty()) {
                std::vector<Interval> a_pre = pre;
                std::vector<Interval> a_post = post;

                a_pre[f] = a_pre_f;
                a_post[f] = post[f].intersection( (type) ? Interval(v + atk_left, v, 1, 0) : Interval(v, v + atk_right, 1, 0) );

                std::vector<float> atk_f_new = s.get_atk_f();
                atk_f_new[f] = (type) ? atk_left : atk_right;
                S_out->emplace(HyperRectangle(a_pre), HyperRectangle(a_post), k+cf, atk_f_new);
            }
        }
    }
    return S_out;
}



class Leaf;

class Tree {

public:
    virtual std::set<SymbolicAttack>* get_sym() { throw std::runtime_error("Tree: get_sym"); return nullptr; }
    
    virtual bool predict(HyperRectangle& hr, Counter& c) { throw std::runtime_error("Tree: predict"); return false; }

    virtual float predict(V_f& ist) { throw std::runtime_error("Tree: predict"); return 0.0; }

    virtual void aux_annotate(std::set<SymbolicAttack>* S, Attacker& A) { throw std::runtime_error("Tree: aux_annotate"); }

    virtual void get_leaves(std::vector<Leaf*>& leaves) { throw std::runtime_error("Tree: get_leaves"); }

    virtual void fill_ths(VS_f& ths) { }

    virtual bool fill_leaf_ths(VS_f& ths) { return true; }

    /* TODO come arriva il 2 qui? L'ensemble lo ha ma non il Tree */
    virtual Counter operator()(const HyperRectangle& hr, size_t n_labels) {
        Counter c = Counter(n_labels);
        this->predict(const_cast<HyperRectangle&>(hr), c);
        return c;
    }

    virtual void annotate(Attacker& A) {
        std::set<SymbolicAttack>* S = new std::set<SymbolicAttack>;
        S->emplace(SymbolicAttack(A.get_dim()));
        aux_annotate(S, A);
    }

    virtual void print(size_t depth=0) { throw std::runtime_error("Tree: print"); }

private:
    std::set<SymbolicAttack> *sym = nullptr;
};



class Node : public Tree {

public:
    Node(size_t f, float v, Tree* tl, Tree* tr):
        f(f), v(v), tl(tl), tr(tr) {}

    /*~Node() {
        if (tr != nullptr)
            delete tr;
        if (tl != nullptr)
            delete tl;
    }*/

    virtual bool predict(HyperRectangle& hr, Counter& c) {
        bool check = false;
        if (hr[f].get_left() < v || (hr[f].get_left_type() == CLOSE && hr[f].get_left() <= v))
            check = tl->predict(hr, c);

        if (!check) {
            if (v < hr[f].get_right())
                check = tr->predict(hr, c);
        }

        return check;    
    }

    virtual float predict(V_f& ist) {
        return (ist[f] <= v) ? tl->predict(ist) : tr->predict(ist);
    }

    virtual void aux_annotate(std::set<SymbolicAttack>* S, Attacker& A) {
        std::set<SymbolicAttack>* Sl = refine_for(S, f, v, A, LEFT);
        std::set<SymbolicAttack>* Sr = refine_for(S, f, v, A, RIGHT);
        tl->aux_annotate(Sl, A);
        tr->aux_annotate(Sr, A);
        S->clear();
    }

    virtual void get_leaves(std::vector<Leaf*>& leaves) {
        tl->get_leaves(leaves);
        tr->get_leaves(leaves);
    }

    virtual void print(size_t depth=0) {
        for (size_t i = 0; i < depth; ++i)
            std::cout << "\t";

        std::cout << "threshold: " << v << " feature: " << f << std::endl;

        for (size_t i = 0; i < depth; ++i)
            std::cout << "\t";
        std::cout << "LEFT" << std::endl;

        tl->print(depth + 1);

        for (size_t i = 0; i < depth; ++i)
            std::cout << "\t";
        std::cout << "RIGHT" << std::endl;

        tr->print(depth + 1);
    }

    virtual void fill_ths(VS_f& ths) {
        ths[f].insert(v);
        tl->fill_ths(ths);
        tr->fill_ths(ths);
    }

    virtual bool fill_leaf_ths(VS_f& ths) {
        bool left = tl->fill_leaf_ths(ths);
        bool right = tl->fill_leaf_ths(ths);
        if (left || right)
            ths[f].insert(v);
        return false;
    }

private:
    size_t f = 0;
    float v = 0;
    Tree* tl = nullptr;
    Tree* tr = nullptr;
};



class Leaf : public Tree {

public:
    Leaf(float val):
        value(val) {}

    /*~Leaf() {
        if (sym != nullptr)
            delete sym;
    }*/

    virtual float get_label() {
        return value;
    }

    virtual std::set<SymbolicAttack>* get_sym() {
        return sym;
    }

    virtual void aux_annotate(std::set<SymbolicAttack>* S, Attacker& A) {
        sym = S;
    }

    virtual bool predict(HyperRectangle& hr, Counter& c) {
        return c.add(value);
    }

    virtual float predict(V_f& ist) {
        return value;
    }

    virtual void get_leaves(std::vector<Leaf*>& leaves) {
        leaves.push_back(this);
    }

    virtual void print(size_t depth=0) {
        for (size_t i = 0; i < depth; ++i)
            std::cout << "\t";
        std::cout << "label: " << value << std::endl;
    }

private:
    float value = 0;
    std::set<SymbolicAttack> *sym = nullptr;
};

bool check_inter(HyperRectangle& N_data, const SymbolicAttack& u) {
    if(!N_data.intersection(u.get_pre()).is_empty())
        return false;
    return true;
}

template<typename type_comp_less>
bool check_inter(HyperRectangle& N_data, std::set<SymbolicAttack, type_comp_less>& U) {
    for (const SymbolicAttack& u : U)
        if(!check_inter(N_data, u))
            return false;
    return true;
}

class Ensemble;
size_t validate_s2(const SymbolicAttack& s, Ensemble& T, std::vector<Tree*>* uT, float* l);

bool check_in2(std::vector<float>& ist, const SymbolicAttack& u, Ensemble& T, float l) {
    if(ist > u.get_pre()) {
        float y;
        std::vector<Tree*>* uT=std::nullptr_t();
        size_t type = validate_s2(u, T, uT, &y);
        if (type==2) {
            if (y!=l)   
                return true;
        }
        else
            return true;
    }
    return false;
}

template<typename type_comp_less>
bool check_in2(std::vector<float>& ist, std::set<SymbolicAttack, type_comp_less>& U, Ensemble& T, float l) {
    for (const SymbolicAttack& u : U)
        if(check_in2(ist, u, T, l))
            return true;
    return false;
}

class Ensemble {

public:
    Ensemble() {}

    Ensemble(std::vector<Tree*>* tl, size_t d, V_f ls, V_st f_idx_v) {
        tree_list = tl;
        labels = ls;
        n_tree = tl->size();
        dim = d;
        n_labels = ls.size();
        f_idx = f_idx_v;

        ths = VS_f(dim);
        for (Tree* t : *tree_list)
            t->fill_ths(ths);

        ths_occ = VM_fst(dim);
            
        for (Tree* t : *tree_list) {
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
    }

    /*~Ensemble() {
        for (Tree* t : *tree_list)
            if (t != nullptr)
                delete t;
    }*/

    V_st get_f_idx() {
        return f_idx;
    }

    void print() {
        for (size_t i = 0; i < (*tree_list).size(); ++i) {
            std::cout << "Tree: " << i << std::endl;
            (*tree_list)[i]->print();
            std::cout << std::endl;
        }
    }

    float predict(V_f ist) {
        std::map<float, size_t> counter;
        for (float l : labels)
            counter[l] = 0;

        for (Tree* t : *tree_list)
            counter[t->predict(ist)]++;
        
        return majority_voting(counter);
    }

    bool score_ensemble(const HyperRectangle& x, float y) {
        float label;
        bool check = predict_ensemble(x, label);
        return (check && y==label);
    }

    /* serve nella validation per verificare se una foresta concorda sulla stessa label */
    bool predict_ensemble(const HyperRectangle& x, float& y) {
        std::map<float, size_t> counter;
        for (float l : this->get_labels())
            counter[l] = 0;

        Counter count;
        for (Tree* t : *tree_list) {
            count = (*t)(x, this->n_labels);

            if (count.size() == 1) {
                auto first = count.get_set().begin();
                counter[*first]++;
            }
        }

        size_t agg = max_map(counter, &y);
        return (agg > int(this->size() / 2));
    }

    float score(VV_f& data, V_f& labels) {
        float count = 0;
        for (size_t i = 0; i < data.size(); ++i) {
            float pred = this->predict(data[i]);
            if (pred == labels[i])
                count++;
        }

        return count / data.size();
    }

    V_f& get_labels() {
        return labels;
    }

    size_t get_n_labels() {
        return n_labels;
    }

    std::vector<Tree*>& get_trees() {
        return (*tree_list);
    }

    size_t size() {
        return n_tree;
    }

    size_t get_dim() {
        return dim;
    }

    VS_f get_ths() {
        return ths;
    }

    VM_fst get_ths_occ() {
        return ths_occ;
    }

private:
    size_t n_tree;
    size_t dim;
    size_t n_labels;
    V_f labels;
    std::vector<Tree*>* tree_list;
    std::vector<std::vector<Leaf*>> leave_list;
    VS_f ths;
    VM_fst ths_occ;
    V_st f_idx;
};
