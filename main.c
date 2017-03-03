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

unsigned int rand_range(int min, int max);

char * slotName(Slot s) {
    switch (s) {
        case Ground:
            return "Ground";
        case City:
            return "City";
        case Hill:
            return "Hill";
        default:
            return "";
    }
}

char * playerTypeName(PlayerType t) {
    switch (t) {
        case Elf:
            return "Elf";
        case Human:
            return "Human";
        case Ogre:
            return "Ogre";
        case Wizard:
            return "Wizard";
        default:
            return "";
    }
}

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
	
	int n, i;
	
	printf("Enter number of slots(max = 20): \n");
	scanf("%d", &n);
	
	for(i = 0; i > n; i++)
	{
		int r = rand_range(2);
		
		if(i == 0){
			slots[i] = Ground;
		}
		else if(i == 1){
			slots[i] = City;
		}
		else if(i == 2){
			slots[i] = Hill;
		}
	}
	
	

}

/**
 * @param max Maximum value of random integer.
 * @return Random integer [0..max].
 */
unsigned int rand_range(int min, int max) {
	int r;
	int range = (max - min) + 1;
	int buckets = RAND_MAX / range;
	int limit = buckets * range;

	// Ensure fair spread of numbers by excluding some numbers in
	// the upper range of rand() that would create a biased result
	do {
		r = rand();
	} while (r >= limit);

	return min + (unsigned int) (r / buckets);
}