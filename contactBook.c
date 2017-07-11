#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void removeNewline(char inputString[50]);
void clearScreen();

int main(void) 
{

	struct contactStruct {
		char phoneNumber[25];
		char firstName[25];
		char lastName[25];
		char companyName[25];
	};

	struct contactStruct contact; 
	struct contactStruct read;


	//Variable Declaration
	int endOfFile = 0;
	int editLocation = 0;
	char tempInput[100] = "-";
	char *menuInput = NULL;
	char *searchInput = NULL;
	char *editInput = NULL;
	bool menuLoop = true;
	bool nameFound1 = false;
	bool nameFound2 = false;
	FILE *filePointer;
	FILE *tempFP;

	clearScreen();
	printf("Welcome to Alex Montague's Contact Book\n");
	printf("This program will let you store and access all of your contacts!\n");
	
	while(menuLoop == true) {	
		
		menuLoop = true;
		printf("Would you like to: 'LIST', 'ADD', 'EDIT', 'VIEW', or 'REMOVE' a contact? or 'EXIT' - ");
		fgets(tempInput, sizeof(tempInput), stdin);
		clearScreen();
		for(int i = 0; i < strlen(tempInput); i++) {
			tempInput[i] = tolower(tempInput[i]);
		}
		menuInput = malloc(sizeof(char) * strlen(tempInput) + 1);
		strcpy(menuInput, tempInput);
		removeNewline(menuInput);




		if(strcmp(menuInput, "add") == 0) {
			clearScreen();
			printf("ADD A CONTACT:\n\n");

			filePointer = fopen("contactList.db", "a+");
			if (filePointer == NULL) {
		        filePointer = fopen("contactList.db", "w+");
			}

			printf("Phone Number: ");
			fgets(tempInput, sizeof(tempInput), stdin);
			strcpy(contact.phoneNumber, tempInput);

			printf("First Name: ");
			fgets(tempInput, sizeof(tempInput), stdin);
			strcpy(contact.firstName, tempInput);

			printf("Last Name: ");
			fgets(tempInput, sizeof(tempInput), stdin);
			strcpy(contact.lastName, tempInput);

			printf("Company Name: ");
			fgets(tempInput, sizeof(tempInput), stdin);
			strcpy(contact.companyName, tempInput);

			removeNewline(contact.phoneNumber);
			removeNewline(contact.firstName);
			removeNewline(contact.lastName);
			removeNewline(contact.companyName);
			fwrite(&contact, sizeof(struct contactStruct), 1, filePointer);
			fclose(filePointer);




		}
		else if(strcmp(menuInput, "edit") == 0) {
			nameFound1 = false;
			clearScreen();
			filePointer = fopen("contactList.db", "a+");
    		if (filePointer == NULL) {
        		filePointer = fopen("contactList.db", "w+");
    		}

			printf("What contact would you like to edit? (First Name): ");
			fgets(tempInput, sizeof(tempInput), stdin);
			editInput = malloc(sizeof(char) * strlen(tempInput) + 1);
			strcpy(editInput, tempInput);
			removeNewline(editInput);
			fseek(filePointer, 0, SEEK_END);
			endOfFile = ftell(filePointer);		// End of the File location
			fseek(filePointer, 0, SEEK_SET);

			do {
				fread(&read, sizeof(struct contactStruct), 1, filePointer);		// Reading one contact in at a time
				removeNewline(read.firstName);
				if(strcmp(editInput, read.firstName) == 0) {								// Might be something wrong here
					editLocation = (ftell(filePointer) - sizeof(struct contactStruct));		// If search name matches, get the start location of the contact
					nameFound1 = true;	// If edit name is found
				}
			}
			while(ftell(filePointer) < endOfFile);
			
			if(nameFound1 == true) {
				tempFP = fopen("tempList.db", "w+");
				fseek(filePointer, 0, SEEK_SET);
				fseek(tempFP, 0 , SEEK_SET);
				
				while(ftell(filePointer) < endOfFile) {
					if(ftell(filePointer) == editLocation) {

						fread(&read, sizeof(struct contactStruct), 1, filePointer);
						printf("\nHere is the OLD contact information given:\n");
						printf("Phone Number: %s\nName: %s %s\nCompany: %s \n\n", read.phoneNumber, read.firstName, read.lastName, read.companyName);
						printf("Enter the NEW contact information below.\n");

						printf("Phone Number: ");
						fgets(tempInput, sizeof(tempInput), stdin);
						strcpy(contact.phoneNumber, tempInput);

						printf("First Name: ");
						fgets(tempInput, sizeof(tempInput), stdin);
						strcpy(contact.firstName, tempInput);

						printf("Last Name: ");
						fgets(tempInput, sizeof(tempInput), stdin);
						strcpy(contact.lastName, tempInput);

						printf("Company Name: ");
						fgets(tempInput, sizeof(tempInput), stdin);
						strcpy(contact.companyName, tempInput);

						removeNewline(contact.phoneNumber);
						removeNewline(contact.firstName);
						removeNewline(contact.lastName);
						removeNewline(contact.companyName);
						fwrite(&contact, sizeof(struct contactStruct), 1, tempFP);
					
					}
					else {
						fread(&read, sizeof(struct contactStruct), 1, filePointer);
						fwrite(&read, sizeof(struct contactStruct), 1, tempFP);
					}
				}
				fclose(tempFP);
				fclose(filePointer);
				remove("contactList.db");
				rename("tempList.db", "contactList.db");
			}
			else if(nameFound1 == false) {	
				printf("The contact you are trying to edit could not be found.\n");
			}




		}
		else if(strcmp(menuInput, "view") == 0) {
			clearScreen();
			filePointer = fopen("contactList.db", "a+");
    		if (filePointer == NULL) {
        		filePointer = fopen("contactList.db", "w+");
    		}

			printf("Who are you searching for? (First Name): ");
			fgets(tempInput, sizeof(tempInput), stdin);
			searchInput = malloc(sizeof(char) * strlen(tempInput) + 1);
			strcpy(searchInput, tempInput);		// Get name being searched for
			removeNewline(searchInput);
			fseek(filePointer, 0, SEEK_END);
			endOfFile = ftell(filePointer);
			fseek(filePointer, 0, SEEK_SET);
			nameFound1 = false;
			nameFound2 = false;

			do {
				fread(&read, sizeof(struct contactStruct), 1, filePointer);
				if(strcmp(searchInput, read.firstName) == 0) {
					printf("\nHere is the contact information given:\n");
					printf("Phone Number: %s\nName: %s %s\nCompany: %s \n\n", read.phoneNumber, read.firstName, read.lastName, read.companyName);
					nameFound1 = true;
				}
				else {
					nameFound2 = true;
				}
			}
			while(ftell(filePointer) < endOfFile);
			if(nameFound1 == false && nameFound2 == true) {
				printf("The contact information for the name entered could not be found.\n");
			}
			fclose(filePointer);




		}
		else if(strcmp(menuInput, "remove") == 0) {
			
			nameFound1 = false;
			clearScreen();
			filePointer = fopen("contactList.db", "a+");
    		if (filePointer == NULL) {
        		filePointer = fopen("contactList.db", "w+");
    		}

			printf("What contact would you like to delete? (First Name): ");
			fgets(tempInput, sizeof(tempInput), stdin);
			editInput = malloc(sizeof(char) * strlen(tempInput) + 1);
			strcpy(editInput, tempInput);
			removeNewline(editInput);
			fseek(filePointer, 0, SEEK_END);
			endOfFile = ftell(filePointer);		// End of the File location
			fseek(filePointer, 0, SEEK_SET);

			do {
				fread(&read, sizeof(struct contactStruct), 1, filePointer);		// Reading one contact in at a time
				removeNewline(read.firstName);
				if(strcmp(editInput, read.firstName) == 0) {								// Might be something wrong here
					editLocation = (ftell(filePointer) - sizeof(struct contactStruct));		// If search name matches, get the start location of the contact
					//printf("DEBUG edit pos: %d\n", editLocation);
					nameFound1 = true;	// If edit name is found
				}
			}
			while(ftell(filePointer) < endOfFile);
			if(nameFound1 == true) {
				tempFP = fopen("tempList.db", "w+");
				fseek(filePointer, 0, SEEK_SET);
				fseek(tempFP, 0 , SEEK_SET);
				do {
					if(ftell(filePointer) == editLocation) {
						fseek(filePointer, sizeof(struct contactStruct), SEEK_CUR);
					}
					else {
						fread(&read, sizeof(struct contactStruct), 1, filePointer);
						fwrite(&read, sizeof(struct contactStruct), 1, tempFP);
					}
				}
				while(ftell(filePointer) < endOfFile);

				fclose(tempFP);
				fclose(filePointer);
				remove("contactList.db");
				rename("tempList.db", "contactList.db");
				printf("Contact Removed.\n");
			}
			else if(nameFound1 == false) {	
				printf("The contact you are trying to remove could not be found.\n");
			}




		}
		else if(strcmp(menuInput, "list") == 0) {
			clearScreen();
        	filePointer = fopen("contactList.db", "a+");
			fseek(filePointer, 0, SEEK_END);
			endOfFile = ftell(filePointer);		// End of the File location
			fseek(filePointer, 0, SEEK_SET);

			if(endOfFile > 0) {
				printf("ALL CONTACTS:\n\n");
				do {
					fread(&read, sizeof(struct contactStruct), 1, filePointer);		// Reading one contact in at a time
					removeNewline(read.firstName);
					removeNewline(read.lastName);
					printf("%s %s\n", read.firstName, read.lastName);
				}
				while(ftell(filePointer) < endOfFile);
				printf("\n");
			}
			else {
				printf("There are currently no contacts to list.\n");
			}
			fclose(filePointer);
		}
		else if(strcmp(menuInput, "exit") == 0) {
			menuLoop = false;
		}
		else {
			printf("Invalid Option, Try Again.\n");
		}
	}

	free(menuInput);
	free(searchInput);
	free(editInput);

	return 0;
}

void removeNewline(char inputString[50]) {
	
	int strLength = 0;

	strLength = strlen(inputString) - 1;
	if(inputString[strLength] == '\n') {
		inputString[strLength] = '\0';
	}
}

void clearScreen() {

	printf("\e[1;1H\e[2J");

}