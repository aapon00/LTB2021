#!/usr/bin/env python3

import pandas

def parse_args():
        import argparse

        parser = argparse.ArgumentParser(description="Summarize all the columns of a CSV file.")
        parser.add_argument('infile', help="A CSV file to summarize.")
        parser.add_argument('-t', '--trim', default=0, type=int, help="Number of leading entries to discard.")
        return parser.parse_args()

def main():
        args = parse_args()
        df = pandas.read_csv(args.infile)[args.trim:]
        df = df[50:-50]
        print(df.describe())

if __name__ == "__main__":
        main()
