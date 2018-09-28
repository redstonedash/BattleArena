#include <iostream>
#include <cassert>
#include <math.h>
#include <time.h>

int numberOfDeaths = 0;
float currentTime = 0;
const int MAX_PLAYERS = 3;

class Player;

//Player PriorityQue;

class Player * PriorityQue;

class Player {
public:
	int ID = 0;
	int HP;
	unsigned int posX;
	unsigned int posY;
	float timeOfNextAction;
	float speed;
	float attackSpeed;
	bool dead = false;
	void move() {
		posX += rand() % 3 - 1;
		posY += rand() % 3 - 1;
		posX %= 11;
		posY %= 11;
		currentTime = timeOfNextAction;
		timeOfNextAction = currentTime + speed;
		std::cout << "Player " << ID << " is moving to " << posX << ", " << posY << "\n";
	}

	void attack() {
		Player *target = NULL;
		int x = posX + rand() % 3 - 1;
		int y = posY + rand() % 3 - 1;
		x %= 11;
		y %= 11;
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (PriorityQue[i].posX == x && PriorityQue[i].posY == y) {
				target = &PriorityQue[i];
			}
		}
		currentTime = timeOfNextAction;
		timeOfNextAction = currentTime + attackSpeed;
		if (target == NULL) return;
		std::cout << "Player " << ID << " is attacking player " << target->ID << "\n";
		if (target->dead) {
			std::cout << "Player " << target->ID << " is already dead \n";
		}
		else {
			int rng = rand() % 101;
			target->HP -= rng;
			std::cout << "Player " << target->ID << " takes " << rng << " damage!\n";
			if (target->HP <= 0) {
				numberOfDeaths++;
				target->dead = true;
				std::cout << "Player " << target->ID << " dies!\n";
			}
		}
	}
};


void sort(Player arr[MAX_PLAYERS]) {
	for (int i = 0; i < MAX_PLAYERS; i++) {
		int minIndex = -1;
		float min = 0;
		for (int j = i+1; j < MAX_PLAYERS; j++) {
			if (arr[j].timeOfNextAction < min || minIndex == -1) {
				min = arr[j].timeOfNextAction;
				minIndex = j;
			}
			if(min<arr[i].timeOfNextAction)
				std::swap(arr[i], arr[minIndex]);
		}
	}
}

int main() {
	PriorityQue = new Player[MAX_PLAYERS];

	srand(time(0));
	//init
	for (int i = 0; i < MAX_PLAYERS; i++) {
		PriorityQue[i].ID = i;
		PriorityQue[i].HP = 100;
		PriorityQue[i].posX = rand() % 11;
		PriorityQue[i].posY = rand() % 11;
		PriorityQue[i].timeOfNextAction = (float) rand()/RAND_MAX;
		PriorityQue[i].speed = (float)rand() / RAND_MAX;
		PriorityQue[i].attackSpeed = ((float)rand() / RAND_MAX)*2.0f;
		std::cout << (float)rand() / RAND_MAX;
	}
	sort(PriorityQue);
	//game loop
	bool isRunning = true;
	Player data;
	while (isRunning) {
		isRunning = false;
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (!PriorityQue[i].dead) {
				isRunning = true;
				PriorityQue[i].move();
				sort(PriorityQue);
				break;
			}
		}
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (!PriorityQue[i].dead) {
				PriorityQue[i].attack();
				sort(PriorityQue);
				break;
			}
		}
		if (numberOfDeaths == MAX_PLAYERS - 1) {
			isRunning = false;
			break;
		}
	}
	//find last player
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (!PriorityQue[i].dead) {
			std::cout << "Player " << PriorityQue[i].ID << " wins with " << PriorityQue[i].HP << " HP remaning!";
		}
	}
	delete(PriorityQue);

	return 0;

}
