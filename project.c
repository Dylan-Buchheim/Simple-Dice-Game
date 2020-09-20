// Dylan Buchheim
// Group N
// Lab 502/Lecture 004
// Lab09
// 4/4/2018

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project.h"

//Defining the player struct --
struct player{
	char name[200];
	int balance;
	int gain;
};

//Global Variable -------------

struct player players[50];
int numberOfPlayers = 0;
FILE *fp;
#define FNAME "players.txt"

//InstantiatePlayer function, returns a player with all the data set from the parameters
struct player InstantiatePlayer(const char *name, const int balance, const int gain){
	struct player p;
	strcpy(p.name, name);
	p.balance = balance;
	p.gain = gain;
	return p;
}

//Write Menu Function ---------
void WriteMenu(){
	printf("\nWelcome to the game of dice!\n");
	printf("0) Top up your balance.\n");
	printf("1) Play the game.\n");
	printf("2) Top five players by balance.\n");
	printf("3) Top five players by winnings.\n");
	printf("4) Save and exit the game.\n");
	printf("Enter your selection: \n");
}

//TopUpBalance function, asks for the players name and allows them to add money to their balance.
void TopUpBalance(){
	//local variables which store the players name and the money they want to add
	char name[200];
	int money;
	//get user input for their name
	printf("Enter your name: ");
	scanf("%s", name);
	getchar();
	//loop through all the names of the players
	for(int i = 0; i < numberOfPlayers; i++){
		//if the input name is found in the list, ask how much money they want to add
		if(strcmp(players[i].name, name) == 0){
			printf("How much money do you want to add? ");
			scanf("%d", &money);
			getchar();
			players[i].balance += money;
			printf("%s's new balance is: %d\n", players[i].name, players[i].balance);
			//press enter to return to menu
			printf("Press enter to continue...");
			getchar();
			MenuSelect();
		}
	}
	printf("Invalid Name! Returning to menu...\n");
	MenuSelect();
}

//FindPlayerIndex function, this function returns the index of the requested player
int FindPlayerIndex(char name[]){
	//loop through all the players in the players list
	for(int i = 0; i < numberOfPlayers;i++){
		//if the given name is the same as the player[i].name
		if(strcmp(name,players[i].name) == 0){
			return i;
		}
	}
	return 200;
}

//RollDice function, this function returns a random dice number
int RollDice(){
	int diceNum = 1 + rand() / (RAND_MAX / (6 - 1 + 1) + 1);
	return diceNum;
}

//CheckBalance function, returns 1 if the players balance is high enough to play, otherwise is return 0
int CheckBalance(int playerIndex){
	if(players[playerIndex].balance > 0){
		return 1;
	}else{
		return 0;
	}
}

//GameWin function, executes the game win scenario
void GameEnd(int playerIndex, int winValue){
	char selection;
	//if the player won
	if(winValue == 1){
		//print the players new balance
		players[playerIndex].balance += 10;
		players[playerIndex].gain += 10;
		printf("You win the game. Your current balance is %d.\n", players[playerIndex].balance);
	}
	//if the player lost
	else{
		//print the players new balance
		players[playerIndex].balance -= 1;
		players[playerIndex].gain -= 1;
		printf("You lost the game. Your current balance is %d.\n", players[playerIndex].balance);
	}
	
	//ask the user if they want to play again
	printf("----------\nWould you like to play again?\n0) Play again.\n1) Return to menu.\nEnter your selection: ");
	scanf("%c", &selection);
	getchar();
	//if the user wants to play again
	if(selection == '0'){
		//check their balance to see if they can play
		if(CheckBalance(playerIndex) == 1){
			PlayGame(playerIndex, 0);
		}else{
			printf("Insufficient funds! Add more funds to play the game. Returning to menu...\n");
			MenuSelect();
		}
		
	}
	//if the user wants to return to menu
	else if(selection == '1'){
		MenuSelect();
	}
	//invalid input
	else{
		printf("Invalid input, returning to menu...\n");
		MenuSelect();
	}
}

//PlayGame function, controls the logic of the dice game
void PlayGame(int playerIndex, int point){
	//prompt the player to roll the dice
	printf("----------\nPress enter to roll the dice.");
	getchar();
	//print the result of the dice roll
	int dice1 = RollDice();
	int dice2 = RollDice();
	int sum = dice1 + dice2;
	printf("The values of the dices are %d and %d whose sum is %d.\n", dice1, dice2, sum);
	//Determine whether the player won or not
	
	//if the sum is 7 or eleven on the first throw
	if((sum == 7 || sum == 11) && point == 0){
		GameEnd(playerIndex, 1);
	}
	//if the sum is equal to their point
	else if(sum == point){
		GameEnd(playerIndex, 1);
	}
	//if the sum is 2 3 or 12 on the first throw
	else if((sum == 2 || sum == 3 || sum == 12) && point == 0){
		GameEnd(playerIndex, 0);
	}
	else if(sum == 7 && point != 0){
		GameEnd(playerIndex, 0);
	}
	//if the sum is any of the other numbers
	else{
		PlayGame(playerIndex, sum);
	}

	
}

//TopFiveBalance function, prints the top five players by balance
void TopFiveBalance(){
	printf("Top Five Players by Balance: \n");
	//sort the players array by balance in decending order.
	for(int x = 0;x < numberOfPlayers;x++){
		for(int y = (x+1); y < numberOfPlayers; y++){
			//if the first balance is less than the second, swap the two players
			if(players[x].balance < players[y].balance){
				struct player temp = players[x];
				players[x] = players[y];
				players[y] = temp;
			}
		}
	}
	//change this, currently just displays the players in the file
	for(int i = 0; i < (numberOfPlayers < 5 ? numberOfPlayers : 5); i++){
		printf("%d. %s: %d \n", (i+1), players[i].name, players[i].balance);
	}
	//press enter to return to menu
	printf("Press enter to continue...");
	getchar();
	//make it return to menu
	MenuSelect();
}

//TopFiveWinnings function, prints the top five players by winnings
void TopFiveWinnings(){
	printf("Top Five Players by Winnings: \n");
	//sort the players array by balance in decending order.
	for(int x = 0;x < numberOfPlayers;x++){
		for(int y = (x+1); y < numberOfPlayers; y++){
			//if the first balance is less than the second, swap the two players
			if(players[x].gain < players[y].gain){
				struct player temp = players[x];
				players[x] = players[y];
				players[y] = temp;
			}
		}
	}
	//change this, currently just displays the players in the file
	for(int i = 0; i < (numberOfPlayers < 5 ? numberOfPlayers : 5); i++){
		printf("%d. %s: %d \n", (i+1), players[i].name, players[i].gain);
	}
	//press enter to return to menu
	printf("Press enter to continue...");
	getchar();
	//make it return to menu
	MenuSelect();
}

//GetPlayerInfo function, retrieves the information from the file and stores it in the players array.
void GetPlayerInfo(FILE *fp){
	for(;;){
		//scan the next line of names
		fscanf(fp, "%s\t%d\t%d", players[numberOfPlayers].name, &players[numberOfPlayers].balance, &players[numberOfPlayers].gain);
		//add one to the number of players
		numberOfPlayers ++;
		//if we are at then end of the file break the loop
		if(feof(fp)){
			break;
		} 
	}
}

//UpdatePlayerInfo function, takes the information in the array and rewrites the players file
void UpdatePlayerInfo(FILE *fp){
	//close the file
	fclose(fp);
	//reopen the file in write mode and overwrite everything in it
	fp = fopen(FNAME, "w+");
	//if the file name does not exist
	if(fp == NULL){
		printf("Can't open %s\n", FNAME);
		exit(EXIT_FAILURE);
	}
	//loop through all the information in the players array and write it to the file
	for(int i = 0; i < numberOfPlayers; i++){
		//if you are on the last player do not add a newline
		if(i == numberOfPlayers - 1){
			fprintf(fp, "%s\t%d\t%d", players[i].name, players[i].balance, players[i].gain);
		}else{
			fprintf(fp, "%s\t%d\t%d\n", players[i].name, players[i].balance, players[i].gain);
		}
	}
	fclose(fp);

}

//Menu selection function -----
void MenuSelect(){
	//varible which stores the users selection, name, and their index in the players list
	char selection, name[200];
	int playerIndex;
	//print the menu options and get the users selection
	WriteMenu();
	scanf("%c", &selection);
	//absorb the enter
	getchar();
	//switch statement which executes the users selection 
	switch(selection){
		case '0':
			TopUpBalance();
		case '1':
			printf("What is your name: ");
			scanf("%s", name);
			getchar();
			playerIndex = FindPlayerIndex(name);
			//if the player was found in the list
			if(playerIndex != 200){
				//check their balance to see if they can play
				if(CheckBalance(playerIndex) == 1){
					PlayGame(playerIndex, 0);
				}else{
					printf("Insufficient funds! Add more funds to play the game. Returning to menu...\n");
					MenuSelect();
				}	
			}
			//if the player was not already in the list, add them and start the game
			else{
				players[numberOfPlayers] = InstantiatePlayer(name,1,0);
				playerIndex = numberOfPlayers;
				numberOfPlayers++;
				PlayGame(playerIndex,0);
			}
			
		case '2':
			TopFiveBalance();
		case '3':
			TopFiveWinnings();
		case '4':
			printf("Exiting...\n");
			UpdatePlayerInfo(fp);
			exit(EXIT_FAILURE);
		default:
			printf("Invalid Input, restarting...\n");
			MenuSelect();
	}
}

//Main Function ---------------
int main(void){
	//open the players.txt file
	fp = fopen(FNAME, "r");
	//if the file name does not exist
	if(fp == NULL){
		printf("Can't open %s\n", FNAME);
		exit(EXIT_FAILURE);
	}
	//Get the updated player info
	GetPlayerInfo(fp);
	//Start the game menu.
	MenuSelect();
}