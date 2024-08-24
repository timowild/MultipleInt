#!python

import pathlib
import sys
from pathlib import Path
from itertools import islice, cycle
import pandas as pd
from cycler import cycler
import matplotlib.pyplot as plt
import matplotlib as mpl
import re

import base

KEEP_STDDEV = True

def better_mpl_line_style_cycle(num_styles = 10):
    prop_cycle = plt.rcParams['axes.prop_cycle']
    colors = list(islice(cycle(prop_cycle.by_key()['color']), num_styles))
    linestyles = list(islice(cycle(['-', '--', ':', '-.']), num_styles))
    plt.rc('axes', prop_cycle=(cycler('color', colors) +
                               cycler('linestyle', linestyles)))

def main(argv):
    BITS_TO_GB = 8 * (1024 ** 3)

    def std_mem(name: str, n_elems: int, prom_red: bool) -> float:      
        if not prom_red:  
            storage, intcount = re.search(r"-u(\d+)x(\d+)", name).group(1, 2)
            storage = int(storage)
            intcount = int(intcount)
            return storage * intcount * n_elems / BITS_TO_GB

        else:
            smaller, larger, intcount = re.search(r"-u(\d+)-u(\d+)x(\d+)", name).group(1, 2, 3)
            smaller, larger, intcount = int(smaller), int(larger), int(intcount)
            return (smaller * n_elems * intcount + larger) / BITS_TO_GB

    def mi_mem(name: str, n_elems: int, prom_red: bool) -> float:
        if not prom_red:
            storage = re.search(r"-mi\<\d+, u(\d+)\>", name).group(1)
            storage = int(storage)
            return storage * n_elems / BITS_TO_GB
        else:
            smaller, larger = re.search(r"-mi\<\d+-\d+, u(\d+)-u(\d+)\>", name).group(1, 2)
            smaller, larger = int(smaller), int(larger)
            return (smaller * n_elems + larger) / BITS_TO_GB

        

    # Some defaults I like
    mpl.style.use(Path(__file__).resolve().parent / 'mystyle.mplstyle')
    # make overlapping lines easier to see
    better_mpl_line_style_cycle()

    # read data, skip context information
    df = base.build_dataframe(argv[0])
    name = pathlib.Path(argv[0]).with_suffix("").stem

    # get additional columns from benchmark "name"
    df[['name', 'n_elements']] = df['name'].str.split(pat='/', expand=True)
    # needed when stats from benchmark repetitions are reported
    df[['n_elements', 'stat']] = df['n_elements'].str.split(pat='_', expand=True)
    # cast numeral strings to ints
    df['n_elements'] = df['n_elements'].astype(int)

    # preserve time_unit for plot labels
    # (assume that it does not differ between rows)
    bw_unit = "GB/s"

    # remove warmup row(s)
    df = df[df['name'] != '_warmup_']
    
    # work only on mean, memory consumption should stay the same
    #df = df[df["stat"] == "mean"]

    # benchmarks that operate on standard library types
    std_df = df[df["name"].str.contains(r"-u\d+x\d", regex=True)][["name", "n_elements", "real_time", "stat"]]
    mi_df = df[df["name"].str.contains(r"\>")][["name", "n_elements", "real_time", "stat"]]

    is_promred = name.startswith("promred-")
    std_df["memory"] = std_df.apply(lambda row: std_mem(row["name"], row["n_elements"], is_promred),axis=1)
    mi_df["memory"] = mi_df.apply(lambda row: mi_mem(row["name"], row["n_elements"], is_promred),axis=1)

    NS_TO_S = 10 ** 9
    std_df["bandwidth"] = std_df["memory"] / (std_df["real_time"] / NS_TO_S)
    mi_df["bandwidth"] = mi_df["memory"] / (mi_df["real_time"] / NS_TO_S)

    bandwidth_df = pd.concat([std_df, mi_df])

    df_mean = bandwidth_df[bandwidth_df['stat'] == 'mean']

    if KEEP_STDDEV:
        df_stddev = bandwidth_df[bandwidth_df['stat'] == 'stddev']

    # pivot data into a form that is nice to plot
    df_mean = df_mean.pivot(index='n_elements', columns='name', values='bandwidth')

    if KEEP_STDDEV:
        df_stddev = df_stddev.pivot(index='n_elements', columns='name', values='bandwidth')

    # plot real time vs n_elements (log-log-plot)
    fig, ax = plt.subplots()
    if KEEP_STDDEV:
        df_mean.plot(yerr=df_stddev, ax=ax)
    else:
        df_mean.plot(ax=ax)

    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_ylabel(f'bandwidth [{bw_unit}]')
    ax.set_title(f"Bandwidth Benchmarks from {name}")
    

    if len(argv) >= 2:
        fig.savefig(argv[1])

    # plot real times for highest n_elements as bar plot (linear y axis)
    fig, ax = plt.subplots()

    if KEEP_STDDEV:
        df_mean.loc[df_mean.index.max()].plot(kind='bar', yerr=df_stddev.loc[df_stddev.index.max()], ax=ax)
    else:
        df_mean.loc[df_mean.index.max()].plot(kind='bar', ax=ax)
    ax.set_ylabel(f'bandwidth [{bw_unit}]')
    # Labels (benchmark names) are often too big and therefore overlap and/or overflow the canvas.
    # You can use rotation and size to get better results
    ax.tick_params(axis='x', labelrotation=0., labelsize=10.)

    # Put newline in front of "<...>"-part of benchmark name for more efficient use of space.
    labels = ax.get_xticklabels()
    for label in labels:
        label.set_text('\n<'.join(label.get_text().rsplit('<')))
    ax.set_xticklabels(labels)

    if len(argv) >= 3:
        fig.savefig(argv[2])

if __name__ == "__main__":
    main(sys.argv[1:])
