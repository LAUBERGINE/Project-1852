#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lib.h"

int main() {
    int choix, nb1;

    srand(time(NULL));
    generer_password_file();
    const char* userKey = "userkey.key";
    const char* machineKey = "machinekey.key";
    const char* keyUser = lireCleDepuisFichier(userKey);
    const char* keyMachine = lireCleDepuisFichier(machineKey);

    do {
        afficher_menu();
        printf("Choix : ");
        scanf("%d", &choix);
        getchar(); 

        switch (choix) {
            case 1:
                system(CLEAR_COMMAND);
                nb1 = rand() % 73 + 1;
                ajouter_identifiants(keyUser, keyMachine, nb1);
                break;
            case 2:
                system(CLEAR_COMMAND);
                modifier_identifiants(keyUser, keyMachine);
                break;
            case 3:
                system(CLEAR_COMMAND);
                supprimer_identifiants(keyUser, keyMachine);
                break;
            case 4:
                system(CLEAR_COMMAND);
                visualiser_identifiants(keyUser, keyMachine);
                break;
            case 5:
                break;
            default:
                system(CLEAR_COMMAND);
        }
    } while (choix != 5);

    return 0;
}
