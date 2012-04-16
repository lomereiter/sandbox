import std.stdio;
import std.algorithm;
import std.array;
import std.parallelism;
import core.sync.mutex;

synchronized class OccurrenceCounter {
    private shared(uint[string]) occurrences;

    synchronized public void increment(string str) {
        occurrences[str] += 1;
    }

    synchronized public @property auto keys() {
        return occurrences.keys;
    }

    synchronized public uint opIndex(string s) const {
        return occurrences[s];
    }
}


void main(string[] args) {
    auto occurrences = new shared(OccurrenceCounter);
    auto fin = File("../data/Cucumber_v2i.gff3");

    string[] lines = [];
    foreach(line; fin.byLine())
        lines ~= line.idup;

    foreach(line; taskPool.parallel(lines, 500)) {
            if (line.length == 0 || line[0] == '#') {
                continue;
            }
            string type = array(splitter(line.dup, '\t'))[2].idup;
            occurrences.increment(type);
    }

    foreach(type; occurrences.keys) {
        writefln("%s: %s", type, occurrences[type]);
    }
}
