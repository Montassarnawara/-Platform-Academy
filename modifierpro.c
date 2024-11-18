#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour mettre à jour les permissions d'un professeur
void mettreAJourPermissions() {
    FILE *file, *tempFile, *permFile;
    char id[20], newPermissions[100];
    char line[200], storedId[20], storedPermissions[100];
    int found = 0;

    // Demander l'ID du professeur et les nouvelles permissions
    printf("Entrez l'ID du professeur : ");
    scanf("%s", id);
    printf("Entrez les nouvelles permissions (séparées par des espaces, ex: ajouter modifier supprimer) : ");
    getchar(); // Nettoyer le buffer
    fgets(newPermissions, sizeof(newPermissions), stdin);
    newPermissions[strcspn(newPermissions, "\n")] = '\0'; // Enlever le saut de ligne

    // Ouvrir les fichiers nécessaires
    file = fopen("authentification.txt", "r");
    tempFile = fopen("temp.txt", "w");
    permFile = fopen("prof_permissions.txt", "a");
    if (file == NULL || tempFile == NULL || permFile == NULL) {
        printf("Erreur : Impossible d'ouvrir un fichier.\n");
        return;
    }

    // Parcourir le fichier authentification.txt ligne par ligne
    while (fgets(line, sizeof(line), file) != NULL) {
        // Rechercher l'ID
        if (sscanf(line, "ID: %s\n", storedId) == 1 && strcmp(storedId, id) == 0) {
            // Copier l'ID
            fprintf(tempFile, "%s", line);
            found = 1;

            // Copier les lignes suivantes (Nom, Département, etc.)
            for (int i = 0; i < 2; i++) { // Supposons 2 lignes fixes avant les permissions
                if (fgets(line, sizeof(line), file) != NULL) {
                    fprintf(tempFile, "%s", line);
                }
            }

            // Ajouter les nouvelles permissions
            fprintf(tempFile, "Permissions: %s\n", newPermissions);

            // Ajouter ce professeur dans le fichier prof_permissions.txt si des permissions existent
            if (strlen(newPermissions) > 0) {
                fprintf(permFile, "ID: %s\nPermissions: %s\n", id, newPermissions);
                fprintf(permFile, "--------------------------------------\n");
            }
        } else {
            // Copier la ligne telle quelle si l'ID ne correspond pas
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);
    fclose(permFile);

    // Remplacer l'ancien fichier par le nouveau
    if (found) {
        remove("authentification.txt");
        rename("temp.txt", "authentification.txt");
        printf("Permissions mises à jour avec succès pour l'ID %s.\n", id);
    } else {
        remove("temp.txt");
        printf("ID %s introuvable dans le fichier.\n", id);
    }
}

// Fonction principale pour tester la mise à jour
int main() {
    mettreAJourPermissions();
    return 0;
}
