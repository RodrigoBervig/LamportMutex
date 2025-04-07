import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Load full benchmark data
df = pd.read_csv("./results/mutex_benchmark.csv")

# ----------------------------
# Plot 1: Filtered execution time bar plot
# ----------------------------
df_filtered = df[(df["adds_per_thread"] >= 1_000_000) & (df["total_threads"] >= 4)]
df_filtered["config"] = df_filtered.apply(
    lambda row: f"{row['adds_per_thread']:,}\n{row['total_threads']} threads", axis=1
)

# Sort configs
df_filtered["config"] = pd.Categorical(
    df_filtered["config"],
    categories=sorted(df_filtered["config"].unique(), key=lambda x: (int(x.split()[0].replace(',', '')), int(x.split()[1]))),
    ordered=True
)

# Plot bar chart
sns.set_theme(style="whitegrid")
plt.figure(figsize=(12, 7))
sns.barplot(
    data=df_filtered,
    x="config",
    y="time_seconds",
    hue="implementation",
    palette="Set2"
)
plt.title("Execution Time (≥ 1M adds/thread and ≥ 4 threads)", fontsize=16)
plt.xlabel("Additions per Thread and Thread Count", fontsize=12)
plt.ylabel("Execution Time (seconds)", fontsize=12)
plt.xticks(rotation=45, ha="right")
plt.legend(title="Implementation")
plt.tight_layout()
plt.savefig("barplot_filtered.png")
plt.show()

# ----------------------------
# Plot 2: Speedup line plot (all configs)
# ----------------------------
pivot = df.pivot_table(
    index=["adds_per_thread", "total_threads"],
    columns="implementation",
    values="time_seconds"
).reset_index()

# Calculate speedup: lamport / pthread
pivot["speedup"] = pivot["lamport"] / pivot["pthread"]

# Label for x-axis
pivot["config"] = pivot.apply(lambda row: f"{row['adds_per_thread']:,}\n{row['total_threads']} threads", axis=1)

# Sort and categorize
pivot = pivot.sort_values(by=["adds_per_thread", "total_threads"])
pivot["config"] = pd.Categorical(pivot["config"], categories=pivot["config"].tolist(), ordered=True)

# Rolling average
N = 5
pivot["moving_avg"] = pivot["speedup"].rolling(window=N, min_periods=1).mean()

# Plot line chart
plt.figure(figsize=(14, 6))
sns.lineplot(
    data=pivot,
    x="config",
    y="speedup",
    marker="o",
    label="Speedup (Lamport / Pthread)",
    color="skyblue"
)
sns.lineplot(
    data=pivot,
    x="config",
    y="moving_avg",
    label=f"{N}-Point Moving Avg",
    color="darkblue",
    linestyle="--"
)
plt.axhline(1, color="red", linestyle="--", label="No Speedup (1.0)")

plt.title("Speedup of Pthread vs Lamport (All Configs)", fontsize=16)
plt.xlabel("Adds/thread + threads)", fontsize=12)
plt.ylabel("Lamport / Pthread Time Ratio", fontsize=12)
plt.xticks(rotation=45, ha="right")
plt.legend()
plt.tight_layout()
plt.savefig("speedup_all_with_moving_avg.png")
plt.show()
