#include <stdlib.h>
#include <stdio.h>
#include "header.h"

int main(){
	char *number = malloc(sizeof(char)*NUM_LEN);
	char *pin = malloc(sizeof(char)*PIN_LEN);
	printf("Numéro de Téléphone: ");
	scanf("%s", number);
	getchar();
	/* number = "+25768502484"; */
	/* pin = "123456"; */
	/* number = "+25779988777"; */
	/* pin = "654321"; */
	struct User *user = checkUser(number);
	if (user == NULL){
		printf("Veuillez créer un compte Lumicash\n");
		user = registerUser(number);
		writeUser(user);
	}

	int attemptsCounter = 0;

	printf("\n");

	do{
		printf("PIN: ");
		scanf("%s", pin);
		getchar();
		if (checkPin(user, pin)){
			break;
		}
		attemptsCounter++;
	} while (attemptsCounter < ATTEMPTS);

	if (attemptsCounter == ATTEMPTS){
		printf("Nombre d'essais dépassé\n");
		exit(EXIT_FAILURE);
	}
	else{
		createMenu(user);
	}
	return 0;
}
