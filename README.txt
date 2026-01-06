# Projet-C
Afin de compiler le projet, il vous suffit d'exécuter :
`gcc -Istructs main.c src\alloc.c src\outils.c src\algo1.c src\algo2.c src\options.c -o projet`

Pour exécuter le projet, vous avez le choix :
- *Un certain nombres de mots à afficher (-n)
- (bonus) Un nombre minimum de caractères pour les mots à afficher (-k)
- *Entre 3 algorithmes différents (-a)
- *Un affichage en console ou dans des fichiers choisis (-s , -l)
- *Un ou plusieurs textes

(Les options * étant obligatoires, et les options à donner dans cet ordre précis.)

Vous pourrez donc exécuter, par example :
`./projet -n 10 -k 10 -a algo1|algo2|algo3 -s fichier -l fichier textes`

Ou alors, avec les fichiers fournis dans notre archive de fichier:
`./projet -n 10 -k 10 -a algo1 -s resultat.txt -l perf.txt textes\test_repetitions.txt textes\les-fleurs-du-mal.txt`