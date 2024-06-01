/*
* FILE : m1.c
* PROJECT : SENG1050 - Assignment # 1
* PROGRAMMER : Justin Blake
* FIRST VERSION : 2024-05-31
* DESCRIPTION :
* The functions in this file are used to receive user input, store them, and print the formatted results.
*/
#pragma warning(disable: 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FlightInfo* findFlight(struct FlightInfo* head, char* destination, char* date);
void deleteNode(struct FlightInfo* node, struct FlightInfo** head, struct FlightInfo** tail);

#define MAX_CHAR 31
#define MAX_STRINGS 10

struct FlightInfo
{
	char* destination;
	char* date;
	float fare;
	struct FlightInfo* nextFlight;
	struct FlightInfo* prevFlight;
};

int main(void) {
	struct FlightInfo* headByFare = NULL;
	struct FlightInfo* headByDestination = NULL;
	struct FlightInfo* tailByFare = NULL;
	struct FlightInfo* tailByDestination = NULL;

	char inputDestination[MAX_CHAR]="";
	char inputDate[MAX_CHAR]="";
	char inputFare[MAX_CHAR]="";
	float parsedFare = 0;

	for (int counter = 0; counter < MAX_STRINGS; counter++)
	{
		printf("\nPlease enter a destination:\n");
		fgets(inputDestination, MAX_CHAR, stdin);
		inputDestination[strcspn(inputDestination, "\n")] = '\0';
		if (strchr(inputDestination, '.') != NULL)
		{
			printf("\nInvalid destination. Entry discarded.\n");
			continue;
		}

		printf("\nPlease enter a date:\n");
		fgets(inputDate, MAX_CHAR, stdin);
		inputDate[strcspn(inputDate, "\n")] = '\0';
		if (strchr(inputDate, '.') != NULL)
		{
			printf("\nInvalid date. Entry discarded.\n");
			continue;
		}

		printf("\nPlease enter the fare:\n");
		fgets(inputFare, MAX_CHAR, stdin);
		parsedFare = atof(inputFare);

		struct FlightInfo* newFlight = (struct FlightInfo*)malloc(sizeof(struct FlightInfo));
		if (newFlight == NULL)
		{
			printf("No more memory available.");
			break;
		}

		newFlight->destination = (char*)malloc((strlen(inputDestination) + 1) * sizeof(char));
		newFlight->date = (char*)malloc((strlen(inputDate) + 1) * sizeof(char));
		if (newFlight->destination == NULL || newFlight->date == NULL)
		{
			printf("No more memory available.\n");
			free(newFlight);
			break;
		}

		strcpy(newFlight->destination, inputDestination);
		strcpy(newFlight->date, inputDate);
		newFlight->fare = parsedFare;
		newFlight->nextFlight = NULL;
		newFlight->prevFlight = NULL;

		// Insert into the sorted linked list by fare
		if (headByFare == NULL)
		{
			headByFare = newFlight;
			tailByFare = newFlight;
		}
		else
		{
			struct FlightInfo* current = headByFare;
			struct FlightInfo* prev = NULL;

			while (current != NULL && current->fare < newFlight->fare)
			{
				prev = current;
				current = current->nextFlight;
			}

			if (prev == NULL)
			{
				newFlight->nextFlight = headByFare;
				headByFare->prevFlight = newFlight;
				headByFare = newFlight;
			}
			else
			{
				prev->nextFlight = newFlight;
				newFlight->prevFlight = prev;
				newFlight->nextFlight = current;

				if (current != NULL)
				{
					current->prevFlight = newFlight;
				}
				else
				{
					tailByFare = newFlight;
				}
			}
		}

		struct FlightInfo* newFlightD = (struct FlightInfo*)malloc(sizeof(struct FlightInfo));
		if (newFlightD == NULL)
		{
			printf("No more memory available.");
			break;
		}

		newFlightD->destination = (char*)malloc((strlen(inputDestination) + 1) * sizeof(char));
		newFlightD->date = (char*)malloc((strlen(inputDate) + 1) * sizeof(char));
		if (newFlightD->destination == NULL || newFlightD->date == NULL)
		{
			printf("No more memory available.\n");
			break;
		}

		strcpy(newFlightD->destination, inputDestination);
		strcpy(newFlightD->date, inputDate);
		newFlightD->fare = parsedFare;
		newFlightD->nextFlight = NULL;
		newFlightD->prevFlight = NULL;

		// Insert into the sorted linked list by destination
		if (headByDestination == NULL) 
		{
			headByDestination = newFlightD;
			tailByDestination = newFlightD;
		}
		else 
		{
			struct FlightInfo* current = headByDestination;
			struct FlightInfo* prev = NULL;

			while (current != NULL && strcmp(current->destination, newFlightD->destination) < 0) 
			{
				prev = current;
				current = current->nextFlight;
			}

			if (prev == NULL) 
			{
				newFlightD->nextFlight = headByDestination;
				headByDestination->prevFlight = newFlightD;
				headByDestination = newFlightD;
			}
			else 
			{
				prev->nextFlight = newFlightD;
				newFlightD->prevFlight = prev;
				newFlightD->nextFlight = current;
				if (current != NULL) 
				{
					current->prevFlight = newFlightD;
				}
				else 
				{
					tailByDestination = newFlightD;
				}
			}
		}
	}

	printf("\nFlights sorted by fare:\n");
	struct FlightInfo* current = headByFare;
	while (current != NULL)
	{
		printf("%-35s%-35s%.2f\n", current->destination, current->date, current->fare);
		current = current->nextFlight;
	}

	printf("\nFlights sorted by destination:\n");
	current = headByDestination;
	while (current != NULL)
	{
		printf("%-35s%-35s%.2f\n", current->destination, current->date, current->fare);
		current = current->nextFlight;
	}


	// Additional user input for destination and date
	printf("\nPlease enter a destination to search:\n");
	fgets(inputDestination, MAX_CHAR, stdin);
	inputDestination[strcspn(inputDestination, "\n")] = '\0';

	printf("\nPlease enter a date to search:\n");
	fgets(inputDate, MAX_CHAR, stdin);
	inputDate[strcspn(inputDate, "\n")] = '\0';

	struct FlightInfo* flightToUpdate = findFlight(headByDestination, inputDestination, inputDate);
	if (flightToUpdate != NULL)
	{
		printf("Flight found\n");
		printf("Enter a new fare:\n");
		fgets(inputFare, MAX_CHAR, stdin);
		parsedFare = atof(inputFare);

		if (parsedFare != flightToUpdate->fare)
		{
			flightToUpdate->fare = parsedFare;

			struct FlightInfo* flightInFareList = findFlight(headByFare, inputDestination, inputDate);
			deleteNode(flightInFareList, &headByFare, &tailByFare);

			struct FlightInfo* newFlight = (struct FlightInfo*)malloc(sizeof(struct FlightInfo));
			if (newFlight == NULL)
			{
				printf("No more memory available.");
				return 1;
			}

			newFlight->destination = strdup(flightToUpdate->destination);
			newFlight->date = strdup(flightToUpdate->date);
			newFlight->fare = parsedFare;
			newFlight->nextFlight = NULL;
			newFlight->prevFlight = NULL;

			if (headByFare == NULL)
			{
				headByFare = newFlight;
				tailByFare = newFlight;
			}
			else
			{
				struct FlightInfo* current = headByFare;
				struct FlightInfo* prev = NULL;

				while (current != NULL && current->fare < newFlight->fare)
				{
					prev = current;
					current = current->nextFlight;
				}

				if (prev == NULL)
				{
					newFlight->nextFlight = headByFare;
					headByFare->prevFlight = newFlight;
					headByFare = newFlight;
				}
				else
				{
					prev->nextFlight = newFlight;
					newFlight->prevFlight = prev;
					newFlight->nextFlight = current;
					if (current != NULL)
					{
						current->prevFlight = newFlight;
					}
					else
					{
						tailByFare = newFlight;
					}
				}
			}
		}
		else
		{
			printf("Fare unchanged.\n");
		}
	}
	else
	{
		printf("No matching flight found.\n");
	}

	// Print the updated flights sorted by fare
	printf("\nFlights sorted by fare:\n");
	current = headByFare;
	while (current != NULL)
	{
		printf("%-35s%-35s%.2f\n", current->destination, current->date, current->fare);
		current = current->nextFlight;
	}

	// Print the updated flights sorted by destination
	printf("\nFlights sorted by destination:\n");
	current = headByDestination;
	while (current != NULL)
	{
		printf("%-35s%-35s%.2f\n", current->destination, current->date, current->fare);
		current = current->nextFlight;
	}

	current = headByFare;
	while (current != NULL)
	{
		struct FlightInfo* next = current->nextFlight;
		free(current->destination);
		free(current->date);
		free(current);
		current = next;
	}

	current = headByDestination;
	while (current != NULL)
	{
		struct FlightInfo* next = current->nextFlight;
		free(current->destination);
		free(current->date);
		free(current);
		current = next;
	}

	return 0;
}

//
// FUNCTION : findFlight
// DESCRIPTION :
// Searches the linked list for user generated input.
// PARAMETERS :
// struct FlightInfo* head: The first entry in a list.
// char* destination: The destination the function is searching for.
// char* date: The date the function is searching for.
// RETURNS :
// current: The list entry that matches what the user entered.
// NULL
struct FlightInfo* findFlight(struct FlightInfo* head, char* destination, char* date)
{
	struct FlightInfo* current = head;
	while (current != NULL)
	{
		if (strcmp(current->destination, destination) == 0 && strcmp(current->date, date) == 0)
		{
			return current;
		}
		current = current->nextFlight;
	}
	return NULL;
}

//
// FUNCTION : deleteNode
// DESCRIPTION :
// Moves pointers around an entry in a linked list then deletes the entry.
// PARAMETERS :
// struct FlightInfo* node: An entry in a linked list.
// struct FlightInfo** head: The first entry of the linked list.
// struct FlightInfo** tail: The last entry of the linked list.
// RETURNS :
// N/A
void deleteNode(struct FlightInfo* node, struct FlightInfo** head, struct FlightInfo** tail)
{
	if (node == NULL)
	{
		return;
	}

	if (node->prevFlight != NULL)
	{
		node->prevFlight->nextFlight = node->nextFlight;
	}
	else
	{
		*head = node->nextFlight;
	}

	if (node->nextFlight != NULL)
	{
		node->nextFlight->prevFlight = node->prevFlight;
	}
	else
	{
		*tail = node->prevFlight;
	}

	free(node->destination);
	free(node->date);
	free(node);
}
