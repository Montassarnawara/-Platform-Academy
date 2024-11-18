#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour représenter un cours ou un TD avec prix
typedef struct {
    char id[20];
    char type[10];     // "Cours" ou "TD"
    char titre[50];
    char contenu[100];
    float prix;        // Prix associé
} CoursTD;

// Structure pour une pile (stack)
typedef struct Node {
    CoursTD coursTD;
    struct Node *next;
} Node;

// Fonction pour créer une pile vide
Node *creerPile() {
    return NULL;
}

// Fonction pour empiler un élément dans la pile
void empiler(Node **top, CoursTD coursTD) {
    Node *nouveau = (Node *)malloc(sizeof(Node));
    if (nouveau == NULL) {
        printf("Erreur : Mémoire insuffisante.\n");
        return;
    }
    nouveau->coursTD = coursTD;
    nouveau->next = *top;
    *top = nouveau;
}

// Fonction pour dépiler un élément de la pile
CoursTD depiler(Node **top) {
    if (*top == NULL) {
        printf("Erreur : La pile est vide.\n");
        CoursTD vide = {"", "", "", "", 0.0};
        return vide;
    }
    Node *temp = *top;
    CoursTD coursTD = temp->coursTD;
    *top = temp->next;
    free(temp);
    return coursTD;
}

// Fonction pour vérifier si la pile est vide
int estVide(Node *top) {
    return top == NULL;
}

// Fonction pour lire les cours/TD depuis le fichier cours_td.txt
int lireCoursTD(const char *fichier, CoursTD *liste, int tailleMax) {
    FILE *file = fopen(fichier, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", fichier);
        return 0;
    }

    int count = 0;
    char ligne[200];
    while (fgets(ligne, sizeof(ligne), file) && count < tailleMax) {
        if (strncmp(ligne, "ID:", 3) == 0) {
            sscanf(ligne, "ID: %s", liste[count].id);
        } else if (strncmp(ligne, "Type:", 5) == 0) {
            sscanf(ligne, "Type: %s", liste[count].type);
        } else if (strncmp(ligne, "Titre:", 6) == 0) {
            sscanf(ligne, "Titre: %[^\n]", liste[count].titre);
        } else if (strncmp(ligne, "Contenu:", 8) == 0) {
            sscanf(ligne, "Contenu: %[^\n]", liste[count].contenu);
        } else if (strncmp(ligne, "--------------------", 20) == 0) {
            liste[count].prix = 0.0; // Prix sera ajouté depuis le fichier prix_cours_td
            count++;
        }
    }

    fclose(file);
    return count;
}

// Fonction pour lire les prix des cours/TD depuis prix_cours_td.txt
void ajouterPrixDepuisFichier(const char *fichier, CoursTD *liste, int taille) {
    FILE *file = fopen(fichier, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", fichier);
        return;
    }

    char ligne[200];
    while (fgets(ligne, sizeof(ligne), file)) {
        char id[20];
        float prix;
        if (sscanf(ligne, "ID: %s\n", id) == 1) {
            // Trouver le cours/TD correspondant par ID
            for (int i = 0; i < taille; i++) {
                if (strcmp(liste[i].id, id) == 0) {
                    // Lire le prix
                    while (fgets(ligne, sizeof(ligne), file)) {
                        if (strncmp(ligne, "Prix:", 5) == 0) {
                            sscanf(ligne, "Prix: %f\n", &prix);
                            liste[i].prix = prix;
                            break;
                        }
                    }
                }
            }
        }
    }

    fclose(file);
}

// Fonction pour afficher les cours/TD et permettre à l'utilisateur de sélectionner
void selectionnerCours(Node **pile, CoursTD *liste, int taille) {
    int choix;
    do {
        printf("\nListe des cours et TD disponibles :\n");
        for (int i = 0; i < taille; i++) {
            printf("%d. ID: %s | Type: %s | Titre: %s | Prix: %.2f\n",
                   i + 1, liste[i].id, liste[i].type, liste[i].titre, liste[i].prix);
        }

        printf("\nEntrez le numéro du cours/TD à sélectionner (ou 0 pour terminer) : ");
        scanf("%d", &choix);



        if (choix < 1 || choix > taille) {
            if (choix != 0) {
                printf("Choix invalide.\n");
            }
            continue;
        }

        // Empiler le cours/TD sélectionné
        empiler(pile, liste[choix - 1]);
        printf("Cours/TD '%s' ajouté à votre sélection.\n", liste[choix - 1].titre);
    } while (choix != 0);
}

// Fonction pour calculer et afficher le total
void afficherTotal(Node *pile) {
    float total = 0.0;
    printf("\nRésumé des cours/TD sélectionnés :\n");
    while (!estVide(pile)) {
        CoursTD coursTD = depiler(&pile);
        printf("- ID: %s | Type: %s | Titre: %s | Prix: %.2f\n",
               coursTD.id, coursTD.type, coursTD.titre, coursTD.prix);
        total += coursTD.prix;
    }
    printf("\nTotal à payer : %.2f\n", total);
}

// Fonction principale
int main() {
    const char *fichierCoursTD = "cours_td.txt";
    const char *fichierPrixCoursTD = "prix_cours_td.txt";

    CoursTD listeCoursTD[100];
    int tailleCoursTD = lireCoursTD(fichierCoursTD, listeCoursTD, 100);

    if (tailleCoursTD == 0) {
        printf("Aucun cours ou TD trouvé dans le fichier '%s'.\n", fichierCoursTD);
        return 0;
    }

    // Ajouter les prix depuis prix_cours_td.txt
    ajouterPrixDepuisFichier(fichierPrixCoursTD, listeCoursTD, tailleCoursTD);

    // Gérer les cours/TD sélectionnés avec une pile
    Node *pile = creerPile();

    // Permettre à l'utilisateur de sélectionner des cours/TD
    selectionnerCours(&pile, listeCoursTD, tailleCoursTD);

    // Afficher le total à payer
    afficherTotal(pile);

    return 0;
}
