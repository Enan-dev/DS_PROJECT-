#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Contact {
    char name[50];
    char phoneNumber[20];
    struct Contact *next;
};

struct CallRecord {
    char calledNumber[20];
    int day;
    int month;
    int year;
    int minute;
    int second;
    struct CallRecord *next;
};

struct UserProfile {
    char name[50];
    char phoneNumber[20];
    struct Contact *contacts;
    struct CallRecord *callRecords;
    struct UserProfile *next;
};

struct UserProfile *head = NULL;
const char *userDataFile = "user_data.txt";
const char *contactDataFile = "contact_data.txt";
const char *callRecordDataFile = "call_record_data.txt";

void loadData() {
    FILE *userFile = fopen(userDataFile, "r");
    if (userFile) {
        char name[50], phoneNumber[20];
        while (fscanf(userFile, "%s %s", name, phoneNumber) == 2) {
            struct UserProfile *newUser = (struct UserProfile *)malloc(sizeof(struct UserProfile));
            if (!newUser) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            strcpy(newUser->name, name);
            strcpy(newUser->phoneNumber, phoneNumber);
            newUser->contacts = NULL;
            newUser->callRecords = NULL;
            newUser->next = head;
            head = newUser;
        }
        fclose(userFile);
    }

    FILE *contactFile = fopen(contactDataFile, "r");
    if (contactFile) {
        char userName[50], contactName[50], contactPhoneNumber[20];
        while (fscanf(contactFile, "%s %s %s", userName, contactName, contactPhoneNumber) == 3) {
            struct UserProfile *user = head;
            while (user) {
                if (strcmp(user->name, userName) == 0) {
                    struct Contact *newContact = (struct Contact *)malloc(sizeof(struct Contact));
                    if (!newContact) {
                        perror("Memory allocation failed");
                        exit(EXIT_FAILURE);
                    }
                    strcpy(newContact->name, contactName);
                    strcpy(newContact->phoneNumber, contactPhoneNumber);
                    newContact->next = user->contacts;
                    user->contacts = newContact;
                    break;
                }
                user = user->next;
            }
        }
        fclose(contactFile);
    }

    FILE *callRecordFile = fopen(callRecordDataFile, "r");
    if (callRecordFile) {
        char calledNumber[20], userName[50];
        int day, month, year, minute, second;
        while (fscanf(callRecordFile, "%s %s %d %d %d %d %d", userName, calledNumber, &day, &month, &year, &minute, &second) == 7) {
            struct UserProfile *user = head;
            while (user) {
                if (strcmp(user->name, userName) == 0) {
                    struct CallRecord *newRecord = (struct CallRecord *)malloc(sizeof(struct CallRecord));
                    if (!newRecord) {
                        perror("Memory allocation failed");
                        exit(EXIT_FAILURE);
                    }
                    strcpy(newRecord->calledNumber, calledNumber);
                    newRecord->day = day;
                    newRecord->month = month;
                    newRecord->year = year;
                    newRecord->minute = minute;
                    newRecord->second = second;
                    newRecord->next = user->callRecords;
                    user->callRecords = newRecord;
                    break;
                }
                user = user->next;
            }
        }
        fclose(callRecordFile);
    }
}

void saveData() {
    FILE *userFile = fopen(userDataFile, "w");
    FILE *contactFile = fopen(contactDataFile, "w");
    FILE *callRecordFile = fopen(callRecordDataFile, "w");

    struct UserProfile *currentUser = head;
    while (currentUser) {
        fprintf(userFile, "%s %s\n", currentUser->name, currentUser->phoneNumber);
        struct Contact *currentContact = currentUser->contacts;
        while (currentContact) {
            fprintf(contactFile, "%s %s %s\n", currentUser->name, currentContact->name, currentContact->phoneNumber);
            currentContact = currentContact->next;
        }
        struct CallRecord *currentRecord = currentUser->callRecords;
        while (currentRecord) {
            fprintf(callRecordFile, "%s %s %d %d %d %d %d\n", currentUser->name, currentRecord->calledNumber, currentRecord->day, currentRecord->month, currentRecord->year, currentRecord->minute, currentRecord->second);
            currentRecord = currentRecord->next;
        }
        currentUser = currentUser->next;
    }

    fclose(userFile);
    fclose(contactFile);
    fclose(callRecordFile);
    printf("Data saved successfully.\n");
}

void userProfile() {
    if (!head) {
        struct UserProfile *defaultUser = (struct UserProfile *)malloc(sizeof(struct UserProfile));
        if (!defaultUser) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        strcpy(defaultUser->name, "Sumaiya Nirjona");
        strcpy(defaultUser->phoneNumber, "01978794694");
        defaultUser->contacts = NULL;
        defaultUser->callRecords = NULL;
        defaultUser->next = head;
        head = defaultUser;
        printf("Default user initialized.\n");
    }
    printf("\nCurrent User Profile:\n");
    printf("Name: %s, Phone Number: %s\n", head->name, head->phoneNumber);
}

void addNewContact() {
    char contactName[50];
    char contactPhoneNumber[20];
    if (!head) {
        printf("No user profile available. Please initialize user profile first.\n");
        return;
    }
    printf("Enter contact name: ");
    scanf("%s", contactName);
    printf("Enter contact phone number: ");
    scanf("%s", contactPhoneNumber);

    struct Contact *currentContact = head->contacts;
    while (currentContact) {
        if (strcmp(currentContact->phoneNumber, contactPhoneNumber) == 0) {
            printf("Phone number '%s' already exists in contacts.\n", contactPhoneNumber);
            return;
        }
        currentContact = currentContact->next;
    }

    struct Contact *newContact = (struct Contact *)malloc(sizeof(struct Contact));
    if (!newContact) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strcpy(newContact->name, contactName);
    strcpy(newContact->phoneNumber, contactPhoneNumber);
    newContact->next = head->contacts;
    head->contacts = newContact;
    printf("Contact '%s' with number '%s' added.\n", contactName, contactPhoneNumber);
}

void addNewCall() {
    char calledNumber[20];
    int day, month, year, minute, second;
    if (!head) {
        printf("No user profile available. Please initialize user profile first.\n");
        return;
    }
    printf("Enter the number you called: ");
    scanf("%s", calledNumber);
    printf("Enter call time (DD MM YYYY MM SS): ");
    if (scanf("%d %d %d %d %d", &day, &month, &year, &minute, &second) != 5) {
        printf("Invalid date/time format.\n");
        while (getchar() != '\n');
        return;
    }

    struct CallRecord *newRecord = (struct CallRecord *)malloc(sizeof(struct CallRecord));
    if (!newRecord) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strcpy(newRecord->calledNumber, calledNumber);
    newRecord->day = day;
    newRecord->month = month;
    newRecord->year = year;
    newRecord->minute = minute;
    newRecord->second = second;
    newRecord->next = head->callRecords;
    head->callRecords = newRecord;
    printf("Call record added for '%s' at %d/%d/%d %d:%d.\n", calledNumber, day, month, year, minute, second);
    saveData();
}

void displayContact() {
    if (!head || !head->contacts) {
        printf("No contacts saved.\n");
        return;
    }
    printf("\nSaved Contacts:\n");
    struct Contact *currentContact = head->contacts;
    while (currentContact) {
        printf("Name: %s, Phone: %s\n", currentContact->name, currentContact->phoneNumber);
        currentContact = currentContact->next;
    }
}

void displayRecord() {
    char searchNumber[20];
    if (!head || !head->callRecords) {
        printf("No call records available.\n");
        return;
    }
    printf("Enter the phone number to display call history: ");
    scanf("%s", searchNumber);
    printf("\nCall History for '%s':\n", searchNumber);
    struct CallRecord *currentRecord = head->callRecords;
    bool found = false;
    while (currentRecord) {
        if (strcmp(currentRecord->calledNumber, searchNumber) == 0) {
            printf("Date: %d/%d/%d, Time: %d:%d\n", currentRecord->day, currentRecord->month, currentRecord->year, currentRecord->minute, currentRecord->second);
            found = true;
        }
        currentRecord = currentRecord->next;
    }
    if (!found) {
        printf("No call records found for '%s'.\n", searchNumber);
    }
}

void deleteContactAndHistory() {
    char deleteNumber[20];
    if (!head) {
        printf("No user profile available.\n");
        return;
    }
    printf("Enter the phone number to delete contact and history: ");
    scanf("%s", deleteNumber);

    struct Contact *currentContact = head->contacts;
    struct Contact *prevContact = NULL;
    bool contactFound = false;

    while (currentContact) {
        if (strcmp(currentContact->phoneNumber, deleteNumber) == 0) {
            if (prevContact) {
                prevContact->next = currentContact->next;
            } else {
                head->contacts = currentContact->next;
            }
            free(currentContact);
            contactFound = true;
            printf("Contact '%s' deleted.\n", deleteNumber);
            break;
        }
        prevContact = currentContact;
        currentContact = currentContact->next;
    }

    struct CallRecord *currentRecord = head->callRecords;
    struct CallRecord *prevRecord = NULL;
    bool historyFound = false;

    while (currentRecord) {
        if (strcmp(currentRecord->calledNumber, deleteNumber) == 0) {
            struct CallRecord *temp = currentRecord;
            if (prevRecord) {
                prevRecord->next = currentRecord->next;
            } else {
                head->callRecords = currentRecord->next;
            }
            currentRecord = currentRecord->next;
            free(temp);
            historyFound = true;
        } else {
            prevRecord = currentRecord;
            currentRecord = currentRecord->next;
        }
    }

    if (!contactFound && !historyFound) {
        printf("This number '%s' is not available in contacts or call history.\n", deleteNumber);
    } else if (!contactFound) {
        printf("Contact '%s' not found, but call history deleted.\n", deleteNumber);
    } else if (!historyFound) {
        printf("Contact '%s' deleted, but no call history found.\n", deleteNumber);
    }
}

void search() {
    int choice;
    printf("Search Options:\n");
    printf("1. Search Contact by Name\n");
    printf("2. Search Contact by Number\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    char input[50];
    bool found = false;
    struct UserProfile *user = head;

    switch (choice) {
        case 1:
            printf("Enter contact name to search: ");
            scanf("%s", input);
            while (user) {
                struct Contact *c = user->contacts;
                while (c) {
                    if (strcmp(c->name, input) == 0) {
                        printf("Contact Found - Name: %s, Phone: %s\n", c->name, c->phoneNumber);
                        found = true;
                    }
                    c = c->next;
                }
                user = user->next;
            }
            break;
        case 2:
            printf("Enter contact number to search: ");
            scanf("%s", input);
            while (user) {
                struct Contact *c = user->contacts;
                while (c) {
                    if (strcmp(c->phoneNumber, input) == 0) {
                        printf("Contact Found - Name: %s, Phone: %s\n", c->name, c->phoneNumber);
                        found = true;
                    }
                    c = c->next;
                }
                user = user->next;
            }
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }

    if (!found) {
        printf("No match found for '%s'.\n", input);
    }
}

int main() {
    loadData();
    int choice;
    while (1) {
        printf("\nMobile User Profile System\n");
        printf("1. User Profile\n");
        printf("2. Add New Contact\n");
        printf("3. Add New Call Record\n");
        printf("4. Display Contacts\n");
        printf("5. Display Call Record History\n");
        printf("6. Delete Contact and History\n");
        printf("7. Search\n");
        printf("8. Save Data\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice) {
            case 1:
                userProfile();
                break;
            case 2:
                addNewContact();
                break;
            case 3:
                addNewCall();
                break;
            case 4:
                displayContact();
                break;
            case 5:
                displayRecord();
                break;
            case 6:
                deleteContactAndHistory();
                break;
            case 7:
                search();
                break;
            case 8:
                saveData();
                break;
            case 0:
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}