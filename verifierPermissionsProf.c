#include <stdio.h>
#include <string.h>

// fct de verifer le lacces de prof
int verifierPermissionsProf(int id, const char *action) {
    FILE *file;
    char line[512];
    int currentId = 0;
    char permissions[100];
    int hasPermission = 0;

    // ouvrire le file
    file = fopen("prof_permissions.txt", "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des permissions.\n");
        return 0;
    }

    //lire et verifer l existence de id
    while (fgets(line, sizeof(line), file) != NULL) {
        // recherche l id de prof
        if (sscanf(line, "ID: %d", &currentId) == 1 && currentId == id) {
            fgets(line, sizeof(line), file);
            sscanf(line, "Permissions: %[^\n]", permissions);

            // verifer l existnece de le choix dans la liste
            if (strstr(permissions, action) != NULL) {
                hasPermission = 1;
            }
            break;
        }
    }

    fclose(file);

    if (!hasPermission) {
        printf("Erreur: L'enseignant avec ID %d n'a pas la permission pour '%s'.\n", id, action);
        return 0;
    }

    printf("Permission accordée: L'enseignant avec ID %d peut effectuer l'action '%s'.\n", id, action);
    return 1;
}

// exp
int main() {
    int id;
    char action[20];


    printf("Entrez l'ID de l'enseignant: ");
    scanf("%d", &id);
    getchar();

    printf("Entrez l'action (ajouter/modifier/supprimer): ");
    fgets(action, sizeof(action), stdin);
    action[strcspn(action, "\n")] = '\0';


    if (verifierPermissionsProf(id, action)) {
        printf("Action autorisée.\n");
    } else {
        printf("Action non autorisée.\n");
    }

    return 0;
}
