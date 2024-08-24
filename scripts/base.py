import json
import pandas as pd
import re
import io

def build_dataframe(filename):
    # The benchmark may have been aborted. This is a quickfix/hack.
    fixed_json = ''
    print(f"Opening: {filename}")
    with open(filename, 'r') as reader:
        fixed_json = reader.read()
        if fixed_json.endswith("    }"):
            fixed_json += ']}'
        if fixed_json.endswith("[\n"):
            fixed_json += ']}'
        if "slurmstepd" in fixed_json:
            fixed_json = re.sub(r'slurmstepd.*$', ']}', fixed_json)

    j = json.loads(fixed_json)
    benchmark_json = j["benchmarks"]
    benchmark_json_str = io.StringIO(json.dumps(benchmark_json))
    df = pd.read_json(path_or_buf=benchmark_json_str, orient='records')
    # Add column with partition name.
    # df['Partition'] = [partition for x in range(0, len(df['size']))]
    return df
