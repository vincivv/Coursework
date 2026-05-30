import json
import matplotlib.pyplot as plt

with open("results.json", "r") as f:
    results = json.load(f)

bfs_data = []
dfs_data = []

for res in results:
    cap_sum = sum(int(c) for c in res["capacities"])
    for alg_key, store in [("bfs", bfs_data), ("dfs", dfs_data)]:
        r = res[alg_key]
        store.append({
            "cap_sum": cap_sum,
            "b":    r.get("b"),
            "D":    r.get("D"),
            "d":    r.get("d"),
            "time": r.get("time"),
        })

bfs_data.sort(key=lambda x: x["cap_sum"])
dfs_data.sort(key=lambda x: x["cap_sum"])

def extract(data, key):
    xs, ys = [], []
    for d in data:
        v = d[key]
        if v is not None:
            xs.append(d["cap_sum"])
            ys.append(v)
    return xs, ys

def make_plot(data, alg_name, filename):
    fig, ax1 = plt.subplots(figsize=(10, 5))

    color_b    = "#e07b00"   # orange  – branching factor
    color_d    = "#1f77b4"   # blue    – shallowest solution depth d
    color_D    = "#2ca02c"   # green   – maximum depth D
    color_time = "#d62728"   # red     – time

    # Left axis: b, d, D
    xs_b, ys_b = extract(data, "b")
    xs_d, ys_d = extract(data, "d")
    xs_D, ys_D = extract(data, "D")

    line_b, = ax1.plot(xs_b, ys_b, "o-",  color=color_b, label="b (branching factor)", linewidth=2)
    line_d, = ax1.plot(xs_d, ys_d, "s-",  color=color_d, label="d (shallowest solution depth)", linewidth=2)
    line_D, = ax1.plot(xs_D, ys_D, "D--", color=color_D, label="D (maximum depth reached)", linewidth=2)

    ax1.set_xlabel("Sum of Capacities", fontsize=12)
    ax1.set_ylabel("b,  d,  and  D", fontsize=12)

    # Right axis: time
    ax2 = ax1.twinx()
    xs_t, ys_t = extract(data, "time")
    line_t, = ax2.plot(xs_t, ys_t, "^--", color=color_time, label="Time (s)", linewidth=2)
    ax2.set_ylabel("Time (seconds)", fontsize=12, color=color_time)
    ax2.tick_params(axis="y", labelcolor=color_time)

    lines  = [line_b, line_d, line_D, line_t]
    labels = [l.get_label() for l in lines]
    ax1.legend(lines, labels, loc="upper left", fontsize=10)

    plt.title(f"{alg_name}: b, d, D & Execution Time vs. Sum of Capacities", fontsize=12, pad=12)
    fig.tight_layout()
    plt.savefig(filename, dpi=150)
    plt.close()
    print(f"Saved {filename}")

make_plot(bfs_data, "BFS", "plot_bfs.png")
make_plot(dfs_data, "DFS", "plot_dfs.png")
print("Done.")