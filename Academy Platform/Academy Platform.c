 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include<stdbool.h>
#include<conio.h>




// foonction etu
void ajouter_etd()
{
    FILE *file;
    if((file=fopen("fiche_etd.txt","a"))==NULL){
        printf("impossible d'ouvrir le fichier ");
    }
    char id[20], NomPre[50], gr[50], password[50], hashedPassword[50];
    printf("entrer le ID de l'etudiant:\n");
    scanf("%s",&id);
    printf("entrer le nom et le prenom de l'etudiant:\n");
    getchar();
    fgets(NomPre,sizeof(NomPre),stdin);
    NomPre[strcspn(NomPre, "\n")] = '\0';
    printf("le groupe de l'etudiant:\n");
    scanf("%s",&gr);
    printf("Entrez le mot de passe : ");
    scanf("%s", password);
    hashPassword(password,hashedPassword);
    fprintf(file,"ID:  %s\n",id);
    fprintf(file,"Nom et prenom: %s\n",NomPre);
    fprintf(file,"Groupe: %s\n",gr);
    fprintf(file,"Mot de passe (haché): %s\n",hashedPassword);
    fprintf(file, "--------------------------------------\n");
    fclose(file);
    printf("etudiant ajouté avec succès !\n");
}
//securité etudiant

// Fonction d'authentification des etudiant

int authentifier() {
    FILE *file;
    char id[20], password[50], hashedPassword[50];
    char line[200], storedId[20], storedHashedPassword[50];
    int found = 0;
    int i =0 ;
    char c ;

    // Demander l'ID et le mot de passe du etd
    printf("Entrez votre ID : ");
    scanf("%s", id);
    printf("Entrez votre mot de passe : ");
    while (i < sizeof(password) - 1) {
        c = getch();
        if (c == '\r') {
            break;
        }
        password[i++] = c;
        printf("*");
    }
    password[i] = '\0';

    // Hacher le mot de passe fourni par l'utilisateur
    hashPassword(password, hashedPassword);

    // Ouvrir le fichier authentification.txt
    file = fopen("fiche_etd.txt", "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier .\n");
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
//hellllooooooooo


void afficher_cour_td() {
    int ID;
    FILE *file;
    printf("Saisir l'identifiant du CourTD à afficher :\n");
    scanf("%d", &ID);
system("cls");
    file = fopen("cours_td.txt", "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return;
    }

    bool test = false;
    int id;
    char ch[256];
    char titre[256] = "";
    char contenu[1024] = "";
    bool trouve = false;

    while (fgets(ch, sizeof(ch), file)) {

        if (strncmp(ch, "ID:", 3) == 0) {
            sscanf(ch, "ID: %d", &id);
            test = (id == ID);
            if (test) {
                trouve = true; //
                strcpy(titre, "");
                strcpy(contenu, "");
            }
        } else if (test && strncmp(ch, "Titre:", 6) == 0) {

            sscanf(ch + 6, "%[^\n]", titre);
        } else if (test && strncmp(ch, "Contenu:", 8) == 0) {

            sscanf(ch + 8, "%[^\n]", contenu);
        }
    }

    fclose(file);

    if (trouve) {
        printf("Titre : %s\n", titre);
        printf("Contenu : %s\n", contenu);
    } else {
        printf("Aucun CourTD trouvé avec l'ID %d.\n", ID);
    }
}
// fonction de verification
// fct de verifer le lacces de prof

int verifierPermissionsProf(int id0, const char *action) {


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
        if (sscanf(line, "ID: %d", &currentId) == 1 && currentId == id0) {
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
        printf("Erreur: L'enseignant avec ID %d n'a pas la permission pour '%s'.\n", id0, action);
        return 0;
    }

    printf("Permission accordée: L'enseignant avec ID %d peut effectuer l'action '%s'.\n", id0, action);
    return 1;
}
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
    int i=0 ;
    char c;
    // Demander l'ID et le mot de passe du professeur
    printf("Entrez votre ID : ");
    scanf("%s", id);
   printf("Entrez votre mot de passe : ");
    while (i < sizeof(password) - 1) {
        c = getch(); // Lire le caractère sans l'afficher
        if (c == '\r') {
            break;
        }
        password[i++] = c;
        printf("*");
    }
    password[i] = '\0';









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


// ==== Fonctions pour l'étudiant ====
#define TAILLE_MAX 256 // Taille maximale pour les chaînes
#define MAX_TESTS 100  // Nombre maximal de tests dans le fichier

typedef struct {

    int id;
    char titre[TAILLE_MAX];
    char url[TAILLE_MAX];
    char reponse[TAILLE_MAX];
    char dateFin[TAILLE_MAX]; // Date de fin sous forme de chaîne
} Test;
// Fonction pour lire tous les tests depuis un fichier unique

int lireTests(const char *nomFichier, Test tests[], int *nombreTests) {

    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        return 0; // Erreur si le fichier n'existe pas
    }

    char ligne[TAILLE_MAX];
    int index = -1;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        // Nouvelle entrée de test
        if (strncmp(ligne, "ID:", 3) == 0) {
            index++;
            sscanf(ligne, "ID: %d", &tests[index].id);
        } else if (strncmp(ligne, "Titre:", 6) == 0) {
            sscanf(ligne, "Titre: %[^\n]", tests[index].titre);
        } else if (strncmp(ligne, "URL:", 4) == 0) {
            sscanf(ligne, "URL: %[^\n]", tests[index].url);
        } else if (strncmp(ligne, "Réponse:", 8) == 0) {
            sscanf(ligne, "Réponse: %[^\n]", tests[index].reponse);
        } else if (strncmp(ligne, "Date de fin:", 12) == 0) {
            sscanf(ligne, "Date de fin: %[^\n]", tests[index].dateFin);
        }
    }

    fclose(fichier);
    *nombreTests = index + 1;
    return 1; // Succès
}
// Fonction pour comparer deux dates au format YYYY-MM-DD

int dateDepassee(const char *dateFin) {

    // Obtenir la date actuelle
    time_t t = time(NULL);
    struct tm *dateActuelle = localtime(&t);

    // Extraire les composants de la date actuelle
    int anneeActuelle = dateActuelle->tm_year + 1900; // Année depuis 1900
    int moisActuel = dateActuelle->tm_mon + 1;        // Mois [0-11]
    int jourActuel = dateActuelle->tm_mday;          // Jour du mois

    // Extraire les composants de la date de fin
    int anneeFin, moisFin, jourFin;
    sscanf(dateFin, "%d-%d-%d", &anneeFin, &moisFin, &jourFin);

    // Comparer les dates
    if (anneeActuelle > anneeFin) return 1;          // Date actuelle après l'année de fin
    if (anneeActuelle < anneeFin) return 0;          // Date actuelle avant l'année de fin
    if (moisActuel > moisFin) return 1;              // Date actuelle après le mois de fin
    if (moisActuel < moisFin) return 0;              // Date actuelle avant le mois de fin
    if (jourActuel > jourFin) return 1;              // Date actuelle après le jour de fin
    return 0;                                        // Sinon, la date actuelle est avant ou égale
}
// Fonction pour afficher un test

void afficherTest(const Test *test) {
    printf("ID: %d\n", test->id);
    printf("Titre: %s\n", test->titre);
    printf("URL: %s\n", test->url);
    printf("Date de fin: %s\n", test->dateFin);
}
//affiher

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
                printf("\nID: %d \n", id);
                fgets(line, sizeof(line), file);
                printf("%s", line); // affiche titre
                fgets(line, sizeof(line), file);


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

void ajouterProfesseur() {

    FILE *file;
    char id[20], nom[50], departement[50], password[50], hashedPassword[50];

    // Saisie des informations du professeur
    printf("Entrez l'ID du professeur : ");
    scanf("%s", id);
    printf("Entrez le nom et prénom du professeur : ");
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

    // Écriture des données dans le fichier
    fprintf(file, "ID: %s\n", id);

    fprintf(file, "Nom: %s\n", nom);
    fprintf(file, "Département: %s\n", departement);
    fprintf(file, "Mot de passe (haché): %s\n", hashedPassword);
    fprintf(file, "--------------------------------------\n");

    fclose(file);
    printf("Professeur ajouté avec succès !\n");
}
// Structure pour représenter un cours ou un TD

typedef struct {

    char id[20];
    char type[10];     // "Cours" ou "TD"
    char titre[50];
    char contenu[100];
    float prix;        // Prix à ajouter
} CoursTD;
// Fonction pour lire les cours/TD à partir du fichier cours_td.txt

int lireCoursTD(const char *fichier, CoursTD *liste, int tailleMax) {
    FILE *file = fopen(fichier, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", fichier);
        return 0;
    }
    int count = 0;
    char ligne[200];
    while (fgets(ligne, sizeof(ligne), file) && count < tailleMax) {
        // Lire les données ligne par ligne
        if (strncmp(ligne, "ID:", 3) == 0) {
            sscanf(ligne, "ID: %s", liste[count].id);
        } else if (strncmp(ligne, "Type:", 5) == 0) {
            sscanf(ligne, "Type: %s", liste[count].type);
        } else if (strncmp(ligne, "Titre:", 6) == 0) {
            sscanf(ligne, "Titre: %[^\n]", liste[count].titre);
        } else if (strncmp(ligne, "Contenu:", 8) == 0) {
            sscanf(ligne, "Contenu: %[^\n]", liste[count].contenu);
        } else if (strncmp(ligne, "Prix:", 5) == 0) {
            sscanf(ligne, "Prix: %f", &liste[count].prix);
        } else if (strncmp(ligne, "--------------------", 20) == 0) {
            // Passer à l'entrée suivante
            count++;
        }
    }

    fclose(file);
    return count;
}

// Fonction pour écrire les cours/TD avec prix dans cours_td.txt
void ecrireCoursTD(const char *fichier, CoursTD *liste, int taille) {
    FILE *file = fopen(fichier, "w");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s.\n", fichier);
        return;
    }

    for (int i = 0; i < taille; i++) {
        fprintf(file, "ID: %s\nType: %s\nTitre: %s\nContenu: %s\nPrix: %.2f\n--------------------\n",
                liste[i].id, liste[i].type, liste[i].titre, liste[i].contenu, liste[i].prix);
    }

    fclose(file);
    printf("Fichier '%s' mis à jour avec succès.\n", fichier);
}

// Fonction pour ajouter ou modifier les prix des cours/TD
void ajouterPrixCoursTD(CoursTD *liste, int taille) {
    printf("\nListe des cours et TD disponibles :\n");
    for (int i = 0; i < taille; i++) {
        printf("%d. ID: %s | Type: %s | Titre: %s | Prix actuel: %.2f\n",
               i + 1, liste[i].id, liste[i].type, liste[i].titre, liste[i].prix);
    }

    int choix;
    do {

        printf("\nEntrez le numéro du cours/TD auquel vous souhaitez ajouter un prix (ou 0 pour terminer) : ");
        scanf("%d", &choix);

        if (choix < 1 || choix > taille) {
            if (choix != 0) {
                printf("Choix invalide.\n");
            }
            continue;
        }

        printf("Entrez le nouveau prix pour '%s' : ", liste[choix - 1].titre);

        scanf("%f", &liste[choix - 1].prix);

        printf("Prix mis à jour pour '%s'.\n", liste[choix - 1].titre);

    } while (choix != 0);
}

// Fonction principale pour gérer le processus
void creerPrixCoursTD() {
    const char *fichierCoursTD = "cours_td.txt";

    CoursTD listeCoursTD[100];
    int tailleCoursTD = lireCoursTD(fichierCoursTD, listeCoursTD, 100);

    if (tailleCoursTD == 0) {
        printf("Aucun cours ou TD trouvé dans le fichier '%s'.\n", fichierCoursTD);
        return;
    }
system("cls");
    // Ajouter ou modifier les prix
    ajouterPrixCoursTD(listeCoursTD, tailleCoursTD);
system("cls");
    // Écrire les données avec prix dans le fichier cours_td.txt (mise à jour)
    ecrireCoursTD(fichierCoursTD, listeCoursTD, tailleCoursTD);
}

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


    while (fgets(line, sizeof(line), file) != NULL) {
        // Rechercher l'ID
        if (sscanf(line, "ID: %s\n", storedId) == 1 && strcmp(storedId, id) == 0) {
            // Copier l'ID
            fprintf(tempFile, "%s", line);
            found = 1;
            for (int i = 0; i < 2; i++) {
                if (fgets(line, sizeof(line), file) != NULL) {
                    fprintf(tempFile, "%s", line);
                }
            }

            // Ajouter les nouvelles autorisation
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
    if (found) {
        remove("authentification.txt");
        rename("temp.txt", "authentification.txt");
        printf("Permissions mises à jour avec succès pour l'ID %s.\n", id);
    } else {
        remove("temp.txt");
        printf("ID %s introuvable dans le fichier.\n", id);
    }
}




// ==== Fonctions pour le professeur ====


typedef struct {


    char url[100];
    char nomMatiere[100];
    char titreTest[50];
    char dateFin[20];
} TestE;
int ouvrirTest(int id) {

    FILE *file, *testFile;
    char *filename = "cours_td.txt";
    char *testFilename = "tests.txt";
    char line[512];
    int currentId = 0;
    int found = 0;
    TestE test;


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
  // 2 modifier

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
  // 3 suprimer
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

void ajouterTest() {

    char titre[100];
    char date[20];

    printf("\n--- Ajouter un nouveau test ---\n");
    printf("Entrez le titre du test : ");
    scanf(" %[^\n]", titre);

    printf("Entrez la date de fin du test (AAAA-MM-JJ) : ");
    scanf(" %[^\n]", date);

    FILE *fichier = fopen("tests.txt", "a");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier des tests.\n");
        return;
    }

    fprintf(fichier, "Titre : %s | Date de fin : %s\n", titre, date);
    fclose(fichier);

    printf("Le test a été ajouté avec succès.\n");
}
void revoirReponses() {

    FILE *fichier = fopen("reponses.txt", "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier des réponses.\n");
        return;
    }

    char ligne[256];
    printf("\n--- Révision des réponses des étudiants ---\n");
    while (fgets(ligne, sizeof(ligne), fichier)) {
        printf("%s", ligne);
    }
    fclose(fichier);
}
// ==== Fonctions pour l'utilisateur externe ====

// Structure pour représenter un cours ou un TD avec prix

typedef struct {

    char id[20];
    char type[10];     // "Cours" ou "TD"
    char titre[50];
    char contenu[100];
    float prix;        // Prix associé
} CoursTDa;
// Structure pour une pile (stack)

typedef struct Node {

    CoursTDa coursTDa;
    struct Node *next;
} Node;
// Fonction pour créer une pile vide

Node *creerPile() {

    return NULL;
}
// Fonction pour empiler un élément dans la pile

void empiler(Node **top, CoursTDa coursTDa) {

    Node *nouveau = (Node *)malloc(sizeof(Node));
    if (nouveau == NULL) {
        printf("Erreur : Mémoire insuffisante.\n");
        return;
    }
    nouveau->coursTDa = coursTDa;
    nouveau->next = *top;
    *top = nouveau;
}
// Fonction pour dépiler un élément de la pile

CoursTDa depiler(Node **top) {

    if (*top == NULL) {
        printf("Erreur : La pile est vide.\n");
        CoursTDa vide = {"", "", "", "", 0.0};
        return vide;
    }
    Node *temp = *top;
    CoursTDa coursTDa = temp->coursTDa;
    *top = temp->next;
    free(temp);
    return coursTDa;
}
// Fonction pour vérifier si la pile est vide

int estVide(Node *top) {

    return top == NULL;
}

// Fonction pour lire les prix des cours/TD depuis  cours_td.txt

void ajouterPrixDepuisFichier(const char *fichier, CoursTDa *liste, int taille) {

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

void selectionnerCours(Node **pile, CoursTDa *liste, int taille) {

    int choix;
    do {

        printf("\nListe des cours et TD disponibles :\n");
system("cls");
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
    char mail[100];
    int pay ;
    printf("\nRésumé des cours/TD sélectionnés :\n");
    while (!estVide(pile)) {
        CoursTDa coursTDa = depiler(&pile);
        printf("- ID: %s | Type: %s | Titre: %s | Prix: %.2f\n",
               coursTDa.id, coursTDa.type, coursTDa.titre, coursTDa.prix);
        total += coursTDa.prix;
    }
    printf("\nTotal à payer : %.2f\n", total);
    printf("TAP *  1  * pour payee\n");
    scanf("%d",&pay);
     if (pay==1){
                char mail[100];
                printf("reecrire le mail pour recoit votre achat\n");
                scanf("%s",&mail);
                system("cls");
                printf("==== votre achat est en cours  d'envoyeé \n===");
                printf("====bonne journeé====\n\n");



}}
// ==== Menus pour les différents utilisateurs ====

void menuAdmin() {

    int choix;
    do {
        printf("\n====== Menu Admin ======\n");
        printf("1. ajouter prof\n");
        printf("2. modifier autorisation\n");
        printf("3. aficher listecoursTD\n");
        printf("4. ajouter_prix\n");
        printf("5. Retour au menu principal\n");
        printf("================================\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);



        switch (choix) {
            case 1:
         char choix;
         system("cls");
    do  {
    ajouterProfesseur();
    printf("Voulez-vous ajouter un autre professeur? (o/n): ");
        scanf(" %c", &choix);
        getchar();
        } while (choix == 'o' || choix == 'O');

                break;
            case 2:
                system("cls");
               mettreAJourPermissions();
                break;

            case 3:
                system("cls");

                char choix1[10];

    printf("Voulez-vous afficher la liste des 'cours' ou des 'td'? ");
    scanf("%s", choix1);

    afficherListeCoursTD(choix1);

                 break;
             case 4:
                 system("cls");
                 creerPrixCoursTD();

            case 5:
                system("cls");
                printf("Retour au menu principal.\n");
                break;
            default:
                system("cls");
                printf("Choix invalide, veuillez réessayer.\n");
        }
    } while (choix != 5);
}
void menuEtudiant() {

    int choix;
    do {
        printf("\n====== Menu Étudiant ======\n");
        printf("1. AfficheRépondre à un test\n");
        printf("2. Afficher la liste cours/ou TD\n");
        printf("3. Retour au menu principal\n");
        printf("================================\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);



        switch (choix) {
            case 1:
                system("cls");
     Test tests[MAX_TESTS];
    int nombreTests = 0;

    // Lire les tests depuis le fichier
    if (!lireTests("tests.txt", tests, &nombreTests)) {
        printf("Erreur : impossible de lire le fichier des tests.\n");
        return 1;
    }

    // Afficher tous les tests disponibles
    printf("Tests disponibles :\n");
    for (int i = 0; i < nombreTests; i++) {
        afficherTest(&tests[i]);
        printf("-----------------------\n");
    }

    // Permettre à l'étudiant de choisir un test
    int choix;
    printf("Entrez l'ID du test que vous voulez choisir : ");
    scanf("%d", &choix);

    // Trouver le test correspondant
    Test *testChoisi = NULL;
    for (int i = 0; i < nombreTests; i++) {
        if (tests[i].id == choix) {
            testChoisi = &tests[i];
            break;
        }
    }

    if (testChoisi == NULL) {
        printf("Test avec l'ID %d introuvable.\n", choix);
        return 1;
    }
    // Vérifier si la date de fin est dépassée
    if (dateDepassee(testChoisi->dateFin)) {
        printf("La date de fin du test \"%s\" est dépassée. Vous ne pouvez pas répondre à ce test.\n", testChoisi->titre);
    } else {
        printf("Vous avez sélectionné le test \"%s\".\n", testChoisi->titre);
        printf("Lien pour télécharger la réponse : %s\n", testChoisi->reponse);
        printf("Réponse enregistrée avec succès !\n");
    }

                break;
            case 2:
                system("cls");
             char choix12[10];
             FILE *cours_td;


    printf("Voulez-vous afficher la liste des 'cours' ou des 'td'? ");
    scanf("%s", choix12);
              afficherListeCoursTD(choix12);

              afficher_cour_td();

                break;
            case 3:
                system("cls");
                printf("Retour au menu principal.\n");
                break;
            default:
                system("cls");
                printf("Choix invalide, veuillez réessayer.\n");
        }
    } while (choix != 3);
}
void menuProfesseur() {

    int choix;
    do {
        printf("\n====== Menu Professeur ======\n");
        printf("1. Ajouter un nouveau coursTD\n");
        printf("2. modifier coursTD\n");
        printf("3.supprimer coursTD\n");
        printf("4.ajoutertest\n");
        printf("5. Retour au menu principal\n");
        printf("================================\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                system("cls");
                 int id0;
    char action[20];


    printf("Entrez l'ID de l'enseignant: ");
    scanf("%d", &id0);
    getchar();

    printf("REcrire la action *ajouter*  : ");
    fgets(action, sizeof(action), stdin);
    action[strcspn(action, "\n")] = '\0';


    if (verifierPermissionsProf(id0, action)) {
        printf("Action autorisée.\n");
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
               ajouterCoursTD(ID,type, titre, contenu);

    } else {
        printf("Action non autorisée.\n");
    }

system("cls");
                break;

            case 2:
                system("cls");
                int ID2;
    char nouveauTitre[100];
    char nouveauContenu[500];

    // info enterre
    printf("Entrez l'ID du Cours/TD à modifier: ");
    scanf("%d", &ID2);

    getchar();

    printf("Entrez le nouveau titre: ");
    fgets(nouveauTitre, sizeof(nouveauTitre), stdin);
    nouveauTitre[strcspn(nouveauTitre, "\n")] = '\0';

    printf("Entrez le nouveau contenu: ");
    fgets(nouveauContenu, sizeof(nouveauContenu), stdin);
    nouveauContenu[strcspn(nouveauContenu, "\n")] = '\0';

    // fct de modif td
    if (modifierCoursTD(ID2, nouveauTitre, nouveauContenu)) {
        printf("Le Cours/TD a été modifié avec succès !\n");
    } else {
        printf("Une erreur s'est produite lors de la modification.\n");
    }
system("cls");
                break;
            case 3:
                system("cls");
                 int id01;
    char action0[20];


    printf("Entrez l'ID de l'enseignant: ");
    scanf("%d", &id01);
    getchar();

    printf("Entrez l'action (ajouter/modifier/supprimer): ");
    fgets(action, sizeof(action0), stdin);
    action[strcspn(action0, "\n")] = '\0';


    if (verifierPermissionsProf(id01, action0)) {
        printf("Action autorisée.\n");
                      int id;
    printf("Entrez l'ID du Cours/TD à supprimer: ");
    scanf("%d", &id);

    if (supprimerCoursTD(id)) {
        printf("Le Cours/TD a été supprimé avec succès !\n");
    } else {
        printf("Une erreur s'est produite lors de la suppression.\n");
    }

    } else {
        printf("Action non autorisée.\n");
    }






system("cls");
                break;

            case 4:
                system("cls");
                int id1;

    printf("Entrez l'ID du cours/TD pour lequel vous voulez ouvrir un test: ");
    scanf("%d", &id1);
    getchar();

    if (ouvrirTest(id1)) {
        printf("Le test a été ouvert et enregistré avec succès.\n");
    } else {
        printf("Une erreur s'est produite lors de l'ouverture du test.\n");
    }
system("cls");
                break;
            case 5:

                printf("Retour au menu principal.\n");
                break;
            default:
              system("cls");
                printf("Choix invalide, veuillez réessayer.\n");
        }
    } while (choix != 5);
}
void menuExterne() {

    int choix;
    do {
        printf("\n====== Menu Utilisateur Externe ======\n");
        printf("1.les cours_TD a achetete.\n");
        printf("2.Retour au menu principal.\n");
        printf("================================\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);
        switch (choix) {
          case 1:
              system("cls");
               const char *fichierCoursTD = "cours_td.txt";

              CoursTDa listeCoursTD[100];
             int tailleCoursTD = lireCoursTD(fichierCoursTD, listeCoursTD, 100);

             if (tailleCoursTD == 0) {
             printf("Aucun cours ou TD trouvé dans le fichier '%s'.\n", fichierCoursTD);
            return 0;
            }
system("cls");
    // Ajouter les prix depuis prix_cours_td.txt
    ajouterPrixDepuisFichier(fichierCoursTD, listeCoursTD, tailleCoursTD);

    // Gérer les cours/TD sélectionnés avec une pile
    Node *pile = creerPile();
system("cls");
    // Permettre à l'utilisateur de sélectionner des cours/TD
    selectionnerCours(&pile, listeCoursTD, tailleCoursTD);
system("cls");
    // Afficher le total à payer
    afficherTotal(pile);
                break;
        default:
                printf("Choix invalide, veuillez réessayer.\n");

    }}while(choix != 2);



}
// ==== Menu principal ====

void afficherMenuPrincipal() {

    printf("\n====== Menu Principal ======\n");
    printf("1. Accéder en tant qu'Admin\n");
    printf("2. Accéder en tant que Professeur\n");
    printf("3. Accéder en tant qu'Étudiant\n");
    printf("4. Accéder en tant qu'Utilisateur Externe\n");
    printf("5. Creer un nouveon compte Etudiant\n");
    printf("6. Quitter\n");
    printf("================================\n");
    printf("Entrez votre choix : ");
}
int main() {

    int choix;
    do {
system("cls");
        afficherMenuPrincipal(); // Afficher le menu principal
        scanf("%d", &choix);
        switch (choix) {
            case 1:

                system("cls");

    char motDePasse[20];
    int i = 0;
    char c;

    printf("Entrez le code d'autorisation de l'administrateur : ");
    while (i < sizeof(motDePasse) - 14415159) {
        c = getch();
        if (c == '\r') {
            break;
        }
        motDePasse[i++] = c;
        printf("*");
    }
    motDePasse[i] = '\0';
    if (strcmp(motDePasse, "14415159") == 0) {
        system("cls");
        menuAdmin();
    } else {
        printf("\nMot de passe incorrect.\n"); // Message d'erreur si le mot de passe est faux
    }
                    system("cls");
                break;

            case 2:
                system("cls");
                 if (authentifierProfesseur()) {
                        system("cls");
        printf("Vous êtes connecté au système.\n");


         menuProfesseur(); // Menu professeur
    } else {
        system("cls");
        printf("Veuillez réessayer.\n");
    }
system("cls");
                break;
            case 3:
system("cls");
            if (authentifier()) {
                system("cls");


                menuEtudiant();    //  Menu étudiant
                system("cls");
            }

            else {
                printf("Veuillez réessayer.\n");
            }
                break;
            case 4:
                 system("cls");
                int pay=1 ;
                // Contenu public

                if (pay==1){
                char mail[100];
                printf("donner votre mail pour resu votre achat\n");
                scanf("%s",&mail);
                system("cls");
                menuExterne();
                }
                break;
            case 5:
system("cls");
                printf("ajouter etudaint");
                  system("cls");
                  ajouter_etd();  // ajouteretud();


            case 6:
                printf("Quitter le programme. Merci pour votre utilisation !\n");
                break;
            default:
                printf("Choix invalide, veuillez réessayer.\n");
        }


    } while (choix != 6);


    return 0;
}

