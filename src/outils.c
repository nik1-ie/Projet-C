#include <ctype.h>
#include "outils.h"

void copie(char dest[], char origine[]) {
    while (*origine != '\0') {
        *dest = *origine;
        dest++;
        origine++;
    }
    *dest = *origine; // '\0'
}

int taille(char chaine[]) {
    int n = 0;
    while (chaine[n] != '\0') {
        n++;
    }
    return n;
}

int comparer_mots(char *a, char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        char ca = a[i];
        char cb = b[i];

        if (ca >= 'A' && ca <= 'Z') ca = ca + ('a' - 'A');
        if (cb >= 'A' && cb <= 'Z') cb = cb + ('a' - 'A');

        if (ca != cb) return (int)ca - (int)cb;
        i++;
    }
    return (int)a[i] - (int)b[i];
}

// Vérifie si un caractère spécial (' ou -) fait partie d'un mot
int mot_complet(char c) {
    unsigned char uc = (unsigned char)c;
    if (isalpha(uc) || c == '-' || c == '\'') {
        return 1;
    }else if ((c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        c == '-' ||
        c == '\'') {
        return 1;
    }
    return 0;
}

int convertir_str_en_int(char *s) { // ASCII
    // '0' -> 48    // '5' -> 53
    // '1' -> 49    // '6' -> 54
    // '2' -> 50    // '7' -> 55
    // '3' -> 51    // '8' -> 56
    // '4' -> 52    // '9' -> 57
    int signe = 1;
    int res = 0;
    int i = 0;

    if (s[0] == '-') {
        signe = -1;
        i++;
    } else if (s[0] == '+') {
        i++;
    }

    while (s[i] >= '0' && s[i] <= '9') {
        int chiffre = s[i] - '0';
        res = res * 10 + chiffre;
        i++;
    }
    return signe * res;
}
