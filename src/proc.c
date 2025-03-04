#include <stdlib.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "header.h"


bool isValidNumber(const char *number){
	regex_t _regex;
	char *patt = "^\\+257(2[2-9]|6[1-9]|7[1-9])[0-9]{6}$";

	if (regcomp(&_regex, patt, REG_EXTENDED)){
		perror("isValidNumber");
		exit(EXIT_FAILURE);
	}

	int validNumber = regexec(&_regex, number, 0, NULL, 0);
	regfree(&_regex);
	return validNumber == 0;
}

struct User* checkUser(const char* number){
	if (!isValidNumber(number)){
		printf("Numéro invalide\n");
		exit(EXIT_FAILURE);
	}
	FILE* file = fopen("utilisateurs.txt", "r");
	if (file == NULL){
		perror("checkUser");
		exit(EXIT_FAILURE);
	}

	char *line = NULL;
	size_t size = 0;
	ssize_t nread;
	char *token;

	// This structure will send back the information on the connected user
	struct User *connectedUser = malloc(sizeof(struct User));

	int offset;
	short int found = 0;

	while ((nread = getline(&line, &size, file)) != -1){
		if (found && offset == 8){
			break;
		}
		offset = 0;
		while ((token = strsep(&line, "|")) != NULL){
			switch (offset){
				case 0:
					connectedUser->nom = token;
					break;
				case 1:
					connectedUser->prenom = token;
					break;
				case 2:
					connectedUser->numero = token;
					break;
				case 3:
					connectedUser->PIN = token;
					break;
				case 4:
					connectedUser->solde = atol(token);
					break;
				case 5:
					connectedUser->dateEnregistrement = token;
					break;
				case 6:
					connectedUser->dateNaissance = token;
					break;
				case 7:
					connectedUser->etat = atoi(token);
					break;
			}
			if (isValidNumber(token) && strcmp(token, number) == 0){
				found = 1;
			}
			offset++;
		}
	}
	if (found)
		return connectedUser;
	return NULL;
}

int createMenu(void){
	char buffer[12];
	int choice = 0;
	printf("1. Transfert\n");
	printf("2. Retirer\n");
	printf("3. Achat des unites\n");
	printf("4. Payer les factures\n");
	printf("5. Payer le marchand\n");
	printf("6. Services bancaires\n");
	printf("7. OBR\n");
	printf("8. Mairie\n");
	printf("9. Engrais\n");
	printf("10. Services\n");
	printf("11. Quitter\n\n");
	printf("Choisissez: ");

	fgets(buffer, 12, stdin);
	choice = atoi(buffer);
	
	if (choice < 1 || choice > 11){
		printf("Choix invalide\n");
		exit(EXIT_FAILURE);
	}

	return choice;
}

bool checkPinValidity(const char* pin){
	if (strlen(pin) != 6){
		return false;
	}
	return true;
}

bool checkPin(struct User *user, const char *pin) {
	if (!checkPinValidity(pin)){
		return false;
	}
	if (strcmp(pin, user->PIN) == 0){
		return true;
	}
	return false;
}

bool checkName(const char *name){
	if (strlen(name) < 2 || strlen(name) > 54){
		return false;
	}
	return true;
}

int is_valid_date(const char *date) {
    struct tm tm = {0};
    int year, month, day;

    if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3) {
        return 0;
    }

    tm.tm_year = year - 1900;  // Years since 1900
    tm.tm_mon  = month - 1;    // Months since January (0-11)
    tm.tm_mday = day;
    tm.tm_isdst = -1;

    time_t t = mktime(&tm);
    return t != -1 && tm.tm_year + 1900 == year && tm.tm_mon + 1 == month && tm.tm_mday == day;
}

struct User *registerUser(void){
	struct User *user = malloc(sizeof(struct User));
	char *numero = malloc(sizeof(char)*NUM_LEN);
	char *pin = malloc(sizeof(char)*PIN_LEN);
	char *nom = malloc(sizeof(char)*54);
	char *prenom = malloc(sizeof(char)*54);
	char *dateEnregistrement = malloc(sizeof(char)*10);
	char *dateNaissance = malloc(sizeof(char)*10);
	unsigned long solde;

	printf("Numéro de Téléphone: ");
	scanf("%s", numero);
	getchar();
	user->numero = numero;
	if (!isValidNumber(user->numero)){
		printf("Numéro invalide\n");
		exit(EXIT_FAILURE);
	}

	printf("Pin: ");
	scanf("%s", pin);
	getchar();
	user->PIN = pin;
	if (!checkPinValidity(user->PIN)){
		printf("PIN invalide\n");
		exit(EXIT_FAILURE);
	}

	printf("Solde: ");
	scanf("%ld", &solde);
	getchar();
	user->solde = solde;
	if (user->solde < 0 || user->solde > 5000000){
		printf("Solde invalide\n");
		exit(EXIT_FAILURE);
	}

	printf("Nom: ");
	scanf("%s", nom);
	getchar();
	user->nom = nom;
	if (!checkName(user->nom)){
		printf("Nom invalide\n");
		exit(EXIT_FAILURE);
	}

	printf("Prenom: ");
	scanf("%s", prenom);
	getchar();
	user->prenom = prenom;
	if (!checkName(user->prenom)){
		printf("Prenom invalide\n");
		exit(EXIT_FAILURE);
	}

	printf("Date d'enregistrement: ");
	scanf("%s", dateEnregistrement);
	getchar();
	user->dateEnregistrement = dateEnregistrement;
	if (!is_valid_date(user->dateEnregistrement)){
		printf("Date d'enregistrement invalide\n");
		exit(EXIT_FAILURE);
	}

	printf("Date de naissance: ");
	scanf("%s", dateNaissance);
	getchar();
	user->dateNaissance = dateNaissance;
	if (!is_valid_date(user->dateNaissance)){
		printf("Date de naissance invalide\n");
		exit(EXIT_FAILURE);
	}

	user->etat = 0;

	return user;
}

void printUser(struct User *user){
	printf("Nom: %s\n", user->nom);
	printf("Prénom: %s\n", user->prenom);
	printf("Numéro: %s\n", user->numero);
	printf("PIN: %s\n", user->PIN);
	printf("Solde: %lu\n", user->solde);
	printf("Date d'enregistrement: %s\n", user->dateEnregistrement);
	printf("Date de naissance: %s\n", user->dateNaissance);
	printf("Etat: %d\n", user->etat);
}
