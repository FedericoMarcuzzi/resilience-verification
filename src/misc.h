#include "mytype.h"

#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>

using namespace std;

/* load JSON file */
std::string read_file(std::string file_name, std::string source) {
    std::ifstream ifile;
    ifile.open(file_name);
    if(!ifile) {
        std::cout << source << " path/file <" << file_name << "> doesn't exist" << std::endl;
        exit(0);
    }

    std::string s((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
    return s;    
}

/* max value in a map */
size_t max_map(std::map<float, size_t>& map, float* label=nullptr, bool flag_print=false) {
    std::map<float, size_t>::iterator itr = map.begin();
    if (label != nullptr)
        *label = itr->first;
    size_t max = itr->second;

    for (itr = map.begin(); itr != map.end(); ++itr) {
        if (flag_print)
            std::cout << "key: " << itr->first << " value: " << itr->second << std::endl;
        if (max < itr->second) {
            max = itr->second;
            if (label != nullptr)
                *label = itr->first;
        }
    }
    if (flag_print)
        std::cout << "max: " << max << std::endl;
    return max;
}

/* major voted value in with a map */
float majority_voting(std::map<float, size_t>& map) {
    std::map<float, size_t>::iterator itr = map.begin();
    size_t max = itr->second;
    float label = itr->first;

    for (itr = map.begin(); itr != map.end(); ++itr)
        if (max < itr->second) {
            max = itr->second;
            label = itr->first;
        }

    return label;
}

/* print vector of float */
void print_ist(V_f v) {
    for (size_t i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";

    std::cout << std::endl;
}