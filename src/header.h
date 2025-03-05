/* Guarding against circular imports */

#ifndef GUARD
#define GUARD

#include <stdbool.h>

#define NUM_LEN 8
#define NAME_LEN 55
#define DATE_LEN 13
#define PIN_LEN 7
#define ATTEMPTS 3

struct User{
	char *nom;
	char *prenom;
	char *numero;
	char *PIN;
	unsigned long int solde;
	char *dateEnregistrement;
	char *dateNaissance;
	int etat;
};

extern bool isValidNumber(const char* number);
extern struct User* checkUser(const char* number);
extern bool checkPinValidity(const char* pin);
extern void askForPin(const char *placeHolder, char *pin, int size);
extern bool checkPin(struct User *user, const char* pin);
extern int createMenu(struct User *user);
extern int is_valid_date(const char *date);
extern void writeUser(struct User *user);
extern struct User *registerUser(char *numero);
extern char* removeNewLine(const char *string);
extern char *cleanLine(const char *string);
extern char* getCurrentDate(void);
extern void printUser(struct User *user);

#endif
