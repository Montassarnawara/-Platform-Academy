#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAILLE_MAX 256 // Taille maximale pour les chaînes
#define MAX_TESTS 100  // Nombre maximal de tests dans le fichier

typedef struct {
    int id;
    char titre[TAILLE_MAX];
    char url[TAILLE_MAX];
    char reponse[TAILLE_MAX];
    char dateFin[TAILLE_MAX]; // Date de fin sous forme de chaîne
} Test;

// Fonction pour lire tous les tests depuis un fichier unique
int lireTests(const char *nomFichier, Test tests[], int *nombreTests) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        return 0; // Erreur si le fichier n'existe pas
    }

    char ligne[TAILLE_MAX];
    int index = -1;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        // Nouvelle entrée de test
        if (strncmp(ligne, "ID:", 3) == 0) {
            index++;
            sscanf(ligne, "ID: %d", &tests[index].id);
        } else if (strncmp(ligne, "Titre:", 6) == 0) {
            sscanf(ligne, "Titre: %[^\n]", tests[index].titre);
        } else if (strncmp(ligne, "URL:", 4) == 0) {
            sscanf(ligne, "URL: %[^\n]", tests[index].url);
        } else if (strncmp(ligne, "Réponse:", 8) == 0) {
            sscanf(ligne, "Réponse: %[^\n]", tests[index].reponse);
        } else if (strncmp(ligne, "Date de fin:", 12) == 0) {
            sscanf(ligne, "Date de fin: %[^\n]", tests[index].dateFin);
        }
    }

    fclose(fichier);
    *nombreTests = index + 1;
    return 1; // Succès
}

// Fonction pour comparer deux dates au format YYYY-MM-DD
int dateDepassee(const char *dateFin) {
    // Obtenir la date actuelle
    time_t t = time(NULL);
    struct tm *dateActuelle = localtime(&t);

    // Extraire les composants de la date actuelle
    int anneeActuelle = dateActuelle->tm_year + 1900; // Année depuis 1900
    int moisActuel = dateActuelle->tm_mon + 1;        // Mois [0-11]
    int jourActuel = dateActuelle->tm_mday;          // Jour du mois

    // Extraire les composants de la date de fin
    int anneeFin, moisFin, jourFin;
    sscanf(dateFin, "%d-%d-%d", &anneeFin, &moisFin, &jourFin);

    // Comparer les dates
    if (anneeActuelle > anneeFin) return 1;          // Date actuelle après l'année de fin
    if (anneeActuelle < anneeFin) return 0;          // Date actuelle avant l'année de fin
    if (moisActuel > moisFin) return 1;              // Date actuelle après le mois de fin
    if (moisActuel < moisFin) return 0;              // Date actuelle avant le mois de fin
    if (jourActuel > jourFin) return 1;              // Date actuelle après le jour de fin
    return 0;                                        // Sinon, la date actuelle est avant ou égale
}

// Fonction pour afficher un test
void afficherTest(const Test *test) {
    printf("ID: %d\n", test->id);
    printf("Titre: %s\n", test->titre);
    printf("URL: %s\n", test->url);
    printf("Date de fin: %s\n", test->dateFin);
}

// Fonction principale
int main() {
    Test tests[MAX_TESTS];
    int nombreTests = 0;

    // Lire les tests depuis le fichier
    if (!lireTests("tests.txt", tests, &nombreTests)) {
        printf("Erreur : impossible de lire le fichier des tests.\n");
        return 1;
    }

    // Afficher tous les tests disponibles
    printf("Tests disponibles :\n");
    for (int i = 0; i < nombreTests; i++) {
        afficherTest(&tests[i]);
        printf("-----------------------\n");
    }

    // Permettre à l'étudiant de choisir un test
    int choix;
    printf("Entrez l'ID du test que vous voulez choisir : ");
    scanf("%d", &choix);

    // Trouver le test correspondant
    Test *testChoisi = NULL;
    for (int i = 0; i < nombreTests; i++) {
        if (tests[i].id == choix) {
            testChoisi = &tests[i];
            break;
        }
    }

    if (testChoisi == NULL) {
        printf("Test avec l'ID %d introuvable.\n", choix);
        return 1;
    }
    // Vérifier si la date de fin est dépassée
    if (dateDepassee(testChoisi->dateFin)) {
        printf("La date de fin du test \"%s\" est dépassée. Vous ne pouvez pas répondre à ce test.\n", testChoisi->titre);
    } else {
        printf("Vous avez sélectionné le test \"%s\".\n", testChoisi->titre);
        printf("Lien pour télécharger la réponse : %s\n", testChoisi->reponse);
        printf("Réponse enregistrée avec succès !\n");
    }

    return 0;
}
