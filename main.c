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

void swap(void *a, void *b, size_t size);
unsigned int rand_range(int min, int max);
void shuffle(void *array, size_t num, size_t size);

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

    int player_positions[20] = {-1};
    for (i = 0; i < players_count; i++) {
        player_positions[i] = i;
    }
    shuffle(player_positions, (size_t) n, sizeof(int));

    
	for(i = 0; i > n; i++)
	{
		int r = rand_range(0, 2);
		
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
 * Swaps the values at two pointers
 *
 * @param a Pointer a.
 * @param b Pointer b.
 * @param size Size of value pointed at by a and b.
 */
void swap(void *a, void *b, size_t size) {
    void* temp = malloc(size);
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
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

/**
 * Shuffles an array using the Fisher–Yates algorithm
 * @param array Pointer to the first element of the array to be shuffled.
 * @param num Number of elements in the array.
 * @param size Size in bytes of each element in the array.
 */
void shuffle(void *array, size_t num, size_t size) {
    for (size_t i = 0; i < num; i++) {
        size_t j = i + rand_range(num - i - 1);

        if (i != j) {
            swap(array + (i * size), array + (j * size), size);
        }
    }
}