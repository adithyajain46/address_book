#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"

int validName(char name[])
{
    int i, count = 0;

    for (i = 0; name[i] != '\0'; i++)
    {
        if (isalnum((unsigned char)name[i]))
            count++;
        else if (name[i] != ' ')
            return 0;
    }

    return count >= 2;
}

int validPhone(char phone[])
{
    int i;

    if (strlen(phone) != 10)
        return 0;

    for (i = 0; phone[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)phone[i]))
            return 0;
    }

    return 1;
}

int validEmail(char email[])
{
    int i, at = -1, atCount = 0;
    int len = strlen(email);

    if (len < 6)
        return 0;

    if (!isalnum((unsigned char)email[0]))
        return 0;

    for (i = 0; email[i] != '\0'; i++)
    {
        if (isupper((unsigned char)email[i]))
            return 0;

        if (email[i] == '@')
        {
            at = i;
            atCount++;
        }
    }

    if (atCount != 1)
        return 0;

    if (strcmp(email + len - 4, ".com") != 0)
        return 0;

    
    if (at >= len - 5)
        return 0;

    return 1;
}

int phoneExists(AddressBook *addressBook, char phone[], int index)
{
    int i;

    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (i != index &&
            strcmp(addressBook->contacts[i].phone, phone) == 0)
            return 1;
    }

    return 0;
}


int emailExists(AddressBook *addressBook, char email[], int index)
{
    int i;

    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (i != index &&
            strcmp(addressBook->contacts[i].email, email) == 0)
            return 1;
    }

    return 0;
}

void listContacts(AddressBook *addressBook)
{
    int i, j;
    char choice;
    Contact temp;

    if (addressBook->contactCount == 0)
    {
        printf("No contacts available.\n");
        return;
    }

    printf("Sort by:\n");
    printf("a. Name\n");
    printf("b. Phone\n");
    printf("c. Email\n");
    printf("Enter choice: ");
    scanf(" %c", &choice);

    for (i = 0; i < addressBook->contactCount - 1; i++)
    {
        for (j = 0; j < addressBook->contactCount - i - 1; j++)
        {
            int result = 0;

            if (choice == 'a')
                result = strcmp(addressBook->contacts[j].name,
                                addressBook->contacts[j + 1].name);
            else if (choice == 'b')
                result = strcmp(addressBook->contacts[j].phone,
                                addressBook->contacts[j + 1].phone);
            else if (choice == 'c')
                result = strcmp(addressBook->contacts[j].email,
                                addressBook->contacts[j + 1].email);
            else
            {
                printf("Invalid choice.\n");
                return;
            }

            if (result > 0)
            {
                temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }

    printf("\nName\t\tPhone\t\tEmail\n");

    for (i = 0; i < addressBook->contactCount; i++)
    {
        printf("%s\t%s\t%s\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
}

void initialize(AddressBook *addressBook)
{
    addressBook->contactCount = 0;
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook)
{
    saveContactsToFile(addressBook);
    exit(0);
}

void createContact(AddressBook *addressBook)
{
    int i, count = 0;

    if (addressBook->contactCount >= 100)
    {
        printf("Address book is full.\n");
        return;
    }

    while (1)
    {
        printf("Enter name: ");
        scanf(" %49[^\n]", addressBook->contacts[addressBook->contactCount].name);

        if (validName(addressBook->contacts[addressBook->contactCount].name))
            break;

        printf("Invalid name. Use letters, digits and spaces.\n");
    }

    while (1)
    {
        printf("Enter phone: ");
        scanf("%19s", addressBook->contacts[addressBook->contactCount].phone);

        if (!validPhone(addressBook->contacts[addressBook->contactCount].phone))
        {
            printf("Phone must contain exactly 10 digits.\n");
            continue;
        }

        if (phoneExists(addressBook,
                        addressBook->contacts[addressBook->contactCount].phone,
                        -1))
        {
            printf("Phone number already exists.\n");
            continue;
        }

        break;
    }

    while (1)
    {
        printf("Enter email: ");
        scanf("%49s", addressBook->contacts[addressBook->contactCount].email);

        if (!validEmail(addressBook->contacts[addressBook->contactCount].email))
        {
            printf("Invalid email format.\n");
            continue;
        }

        if (emailExists(addressBook,
                        addressBook->contacts[addressBook->contactCount].email,
                        -1))
        {
            printf("Email already exists.\n");
            continue;
        }

        break;
    }

    addressBook->contactCount++;
    printf("Contact created successfully.\n");
}

int searchname(AddressBook *addressBook, char search[], int matchingIndex[])
{
    int i, count = 0;

    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (strcasestr(addressBook->contacts[i].name, search) != NULL)
            matchingIndex[count++] = i;
    }

    return count;
}

int searchphone(AddressBook *addressBook, char search[], int matchingIndex[])
{
    int i, count = 0;

    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (strcasestr(addressBook->contacts[i].phone, search) != NULL)
            matchingIndex[count++] = i;
    }

    return count;
}

int searchemail(AddressBook *addressBook, char search[], int matchingIndex[])
{
    int i, count = 0;

    for (i = 0; i < addressBook->contactCount; i++)
    {
        if (strcasestr(addressBook->contacts[i].email, search) != NULL)
            matchingIndex[count++] = i;
    }

    return count;
}

void searchContact(AddressBook *addressBook)
{
    char choice;
    char search[50];
    int matchingIndex[100];
    int count, i;

    printf("Search by:\n");
    printf("a. Name\n");
    printf("b. Phone\n");
    printf("c. Email\n");
    printf("Enter choice: ");
    scanf(" %c", &choice);

    printf("Enter search value: ");
    scanf(" %49[^\n]", search);

    if (choice == 'a')
        count = searchname(addressBook, search, matchingIndex);
    else if (choice == 'b')
        count = searchphone(addressBook, search, matchingIndex);
    else if (choice == 'c')
        count = searchemail(addressBook, search, matchingIndex);
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    if (count == 0)
    {
        printf("No matching contacts found.\n");
        return;
    }

    printf("\nMatching contacts:\n");

    for (i = 0; i < count; i++)
    {
        int index = matchingIndex[i];

        printf("%d. Name: %s | Phone: %s | Email: %s\n",
               i + 1,
               addressBook->contacts[index].name,
               addressBook->contacts[index].phone,
               addressBook->contacts[index].email);
    }
}

void editName(AddressBook *addressBook, int index)
{
    char newName[50];

    while (1)
    {
        printf("Enter new name: ");
        scanf(" %49[^\n]", newName);

        if (!validName(newName))
        {
            printf("Invalid name. Use letters, digits and spaces.\n");
            continue;
        }

        strcpy(addressBook->contacts[index].name, newName);
        printf("Name updated successfully.\n");
        return;
    }
}

void editPhone(AddressBook *addressBook, int index)
{
    char newPhone[20];

    while (1)
    {
        printf("Enter new phone: ");
        scanf("%19s", newPhone);

        if (!validPhone(newPhone))
        {
            printf("Phone must contain exactly 10 digits.\n");
            continue;
        }

        if (phoneExists(addressBook, newPhone, index))
        {
            printf("Phone number already exists.\n");
            continue;
        }

        strcpy(addressBook->contacts[index].phone, newPhone);
        printf("Phone updated successfully.\n");
        return;
    }
}

void editEmail(AddressBook *addressBook, int index)
{
    char newEmail[50];

    while (1)
    {
        printf("Enter new email: ");
        scanf("%49s", newEmail);

        if (!validEmail(newEmail))
        {
            printf("Invalid email format.\n");
            continue;
        }

        if (emailExists(addressBook, newEmail, index))
        {
            printf("Email already exists.\n");
            continue;
        }

        strcpy(addressBook->contacts[index].email, newEmail);
        printf("Email updated successfully.\n");
        return;
    }
}

void editContact(AddressBook *addressBook)
{
    char choice;
    char search[50];
    int matchingIndex[100];
    int count, i, selected, fieldChoice;

    printf("Search contact by:\n");
    printf("a. Name\n");
    printf("b. Phone\n");
    printf("c. Email\n");
    printf("Enter choice: ");
    scanf(" %c", &choice);

    printf("Enter search value: ");
    scanf(" %49[^\n]", search);

    if (choice == 'a')
        count = searchname(addressBook, search, matchingIndex);
    else if (choice == 'b')
        count = searchphone(addressBook, search, matchingIndex);
    else if (choice == 'c')
        count = searchemail(addressBook, search, matchingIndex);
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    if (count == 0)
    {
        printf("No matching contacts found.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        int index = matchingIndex[i];

        printf("%d. Name: %s | Phone: %s | Email: %s\n",
               i + 1,
               addressBook->contacts[index].name,
               addressBook->contacts[index].phone,
               addressBook->contacts[index].email);
    }

    printf("Select contact number to edit: ");
    scanf("%d", &selected);

    if (selected < 1 || selected > count)
    {
        printf("Invalid selection.\n");
        return;
    }

    printf("Edit:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");
    printf("Enter choice: ");
    scanf("%d", &fieldChoice);

    if (fieldChoice == 1)
        editName(addressBook, matchingIndex[selected - 1]);
    else if (fieldChoice == 2)
        editPhone(addressBook, matchingIndex[selected - 1]);
    else if (fieldChoice == 3)
        editEmail(addressBook, matchingIndex[selected - 1]);
    else
        printf("Invalid choice.\n");
}

void deleteContact(AddressBook *addressBook)
{
    char choice;
    char search[50];
    int matchingIndex[100];
    int count, i, selected;

    printf("Search contact to delete by:\n");
    printf("a. Name\n");
    printf("b. Phone\n");
    printf("c. Email\n");
    printf("Enter choice: ");
    scanf(" %c", &choice);

    printf("Enter search value: ");
    scanf(" %49[^\n]", search);

    if (choice == 'a')
        count = searchname(addressBook, search, matchingIndex);
    else if (choice == 'b')
        count = searchphone(addressBook, search, matchingIndex);
    else if (choice == 'c')
        count = searchemail(addressBook, search, matchingIndex);
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    if (count == 0)
    {
        printf("No matching contacts found.\n");
        return;
    }

    for (i = 0; i < count; i++)
    {
        int index = matchingIndex[i];

        printf("%d. Name: %s | Phone: %s | Email: %s\n",
               i + 1,
               addressBook->contacts[index].name,
               addressBook->contacts[index].phone,
               addressBook->contacts[index].email);
    }

    printf("Select contact number to delete: ");
    scanf("%d", &selected);

    if (selected < 1 || selected > count)
    {
        printf("Invalid selection.\n");
        return;
    }

    selected = matchingIndex[selected - 1];

    for (i = selected; i < addressBook->contactCount - 1; i++)
        addressBook->contacts[i] = addressBook->contacts[i + 1];

    addressBook->contactCount--;

    printf("Contact deleted successfully.\n");
}