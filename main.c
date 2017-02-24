#include <stdio.h>

typedef enum Slot {
	Ground, City, Hill
} Slot;

typedef enum PlayerType {
	Elf, Human, Ogre, Wizard
} PlayerType;

typedef struct Player {
	char *name;
	enum PlayerType type;
	int life;
	int smartness;
	int strength;
	int magic;
	int luck;
	int dexterity;
} Player;

int main(void) {
	Player players[6];
	Slot slots[20];

	printf("Hello world!");
}