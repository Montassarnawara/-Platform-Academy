#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour hacher un mot de passe
void hashPassword(const char *password, char *hashedPassword) {
    unsigned int hash = 0;
    while (*password) {
        hash = (hash * 31) + *password++;
    }
    sprintf(hashedPassword, "%u", hash);
}

// Fonction pour ajouter un professeur avec mot de passe hach�
void ajouterProfesseur() {
    FILE *file;
    char id[20], nom[50], departement[50], password[50], hashedPassword[50];

    // Saisie des informations du professeur
    printf("Entrez l'ID du professeur : ");
    scanf("%s", id);
    printf("Entrez le nom et pr�nom du professeur : ");
    getchar(); // Nettoyage du buffer
    fgets(nom, sizeof(nom), stdin);
    nom[strcspn(nom, "\n")] = '\0'; // Enlever le saut de ligne
    printf("Entrez le departement : ");
    fgets(departement, sizeof(departement), stdin);
    departement[strcspn(departement, "\n")] = '\0';

    printf("Entrez le mot de passe : ");
    scanf("%s", password);

    // Hachage du mot de passe
    hashPassword(password, hashedPassword);

    // Ouverture du fichier en mode ajout
    file = fopen("authentification.txt", "a");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier authentification.txt.\n");
        return;
    }

    // �criture des donn�es dans le fichier
    fprintf(file, "ID: %s\n", id);

    fprintf(file, "Nom: %s\n", nom);
    fprintf(file, "D�partement: %s\n", departement);
    fprintf(file, "Mot de passe (hach�): %s\n", hashedPassword);
    fprintf(file, "--------------------------------------\n");

    fclose(file);
    printf("Professeur ajout� avec succ�s !\n");
}

// Fonction principale pour tester la cr�ation
int main() {
    char choix;
    do  {
    ajouterProfesseur();
    printf("Voulez-vous ajouter un autre professeur? (o/n): ");
        scanf(" %c", &choix);
        getchar();
        } while (choix == 'o' || choix == 'O');


    return 0;
}
