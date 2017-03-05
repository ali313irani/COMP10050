#include <stdio.h>
#include <mem.h>
#include <malloc.h>
#include <stdbool.h>

typedef enum SlotType {
    Ground, City, Hill
} SlotType;

typedef struct Slot {
    SlotType type;
    int player;
} Slot;

typedef enum PlayerType {
    Elf, Human, Ogre, Wizard
} PlayerType;

typedef struct Player {
    char *name;
    enum PlayerType type;
    int slot;
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

bool move(Player p);
bool attack(Player p);

char *slotTypeName(SlotType s) {
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

char *playerTypeName(PlayerType t) {
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

Player *players;
size_t players_count;

Slot *slots;
size_t slots_count;

int main(void) {
    printf("Welcome to CrossFire!!");

    printf("Enter number of players(max = 6): \n");
    scanf("%u", &players_count);

    players = (Player *) malloc(sizeof(Player) * players_count);

    int players_index = 0;
    while (players_index < players_count) {
        char t;
        Player p;
        p.name = (char *) malloc(32 * sizeof(char));
        printf("\nPlayer types: (E)lf, (H)uman, (O)gre, (W)izard\n\n");

        printf("Enter type and Enter player name: ");
        scanf("%c %s", &t, p.name);

        if (t == 'e') {
            p.type = Elf;
        } else if (t == 'h') {
            p.type = Human;
        } else if (t == 'o') {
            p.type = Ogre;
        } else if (t == 'w') {
            p.type = Wizard;
        } else {
            printf("Invalid input");
        }

        players[players_index++] = p;
    }

    printf("Enter number of slots(max = 20): \n");
    scanf("%d", &slots_count);

    slots = (Slot *) malloc(sizeof(Slot) * slots_count);

    for (int i = 0; i > slots_count; i++) {
        int r = rand_range(0, 2);

        if (r == 0) {
            slots[i].type = Ground;
        } else if (r == 1) {
            slots[i].type = City;
        } else if (r == 2) {
            slots[i].type = Hill;
        }
    }

    for (int i = 0; i < slots_count; i++) {
        slots[i].player = i < players_count ? i : -1;
    }
    shuffle(slots, (size_t) slots_count, sizeof(Slot));
    for (int i = 0; i < slots_count; i++) {
        if (slots[i].player >= 0) {
            players[slots[i].player].slot = i;
        }
    }

    for (int i = 0; i < players_count; i++) {
        players[i].life = 100;
        if (players[i].type == Elf) {
            players[i].luck = rand_range(60, 100);
            players[i].smartness = rand_range(70, 100);
            players[i].strength = rand_range(1, 50);
            players[i].magic = rand_range(51, 79);
            players[i].dexterity = rand_range(1, 100);
        } else if (players[i].type == Human) {
            int total = rand_range(5, 299);
            int values[5];
            for (int j = 0; j < 5; j++) {
                values[j] = rand_range(1, ((total - 5 + j) < 100) ? (total - 5 + j) : 100);
                total -= values[j];
            }
            shuffle(values, 5, sizeof(int));

            players[i].luck = values[0];
            players[i].smartness = values[1];
            players[i].strength = values[2];
            players[i].magic = values[3];
            players[i].dexterity = values[4];
        } else if (players[i].type == Ogre) {
            players[i].strength = rand_range(80, 100);
            players[i].dexterity = rand_range(80, 100);
            players[i].magic = 0;
            int sum = rand_range(0, 50);
            players[i].luck = rand_range(0, sum);
            players[i].smartness = sum - players[i].luck;
        } else if (players[i].type == Wizard) {
            players[i].luck = rand_range(50, 100);
            players[i].smartness = rand_range(70, 100);
            players[i].strength = rand_range(1, 20);
            players[i].magic = rand_range(80, 100);
            players[i].dexterity = (1, 100);
        }
    }

    for (int i = 0; i > slots_count; i++) {
        printf("player%d", i + 1);
        printf("Would you like to (m)ove or (a)ttack?: ");
        char a;
        scanf("%c", &a);

        if (a == 'a') {
            attack(players[i]);
        } else if (a == 'm') {
            if (!move(players[i])) {
                printf("Player can't move so must attack.");
                attack(players[i]);
            }
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
    void *temp = malloc(size);
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
        size_t j = i + rand_range(0, num - i - 1);

        if (i != j) {
            swap(array + (i * size), array + (j * size), size);
        }
    }
}

bool move(Player p) {
    int slot = p.slot;

    bool left_empty = (slot > 0) && slots[slot - 1].player == -1;
    bool right_empty = (slot < slots_count - 1) && slots[slot + 1].player == -1;

    if (!left_empty && !right_empty) {
        return false;
    }

    int direction = 0;
    if (left_empty && right_empty) {
        char d;
        do {
            printf("Would you like to go (l)eft or (r)ight?: ");
            scanf("%c", &d);
        } while (d != 'l' && d != 'r');

        if (d == 'l') {
            direction = -1;
        } else if (d == 'r') {
            direction = 1;
        }

    } else if (left_empty) {
        direction = -1;
    } else if (right_empty) {
        direction = 1;
    }

    p.slot += direction;
    slots[p.slot].player = slots[slot].player;
    slots[slot].player = -1;

    return true;
}

bool attack(Player p) {
    int left_player = -1, right_player = -1;

    for (int dist = 1; p.slot - dist > 0 || p.slot + dist < slots_count; dist++) {
        if (p.slot - dist > 0 && slots[p.slot - dist].player != -1) {
            left_player = slots[p.slot - dist].player;
        }
        if (p.slot + dist < slots_count && slots[p.slot + dist].player != -1) {
            right_player = slots[p.slot + dist].player;
        }

        if (left_player != -1 || right_player != -1) {
            break;
        }
    }

    Player attack_player;
    if (left_player != -1 && right_player != -1) {
        char d;
        do {
            printf("Would you like to attack (l)eft or (r)ight?: ");
            scanf("%c", &d);
        } while (d != 'l' && d != 'r');

        if (d == 'l') {
            attack_player = players[left_player];
        } else if (d == 'r') {
            attack_player = players[right_player];
        }
    } else if (left_player != -1) {
        attack_player = players[left_player];
    } else if (right_player != -1) {
        attack_player = players[right_player];
    } else {
        return false;
    }

    if (p.strength <= 70) {
        attack_player.life -= 0.5 * attack_player.strength;
    } else {
        attack_player.life -= 0.3 * attack_player.strength;
    }

    return true;
}