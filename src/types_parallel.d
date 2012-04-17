import std.stdio;
import std.algorithm;
import std.array;
import std.parallelism;
import std.functional;

string make_parallel(string funcname, string methodname,
                     bool functional = false) {
    auto s = "auto " ~ funcname ~ "(Range, Args...)(Range r, TaskPool pool, Args args) {" ~
                 "return pool." ~ methodname ~ (functional ? "!(fun)" : "") ~ "(r, args);" ~
             "}";
    return functional ? "template " ~ funcname ~ "(fun...) {" ~ s ~ "}" : s;
}

mixin(make_parallel("parallelMap", "map", true));
mixin(make_parallel("parallelForeach", "parallel"));
mixin(make_parallel("asyncBuf", "asyncBuf"));

template eachDo(fun...) {
    auto eachDo(Range)(Range r) {
        foreach(elem; r) {
            unaryFun!fun(elem);
        }
    }
}

void main(string[] args) {
    auto fin = File("../data/Cucumber_v2i.gff3");

    int[string] occurrences;

    auto pool = new TaskPool(totalCPUs);

    fin.byLine()
       .map!"a.idup"()
       .filter!"a.length != 0 && a[0] != '#'"()
       .asyncBuf(pool, 100)
       .parallelMap!"array(splitter(a, '\t'))[2].idup"(pool)
       .parallelForeach(pool)
       .eachDo!(type => ++occurrences[type])();

    pool.finish();

    foreach(type; occurrences.keys) {
        writefln("%s: %s", type, occurrences[type]);
    }

}
