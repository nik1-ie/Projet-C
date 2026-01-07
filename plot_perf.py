import sys
import matplotlib.pyplot as plt

if len(sys.argv) < 2:
    print("python plot_perf.py perf1.txt perf2.txt ...")
    sys.exit(1)

fichiers = sys.argv[1:]

noms = ["cumul_alloc", "cumul_desalloc", "max_alloc", "temps_execution"]
nb_noms = len(noms)
n_files = len(fichiers)

width = 0.8 / nb_noms
x = range(n_files) 

temps_exec_reel = []
infos_textes = []

valeurs_par_fichier = []
for i, fichier in enumerate(fichiers):
    with open(fichier, "r", encoding="utf-8") as f:
        lignes = f.readlines()

    ca = int(lignes[0][12:]) 
    cd = int(lignes[1][15:])
    ma = int(lignes[2][10:])
    te = int(lignes[3][16:])
    nbt = int(lignes[4][14:])
    nbd = int(lignes[5][18:])

    temps_exec_reel.append(te)

    nv_te = te * 10000

    valeurs_par_fichier.append([ca, cd, ma, nv_te])

    infos_textes.append(
        f"f{i+1} : total={nbt}, distincts={nbd}"
    )

for j in range(nb_noms):
    valeurs = [valeurs_par_fichier[i][j] for i in range(n_files)]
    decalage = (j - (nb_noms - 1) / 2) * width
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


leg1 = plt.legend(loc="upper left")
plt.legend(infos_textes, title="Mots", loc="center left", handlelength=0)
plt.gca().add_artist(leg1)

plt.title("Performances")
plt.grid(axis="y", linestyle="--", alpha=0.3)
plt.show()
