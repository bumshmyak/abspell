#!/usr/bin/python

import codecs
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

def generate_variations(iterable):
    for query in iterable:
        yield ( query, [ ( 1.0, query ) ] )

def serialize_variations(iterable):
    for query, variations in iterable:
        yield query + u"\t" + u"\t".join(
            u":".join(unicode(x) for x in variation)
            for variation in variations
        )

def main():
    if len(sys.argv) < 2:
        print >>sys.stderr, "Usage: {0} <canonical>".format(sys.argv[0])
        sys.exit(1)

    try:
        write(codecs.getwriter("utf-8")(sys.stdout),
            serialize_variations(
                generate_variations(from_file(sys.argv[1]))
            )
        )
    except Exception as e:
        print >>sys.stderr, "ERROR: {0}".format(e)
        sys.exit(1)

if __name__ == "__main__":
    main()
