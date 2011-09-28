#!/usr/bin/python

import codecs
import itertools
import operator
import sys

def from_file(name):
    with open(name, "r") as handle:
        iterable = codecs.getreader("utf-8")(handle)
        for item in iterable:
            yield item.rstrip("\r\n")

def write(stream, iterable):
    for item in iterable:
        stream.write(item)
        stream.write("\n")

def read_canonical_variations(iterable):
    for line_number, line in enumerate(iterable):
        try:
            columns = line.split("\t")

            query = columns[0]
            variations = columns[1:]

            yield query, set(variations)
        except Exception as e:
            raise Exception("Exception on line #{0}: #{1}".format(
                1 + line_number, str(e)
            ))

def read_spelled_variations(iterable):
    for line_number, line in enumerate(iterable):
        try:
            columns = line.split("\t")

            query = columns[0]
            variations = [ column.split(":") for column in columns[1:] ]
            variations = [ (float(p), unicode(v)) for p, v in variations ]
            total_probability = sum(p for p, v in variations)
            variations = [ (p / total_probability, v) for p, v in variations ]

            yield query, variations
        except Exception as e:
            raise Exception("Exception on line #{0}: {1}".format(
                1 + line_number, str(e)
            ))

def evaluate(canonical_iterable, spelled_iterable):
    iterable = itertools.izip(canonical_iterable, spelled_iterable)

    accum_precision = 0.0
    accum_recall = 0.0
    accum_queries = 0.0

    for canonical_item, spelled_item in iterable:
        canonical_query, canonical_variations = canonical_item
        spelled_query, spelled_variations = spelled_item

        if canonical_query != spelled_query:
            raise Exception("Queries differ in canonical dataset and spelled dataset.")
        
        precision = compute_precision(canonical_variations, spelled_variations)
        if precision == 0:
            print "---"
            print canonical_query
            print spelled_variations
            print canonical_variations

        accum_precision += precision
        accum_recall += compute_recall(canonical_variations, spelled_variations)
        accum_queries += 1.0

    EP = accum_precision / accum_queries
    ER = accum_recall / accum_queries
    EF = 2.0 * EP * ER / (EP + ER)

    return EP, ER, EF

def compute_precision(cv, sv):
    return sum(p for p, v in sv if v in cv)

def compute_recall(cv, sv):
    return sum(1.0 for v in cv if v in map(operator.itemgetter(1), sv)) / len(cv)

def main():
    if len(sys.argv) < 3:
        print >>sys.stderr, "Usage: {0} <canonical> <spelled>".format(sys.argv[0])
        sys.exit(1)

    try:
        canonical = read_canonical_variations(from_file(sys.argv[1]))
        spelled = read_spelled_variations(from_file(sys.argv[2]))

        EP, ER, EF = evaluate(canonical, spelled)

        print "EP: {EP:0.4f}\nER: {ER:0.4}\nEF: {EF:0.4}".format(**vars())
    except Exception as e:
        print >>sys.stderr, "ERROR: {0}".format(e)
        sys.exit(1)

if __name__ == "__main__":
    main()

