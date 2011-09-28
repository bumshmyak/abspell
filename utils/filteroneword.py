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

def read_canonical_variations(iterable):
    for line_number, line in enumerate(iterable):
        try:
            columns = line.split("\t")

            query = columns[0]
            variations = columns[1:]

            yield line, query, set(variations)
        except Exception as e:
            raise Exception("Exception on line #{0}: #{1}".format(
                1 + line_number, str(e)
            ))

def main():
    if len(sys.argv) < 2:
        print >>sys.stderr, "Usage: {0} <canonical>".format(sys.argv[0])
        sys.exit(1)

    try:
        canonical = read_canonical_variations(from_file(sys.argv[1]))
        for canonical_item in canonical:
            if len(canonical_item[1].split(' ')) == 1:
                print canonical_item[0]
    except Exception as e:
        print >>sys.stderr, "ERROR: {0}".format(e)
        sys.exit(1)

if __name__ == "__main__":
    main()

