#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <cstdlib>

#include <memory>

#include <tbb/tbb.h>

#include "splitter.hpp"
#include "timer.hpp"

using namespace std;

typedef vector<string> Chunk;
typedef std::shared_ptr<Chunk> ChunkPtr;

int main(int argc, const char** argv) {

    tbb::task_scheduler_init(3);

    ifstream fin("../data/Cucumber_v2i.gff3");
    
    tbb::concurrent_unordered_map<string, unsigned int> occurrences;
    
    const size_t CHUNK_SIZE = argc == 1 ? 100 : atoi(argv[1]);

timeit([&](){
    tbb::parallel_pipeline(
            5,
            tbb::make_filter<void, ChunkPtr>(
                tbb::filter::serial_in_order,
                [&](tbb::flow_control& fc) -> ChunkPtr {
                    auto chunk = std::make_shared<Chunk>();
                    chunk->reserve(CHUNK_SIZE);
                    for (size_t pushed = 0; pushed < CHUNK_SIZE && fin.good(); ) {
                        string s;
                        getline(fin, s);
                        if (!s.empty() && s[0] != '#') {
                            ++pushed;
                            chunk->push_back(s);
                        }
                    }
                    if (chunk->size() == 0) {
                        fc.stop();
                        return nullptr;
                    }
                    return std::move(chunk);
                }) 
        &
            tbb::make_filter<ChunkPtr, ChunkPtr>(
                tbb::filter::parallel,
                [&](ChunkPtr lines) -> ChunkPtr { // in-place
                    for (auto& line : *lines) {
                        StringSplitter<'\t'> splitter(line);
                        assert(splitter.length >= 3);
                        line = splitter[2]; 
                    }
                    return std::move(lines);
                }) 
        &
            tbb::make_filter<ChunkPtr, void>(
                tbb::filter::parallel,
                [&](ChunkPtr types) {
                    for (const auto& type : *types) {
                        occurrences[type] += 1;
                    }
                }));
}, "total");

    for (const auto& type : occurrences) {
        cout << type.first << ": " << type.second << endl;
    }

    return 0;
}
