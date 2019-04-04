import networkx as nx
import matplotlib.pyplot as plt
import argparse
from collections import abc


class AdjList(abc.Sequence):
    def __init__(self, n=0):
        self.ls = [[] for _ in range(n)]

    def __str__(self):
        return str(self.ls)

    def __setitem__(self, key, item):
        self.ls[key] = item

    def __getitem__(self, key):
        return self.ls[key]

    def __len__(self):
        return len(self.ls)

    def display(self):
        g = nx.DiGraph()
        for i, ls in enumerate(self):
            g.add_node(i, name=i)
            for v in ls:
                g.add_edge(i, v)
        pos = nx.drawing.nx_agraph.graphviz_layout(g, prog='dot')
        nx.draw(g, pos, with_labels=True)
        plt.show()


def read_input_adjlist(n):
    """
    read adjlist of the form from stdin
    -1: 1, 2
    0: 3
    1: 0, 1
    ...
    n: 7, 11

    Read exactly n rows
    """
    al = AdjList(n)
    for i in range(n):
        a, _, b = input().partition(':')
        ls = [int(x) for x in b.split(',') if x]
        al[i] = ls
    return al


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--n', type=int, required=True)
    args = parser.parse_args()
    n = args.n
    al = read_input_adjlist(n)
    al.display()
