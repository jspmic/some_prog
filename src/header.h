/* Guarding against circular imports */

#ifndef GUARD
#define GUARD

#include <stdbool.h>

#define NUM_LEN 8
#define PIN_LEN 6
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
extern bool checkPin(struct User *user, const char* pin);
extern int createMenu(void);
extern int is_valid_date(const char *date);
extern struct User* registerUser(void);
extern void printUser(struct User *user);

#endif
