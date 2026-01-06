import sys
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
    print("Usage : python plot_perf_naif_multi_bar.py perf1.txt perf2.txt ...")
    sys.exit(1)

fichiers = sys.argv[1:]

noms = ["cumul_alloc", "cumul_desalloc", "max_alloc", "temps_execution"]
n_metrics = len(noms)
n_files = len(fichiers)

width = 0.8 / n_metrics
x = range(n_files) 

temps_exec_reel = []

valeurs_par_fichier = []
for fichier in fichiers:
    with open(fichier, "r", encoding="utf-8") as f:
        lignes = f.readlines()

    ca = int(lignes[0][12:]) 
    cd = int(lignes[1][15:])
    ma = int(lignes[2][10:])
    te = int(lignes[3][16:])

    temps_exec_reel.append(te)

    nv_te = te * 10000

    valeurs_par_fichier.append([ca, cd, ma, nv_te])

for j in range(n_metrics):
    valeurs = [valeurs_par_fichier[i][j] for i in range(n_files)]
    decalage = (j - (n_metrics - 1) / 2) * width
    positions = [xi + decalage for xi in x]

    bars = plt.bar(positions, valeurs, width=width, label=noms[j])

    if j == 3:
        for idx_b, bar in enumerate(bars):
            hauteur = bar.get_height()
            t_reel = temps_exec_reel[idx_b]
            plt.text(
                bar.get_x() + bar.get_width() / 2,
                hauteur,
                f"{t_reel}s",
                ha="center",
                va="bottom",
                color="red",
                fontsize=8,
            )

plt.title("Performances")
plt.legend()
plt.grid(axis="y", linestyle="--", alpha=0.3)
plt.show()
