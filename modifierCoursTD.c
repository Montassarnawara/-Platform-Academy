#include <stdio.h>
#include <string.h>

// modifer la contenu d un file
int modifierCoursTD(int id, char nouveauTitre[], char nouveauContenu[]) {
    FILE *file, *tempFile;
    char *filename = "cours_td.txt";
    char *tempFilename = "temp_cours_td.txt";
    char line[512];
    int currentId = 0;
    int found = 0;

    //ovrire le file
    file = fopen(filename, "r");
    tempFile = fopen(tempFilename, "w");

    if (file == NULL || tempFile == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier.\n");
        return 0;
    }

    // lire de file principale
    while (fgets(line, sizeof(line), file) != NULL) {
        //recherche par id
        if (sscanf(line, "ID: %d", &currentId) == 1) {
            if (currentId == id) {
                found = 1;
                fprintf(tempFile, "ID: %d\n", id);
                fprintf(tempFile, "Titre: %s\n", nouveauTitre);
                fprintf(tempFile, "Contenu: %s\n", nouveauContenu);

                //suiv le nom et le titre ancienn e
                fgets(line, sizeof(line), file);
                fgets(line, sizeof(line), file);
                fgets(line, sizeof(line), file);
                continue;
            }
        }
        //  copie le info dans le file inter
        fputs(line, tempFile);
    }


    fclose(file);
    fclose(tempFile);

    // reformuler le file si exite
    if (found) {
        remove(filename);
        rename(tempFilename, filename);
        printf("Modification réussie: Cours/TD avec ID %d a été modifié avec succès.\n", id);
        return 1;
    } else {
        remove(tempFilename); //efface le file inter si l id nexiste pas
        printf("Erreur: Cours/TD avec ID %d introuvable.\n", id);
        return 0;
    }
}

// exp
int main() {
    int ID;
    char nouveauTitre[100];
    char nouveauContenu[500];

    // info enterre
    printf("Entrez l'ID du Cours/TD à modifier: ");
    scanf("%d", &ID);

    getchar();

    printf("Entrez le nouveau titre: ");
    fgets(nouveauTitre, sizeof(nouveauTitre), stdin);
    nouveauTitre[strcspn(nouveauTitre, "\n")] = '\0';

    printf("Entrez le nouveau contenu: ");
    fgets(nouveauContenu, sizeof(nouveauContenu), stdin);
    nouveauContenu[strcspn(nouveauContenu, "\n")] = '\0';

    // fct de modif td
    if (modifierCoursTD(ID, nouveauTitre, nouveauContenu)) {
        printf("Le Cours/TD a été modifié avec succès !\n");
    } else {
        printf("Une erreur s'est produite lors de la modification.\n");
    }

    return 0;
}
