import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Загружаем свежие данные
df = pd.read_csv('amdb_research_data.csv')

# Считаем статистику
t_stats = df.groupby(['Density', 'Type'])['Time_ms'].agg(['mean', 'std']).unstack()
n_stats = df.groupby(['Density', 'Type'])['Nodes'].agg(['mean', 'std']).unstack()

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 7))

# График 1: Логарифмическое время (чтобы видеть микро-разницу)
for t, color in zip(['Random', 'Structured'], ['#e74c3c', '#27ae60']):
    m = t_stats['mean'][t]
    s = t_stats['std'][t]
    # +0.0001 чтобы не было ошибки логарифма от нуля
    ax1.plot(t_stats.index, np.log10(m + 0.0001), 'o-', color=color, label=f'{t}')
    ax1.fill_between(t_stats.index, np.log10(m-s + 0.0001), np.log10(m+s + 0.0001), color=color, alpha=0.1)

ax1.set_title('Logarithmic Runtime (ms)', fontsize=14)
ax1.set_xlabel('Density (N/Bits)')
ax1.set_ylabel('log10(Time)')
ax1.grid(True, which="both", ls="--", alpha=0.5)
ax1.legend()

# График 2: Узлы с Error Bars
for t, color in zip(['Random', 'Structured'], ['#e74c3c', '#27ae60']):
    m = n_stats['mean'][t]
    s = n_stats['std'][t]
    ax2.plot(n_stats.index, np.log10(m + 1), 's-', color=color, label=f'{t}')
    ax2.fill_between(n_stats.index, np.log10(m-s + 1), np.log10(m+s + 1), color=color, alpha=0.1)

ax2.set_title('Computational Complexity (Nodes)', fontsize=14)
ax2.set_xlabel('Density (N/Bits)')
ax2.set_ylabel('log10(Nodes)')
ax2.grid(True, which="both", ls="--", alpha=0.5)
ax2.legend()

plt.tight_layout()
plt.savefig(os.path.expanduser("~/Desktop/SCIENTIFIC_RESULT.png"), dpi=300)
print("✅ Финальный научный график сохранен на рабочий стол!")
