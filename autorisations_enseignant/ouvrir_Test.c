 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char url[100];
    char nomMatiere[100];
    char titreTest[50];
    char dateFin[20];
} Test;

int ouvrirTest(int id) {
    FILE *file, *testFile;
    char *filename = "cours_td.txt";
    char *testFilename = "tests.txt";
    char line[512];
    int currentId = 0;
    int found = 0;
    Test test;


    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des cours.\n");
        return 0;
    }

    //verifer la existence de cours
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "ID: %d", &currentId) == 1 && currentId == id) {
            found = 1;
            fgets(line, sizeof(line), file); //lire la titre
            strncpy(test.nomMatiere, line + 7, sizeof(test.nomMatiere)); //retrner la matiere
            test.nomMatiere[strcspn(test.nomMatiere, "\n")] = '\0';
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("Erreur: Aucun cours ou TD trouvé avec l'ID %d.\n", id);
        return 0;
    }


    printf("Entrez le titre du test (alphanumérique): ");
    fgets(test.titreTest, sizeof(test.titreTest), stdin);
    test.titreTest[strcspn(test.titreTest, "\n")] = '\0';

    printf("Entrez l'URL du test: ");
    fgets(test.url, sizeof(test.url), stdin);
    test.url[strcspn(test.url, "\n")] = '\0';

    printf("Entrez la date de fin du test (format: YYYY-MM-DD): ");
    fgets(test.dateFin, sizeof(test.dateFin), stdin);
    test.dateFin[strcspn(test.dateFin, "\n")] = '\0';

    // enrigestre dans "tests.txt"
    testFile = fopen(testFilename, "a");
    if (testFile == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier des tests.\n");
        return 0;
    }

    fprintf(testFile, "ID: %d\n", id);
    fprintf(testFile, "Matière: %s\n", test.nomMatiere);
    fprintf(testFile, "Titre: %s\n", test.titreTest);
    fprintf(testFile, "URL: %s\n", test.url);
    fprintf(testFile, "Date de fin: %s\n", test.dateFin);
    fprintf(testFile, "----------------------------------------\n");

    fclose(testFile);

    printf("Le test a été créé avec succès pour le cours/TD avec l'ID %d.\n", id);
    return 1;
}

// exp
int main() {
    int id;

    printf("Entrez l'ID du cours/TD pour lequel vous voulez ouvrir un test: ");
    scanf("%d", &id);
    getchar();

    if (ouvrirTest(id)) {
        printf("Le test a été ouvert et enregistré avec succès.\n");
    } else {
        printf("Une erreur s'est produite lors de l'ouverture du test.\n");
    }

    return 0;
}
