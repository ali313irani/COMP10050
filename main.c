#include <stdio.h>
#include <mem.h>
#include <malloc.h>

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

unsigned int rand_range(int max);

int main(void) {
	Player players[6];
	Slot slots[20];
	int players_count = 0;
	char t;
	
	printf("Welcome to CrossFire!!");
	
	while(players_count < 6){
		Player p;
		p.name = (char*) malloc(32 * sizeof(char));
		printf("\nPlayer types: (E)lf, (H)uman, (O)gre, (W)izard\n\n");
		
		printf("Enter type and Enter player name: ");
		scanf("%c %s", &t, p.name);
		
		if(t == 'e'){
			p.type = Elf;
		}
		else if(t == 'h'){
			p.type = Human;
		}
		else if(t == 'o'){
			p.type = Ogre;
		}
		else if(t == 'w'){
			p.type = Wizard;
		}
		else{
			break;
		}
		
		printf("%s ", p.name);
		players[players_count++] = p;
		
	}
	
}

/**
 * @param max Maximum value of random integer.
 * @return Random integer [0..max].
 */
unsigned int rand_range(int max) {
	int r;
	int range = max + 1;
	int buckets = RAND_MAX / range;
	int limit = buckets * range;

	// Ensure fair spread of numbers by excluding some numbers in
	// the upper range of rand() that would create a biased result
	do {
		r = rand();
	} while (r >= limit);

	return (unsigned int) (r / buckets);
}