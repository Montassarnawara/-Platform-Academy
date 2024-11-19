#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour représenter un cours ou un TD
typedef struct {
    char id[20];
    char type[10];     // "Cours" ou "TD"
    char titre[50];
    char contenu[100];
    float prix;        // Prix à ajouter
} CoursTD;

// Fonction pour lire les cours/TD à partir du fichier cours_td.txt
int lireCoursTD(const char *fichier, CoursTD *liste, int tailleMax) {
    FILE *file = fopen(fichier, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", fichier);
        return 0;
    }

    int count = 0;
    char ligne[200];
    while (fgets(ligne, sizeof(ligne), file) && count < tailleMax) {
        // Lire les données ligne par ligne
        if (strncmp(ligne, "ID:", 3) == 0) {
            sscanf(ligne, "ID: %s", liste[count].id);
        } else if (strncmp(ligne, "Type:", 5) == 0) {
            sscanf(ligne, "Type: %s", liste[count].type);
        } else if (strncmp(ligne, "Titre:", 6) == 0) {
            sscanf(ligne, "Titre: %[^\n]", liste[count].titre);
        } else if (strncmp(ligne, "Contenu:", 8) == 0) {
            sscanf(ligne, "Contenu: %[^\n]", liste[count].contenu);
        } else if (strncmp(ligne, "--------------------", 20) == 0) {
            // Initialiser le prix à 0.0 et passer à l'entrée suivante
            liste[count].prix = 0.0;
            count++;
        }
    }

    fclose(file);
    return count;
}

// Fonction pour écrire les cours/TD avec prix dans prix_cours_td.txt
void ecrirePrixCoursTD(const char *fichier, CoursTD *liste, int taille) {
    FILE *file = fopen(fichier, "w");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", fichier);
        return;
    }

    for (int i = 0; i < taille; i++) {
        fprintf(file, "ID: %s\nType: %s\nTitre: %s\nContenu: %s\nPrix: %.2f\n--------------------\n",
                liste[i].id, liste[i].type, liste[i].titre, liste[i].contenu, liste[i].prix);
    }

    fclose(file);
    printf("Fichier '%s' créé avec succès.\n", fichier);
}

// Fonction pour ajouter ou modifier les prix des cours/TD
void ajouterPrixCoursTD(CoursTD *liste, int taille) {
    printf("\nListe des cours et TD disponibles :\n");
    for (int i = 0; i < taille; i++) {
        printf("%d. ID: %s | Type: %s | Titre: %s | Prix actuel: %.2f\n",
               i + 1, liste[i].id, liste[i].type, liste[i].titre, liste[i].prix);
    }

    int choix;
    do {
        printf("\nEntrez le numéro du cours/TD auquel vous souhaitez ajouter un prix (ou 0 pour terminer) : ");
        scanf("%d", &choix);

        if (choix < 1 || choix > taille) {
            if (choix != 0) {
                printf("Choix invalide.\n");
            }
            continue;
        }

        printf("Entrez le nouveau prix pour '%s' : ", liste[choix - 1].titre);
        scanf("%f", &liste[choix - 1].prix);
        printf("Prix mis à jour pour '%s'.\n", liste[choix - 1].titre);
    } while (choix != 0);
}

// Fonction principale pour gérer le processus
void creerPrixCoursTD() {
    const char *fichierCoursTD = "cours_td.txt";
    const char *fichierPrixCoursTD = "prix_cours_td.txt";

    CoursTD listeCoursTD[100];
    int tailleCoursTD = lireCoursTD(fichierCoursTD, listeCoursTD, 100);

    if (tailleCoursTD == 0) {
        printf("Aucun cours ou TD trouvé dans le fichier '%s'.\n", fichierCoursTD);
        return;
    }

    // Ajouter ou modifier les prix
    ajouterPrixCoursTD(listeCoursTD, tailleCoursTD);

    // Écrire les données avec prix dans le fichier prix_cours_td.txt
    ecrirePrixCoursTD(fichierPrixCoursTD, listeCoursTD, tailleCoursTD);
}

// Fonction principale
int main() {
    creerPrixCoursTD();
    return 0;
}
