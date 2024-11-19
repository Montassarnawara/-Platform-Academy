#include <stdio.h>
#include <string.h>
// def des fct de ajouter
int ajouterCoursTD(int ID ,int  type, char titre[], char contenu[]) {
    FILE *file;
    char *filename = "cours_td.txt";
    // donner le type de donnee
    char *typeContent = (type == 1) ? "Cours" : "TD";
    // ouvrire un file mode ajoute
    file = fopen(filename, "a");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier.\n");
        return 0;
    }
    //ecrire les donner dans le file
    fprintf(file,"ID: %d\n",ID);
    fprintf(file, "Type: %s\n", typeContent);
    fprintf(file, "Titre: %s\n", titre);
    fprintf(file, "Contenu: %s\n", contenu);
    fprintf(file, "--------------------\n"); //space pour buen mettre les donner
    fclose(file);
    printf("Ajout réussi : %s ajouté avec succès.\n", typeContent);
    return 1;
}

int main() {
    char choix ;
    int ID;
    int type;
    char titre[100];
    char contenu[500];
    do {
    printf("Entre un ID pour le donner : ");
    scanf("%d", &ID);
    printf("Entrez le type de contenu (1 pour Cours, 2 pour TD): ");
    scanf("%d", &type);
    getchar(); // efaase le enter de sacnf pour le fin de ligne
    printf("Entrez le titre: ");
    fgets(titre, sizeof(titre), stdin);
    titre[strcspn(titre, "\n")] = '\0'; // efface la novelle ligne
    printf("Entrez le contenu: ");
    fgets(contenu, sizeof(contenu), stdin);
    contenu[strcspn(contenu, "\n")] = '\0'; //efface le novelle ligen la fin
    ajouterCoursTD(ID,type, titre, contenu);  // ajouet le contenu
    printf("Voulez-vous ajouter un autre cours? (o/n): ");
        scanf(" %c", &choix);
        getchar();
       } while (choix == 'o' || choix == 'O');


    return 0;
}
