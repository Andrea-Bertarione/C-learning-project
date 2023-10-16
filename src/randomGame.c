#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

const char* const dictionaryStatsIndex[] = {"strength", "vitality", "dexterity", "intelligence", "charisma"};
const char* const dictionaryColorStatsIndex[] = {"\033[0;31m", "\033[0;36m", "\033[0;33m", "\033[0;34m", "\033[0;35m"};
const char* stopColorCode = "\033[0m";
const int dictionaryStatsSize = 5;
const bool runDev = true;

const int starterLevel = 3;
const int statPointsXLevel = 3;

struct Player_s{
    int id;
    char name[30];
    int hp;
    int maxhp;
    int level;
    int xp;
    int stats[5];
}  Player_Default = {0, "", 100, 100, 0, 0, {10, 10, 10, 10, 10}};

typedef struct Player_s Player;
void printError(char *error);
int getStat(Player *character, const char *dictionaryValue);
void updateStat(Player *character, const char *dictionaryValue, int amount);
void updateStatErrors(int errorCode);
void changeName(Player *character, char *name);
void changeNameErrors(int errorCode);
void printStats(Player *character);
void levelUp(Player *character, int lvlAmount);
int maxHpFormula(int vitalityLvl);
int xpRequiredFormula(int characterLvl);

void printError(char* error) {
    printf("\033[0;31m%s%s\n", error, stopColorCode);
}

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

void updateStat(Player *character, const char *dictionaryValue, int amount) {
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
        printError("Error: Selected stat doesn't exists"); 
        exit(0);
    }
    if (errorCode == 2) { 
        printError("Error: Selected stat is the same or wasn't updated correctly"); 
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
        printError("Error: Name can't be over 30 characters");
        exit(0);
    }
    if (errorCode == 2) {
        printError("Error: Name can't be less then 5 characters"); 
        exit(0);
    }
    if (errorCode == 3) { 
        printError("Error: Selected name wasn't updated correctly"); 
        exit(0);
    }
}

void printStats(Player *character) {
    printf("\033[0;43m------------------------------------------------------------------\033[0m\n"); 

    printf("\n\n");
    
    for (int i = 0; i < dictionaryStatsSize; i++) {
        printf("      ");
        printf("%s%s(%d)%s +", dictionaryColorStatsIndex[i], dictionaryStatsIndex[i], getStat(character, dictionaryStatsIndex[i]), stopColorCode);

        if (i%2 == 0 && i != 0) {
            printf("\n\n");
            printf("      ");
        }
    }

    printf("\n");
    printf("\033[0;43m------------------------------------------------------------------\033[0m\n\n");
}

void levelUp(Player *character, int lvlAmount) {
    char lvlStr[(int)((ceil(log10(lvlAmount))+1)*sizeof(char))];
    sprintf(lvlStr, "%d", lvlAmount);

    const char *multipleLvlString = strcat(lvlStr, " volte");

    char choice[10];
    int choosenPointsAmount = 1;
    int remainingStatPoints = lvlAmount * statPointsXLevel;

    while(remainingStatPoints > 0) {
        printf("\033[0;33mAumento livello %s\033[0m\n", lvlAmount > 1 ? multipleLvlString : " ");
    
        printStats(character);
        printf("\033[0;34mScrivi il nome o abbreviazione(primo carattere) della statistica che vuoi aumentare\033[0m\n");
        printf("ti rimangono: \033[0;31m%d\033[0m punti\n", remainingStatPoints);
        scanf("%s", choice);

        bool found = false;

        for (int i = 0; i < dictionaryStatsSize; i++) {
            if (choice[0] == dictionaryStatsIndex[i][0] || strcmp(choice, dictionaryStatsIndex[i]) == 0) {
                found = true;

                int pointsChoosenAmount;
                printf("Inserire quanti punti attribuire alla statistica %s%s%s\n", dictionaryColorStatsIndex[i], dictionaryStatsIndex[i], stopColorCode);
                scanf("%d", &pointsChoosenAmount);

                if (pointsChoosenAmount < 1) {
                    printError("Error: The amount of points selected is invalid");
                    return;
                }
                else {
                    if (pointsChoosenAmount > remainingStatPoints) {
                        pointsChoosenAmount = remainingStatPoints;
                    }

                    remainingStatPoints -= pointsChoosenAmount;
                    updateStat(character, dictionaryStatsIndex[i], character->stats[i] + pointsChoosenAmount);
                    if (strcmp(dictionaryStatsIndex[i], "vitality") == 0) {
                        character->maxhp = maxHpFormula(character->stats[i]);
                        character->hp = character->maxhp;
                    }
                }
            }
        }

        system("clear");

        if (!found) {
            printError("Error: stat not found, try again");
        }
    }

    character->level += lvlAmount;
}

int maxHpFormula(int vitalityLvl) {
    return 100 + (10 * sqrt(vitalityLvl - 10));
}

int xpRequiredFormula(int characterLvl) {
    return 100 + pow(((float)characterLvl/0.4), 2);
}

int main (void) {
    Player character = Player_Default;

    //updateStat(&character, "strength", 15);
    char tempName[30];

    system("clear");
    printf("\033[0;32mInizio creazione del personaggio\033[0m\n");
    printf("\033[0;42m------------------------------------------------------------------\033[0m\n");
    printf("Inserire il nome del personaggio per questa avventura: ");
    scanf("%s", tempName);

    changeName(&character, tempName);

    printf("Nome selezionato: \033[0;34m%s\033[0m\n", character.name);
    printf("\n");
    printf("\033[0;32mOttenuti %d livelli bonus\033[0m\n", starterLevel);
    printf("\033[0;42m------------------------------------------------------------------\033[0m\n");
    system("clear");

    levelUp(&character, 3);

    system("clear");

    printf("\033[0;32mStatistiche del personaggio\033[0m\n");
    printf("\033[0;42m------------------------------------------------------------------\033[0m\n");
    printf("\n    Nome: %s\n", character.name);
    printf("    Livello: %d\n", character.level);
    printf("    Esperienza: %d/%d\n", character.xp, xpRequiredFormula(character.level));
    printf("    Punti vita: %d/%d\n\n", character.hp, character.maxhp);

    printStats(&character);

    printf("\033[0;42m------------------------------------------------------------------\033[0m\n");
    
    return 0;
}