//Event ticket management program in the form of a binary tree, using structures
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EVENTS 10
#define MAX_TITLE 50
#define MAX_NAME 30

struct Ticket {
    char section;
    int seatNumber;
    char afm[10];
    char firstName[MAX_NAME];
    char lastName[MAX_NAME];
    struct Ticket* left;
    struct Ticket* right;
};

struct Event {
    int code;
    char title[MAX_TITLE];
    char date[11];
    char time[6];
    struct Ticket* tickets;
    struct Event* left;
    struct Event* right;
};

struct Event* createEvent(int code, const char* title, const char* date, const char* time);
struct Event* addEvent(struct Event* root, struct Event* newEvent);
struct Event* searchEvent(struct Event* root, int code);
struct Event* deleteEvent(struct Event* root, int code);
void printEvents(struct Event* root);
void freeEvents(struct Event* root);

struct Ticket* createTicket(char section, int seatNumber, const char* afm, const char* firstName, const char* lastName);
struct Ticket* addTicket(struct Ticket* root, struct Ticket* newTicket);
struct Ticket* searchTicket(struct Ticket* root, char section, int seatNumber);
void printTickets(struct Ticket* root);
void freeTickets(struct Ticket* root);

void clearAll(struct Event** root);

int isValidTitle(const char* title);
int isValidDate(const char* date);
int isValidTime(const char* time);
int isValidAFM(const char* afm);
int isValidName(const char* name);
int isValidSection(char section);
int isValidSeatNumber(int seatNumber);

int main() {
    struct Event* events = NULL;
    int choice;
    int code;
    char title[MAX_TITLE], date[11], time[6];
    char section;
    int seatNumber;
    char afm[10], firstName[MAX_NAME], lastName[MAX_NAME];
    struct Event* event;

    do {
        printf("\n1. Add Event\n");
        printf("2. Search Event\n");
        printf("3. Delete Event\n");
        printf("4. Print All Events\n");
        printf("5. Add Ticket\n");
        printf("6. Search Ticket\n");
        printf("7. Print All Tickets for Event\n");
        printf("8. Clear All Data\n");
        printf("9. Exit\n");

        do {
            printf("\n\n\tEnter your choice: ");
            scanf("%d", &choice);
        } while (choice < 1 || choice > 9);

        switch (choice) {
            case 1:
                printf("Enter event code: ");
                scanf("%d", &code);

                do {
                    printf("Enter event title: ");
                    scanf(" %[^\n]%*c", title); 
                } while (!isValidTitle(title));

                do {
                    printf("Enter event date (DD/MM/YYYY): ");
                    scanf("%s", date);
                } while (!isValidDate(date));

                do {
                    printf("Enter event time (HH:MM): ");
                    scanf("%s", time);
                } while (!isValidTime(time));

                event = createEvent(code, title, date, time);
                events = addEvent(events, event);
                printf("Event added successfully!\n");
                break;

            case 2:
                printf("Enter event code to search: ");
                scanf("%d", &code);
                event = searchEvent(events, code);
                if (event) {
                    printf("Event found: %d - %s on %s at %s\n", event->code, event->title, event->date, event->time);
                } else {
                    printf("Event not found.\n");
                }
                break;

            case 3:
                printf("Enter event code to delete: ");
                scanf("%d", &code);
                events = deleteEvent(events, code);
                printf("Event deleted successfully (if it existed).\n");
                break;

            case 4:
                printEvents(events);
                break;

            case 5:
                printf("Enter event code for the ticket: ");
                scanf("%d", &code);
                event = searchEvent(events, code);
                if (!event) {
                    printf("Event not found.\n");
                    break;
                }

                do {
                    printf("Enter section (a-h): ");
                    scanf(" %c", &section);
                } while (!isValidSection(section));

                do {
                    printf("Enter seat number (1-500): ");
                    scanf("%d", &seatNumber);
                } while (!isValidSeatNumber(seatNumber));

                printf("Enter AFM: ");
                scanf("%s", afm);
                while (!isValidAFM(afm)) {
                    printf("Invalid AFM. Enter again: ");
                    scanf("%s", afm);
                }

                printf("Enter first name: ");
                scanf("%s", firstName);
                while (!isValidName(firstName)) {
                    printf("Invalid first name. Enter again: ");
                    scanf("%s", firstName);
                }

                printf("Enter last name: ");
                scanf("%s", lastName);
                while (!isValidName(lastName)) {
                    printf("Invalid last name. Enter again: ");
                    scanf("%s", lastName);
                }

                struct Ticket* ticket = createTicket(section, seatNumber, afm, firstName, lastName);
                event->tickets = addTicket(event->tickets, ticket);
                printf("Ticket added successfully!\n");
                break;

            case 6:
                printf("Enter event code: ");
                scanf("%d", &code);
                event = searchEvent(events, code);
                if (!event) {
                    printf("Event not found.\n");
                    break;
                }

                printf("Enter section (a-h): ");
                scanf(" %c", &section);
                printf("Enter seat number (1-500): ");
                scanf("%d", &seatNumber);

                struct Ticket* ticketFound = searchTicket(event->tickets, section, seatNumber);
                if (ticketFound) {
                    printf("Ticket found: %c%d - %s %s (AFM: %s)\n",
                           ticketFound->section, ticketFound->seatNumber,
                           ticketFound->firstName, ticketFound->lastName,
                           ticketFound->afm);
                } else {
                    printf("Ticket not found.\n");
                }
                break;

            case 7:
                printf("Enter event code: ");
                scanf("%d", &code);
                event = searchEvent(events, code);
                if (!event) {
                    printf("Event not found.\n");
                    break;
                }

                printf("Tickets for event %d - %s:\n", event->code, event->title);
                printTickets(event->tickets);
                break;

            case 8:
                clearAll(&events);
                printf("All data cleared.\n");
                break;

            case 9:
                printf("Terminated.\n");
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 9);

    return 0;
}

struct Event* createEvent(int code, const char* title, const char* date, const char* time) {
    struct Event* newEvent = (struct Event*)malloc(sizeof(struct Event));
    if (!newEvent) {
        printf("Memory allocation failed for event.\n");
        exit(EXIT_FAILURE);
    }
    newEvent->code = code;  
    strcpy(newEvent->title, title);  
    strcpy(newEvent->date, date);  
    strcpy(newEvent->time, time); 
    newEvent->tickets = NULL;  
    newEvent->left = newEvent->right = NULL;  
    return newEvent;  

}

struct Event* addEvent(struct Event* root, struct Event* newEvent) {
    if (!root) return newEvent;
    if (newEvent->code < root->code)
        root->left = addEvent(root->left, newEvent); 
    else if (newEvent->code > root->code)
        root->right = addEvent(root->right, newEvent); 
    return root;
}

struct Event* searchEvent(struct Event* root, int code) {
    if (!root || root->code == code) return root;  
    if (code < root->code) return searchEvent(root->left, code); 
    return searchEvent(root->right, code); 
} 

struct Event* deleteEvent(struct Event* root, int code) {
    if (!root) return NULL;
    if (code < root->code)
        root->left = deleteEvent(root->left, code); 
    else if (code > root->code)
        root->right = deleteEvent(root->right, code); 
    else {
        if (!root->left) { 
            struct Event* temp = root->right;
            freeTickets(root->tickets);
            free(root);
            return temp;
        } else if (!root->right) { { 
            struct Event* temp = root->left;
            freeTickets(root->tickets);
            free(root);
            return temp;
        }

        
        struct Event* temp = root->right;
        while (temp && temp->left) temp = temp->left;
        root->code = temp->code;
        strcpy(root->title, temp->title);
        strcpy(root->date, temp->date);
        strcpy(root->time, temp->time);
        root->tickets = temp->tickets;
        root->right = deleteEvent(root->right, temp->code);
    }
    return root;
    }
}


void printEvents(struct Event* root) {
    if (!root) return;
    printEvents(root->left);
    printf("%d - %s on %s at %s\n", root->code, root->title, root->date, root->time);
    printEvents(root->right);
}


void freeEvents(struct Event* root) {
    if (!root) return;
    freeEvents(root->left);
    freeEvents(root->right);
    freeTickets(root->tickets);
    free(root);
}


struct Ticket* createTicket(char section, int seatNumber, const char* afm, const char* firstName, const char* lastName) {
    struct Ticket* newTicket = (struct Ticket*)malloc(sizeof(struct Ticket));
    if (!newTicket) {
        printf("Memory allocation failed for ticket.\n");
        exit(EXIT_FAILURE);
    }
    newTicket->section = section;
    newTicket->seatNumber = seatNumber;
    strcpy(newTicket->afm, afm);
    strcpy(newTicket->firstName, firstName);
    strcpy(newTicket->lastName, lastName);
    newTicket->left = newTicket->right = NULL;
    return newTicket;
}

struct Ticket* addTicket(struct Ticket* root, struct Ticket* newTicket) {
    if (!root) return newTicket;
    if (newTicket->seatNumber < root->seatNumber) {
        root->left = addTicket(root->left, newTicket);
    } else if (newTicket->seatNumber > root->seatNumber) {
        root->right = addTicket(root->right, newTicket);
    } else if (newTicket->section < root->section) {
        root->left = addTicket(root->left, newTicket);
    } else {
        root->right = addTicket(root->right, newTicket);
    }
    return root;
}

struct Ticket* searchTicket(struct Ticket* root, char section, int seatNumber) {
    if (!root || (root->section == section && root->seatNumber == seatNumber)) return root;
    if (seatNumber < root->seatNumber ||
        (seatNumber == root->seatNumber && section < root->section))
        return searchTicket(root->left, section, seatNumber);
    return searchTicket(root->right, section, seatNumber);
}

void printTickets(struct Ticket* root) {
    if (!root) return;
    printTickets(root->left);
    printf("%c%d - %s %s (AFM: %s)\n", root->section, root->seatNumber, root->firstName, root->lastName, root->afm);
    printTickets(root->right);
}

void freeTickets(struct Ticket* root) {
    if (!root) return;
    freeTickets(root->left);
    freeTickets(root->right);
    free(root);
}

void clearAll(struct Event** root) {
    freeEvents(*root);
    *root = NULL;
}


int isValidTitle(const char* title) {
    if (strlen(title) == 0) {
        printf("Title cannot be empty.\n");
        return 0;
    }
    return 1;
}

int isValidDate(const char* date) {
    int day, month, year;
    if (sscanf(date, "%2d/%2d/%4d", &day, &month, &year) != 3) {
        printf("Invalid date format. Expected DD/MM/YYYY.\n");
        return 0;
    }
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        printf("Invalid date.\n");
        return 0;
    }
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        printf("Invalid date.\n");
        return 0;
    }
    if (month == 2 && (day > 29 || (day == 29 && (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0))))) {
        printf("Invalid date.\n");
        return 0;
    }
    return 1;
}

int isValidTime(const char* time) {
    int hour, minute;
    if (sscanf(time, "%2d:%2d", &hour, &minute) != 2) {
        printf("Invalid time format. Expected HH:MM.\n");
        return 0;
    }
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        printf("Invalid time.\n");
        return 0;
    }
    return 1;
}

int isValidAFM(const char* afm) {
if (strlen(afm) != 9) {
        printf("AFM must be exactly 9 digits.\n");
        return 0;
    }

    for (int i = 0; i < 9; i++) {
        if (!isdigit(afm[i])) {
            printf("AFM must consist of digits only.\n");
            return 0;
        }
    }    return 1;
}

int isValidName(const char* name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i])) {
            return 0;
        }
    }
    return 1;
}

int isValidSection(char section) {
    return section >= 'a' && section <= 'h';
}

int isValidSeatNumber(int seatNumber) {
    return seatNumber >= 1 && seatNumber <= 500;
}
