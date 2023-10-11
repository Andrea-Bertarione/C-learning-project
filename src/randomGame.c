#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

const char* const dictionaryStatsIndex[] = {"strength", "vitality", "speed", "intelligence", "charisma"};
const int dictionaryStatsSize = 5;
const bool runDev = true;

const int starterLevel = 3;
const int statPointsXLevel = 3;

struct Player_s{
    int id;
    char name[30];
    int hp;
    int level;
    int xp;
    int stats[5];
}  Player_Default = {0, "", 100, 0, 0, {10, 10, 10, 10, 10}};

typedef struct Player_s Player;
int getStat(Player *character, const char *dictionaryValue);
void updateStat(Player *character, char *dictionaryValue, int amount);
void updateStatErrors(int errorCode);
void changeName(Player *character, char *name);
void changeNameErrors(int errorCode);
void levelUp(Player *character, int lvlAmount);
void levelUpErrors(int errorCode);

int getStat(Player *character, const char *dictionaryValue) {
    int selectedStat = -1;

    for (int i = 0; i < dictionaryStatsSize; i++) {
        if (dictionaryStatsIndex[i] == dictionaryValue) {
            selectedStat = i;
            break;
        }
    }

    if (selectedStat == -1) {
        updateStatErrors(1);
        return 0;
    }

    return character->stats[selectedStat];
}

void updateStat(Player *character, char *dictionaryValue, int amount) {
    int selectedStat = -1;

    for (int i = 0; i < dictionaryStatsSize; i++) {
        if (dictionaryStatsIndex[i] == dictionaryValue) {
            selectedStat = i;
            break;
        }
    }

    if (selectedStat == -1) {
        updateStatErrors(1);
        return;
    }

    character->stats[selectedStat] = amount;

    if (character->stats[selectedStat] != amount) {
        updateStatErrors(2);
        return;
    }
}

void updateStatErrors(int errorCode) {
    if (!runDev) { 
        return; 
    }
    if (errorCode == 0) { 
        return;
    }
    if (errorCode == 1) {
        printf("\033[0;31mError: Selected stat doesn't exists\033[0m\n"); 
        exit(0);
    }
    if (errorCode == 2) { 
        printf("\033[0;31mError: Selected stat is the same or wasn't updated correctly\033[0m\n"); 
        exit(0);
    }
}

void changeName(Player *character, char *name) {
    if (strlen(name) > 30) {
        changeNameErrors(1);
        return;
    }
    if (strlen(name) < 5) {
        changeNameErrors(2);
        return;
    }

    strcpy(character->name, name);
    //printf("%s \n", character->name);
    //printf("%s \n", name);

    if (strcmp(character->name, name) != 0) {
        changeNameErrors(3);
        return;
    }
}

void changeNameErrors(int errorCode) {
     if (!runDev) { 
        return; 
    }
    if (errorCode == 0) { 
        return;
    }
    if (errorCode == 1) {
        printf("\033[0;31mError: Name can't be over 30 characters\033[0m\n");
        exit(0);
    }
    if (errorCode == 2) {
        printf("\033[0;31mError: Name can't be less then 5 characters\033[0m\n"); 
        exit(0);
    }
    if (errorCode == 3) { 
        printf("\033[0;31mError: Selected name wasn't updated correctly\033[0m\n"); 
        exit(0);
    }
}

void levelUp(Player *character, int lvlAmount) {
    char lvlStr[(int)((ceil(log10(lvlAmount))+1)*sizeof(char))];
    sprintf(lvlStr, "%d", lvlAmount);

    const char *multipleLvlString = strcat(lvlStr, " volte");
    printf("\033[0;33mAumento livello %s\033[0m\n", lvlAmount > 1 ? multipleLvlString : " ");
    printf("\033[0;43m------------------------------------------------------------------\033[0m\n"); 

    printf("\n\n");
    
    for (int i = 0; i < dictionaryStatsSize; i++) {
        printf("      ");
        printf("%s(%d) +", dictionaryStatsIndex[i], getStat(character, dictionaryStatsIndex[i]));

        if (i%2 == 0 && i != 0) {
            printf("\n\n");
            printf("      ");
        }
    }
}

int main (void) {
    Player character = Player_Default;

    //updateStat(&character, "strength", 15);
    char tempName[30];

    printf("\033[0;32mInizio creazione del personaggio\033[0m\n");
    printf("\033[0;42m------------------------------------------------------------------\033[0m\n");
    printf("Inserire il nome del personaggio per questa avventura: ");
    scanf("%s", &tempName);

    changeName(&character, tempName);

    printf("Nome selezionato: \033[0;34m%s\033[0m\n", character.name);
    printf("\n");
    printf("\033[0;32mOttenuti %d livelli bonus\033[0m\n", starterLevel);
    printf("\033[0;42m------------------------------------------------------------------\033[0m\n");
    system("clear");

    levelUp(&character, 3);
    
    return 0;
}