#!/usr/bin/python

import itertools
import operator
import random
import sys

from collections import deque

def from_file(name):
    with open(name, "r") as handle:
        for line in handle:
            line = line.rstrip("\r\n")
            yield line

def write(iterable):
    for item in iterable:
        sys.stdout.write(item)
        sys.stdout.write("\n")

def op_identity(ctx, positional):
    if len(positional) != 1:
        raise Exception("Syntax violation.")

    return from_file(positional[0])

def op_sample(ctx, positional):
    if len(positional) != 2:
        raise Exception("Syntax violation.")

    n = int(positional[0])
    x = from_file(positional[1])
    x = list(x)
    x = random.sample(x, n)

    return x

def op_shuffle(ctx, positional):
    if len(positional) != 1:
        raise Exception("Syntax violation.")

    x = from_file(positional[0])
    x = list(x)

    random.shuffle(x)

    return x

def op_subtract(ctx, positional):
    if len(positional) != 2:
        raise Exception("Syntax violation.")

    a = from_file(positional[0])
    b = from_file(positional[1])

    return set(a) - set(b)

def op_noop():
    raise Exception("No operation.")
    pass

def main():
    import argparse
    parser = argparse.ArgumentParser(description = "Set operations.")

    # Positional.
    positional = parser.add_argument_group(title = "Positional arguments")
    positional.add_argument("positional", metavar = "A",
        type = str, nargs = "+",
        help = "Arguments for the operation.")

    # Operations.
    operations = parser.add_mutually_exclusive_group(required = True)
    operations.add_argument("--identity", dest = "op",
        action = "store_const", const = op_identity, default = op_noop,
        help = "Perform nothing. (Syntax: <S>)")
    operations.add_argument("--sample", dest = "op",
        action = "store_const", const = op_sample, default = op_noop,
        help = "Perform sampling. (Syntax: <N> <S>; samples N items from S)")
    operations.add_argument("--shuffle", dest = "op",
        action = "store_const", const = op_shuffle, default = op_noop,
        help = "Perform shuffling. (Syntax: <S>)")
    operations.add_argument("--subtract", dest = "op",
        action = "store_const", const = op_subtract, default = op_noop,
        help = "Perform set subtraction. (Syntax: <A> <B>; subtracts B from A)")

    args = parser.parse_args()
    write(args.op(args, args.positional))

if __name__ == "__main__":
    main()
