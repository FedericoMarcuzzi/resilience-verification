Tree* build_tree_rec(std::tuple<V_st, V_f, V_st, V_st, V_f> v, size_t p, std::map<size_t, size_t> f_map) {
    /* è una foglia */
    if (std::get<0>(v)[p] == 0)
        return new Leaf(std::get<4>(v)[p]);
    
    Tree* tl = build_tree_rec(v, std::get<2>(v)[p] - 1, f_map);
    Tree* tr = build_tree_rec(v, std::get<3>(v)[p] - 1, f_map);

    return new Node( f_map[std::get<0>(v)[p] - 1], std::get<1>(v)[p], tl, tr);
}


Ensemble* load_model(std::string file_name, bool smart=false) {
    std::string s = read_file(file_name, "Model");
    auto e = jc::decode_json<std::tuple<size_t, V_f, std::vector<std::tuple<V_st, V_f, V_st, V_st, V_f>>>>(s);

    std::vector<Tree*>* t = new std::vector<Tree*>;
    
    std::set<size_t> f_set;
    if (smart)
        for (auto &td : std::get<2>(e))
            for (size_t f : std::get<0>(td)) {
                if (f != 0)
                    f_set.insert(f - 1);
            }      
    else
        for (size_t i = 0; i < std::get<0>(e); ++i)
            f_set.insert(i);

    std::map<size_t, size_t> f_map;
    size_t i = 0;
    for (size_t f : f_set) {
        f_map.insert({f, i});
        ++i;
    }

    size_t dim = f_map.size();

    for (auto &td : std::get<2>(e))
        t->push_back(build_tree_rec(td, 0, f_map));

    V_st f_idx(f_set.begin(), f_set.end());

    return new Ensemble(t, dim, std::get<1>(e), f_idx);
}

void load_dataset(std::string file_name, VV_f& data, V_f& labels) {
    std::string s = read_file(file_name, "Dataset");
    auto val = jc::decode_json<std::tuple<V_f, VV_f>>(s);
    labels = std::get<0>(val);
    data = std::get<1>(val);
}

VV_f prune_dataset(VV_f& data, V_st f_idx) {
    size_t rows = data.size();
    size_t cols = data[0].size();
    size_t o_size = f_idx.size();
    VV_f output(rows);

    for (size_t i = 0, j = 0; i < cols; ++i) {
        if (i == f_idx[j]) {
            for (size_t k = 0; k < rows; ++k)
                output[k].push_back(data[k][i]);
            ++j;
        }
    }

    return output;
}