#include <stdio.h>
#include <mem.h>
#include <malloc.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

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
    bool alive;
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

int move(Player *p);
bool attack(Player p);

void print_slots(int position, int direction_movement_from);

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

char *slotTypeLetter(SlotType s) {
    switch (s) {
        case Ground:
            return "G";
        case City:
            return "C";
        case Hill:
            return "H";
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

char *playerPrintName(Player p) {
    char *printName = (char *) malloc(64 * sizeof(char));
    sprintf(printName, "%s (%s) [%dHP]", p.name, playerTypeName(p.type), p.life);
    return printName;
}

Player *players;
size_t players_count;
size_t players_alive;

Slot *slots;
size_t slots_count = 20;

int main(void) {
    srand(time(NULL));

    printf("Welcome to CrossFire!!\n");

    bool demo_mode;
    char d;
    do {
        printf("Would you like to use (d)emo mode or (r)egular mode?:\n");
        scanf(" %c", &d);
    } while (d != 'd' && d != 'r');

    demo_mode = d == 'd';

    if (!demo_mode) {
        printf("Enter number of players(max = 6):\n");
        scanf("%u", &players_count);
    } else {
        players_count = 4;
    }

    players = (Player *) malloc(sizeof(Player) * players_count);


    if (!demo_mode) {
        printf("Player types: (e)lf, (h)uman, (o)gre, (w)izard\n");
        int players_index = 0;
        while (players_index < players_count) {
            char *name = (char *) malloc(32 * sizeof(char));
            char t;
            printf("Enter type and player name:\n");
            scanf(" %c %[^\n]", &t, name);

            PlayerType type;

            if (t == 'e') {
                type = Elf;
            } else if (t == 'h') {
                type = Human;
            } else if (t == 'o') {
                type = Ogre;
            } else if (t == 'w') {
                type = Wizard;
            } else {
                printf("Invalid type.\n");
                continue;
            }

            Player p;
            p.type = type;
            p.name = name;

            players[players_index++] = p;
        }
    } else {
        char *demo_names[] = {"John Sheekey", "Mark Dukes", "Henry McLoughlin", "Liliana Pasquale"};
        PlayerType demo_types[] = {Human, Ogre, Wizard, Elf};

        for (int i = 0; i < players_count; i++) {
            Player p;
            p.type = demo_types[i];
            p.name = demo_names[i];

            players[i] = p;
        }
    }

    for (int i = 0; i < players_count; i++) {
        players[i].alive = true;
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

    if (!demo_mode) {
        printf("Enter number of slots(max = 20): \n");
        scanf("%d", &slots_count);
    } else {
        slots_count = 12;
    }

    slots = (Slot *) malloc(sizeof(Slot) * slots_count);

    for (int i = 0; i < slots_count; i++) {
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

    players_alive = players_count;

    int round = 1;
    while (players_alive > 1) {
        printf("╔══════════╗\n");
        printf("║ Round %2d ║\n", round);
        printf("╚══════════╝\n");
        for (int i = 0; i < players_count; i++) {
            Player p = players[i];
            if (!p.alive) continue;

            char *print_name = playerPrintName(p);
            int print_length = strlen("| Player 0 -  │") + strlen(print_name);

            printf("╭"); for (int x = 0; x < print_length - 4; x++) printf("─"); printf("╮\n");
            printf("│ Player %d - %s │\n", i + 1, print_name);
            printf("╰"); for (int x = 0; x < print_length - 4; x++) printf("─"); printf("╯\n");

            print_slots(players[i].slot, 0);

            char a;
            printf("Would you like to (m)ove or (a)ttack?:\n");
            scanf(" %c", &a);

            if (a == 'a') {
                attack(players[i]);

                print_slots(players[i].slot, 0);
            } else if (a == 'm') {
                int m = move(&players[i]);
                if (m == 0) {
                    printf("Player can't move so must attack.\n");
                    attack(players[i]);

                    print_slots(players[i].slot, 0);
                } else {
                    print_slots(players[i].slot, m);
                }
            }

            printf("\n\n");
        }

        printf("End of round %d\n", round);

        for (int i = 0; i < players_count; i++) {
            printf("%s\n", playerPrintName(players[i]));
        }

        printf("Press enter to continue...\n");
        fflush(stdin);
        while(getchar() != '\n');

        round++;
    }
}

void print_slots(int position, int direction_movement_to) {
    // Row 1
    printf(position != 0 ? "┌" : "┏");
    for (int i = 0; i < slots_count; i++) {
        if (slots[i].player != -1) {
            Player p = players[slots[i].player];

            char *player_print_name = playerPrintName(p);
            int player_name_length = strlen(player_print_name);

            for (int j = 0; j < player_name_length + 2; j++) {
                printf("%s", position != i ? "─" : "━");
            }
        } else {
            printf("%s", position != i ? "─" : "━");
        }
        if (i < (slots_count - 1)) {
            if (position == i) {
                printf("┱");
            } else if (position == i + 1) {
                printf("┲");
            } else {
                printf("┬");
            }
        }
    }
    printf(position != slots_count - 1 ? "┐" : "┓");

    printf("\n");

    // Row 2
    printf(position != 0 ? "│" : "┃");
    for (int i = 0; i < slots_count; i++) {
        if (slots[i].player != -1) {
            Player p = players[slots[i].player];
            printf(" %s ", playerPrintName(p));
        } else {
            if (i == position - direction_movement_to) {
                printf(direction_movement_to == -1 ? "←" : "→");
            } else {
                printf(" ");
            }
        }
        if (i < (slots_count - 1)) {
            printf(position == i || position == i + 1 ? "┃" : "│");
        }
    }
    printf(position != slots_count - 1 ? "│" : "┃");

    printf("\n");

    // Row 3
    printf(position != 0 ? "└" : "┗");
    for (int i = 0; i < slots_count; i++) {
        if (slots[i].player != -1) {
            Player p = players[slots[i].player];

            char *player_print_name = playerPrintName(p);
            char *slot_type_name = slotTypeName(slots[i].type);

            int player_name_length = strlen(player_print_name);
            int slot_type_name_length = strlen(slot_type_name);

            int free_space = player_name_length - slot_type_name_length;

            for (int j = 0; j < free_space / 2 + 1; j++) {
                printf("%s", position != i ? "─" : "━");
            }
            printf("%s", slotTypeName(slots[i].type));
            for (int j = 0; j < (int)((float)free_space / 2 + 0.5) + 1; j++) {
                printf("%s", position != i ? "─" : "━");
            }
        } else {
            printf("%s", slotTypeLetter(slots[i].type));
        }
        if (i < (slots_count - 1)) {
            if (position == i) {
                printf("┹");
            } else if (position == i + 1) {
                printf("┺");
            } else {
                printf("┴");
            }
        }
    }
    printf(position != slots_count - 1 ? "┘" : "┛");

    printf("\n");
}

int move(Player *p) {
    int slot = (*p).slot;

    bool left_empty = (slot > 0) && slots[slot - 1].player == -1;
    bool right_empty = (slot < slots_count - 1) && slots[slot + 1].player == -1;

    if (!left_empty && !right_empty) {
        return 0;
    }

    int direction = 0;
    if (left_empty && right_empty) {
        char d;
        do {
            printf("Would you like to go (l)eft or (r)ight?:\n");
            scanf(" %c", &d);
        } while (d != 'l' && d != 'r');

        if (d == 'l') {
            direction = -1;
        } else { // if (d == 'r') {
            direction = 1;
        }

    } else if (left_empty) {
        direction = -1;
    } else { // if (right_empty) {
        direction = 1;
    }

    (*p).slot += direction;
    slots[(*p).slot].player = slots[slot].player;
    slots[slot].player = -1;

    return direction;
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

    Player *attack_player;
    if (left_player != -1 && right_player != -1) {
        char d;
        do {
            printf("Would you like to attack (l)eft or (r)ight?:\n");
            scanf(" %c", &d);
        } while (d != 'l' && d != 'r');

        if (d == 'l') {
            attack_player = &players[left_player];
        } else { //if (d == 'r') {
            attack_player = &players[right_player];
        }
    } else if (left_player != -1) {
        attack_player = &players[left_player];
    } else { //if (right_player != -1) {
        attack_player = &players[right_player];
    }

    printf("Attacking %s\n", playerPrintName((*attack_player)));

    if (p.strength <= 70) {
        (*attack_player).life -= 0.5 * (*attack_player).strength;
    } else {
        (*attack_player).life -= 0.3 * (*attack_player).strength;
    }

    if ((*attack_player).life <= 0) {
        (*attack_player).life = 0;
        (*attack_player).alive = false;
        slots[(*attack_player).slot].player = -1;
        (*attack_player).slot = -1;

        players_alive--;
        printf("%s killed - RIP\n", playerPrintName((*attack_player)));
    }

    return true;
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