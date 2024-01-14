"""Generate nn images from `dump_nn()` output."""

import glob
import igraph as ig
import matplotlib.pyplot as plt

def get_files():
    return glob.glob('out/*.nn')

def plot(file):
    save_file = file.replace(".nn", "") + ".png"

    g = ig.Graph.Read_Ncol(file, names=True, weights=True)
    for v in g.vs:
        v['size'] = 35
        name = v['label'] = v['name']
        if name.startswith('I'):
            v['color'] = 'lightblue'
        elif name.startswith('H'):
            v['color'] = 'lightpink'
        else:
            v['color'] = 'lightgrey'

    for e in g.es:
        w = e['weight'] / 2**16 * 2 - 1
        if e['weight'] < 0:
            e['color'] = 'lightcoral'
        elif e['weight'] == 0:
            e['color'] = 'grey'
        else:
            e['color'] = 'green'

        e['width'] = 1 + w

    ig.plot(g, save_file, edge_curved=True, margin=64, layout='circle')

if __name__ == "__main__":
    for file in get_files():
        plot(file)