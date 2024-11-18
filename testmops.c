#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour hacher un mot de passe (simple, à améliorer en pratique)
void hashPassword(const char *password, char *hashedPassword) {
    unsigned int hash = 0;
    while (*password) {
        hash = (hash * 31) + *password++;
    }
    sprintf(hashedPassword, "%u", hash);
}

// Fonction d'authentification des professeurs
int authentifierProfesseur() {
    FILE *file;
    char id[20], password[50], hashedPassword[50];
    char line[200], storedId[20], storedHashedPassword[50];
    int found = 0;

    // Demander l'ID et le mot de passe du professeur
    printf("Entrez votre ID : ");
    scanf("%s", id);
    printf("Entrez votre mot de passe : ");
    scanf("%s", password);

    // Hacher le mot de passe fourni par l'utilisateur
    hashPassword(password, hashedPassword);

    // Ouvrir le fichier authentification.txt
    file = fopen("authentification.txt", "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier authentification.txt.\n");
        return 0; // Échec
    }

    // Rechercher l'ID et vérifier le mot de passe
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "ID: %s\n", storedId) == 1) {
            if (strcmp(storedId, id) == 0) {
                // L'ID est trouvé, rechercher le mot de passe
                while (fgets(line, sizeof(line), file) != NULL) {
                    if (sscanf(line, "Mot de passe (haché): %s\n", storedHashedPassword) == 1) {
                        if (strcmp(storedHashedPassword, hashedPassword) == 0) {
                            found = 1; // Authentification réussie
                        }
                        break;
                    }
                }
            }
        }
    }

    fclose(file);

    if (found) {
        printf("Authentification réussie. Bienvenue !\n");
        return 1; // Succès
    } else {
        printf("Authentification échouée. ID ou mot de passe incorrect.\n");
        return 0; // Échec
    }
}

// Exemple d'utilisation
int main() {
    if (authentifierProfesseur()) {
        printf("Vous êtes connecté au système.\n");
    } else {
        printf("Veuillez réessayer.\n");
    }
    return 0;
}
