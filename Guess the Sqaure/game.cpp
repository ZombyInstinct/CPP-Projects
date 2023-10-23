/*
 * game.cpp
 *
 *  Created on: Oct 21, 2023
 *      Author: Zaki Ilyas
 */
#include <iostream>
#include <string>
#include <iostream>
#include <ios>
#include <limits>
#include <fstream>
#include <cstdio>

using namespace std;

/*-------------------------------------
 * class Player
 * stores information about the player
 -------------------------------------*/
class Player
{
public:
	Player() {};
	Player(string nm, int ng): name(nm), numGuesses(ng) {};

	string getName() {return name;};
	int getGuesses() {return numGuesses;};

	void setName(string n) {name = n;};
	void setGuesses(int i) {numGuesses = i;};
private:
	string name;
	int numGuesses;
};

/*-------------------------------------
 * class Leaderboard
 * stores leaderboard information and
 * certain functions
 -------------------------------------*/
class Leaderboard
{
public:
	Leaderboard() {};

	void fillLeadersList(string filename);
	void insertPlayer(Player p);
	void printLeaders();
	void fileWriter(string filename);

private:
	static const int NUM_LEADERS = 5;
	Player leaders[NUM_LEADERS];
};

/*-------------------------------------
 * insertPlayer (Player p)
 * sorts the leaderboard
 -------------------------------------*/
void Leaderboard::insertPlayer(Player p)
{
	//orders the leaders in the array
	for (int i = 0; i < 5; i++)
	{
		int guess = leaders[i].getGuesses();
		if (guess >= p.getGuesses() || guess == 0)
		{
			for (int j = i; j < 5; j++)
			{
				Player temp = leaders[j];
				leaders[j] = p;
				p = temp;
			}
			break;
		}
	}
};

/*-------------------------------------
 * printLeaders()
 * prints the leaderboard
 -------------------------------------*/
void Leaderboard::printLeaders()
{
	cout << "\nLeaderboard\n" << "--------------------\n";
		for (int i = 0; i < 5; i++)
		{
			if (leaders[i].getGuesses() > 0)
			{
				cout << leaders[i].getName() << " made " << leaders[i].getGuesses() << " guess(es).\n";
			}
		}
		cout << "--------------------\n\n";
};

/*-------------------------------------
 * fillLeadersList(string filename)
 * opens a file if it exists and copies
 * the leaderboard
 -------------------------------------*/
void Leaderboard::fillLeadersList(string filename)
{
	//opens file
	ifstream fp;
	fp.open(filename);

	string line;
	int indexT = 0;
	string tempName;
	int tempGuess;


	if (fp.is_open())
	{
		while (getline (fp, line))
		{
			for (int i = 0; i < line.length(); i++)
			{
				char c =  line[i];
				if (c == '~')
				{
					indexT = i;
					break;
				}
			}
			tempName = line.substr(0, indexT);
			tempGuess = stoi(line.substr(indexT+1, line.length()-(indexT+1)));
			Player p(tempName, tempGuess);
			insertPlayer(p);
		}
		fp.close();

		cout << ("Previous scores were found the leaderboard has updated.\n");
		printLeaders();
	}
};

/*-------------------------------------
 * fileWriter(string filename)
 * pastes the list into the file
 -------------------------------------*/
void Leaderboard::fileWriter(string filename)
{

	ofstream fp (filename);

	if (fp.is_open())
	{
		for (int i = 0; i < 5; i++)
		{
			if (leaders[i].getGuesses() > 0)
			{
				fp << leaders[i].getName() << "~" << leaders[i].getGuesses() << "\n";
			}
		}
		fp.close();
	}


	cout << ("\n");
};


/*-------------------------------------
 * getGuess()
 * returns a valid number from the user
 -------------------------------------*/
int getGuess()
{
	int guess;
	cin >> guess;
	while (guess < 10 || guess > 100)
	{
		cout << ("Please input a valid answer (must be from 10-100).\n ");
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin >> guess;
	}
	return guess;
}

/*-------------------------------------
 * playGuessingGame()
 * uses time to generate random seed
 * plays out one round of the game
 -------------------------------------*/
void playGuessingGame(Leaderboard* leaderBoard)
{
	string name;
	cout << ("Please enter your name to begin: ");
	cin >> name;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	srand(time(0));
	int num = (rand() % 91) + 10;
	float root = sqrt(num);

	cout << root << " is the square root of what number?\nYour guess: ";

	int numGuesses = 0;
	while (num > 0)
	{
		int guess = getGuess();
		numGuesses ++;
		if (guess > num)
		{
			cout << ("Too high, try again: ");
		}
		else if (guess < num)
		{
			cout << ("Too low, try again: ");
		}
		else
		{
			cout << ("YOU GOT IT!!\n");
			cout << "You made " << numGuesses << " guess(es).\n";
			num = 0;
		}
	}

	//creates the player object
	Player p(name, numGuesses);
	leaderBoard->insertPlayer(p);
	leaderBoard->printLeaders();

}

/*-------------------------------------
 * main()
 * prompts user
 * plays games
 * prompts quit
 -------------------------------------*/
int main()
{

	Leaderboard leaderBoard;
	string file;
	file = "file1.txt";

	leaderBoard.fillLeadersList(file);

		cout << ("Welcome! Press any key to continue or 'q' to quit\n");

		char myC;
		cin >> myC;

		while (myC != 'q')
		{

			playGuessingGame(&leaderBoard);
			cout << ("Press any key to continue or 'q' to quit.\n");

			cin >> ws;
			cin >> myC;
		}

		leaderBoard.fileWriter(file);
		cout << ("Bye! Play again soon.");

	return 0;
}

