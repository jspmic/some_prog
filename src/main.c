#include <stdlib.h>
#include <stdio.h>
#include "header.h"

int main(){
	char *number = malloc(sizeof(char)*NUM_LEN);
	char *pin = malloc(sizeof(char)*PIN_LEN);
	/* printf("Numéro de Téléphone: "); */
	/* scanf("%s", number); */
	/* getchar(); */
	number = "+25768502484";
	pin = "123456";
	struct User *user = checkUser(number);
	if (user == NULL){
		printf("Utilisateur non trouvé\n");
		registerUser();
	}

	/* printUser(user); // For debugging purposes */

	int attemptsCounter = 0;
	int choice = 0;

	printf("\n");

	do{
		/* printf("PIN: "); */
		/* scanf("%s", pin); */
		/* getchar(); */
		if (checkPin(user, pin)){
			break;
		}
		attemptsCounter++;
	} while (attemptsCounter < ATTEMPTS);

	printf("\n");

	if (attemptsCounter == ATTEMPTS){
		printf("Nombre d'essais dépassé\n");
		exit(EXIT_FAILURE);
	}
	else{
		choice = createMenu();
	}
	printf("Choix: %d\n", choice);
	return 0;
}
