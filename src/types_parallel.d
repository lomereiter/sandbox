import std.stdio;
import std.algorithm;
import std.array;
import std.parallelism;
import core.sync.mutex;

void main(string[] args) {
    __gshared uint[string] occurrences;
      
    Mutex m = new Mutex();

    auto fin = File("../data/Cucumber_v2i.gff3");
  
    foreach(line; taskPool.parallel(fin.byLine())) {
        if (line.length == 0 || line[0] == '#') {
            continue;
        }
        auto type = array(splitter(line.dup, '\t'))[2].idup;
        synchronized(m) { // how the hell to synchronize the access?????
            occurrences[type] += 1;
        }
    }

    foreach(type; occurrences.keys) {
        writefln("%s: %s", type, occurrences[type]);
    }
}
