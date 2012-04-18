import std.concurrency;
import std.algorithm;
import std.array : array;
import std.stdio;
import std.parallelism : totalCPUs;

alias immutable(string)[] Chunk;
alias int[string] Dict;

void reader(string filename, size_t chunk_size, uint parsers, Tid counter_tid) {
    auto f = File(filename);
   
    string[] buffer;
    
    foreach(line; f.byLine()) {
        if (line.length > 0 && line[0] != '#') {
            buffer ~= line.idup;
            if (buffer.length == chunk_size) {
                send(receiveOnly!Tid(), cast(Chunk)buffer);
                buffer = [];
            }
        }
    }

    if (buffer.length > 0) {
        send(receiveOnly!Tid(), cast(Chunk)buffer);
    } 

    while (parsers > 0) {
        send(receiveOnly!Tid(), true); // finished
        parsers--;
    }

    send(counter_tid, true); // finished
}

void parser(Tid counter_tid, Tid reader_tid) {
    for(bool finished = false; !finished ; ) {
        send(reader_tid, thisTid);
        receive(
            (Chunk chunk) {
                string[] types;
                foreach(line; chunk) {
                    string[16] words;
                    size_t first = 0;
                    size_t item = 0;
                    for(size_t i = 0; i < line.length; i++) {
                        if (line[i] == '\t') {
                            assert(item < 9);
                            words[item++] = line[first .. i-1];
                            first = i + 1;
                            i++;
                        }
                    }
                    if (first < line.length && line[first] != '\t') {
                        assert(item < 9);
                        words[item] = line[first..$];
                    }
                    types ~= words[2];
                }
                send(counter_tid, cast(Chunk)types);
            },
            (bool _finished) {
                finished = _finished;
            }
        ); 
    }
}

void counter(Tid main_tid) {
    Dict occurrences;
    for(bool finished = false; !finished ; ) {
        receive(
            (Chunk chunk) {
                foreach(type; chunk) {
                    occurrences[type] += 1;
                }
            },
            (bool _finished) {
                finished = _finished;
            }
        );
    }
    foreach(type; occurrences.keys) {
        writefln("%s: %s", type, occurrences[type]);
    }
    send(main_tid, true);
}

void main() {
    auto counter_tid = spawn(&counter, thisTid);
    const auto PARSERS = totalCPUs;
    auto reader_tid = spawn!(string, size_t, uint, Tid)(&reader, 
                            "../data/Cucumber_v2i.gff3", 1000, PARSERS, counter_tid);
    for (int i = 0; i < PARSERS; i++) {
        spawn!(Tid, Tid)(&parser, counter_tid, reader_tid);
    }
    receiveOnly!bool(); // wait for finish
}
