from collections import defaultdict
import math
import matplotlib.pyplot as plt
import matplotlib as mpl

from itertools import islice, cycle
from cycler import cycler

from pathlib import Path
import pandas as pd

import sys

def better_mpl_line_style_cycle(num_styles = 10):
    prop_cycle = plt.rcParams['axes.prop_cycle']
    colors = list(islice(cycle(prop_cycle.by_key()['color']), num_styles))
    linestyles = list(islice(cycle(['-', '--', ':', '-.']), num_styles))
    plt.rc('axes', prop_cycle=(cycler('color', colors) +
                               cycler('linestyle', linestyles)))

def main(argv):
    # Some defaults I like
    mpl.style.use(Path(__file__).resolve().parent / 'mystyle.mplstyle')
    # make overlapping lines easier to see
    better_mpl_line_style_cycle()

    BITS_TO_GB = 8 * 1024 ** 3

    def multiple_int(int_count: int, storage_size_in_bit: int, n_elements: int) -> int:
        return storage_size_in_bit * n_elements / BITS_TO_GB


    def normal_int(int_count: int, storage_size_in_bit: int, n_elements: int) -> int:
        return int_count * storage_size_in_bit * n_elements / BITS_TO_GB


    data_widths = [8, 16, 32, 64]
    n_elementss = [2 ** p for p in range(14, 29 + 1)]

    mi_benchmarks = defaultdict(dict)
    ni_benchmarks = defaultdict(dict)


    for data_width in data_widths:
        int_counts = [i for i in range(1, int(math.log2(data_width)) + 1)]

        print(data_width, int_counts)

        for int_count in int_counts:
            mi_bits_occupied = {}
            ni_bits_occupied = {}

            for n_elements in n_elementss:
                mi_bits_occupied[n_elements] = multiple_int(
                    int_count=int_count, storage_size_in_bit=data_width, n_elements=n_elements
                )
                ni_bits_occupied[n_elements] = normal_int(
                    int_count=int_count, storage_size_in_bit=data_width, n_elements=n_elements
                )

            mi_benchmarks[data_width][int_count] = mi_bits_occupied
            ni_benchmarks[data_width][int_count] = ni_bits_occupied


    # Side by side comparisons for same data width between mi and ni
    for data_width in data_widths:
        mi_benchmark = mi_benchmarks[data_width]
        ni_benchmark = ni_benchmarks[data_width]

        mi_columns = [f"irr::mi<IntCount={x}, u{data_width}> @ BitWidth = {int(data_width / x) - 1}" for x in mi_benchmark.keys()]
        ni_columns = [f"u{data_width} x IntCount={x}" for x in ni_benchmark.keys()]

        mi_df = pd.DataFrame(mi_benchmarks[data_width])#, columns=mi_columns)
        mi_df.columns = mi_columns

        ni_df = pd.DataFrame(ni_benchmarks[data_width])#, columns=ni_columns)
        ni_df.columns = ni_columns

        fig, ax = plt.subplots()
        ax.set_xscale('log')
        ax.set_yscale('log')

        ax.set_xlabel(f'n_elements')
        ax.set_ylabel(f'GB')
        ax.set_title(f"Memory Consumption for irr::mi<IntCount, u{data_width}> and IntCount x u{data_width}")
        ax.tick_params(axis='x', labelrotation=0., labelsize=10.)

        mi_df.plot(ax=ax)
        ni_df.plot(ax=ax)

        plt.savefig(f"u{data_width} - {argv[0]}")

        #for int_count in int_counts:
        #    mi_benchmark = mi_benchmarks[(data_width, int_count)]
        #    ni_benchmark = ni_benchmarks[(data_width, int_count)]

if __name__ == "__main__":
    main(sys.argv[1:])