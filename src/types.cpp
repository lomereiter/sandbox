#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>

#ifdef TBB
#include <tbb/tbb.h>
#endif

#include "splitter.hpp"
#include "timer.hpp"

using namespace std;

int main() {

#ifdef TBB
    tbb::task_scheduler_init init;
#endif

    ifstream fin("../data/Cucumber_v2i.gff3");

    vector<string> lines;

    unordered_map<string, unsigned int> occurrences;

timeit([&](){

timeit([&](){
    while (fin.good()) {
        string s;
        getline(fin, s);
        if (s.empty() || s[0] == '#') { continue; }
        lines.push_back(std::move(s));
    }
}, "reading the file");

    vector<string> types(lines.size());

    auto store_type = [&](size_t i) {
        StringSplitter<'\t'> splitter(lines[i]);
        assert(splitter.length >= 3);
        types[i] = splitter[2]; 
        lines[i].clear();
    };

timeit([&](){
#ifdef TBB
    auto num_threads = tbb::task_scheduler_init::default_num_threads();

    tbb::parallel_for(tbb::blocked_range<size_t>(0, lines.size(), lines.size() / num_threads / 3),
                      [&](const tbb::blocked_range<size_t>& range) {
                          for (size_t i = range.begin(); i != range.end(); ++i) {
                              store_type(i);
                          }
                      });

#else
    size_t sz = lines.size();
    for (size_t i = 0; i < sz; ++i) {
        store_type(i);
    }
#endif
}, "getting types");

timeit([&](){
    for (const auto& type : types)
        occurrences[type] += 1;

}, "counting type occurrences");

}, "total");

    for (const auto& type : occurrences) {
        cout << type.first << ": " << type.second << endl;
    }
    return 0;
}
