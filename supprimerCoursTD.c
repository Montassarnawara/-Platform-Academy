#include <stdio.h>
#include <string.h>

// def de fct suprisson
int supprimerCoursTD(int id) {
    FILE *file, *tempFile;
    char *filename = "cours_td.txt";
    char *tempFilename = "temp_cours_td.txt";
    char line[512];
    int currentId = 0;
    int found = 0;

    // ouvrire les file principale et inter
    file = fopen(filename, "r");
    tempFile = fopen(tempFilename, "w");

    if (file == NULL || tempFile == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier.\n");
        return 0;
    }

    //lire les donner
    while (fgets(line, sizeof(line), file) != NULL) {
        //test l exstonce de id
        if (sscanf(line, "ID: %d", &currentId) == 1) {
            if (currentId == id) {
                found = 1;

                // skip
                fgets(line, sizeof(line), file);
                fgets(line, sizeof(line), file);
                fgets(line, sizeof(line), file);
                continue;
            }
        }
        // copie ligne vers la file inter
        fputs(line, tempFile);
    }


    fclose(file);
    fclose(tempFile);

    // chonge la file principale si l id est existe
    if (found) {
        remove(filename);
        rename(tempFilename, filename);
        printf("Suppression réussie: Cours/TD avec ID %d a été supprimé avec succès.\n", id);
        return 1;
    } else {
        remove(tempFilename); // efface l file inter si l id ne existe pas
        printf("Erreur: Cours/TD avec ID %d introuvable.\n", id);
        return 0;
    }
}

// exp
int main() {
    int id;
    printf("Entrez l'ID du Cours/TD à supprimer: ");
    scanf("%d", &id);

    if (supprimerCoursTD(id)) {
        printf("Le Cours/TD a été supprimé avec succès !\n");
    } else {
        printf("Une erreur s'est produite lors de la suppression.\n");
    }

    return 0;
}
