#include <stdio.h>
#include <string.h>
void afficherListeCoursTD(const char *type) {
    FILE *file;
    char line[512];
    int afficherCours = 0; // 1 pour cours 0 pour td
    // verifer la choix d affiches
    if (strcmp(type, "cours") == 0) {
        afficherCours = 1;
    } else if (strcmp(type, "td") != 0) {
        printf("Erreur: Type invalide. Utilisez 'cours' ou 'td'.\n");
        return;
    }

    file = fopen("cours_td.txt", "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier.\n");
        return;
    }

    printf("Liste des %s:\n", afficherCours ? "Cours" : "TD");
    printf("---------------------------------------------\n");

    //lire les file est affiche
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "ID:", 3) == 0) {
            // lire les id
            int id;
            sscanf(line, "ID: %d", &id);
            fgets(line, sizeof(line), file); //lire la titre
            if ((afficherCours && strstr(line, "Cours") != NULL) ||
                (!afficherCours && strstr(line, "TD") != NULL)) {
                printf("%s", line); // affiche titre
                fgets(line, sizeof(line), file);
                printf("%s", line);
                fgets(line, sizeof(line), file); // lire la ligne entre les mataire
                printf("---------------------------------------------\n");
            } else {
                //skip les information si non pas le cas
                fgets(line, sizeof(line), file);
                fgets(line, sizeof(line), file);
            }
        }
    }


    fclose(file);
}

int main() {
    char choix[10];

    printf("Voulez-vous afficher la liste des 'cours' ou des 'td'? ");
    scanf("%s", choix);

    afficherListeCoursTD(choix);

    return 0;
}
