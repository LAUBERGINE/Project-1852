#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lib.h"

int generateRandomNumber() {
    time_t t;
    time(&t);
    int seed = (int)t;

    int random_number = ((seed * 110155689853693621) + 78120236348) % 4512038890;

    return random_number;
}

char* genererCleMachineAleatoire(const char* caracteres, int longueur_cle) {
    char* cle = (char*)malloc(longueur_cle + 1);
    char* caracteres_disponibles = strdup(caracteres);

    for (int i = 0; i < longueur_cle; ++i) {
        int index = generateRandomNumber() % strlen(caracteres_disponibles);
        cle[i] = caracteres_disponibles[index];

        memmove(&caracteres_disponibles[index], &caracteres_disponibles[index + 1], strlen(caracteres_disponibles) - index);
    }

    cle[longueur_cle] = '\0';
    free(caracteres_disponibles);

    return cle;
}

char* genererCleUserAleatoire(const char* caracteres, int longueur_cle) {
    char* cle = (char*)malloc(longueur_cle + 1);
    char* caracteres_disponibles = strdup(caracteres);

    srand(time(NULL));

    for (int i = 0; i < longueur_cle; ++i) {
        int index = rand() % strlen(caracteres_disponibles);
        cle[i] = caracteres_disponibles[index];

        memmove(&caracteres_disponibles[index], &caracteres_disponibles[index + 1], strlen(caracteres_disponibles) - index);
    }

    cle[longueur_cle] = '\0';
    free(caracteres_disponibles);

    return cle;
}

char* generer_userkey_aleatoire() {
    const char* caracteres_userkey = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-*!?";
    const int longueur_cle_userkey = 67;
    return genererCleUserAleatoire(caracteres_userkey, longueur_cle_userkey);
}

char* generer_machinekey_aleatoire() {
    const char* caracteres_machinekey = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-*!?";
    const int longueur_cle_machinekey = 67;
    return genererCleMachineAleatoire(caracteres_machinekey, longueur_cle_machinekey);
}

void generer_user_key_file(const char* cle) {
    FILE* fichier = fopen("userkey.key", "r");
    if (fichier == NULL) {
        fichier = fopen("userkey.key", "w");
        if (fichier == NULL) {
            perror("Erreur lors de la création du fichier userkey.key");
            exit(EXIT_FAILURE);
        }

        fprintf(fichier, "%s", cle);
        fclose(fichier);
        printf("Fichier userkey.key généré\n");
    } else {
        fclose(fichier);
    }
}

void generer_machine_key_file(const char* cle) {
    FILE* fichier = fopen("machinekey.key", "r");
    if (fichier == NULL) {
        fichier = fopen("machinekey.key", "w");
        if (fichier == NULL) {
            perror("Erreur lors de la création du fichier machinekey.key");
            exit(EXIT_FAILURE);
        }

        fprintf(fichier, "%s", cle);
        fclose(fichier);
        printf("Fichier machinekey.key généré\n");
    } else {
        fclose(fichier);
    }
}

void generer_password_file() {
    FILE* fichier = fopen("password.txt", "r");
    if (fichier == NULL) {
        fichier = fopen("password.txt", "w");
        if (fichier == NULL) {
            perror("Erreur lors de la création du fichier password.txt");
            exit(EXIT_FAILURE);
        }
        char* cle = generer_userkey_aleatoire();
        char* cle2 = generer_machinekey_aleatoire();
        generer_user_key_file(cle);
        generer_machine_key_file(cle2);
        fclose(fichier);
        printf("Fichier password.txt généré\n");
    } else {
        fclose(fichier);
    }
}

char* lireCleDepuisFichier(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier [Les fichiers .key ne sont pas dans le dossier parent]");
        sleep(5);
        exit(EXIT_FAILURE);
    }

    fseek(fichier, 0, SEEK_END);
    long taille = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);

    char* contenu = (char*)malloc(taille + 1);
    if (contenu == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    fread(contenu, 1, taille, fichier);
    contenu[taille] = '\0';

    fclose(fichier);

    return contenu;
}

char* saisir_mot_de_passe() {
    char* password = (char*)malloc(100);
    fgets(password, 100, stdin);
    password[strcspn(password, "\n")] = '\0';
    return password;
}

char* base64_encode(const char* input, size_t length) {
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    size_t encoded_size = ((length + 2) / 3) * 4 + 1;
    char* encoded_buffer = (char*)malloc(encoded_size);

    size_t output_index = 0;

    for (size_t i = 0; i < length; i += 3) {
        unsigned int fragment = (unsigned char)input[i];
        fragment = (i + 1 < length) ? (fragment << 8) | (unsigned char)input[i + 1] : fragment << 8;
        fragment = (i + 2 < length) ? (fragment << 8) | (unsigned char)input[i + 2] : fragment << 8;

        encoded_buffer[output_index++] = base64_chars[(fragment >> 18) & 0x3F];
        encoded_buffer[output_index++] = base64_chars[(fragment >> 12) & 0x3F];

        if (i + 1 < length) {
            encoded_buffer[output_index++] = base64_chars[(fragment >> 6) & 0x3F];
        } else {
            encoded_buffer[output_index++] = '=';
        }

        if (i + 2 < length) {
            encoded_buffer[output_index++] = base64_chars[fragment & 0x3F];
        } else {
            encoded_buffer[output_index++] = '=';
        }
    }

    encoded_buffer[output_index] = '\0';

    return encoded_buffer;
}

char* base64_decode(const char* input, size_t length) {
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const size_t input_size = length;
    size_t decoded_size = (input_size / 4) * 3;

    if (input[input_size - 1] == '=') {
        decoded_size--;
        if (input[input_size - 2] == '=')
            decoded_size--;
    }

    char* decoded_buffer = (char*)malloc(decoded_size + 1);
    size_t output_index = 0;

    unsigned int fragment = 0;
    int fragment_bits = 0;

    for (size_t i = 0; i < input_size; i++) {
        char c = input[i];
        if (c == '=') {
            break;
        }

        char value;
        const char* ptr = strchr(base64_chars, c);
        if (ptr != NULL) {
            value = ptr - base64_chars;
        } else {
            free(decoded_buffer);
            return NULL;
        }

        fragment = (fragment << 6) | value;
        fragment_bits += 6;

        while (fragment_bits >= 8) {
            decoded_buffer[output_index++] = (fragment >> (fragment_bits - 8)) & 0xFF;
            fragment_bits -= 8;
        }
    }

    decoded_buffer[output_index] = '\0';

    return decoded_buffer;
}

char* chiffrement(const char* msg, const char* d, const char* c) {
    size_t msg_length = strlen(msg);
    char* res = (char*)malloc(msg_length + 1);
    for (size_t i = 0; i < msg_length; i++) {
        const char* indice = strchr(c, msg[i]);
        if (indice != NULL) {
            res[i] = d[indice - c];
        } else {
            res[i] = msg[i];
        }
    }

    res[msg_length] = '\0';
    return res;
}

char* dechiffrement(const char* msg, const char* d, const char* c) {
    size_t msg_length = strlen(msg);
    char* res = (char*)malloc(msg_length + 1);

    for (size_t i = 0; i < msg_length; i++) {
        const char* indice = strchr(d, msg[i]);
        if (indice != NULL) {
            res[i] = c[indice - d];
        } else {
            res[i] = msg[i];
        }
    }

    res[msg_length] = '\0';
    return res;
}

char* encode_and_chiffre_mot_de_passe(const char* password, const char* keyUser, const char* keyMachine, int nb) {
    char* encoded_password = base64_encode(password, strlen(password));

    for (int i = 0; i < nb; i++) {
        encoded_password = chiffrement(encoded_password, keyMachine, keyUser);
    }

    return encoded_password;
}

char* dechiffre_mot_de_passe(const char* encoded_password, const char* keyUser, const char* keyMachine, int nb) {

    char* decrypted_password = strdup(encoded_password);
    for (int i = 0; i < nb; i++) {
        decrypted_password = dechiffrement(decrypted_password, keyMachine, keyUser);
    }

    char* decoded_password = base64_decode(decrypted_password, strlen(decrypted_password));

    free(decrypted_password);

    return decoded_password;
}

void ajouter_identifiants(const char* keyUser, const char* keyMachine, int nb) {
    FILE* fichier = fopen("password.txt", "a");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char site[100], login[100], password[100];

    printf("Site : ");
    fgets(site, sizeof(site), stdin);
    site[strcspn(site, "\n")] = '\0'; 

    printf("Login : ");
    fgets(login, sizeof(login), stdin);
    login[strcspn(login, "\n")] = '\0';

    printf("Mot de passe : ");
    strcpy(password, saisir_mot_de_passe());

    char* encoded_password = encode_and_chiffre_mot_de_passe(password, keyUser, keyMachine, nb);

    fprintf(fichier, "%s;%s;%d;%s\n", site, login, nb, encoded_password);

    free(encoded_password);
    fclose(fichier);

    printf("Identifiants ajoutés avec succès.\n");
}

void vider_tampon_entree() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void modifier_identifiants(const char* keyUser, const char* keyMachine) {
    FILE* fichier = fopen("password.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Erreur lors de la création du fichier temporaire");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    char ligne[256];
    int modified = 0;

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char site[100], login[100], encoded_password[100], newPassword[100];
        int nb;
        sscanf(ligne, "%99[^;];%99[^;];%d;%99[^\n]", site, login, &nb, encoded_password);

        char* password = dechiffre_mot_de_passe(encoded_password, keyUser, keyMachine, nb);

        printf("Site: %s\n", site);
        printf("Login: %s\n", login);
        printf("Mot de passe: %s\n", password);

        free(password);

        printf("Voulez-vous modifier cet identifiant ? (O/N) : ");
        char reponse;
        scanf(" %c", &reponse);

        if (reponse == 'N' || reponse == 'n') {
            fprintf(tempFile, "%s;%s;%d;%s\n", site, login, nb, encoded_password);
        } else {
            printf("Nouveau site : ");
            scanf("%s", site);
            vider_tampon_entree();

            printf("Nouveau login : ");
            scanf("%s", login);
            vider_tampon_entree();

            printf("Nouveau mot de passe : ");
            strcpy(newPassword, saisir_mot_de_passe());
            char* newEncodedPassword = encode_and_chiffre_mot_de_passe(newPassword, keyUser, keyMachine, nb);

            fprintf(tempFile, "%s;%s;%d;%s\n", site, login, nb, newEncodedPassword);

            free(newEncodedPassword);
            modified++;
        }
    }

    fclose(fichier);
    fclose(tempFile);

    remove("password.txt");
    rename("temp.txt", "password.txt");

    printf("%d identifiants modifiés avec succès.\n", modified);
}

void supprimer_identifiants(const char* keyUser, const char* keyMachine) {
    FILE* fichier = fopen("password.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    FILE* tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Erreur lors de la création du fichier temporaire");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    char ligne[256];
    int deleted = 0;

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char site[100], login[100], encoded_password[100];
        int nb;
        sscanf(ligne, "%99[^;];%99[^;];%d;%99[^\n]", site, login, &nb, encoded_password);

        char* password = dechiffre_mot_de_passe(encoded_password, keyUser, keyMachine, nb);

        printf("Site: %s\n", site);
        printf("Login: %s\n", login);
        printf("Mot de passe: %s\n", password);

        free(password);

        printf("Voulez-vous supprimer cet identifiant ? (O/N) : ");
        char reponse;
        scanf(" %c", &reponse);

        if (reponse == 'N' || reponse == 'n') {
            fprintf(tempFile, "%s;%s;%d;%s\n", site, login, nb, encoded_password);
        } else {
            deleted++;
        }
    }

    fclose(fichier);
    fclose(tempFile);

    remove("password.txt");
    rename("temp.txt", "password.txt");

    printf("%d identifiants supprimés avec succès.\n", deleted);
}

void visualiser_identifiants(const char* keyUser, const char* keyMachine) {
    FILE* fichier = fopen("password.txt", "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char site[100], login[100], encoded_password[100];
        int nb;
        sscanf(ligne, "%99[^;];%99[^;];%d;%99[^\n]", site, login, &nb, encoded_password);

        char* password = dechiffre_mot_de_passe(encoded_password, keyUser, keyMachine, nb);

        printf("Site: %s\n", site);
        printf("Login: %s\n", login);
        printf("Mot de passe: %s\n", password);

        free(password);
    }

    fclose(fichier);
}

void afficher_menu() {
    printf("\nMenu :\n");
    printf("1. Ajouter les identifiants\n");
    printf("2. Modifier des identifiants\n");
    printf("3. Supprimer des identifiants\n");
    printf("4. Visualiser les identifiants\n");
    printf("5. Quitter\n");
}

