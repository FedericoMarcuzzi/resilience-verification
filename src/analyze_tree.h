/* analyze tree (same as in the article) */
template <class ordering_criterion>
void analyze_tree(std::set<SymbolicAttack, ordering_criterion>& U, Tree &t, Attacker& A) {
    t.annotate(A);
    std::vector<Leaf*> leaves;
    t.get_leaves(leaves);

    float ext_lb, int_lb;

    for (Leaf* ext_lf : leaves) {
        ext_lb = ext_lf->get_label();
        for (const SymbolicAttack& ext_s : *ext_lf->get_sym()) {
            if (ext_s.get_paid() == 0) {
                for (Leaf* int_lf : leaves) {
                    int_lb = int_lf->get_label();
                    if (ext_lb != int_lb) {
                        for (const SymbolicAttack& int_s : *int_lf->get_sym()) {
                            if (int_s.get_paid() > 0) {
                                HyperRectangle inter_s_pre = ext_s.get_pre().intersection(int_s.get_pre());
                                if (!inter_s_pre.is_empty()) {
                                    HyperRectangle inter_s_post = int_s.get_post().intersection(inter_s_pre + A.get_atk());
                                    U.emplace(SymbolicAttack(inter_s_pre, inter_s_post, int_s.get_paid(), int_s.get_atk_f()));
                                }
                            }
                        }
                    }
                }
            }  
        }
    }
};