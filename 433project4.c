/*
William Wells
&
Chawon White
CS433 Project 4
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

//Return 0 if successful and return -1 if fail
int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);
void printMatrix();
void printAll();
int isSafe();
void initMenu();

int available[NUMBER_OF_RESOURCES];
//Maximum # of Resources
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
//Currently Allocated Resourced
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
//Resources still needed to complete
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];


int main(int argc, char *argv[])
{
	//Build Available Resource Array from CLI
	int size = argc - 1;
	for (int i = 0; i < size; i++)
	{
		available[i] = atoi(argv[i + 1]);
	}

	//Read input file
	FILE *fp = fopen("input.txt", "r");
	const char delim[2] = ",";
	char *token;
	char *newline = "\n";
	int i;
	if (fp != NULL)
	{
		char line[NUMBER_OF_RESOURCES * 2];
		int r = 0;
		int c = 0;
		while (fgets(line, sizeof line, fp) != NULL)
		{
			token = strtok(line, delim);
			while (token != NULL && strcmp(token, newline))
			{
				maximum[r][c] = atoi(token);
				token = strtok(NULL, delim);
				c++;
			}
			fgets(line, sizeof line, fp);
			c = 0;
			r++;
		}
		fclose(fp);
	}
	else
	{
		printf("Error opening text file");
		exit(-1);
	}

	for (int r = 0; r < NUMBER_OF_CUSTOMERS; r++) {
		for (int c = 0; c < NUMBER_OF_RESOURCES; c++) {
			allocation[r][c] = 0;
		}
	}

	for (int r = 0; r < NUMBER_OF_CUSTOMERS; r++) {
		for (int c = 0; c < NUMBER_OF_RESOURCES; c++) {
			need[r][c] = maximum[r][c];
		}
	}
	printAll();

	char option[20];
	while (strcmp(option, "EX"))
	{
		int customer = 0;
		int params[NUMBER_OF_RESOURCES];
		initMenu();
		scanf("%s %d %d %d %d %d", option, &customer, &params[0], &params[1], &params[2], &params[3]);
		
		if (!strcmp(option, "RQ"))
		{
			request_resources(customer, params);
		}
		else if (!strcmp(option, "RL"))
		{
			release_resources(customer, params);
		}
		else if (!strcmp(option, "*"))
		{
			printAll();
		}
	}
	return 0;
}

void printMatrix(int matrix[][NUMBER_OF_RESOURCES])
{
	for (int r = 0; r < NUMBER_OF_CUSTOMERS; r++)
	{
		for (int c = 0; c < NUMBER_OF_RESOURCES; c++)
		{
			printf("%d ", matrix[r][c]);
		}
		printf("\n");
	}
	printf("\n");
}

void printAll()
{
	printf("MAXIMUM\n");
	printf("-------\n");
	printMatrix(maximum);
	printf("ALLOCATION\n");
	printf("----------\n");
	printMatrix(allocation);
	printf("NEED\n");
	printf("----\n");
	printMatrix(need);
	printf("AVAILABLE\n");
	printf("---------\n");
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
	{
		printf("%d ", available[i]);
	}
	printf("\n\n");
}

int isSafe()
{
	//0 is False and 1 is True
	int work[NUMBER_OF_RESOURCES];
	int finish[NUMBER_OF_RESOURCES];
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
	{
		work[i] = available[i];
		finish[i] = 0;
	}

	int check = 0;
	int index = 0;
	int finishCheck = 0;
	for (int r = 0; r < NUMBER_OF_CUSTOMERS;r++)
	{
		for (int c = 0; c < NUMBER_OF_RESOURCES; c++)
		{
			if (need[r][c] <= work[c])
			{
				check = 1;
			}
			else
			{
				check = 0;
			}
			if (check == 0)
			{
				break;
			}
		}
		//If a customer was found
		if (check == 1 && finish[index] == 0)
		{
			finish[index] = 1;
			//index++;
			//Check for finish conditions
			for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
			{
				if (finish[i] == 1)
				{
					finishCheck = 1;
				}
				else
				{
					finishCheck = 0;
					break;
				}
			}
			if (finishCheck == 1)
			{
				return 1;
			}
			for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
			{
				work[i] += need[r][i];
			}
			r = -1;
			index = -1;
		}
		index++;
	}
	return 0;
}

int request_resources(int customer, int request[])
{
	for (int c = 0; c < NUMBER_OF_RESOURCES; c++)
	{
		allocation[customer][c] += request[c];
	}
	for (int c = 0; c < NUMBER_OF_RESOURCES; c++)
	{
		need[customer][c] = (need[customer][c] - allocation[customer][c]);
	}
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
	{
		available[i] = available[i] - request[i];
	}

	if (isSafe())
	{
		printf("Resources Allocated\n\n");
		return 0;
	}
	else
	{
		printf("Resource Allocation Failed\n\n");
		for (int c = 0; c < NUMBER_OF_RESOURCES; c++)
		{
			need[customer][c] += allocation[customer][c];
		}
		for (int c = 0; c < NUMBER_OF_RESOURCES; c++)
		{
			allocation[customer][c] -= request[c];
		}
		for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
		{
			available[i] += request[i];
		}
		return -1;
	}
}

void release_resources(int customer, int release[])
{
	//Check if possible
	for (int c = 0; c < NUMBER_OF_RESOURCES; c++)
	{
		if (allocation[customer][c] - release[c] != 0 || allocation[customer][c] - release[c] > 0)
		{
			printf("Error: To many resources were requested to be released");
			exit(-1);
		}
	}
	printf("Resources Released\n\n");
	for (int c = 0; c < NUMBER_OF_RESOURCES; c++)
	{
		need[customer][c] += allocation[customer][c];
	}
	for (int c = 0; c < NUMBER_OF_RESOURCES; c++)
	{
		allocation[customer][c] -= release[c];
	}
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
	{
		available[i] += release[i];
	}
}

void initMenu()
{
	printf("Request Resources (RQ)\n");
	printf("Release Resources (RL)\n");
	printf("Print current state (* 0 0 0 0 0)\n");
	printf("Exit (EX 0 0 0 0 0)\n");
}
