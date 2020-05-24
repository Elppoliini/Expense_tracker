#include <stdio.h>
#include <stdlib.h>

void printCommands() {
    printf("Commands:\n");
    printf("    1. Initialize (clear all records)\n");
    printf("    2. Save financial records\n");
    printf("    3. Read financial records\n");
    printf("    4. Add money\n");
    printf("    5. Spend money\n");
    printf("    6. Print a report\n");
    printf("    7. Exit program\n");
    printf("\n");
};

int readCommand(int min, int max) {
    int commandnr = 0;
    int returnvalue = scanf("%d", &commandnr);
    if(returnvalue != 1 || commandnr < min || commandnr > max) {
        return -1;
    }
    return commandnr;
};

struct financialTransaction {
    int type; //Positive is income and negative expense
    char description[81];
    float moneyAmount;
    char date[11]; // dd.mm.yyyy
};

void initialize(struct financialTransaction **list) {
    free(*list);
    *list = NULL;
};

int main() {
    printf("Expense tracker\n");
    struct financialTransaction *records = NULL;
    char running = 1;
    
    while(running) {
        printCommands();
        int command = readCommand(1, 7);

        switch(command) {
            case 1: {
                initialize(&records);
                printf("Records are initialized\n");
                break;
            }
            case 7: {
                running = 0;
                printf("Exiting the program\n");
                break;
            }
            default: {
                printf("Invalid command\n");
            }

        }
    }
    return 0;
};
