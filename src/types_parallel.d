import std.stdio;
import std.algorithm;
import std.array;
import std.parallelism;

void main(string[] args) {
    uint[string] occurrences;

    auto fin = File("../data/Cucumber_v2i.gff3");
   
    foreach(line; taskPool.parallel(fin.byLine())) {
        if (line.length == 0 || line[0] == '#') {
            continue;
        }
        auto type = array(splitter(line, '\t'))[2].idup;
        occurrences[type] += 1;
    }

    foreach(type; occurrences.keys) {
        writefln("%s: %s", type, occurrences[type]);
    }
}
