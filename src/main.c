#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void clearInputLine() {
int c;
    while( (c = fgetc(stdin)) != EOF) {
        if(c == '\n') {
            break;
        } 
    }
}

int readCommand(int min, int max) {
    int commandnr = 0;
    while(commandnr < min || commandnr > max) {
        char input[500];
        scanf("%s", &input);
        if(strlen(input) ==1 && isdigit(input[0]) ) {
            commandnr = atoi(input);
        }
        if(commandnr >= min && commandnr <= max) break;
        printf("Invalid input. Please enter the command number between %d - %d\n", min, max);
        clearInputLine();
    }
    clearInputLine();
    return commandnr;
};

struct Date {
    int day;
    int month;
    int year;
};

struct FinancialTransaction {
    int type; // positive is income and negative expense
    char description[81];
    float moneyAmount;
    struct Date dateOfTransaction;
};

void initialize(struct FinancialTransaction **list) {
    free(*list);
    *list = NULL;
};

float askMoneyAmount() {
    float money = -1;
    printf("Give amount:\n");
    while(money == -1) {
        char input[300];
        scanf("%s", &input);
        int dots = 0; // float has one or zero . in it
        int alphas = 0; // no alphas are wanted
        int puncts = 0; // no puncts are wanted except one . if any
        for(int i = 0; input[i] != 0; i++) {
            if(isdigit(input[i])) continue;
            if(ispunct(input[i]) && input[i] != '.') {
                puncts++;
            }
            if(input[i] == '.' && dots < 2) {
                dots++;
            } else if (isalpha(input[i])){
                alphas++;
                break;
            }
        }
        if(alphas == 0 && puncts == 0 && dots >=0 && dots <=1) {
            money = atof(input);
            break;
        }
        printf("The value needs to be a float\n");
        clearInputLine();
    }
    return money;
};

int isLeapYear(int year) {
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)); // if the year is leap year return 1
}

int isValidDate(struct Date *date, unsigned int minYear, unsigned int maxYear)
{
    // check range of year,month and day
    if (date->year > maxYear || date->year < minYear) { // if year is invalid return 0
        return 0;
    }
    if (date->month < 1 || date->month > 12) { // if month is invalid return 0
        return 0;
    }
    if (date->day < 1 || date->day > 31) { // if day is invalid return 0
        return 0;
    }
    if (date->month == 2) { // check leap year cases 
        if (isLeapYear(date->year)) {
            return (date->day <= 29);
        } else {
            return (date->day <= 28);
        } 
    }
    if (date->month == 4 || date->month == 6 || date->month == 9 || date->month == 11) { // check months which have only 30 days
        return (date->day <= 30);
    }
    return 1;
}

struct Date askDate() {
    struct Date newDate = {0};
    while(!isValidDate(&newDate, 2000, 2020)) {
        printf("Enter date in format day/month/year:\n");
        char input[300];
        scanf("%s", &input);
        int d;
        int m;
        int y;
        if(sscanf(input, "%d/%d/%d", &d, &m, &y ) == 3) {
            newDate.day = d;
            newDate.month = m;
            newDate.year = y;
        }
        clearInputLine();
    }
    return newDate;
}

void addDescription(char *description) {
    while(1){
        printf("Add description:\n");
        char temp[81];
        if(fgets(temp, 81, stdin) != NULL) {
            if(temp[strlen(temp)-1] == '\n') { // if temp ends to line feed, change it to null
                strcpy(description, temp); 
                description[strlen(description)-1] = 0;
            } else {
                printf("Input was too long. Maximum length is 80.\n");
                clearInputLine();
                continue;
            }
            break;
        } else {
            printf("Something went wrong. Input couldn't be read.\n");
        }
    }
}

void addTransaction(int value, struct FinancialTransaction **list, unsigned int *listSize) {
    char description[81] = {0};
    addDescription(description);
    printf("Description is %s\n", description);
    float moneyAmount = askMoneyAmount();
    struct Date newDate = askDate();
    (*listSize)++;
    struct FinancialTransaction *temp = realloc(*list, (*listSize) * sizeof(struct FinancialTransaction));
    if(temp == NULL) {
        printf("Something went wrong with reallocing the list. Transaction was not added.\n");
        (*listSize)--;
        return;
    }
    *list = temp;
    // add struct to list
    unsigned int index = (*listSize)-1;
    (*list)[index].type = value;  
    (*list)[index].moneyAmount = moneyAmount;
    (*list)[index].dateOfTransaction = newDate;
    strncpy((*list)[index].description, description, 81);
}

int main() {
    printf("Expense tracker\n\n");
    struct FinancialTransaction *records = NULL;
    unsigned int listSize = 0;
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
            case 4: {
                printf("New Income\n");
                addTransaction(1, &records, &listSize);
                break;
            }
            case 5: {
                printf("New expense\n");
                addTransaction(-1, &records, &listSize);
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
