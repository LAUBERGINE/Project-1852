#ifndef LIB_H
#define LIB_H

#include "lib.c"

void generer_user_key_file(const char* cle);
void generer_password_file();
void ajouter_identifiants(const char* keyUser, const char* keyMachine, int nb);
void modifier_identifiants(const char* keyUser, const char* keyMachine);
void supprimer_identifiants(const char* keyUser, const char* keyMachine);
void visualiser_identifiants(const char* keyUser, const char* keyMachine);
char* saisir_mot_de_passe();
char* encode_and_chiffre_mot_de_passe(const char* password, const char* keyUser, const char* keyMachine, int nb);
char* dechiffre_mot_de_passe(const char* encoded_password, const char* keyUser, const char* keyMachine, int nb);
void afficher_menu();
char* lireCleDepuisFichier(const char* nomFichier);

#endif
