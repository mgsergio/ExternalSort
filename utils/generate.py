#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import random
import string

LETTERS = string.ascii_letters + string.digits

def random_string(rnd, length):
    return ''.join(rnd.choice(LETTERS)
                   for _ in range(length))

def generate(ost, rnd, max_bytes, min_str_len, max_str_len):
    assert min_str_len > 0
    current_bytes = 0
    while current_bytes < max_bytes:
        length = int(rnd.uniform(min_str_len, max_str_len))
        if length + 1 + current_bytes > max_bytes:
            length = max_bytes - current_bytes - 1
            if length < min_str_len:
                break
        current_bytes += length + 1
        ost.write(random_string(rnd, length))
        ost.write('\n')

def convert_size(size_str):
    """Converts a value with a size suffix to a corresponding number of bytes.

    >>> convert_size('1')
    1
    >>> convert_size('1B')
    1
    >>> convert_size('1K')
    1024
    >>> convert_size('3M')
    3145728
    >>> convert_size('M')
    Traceback (most recent call last):
        ...
    AssertionError
    >>> convert_size('2E')
    Traceback (most recent call last):
        ...
    AssertionError
    """
    factors = ['B', 'K', 'M', 'G']
    assert all(c.isdigit() or (c in factors and i == len(size_str))
               for i, c in enumerate(size_str, 1))
    assert size_str[0].isdigit()

    if not size_str[-1].isdigit():
        factor = 1024 ** factors.index(size_str[-1])
        return int(size_str[:-1]) * factor
    return int(size_str)


class ConvertSizeAction(argparse.Action):
    def __init__(self, option_strings, dest, nargs=None, **kwargs):
        if nargs is not None:
            raise ValueError("nargs not allowed")
        super().__init__(option_strings, dest, **kwargs)

    def __call__(self, parser, namespace, values, option_string=None):
        setattr(namespace, self.dest, convert_size(values))

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('output', type=argparse.FileType('w'),
                        help='Path to output file or - for stdout')
    parser.add_argument('--min_str_len', type=str, default=convert_size('10B'),
                        action=ConvertSizeAction,
                        help='Generated string will be at least that long')
    parser.add_argument('--max_str_len', type=str, default=convert_size('100K'),
                        action=ConvertSizeAction,
                        help='Generated string will be at at most that long')
    parser.add_argument('--max_bytes', type=str,
                        action=ConvertSizeAction,
                        required=True,
                        help='Generated file will be nearly that long.')

    args = parser.parse_args()
    generate(args.output,
             rnd=random.SystemRandom(),
             max_bytes=args.max_bytes,
             min_str_len=args.min_str_len,
             max_str_len=args.max_str_len)
