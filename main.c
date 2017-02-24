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