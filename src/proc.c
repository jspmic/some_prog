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
	fclose(file);
	if (found)
		return connectedUser;
	return NULL;
}

void askForPin(const char *placeHolder, char *pin, int size){
	printf("%s", placeHolder);
	fgets(pin, size, stdin);
	if (!checkPinValidity(pin)){
		printf("PIN invalide\n");
		exit(EXIT_FAILURE);
	}
}

void servicesMenu(struct User *user){
	char buffer[12];
	int choice = 0;

	printf("\n1. Consulter le solde\n");
	printf("2. Changer le code PIN\n");
	printf("3. Changer la langue\n");
	printf("4. Historique des transactions\n");
	printf("5. Identification du client\n");
	printf("6. Consulter les frais\n");
	printf("0. Retour\n");
	printf("\nChoisissez: ");

	fgets(buffer, 12, stdin);
	choice = atoi(buffer);
	
	if (choice < 0 || choice > 6){
		printf("Choix invalide\n");
		exit(EXIT_FAILURE);
	}

	char *pin = malloc(sizeof(char)*PIN_LEN);
	char *new_pin = malloc(sizeof(char)*PIN_LEN);

	switch (choice){
		case 1:
			printf("Votre solde est %ld FBu. Merci\n", user->solde);
			break;

		case 2:
			printf("Entrez votre code PIN: ");
			scanf("%s", pin);
			getchar();
			if (!checkPin(user, pin)){
				printf("Le code PIN n'est pas correct. Veuillez verifier a nouveau\n");
				exit(EXIT_FAILURE);
			}

			printf("Entrez le nouveau code PIN(6 chiffres): ");
			scanf("%s", new_pin);
			getchar();
			if (!checkPinValidity(new_pin)){
				printf("PIN invalide\n");
				exit(EXIT_FAILURE);
			}
			user->PIN = new_pin;
			writeUser(user);
			break;

		case 5:
			askForPin("Entrez votre code PIN: ", pin, PIN_LEN);
			if (!checkPin(user, pin)){
				printf("Le code PIN n'est pas correct. Veuillez verifier a nouveau\n");
				exit(EXIT_FAILURE);
			}
			printf("\nInformation du compte Lumicash:\n");
			printf("Noms: %s %s\n", user->nom, user->prenom);
			printf("Activation: %s\n", user->dateEnregistrement);
			printf("Né le %s\n", user->dateNaissance);
			printf("Solde: %ld\n", user->solde);
			printf("Merci d'avoir utilise Lumicash\n");
			break;

		case 0:
			createMenu(user);
			break;

		default:
			printf("La fonctionnalité n'est pas implémentée\n");
			break;
	}
}

int createMenu(struct User *user){
	char buffer[12];
	int choice = 0;
	printf("\n1. Transfert\n");
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

	switch (choice){
		case 10:
			servicesMenu(user);
			break;
		case 11:
			exit(EXIT_SUCCESS);
			break;
		default:
			printf("La fonctionnalité n'est pas implémentée\n");
			break;
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
	char *nom = malloc(sizeof(char)*NAME_LEN);
	char *prenom = malloc(sizeof(char)*NAME_LEN);
	char *dateEnregistrement = malloc(sizeof(char)*DATE_LEN);
	char *dateNaissance = malloc(sizeof(char)*DATE_LEN);

	printf("Numéro de Téléphone: ");
	scanf("%s", numero);
	getchar();
	user->numero = removeNewLine(numero);
	if (!isValidNumber(user->numero) || checkUser(user->numero) != NULL){
		printf("Numéro invalide ou existant\n");
		exit(EXIT_FAILURE);
	}

	askForPin("Entrer votre code PIN(6 chiffres): ", pin, PIN_LEN);
	user->PIN = removeNewLine(pin);

	user->solde = 0;

	printf("%d\n", getchar());
	printf("Nom: ");
	fgets(nom, NAME_LEN, stdin);
	user->nom = removeNewLine(nom);
	if (!checkName(user->nom)){
		printf("Nom invalide\n");
		exit(EXIT_FAILURE);
	}

	printf("Prenom: ");
	fgets(prenom, NAME_LEN, stdin);
	user->prenom = removeNewLine(prenom);
	if (!checkName(user->prenom)){
		printf("Prenom invalide\n");
		exit(EXIT_FAILURE);
	}
	printf("Date d'enregistrement: ");
	fgets(dateEnregistrement, DATE_LEN, stdin);
	user->dateEnregistrement = removeNewLine(dateEnregistrement);
	if (!is_valid_date(user->dateEnregistrement)){
		printf("Date d'enregistrement invalide\n");
		exit(EXIT_FAILURE);
	}

	printf("Date de naissance: ");
	fgets(dateNaissance, DATE_LEN, stdin);
	user->dateNaissance = removeNewLine(dateNaissance);
	if (!is_valid_date(user->dateNaissance)){
		printf("Date de naissance invalide\n");
		exit(EXIT_FAILURE);
	}

	user->etat = 0;
	/* printUser(user); */
	return user;
}

// This function will override the user's data in the utilisateurs.txt file
void writeUser(struct User *user){
	char *line = NULL;
	size_t size = 0;
	ssize_t nread;
	char *token;

	FILE *file = fopen("utilisateurs.txt", "a");

	while ((nread = getline(&line, &size, file)) != -1){
		while ((token = strsep(&line, "|")) != NULL){
			if (isValidNumber(token) && strcmp(token, user->numero) == 0){
				// do something when the user is found out
				break;
			}
		}
	}
	fprintf(file, "%s|%s|%s|%s|%ld|%s|%s|%d",
			user->nom, user->prenom, user->numero,
			user->PIN, user->solde, user->dateEnregistrement,
			user->dateNaissance, user->etat);
	fclose(file);
}

char* removeNewLine(const char *string){
	char *newString = malloc(sizeof(char) * (strlen(string)-1));
	for (int i = 0; i < strlen(string); i++){
		if (string[i] != '\n')
			newString[i] = string[i];
	}
	return newString;
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
