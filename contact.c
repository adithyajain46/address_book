#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
//#include "populate.h"

void listContacts(AddressBook *addressBook)
{
    int i, j;
    char choice;
    Contact temp;

    printf("a. Sort by name\n");
    printf("b. Sort by phone\n");
    printf("c. Sort by email\n");
    printf("Enter your choice: ");
    scanf(" %c", &choice);

    for(i = 0; i < addressBook->contactCount - 1; i++)
    {
        for(j = 0; j < addressBook->contactCount - i - 1; j++)
        {
            if(choice == 'a')
            {
                if(strcmp(addressBook->contacts[j].name,addressBook->contacts[j + 1].name) > 0)
                {
                    temp = addressBook->contacts[j];
                    addressBook->contacts[j] = addressBook->contacts[j + 1];
                    addressBook->contacts[j + 1] = temp;
                }
            }

            else if(choice == 'b')
            {
                if(strcmp(addressBook->contacts[j].phone,addressBook->contacts[j + 1].phone) > 0)
                {
                    temp = addressBook->contacts[j];
                    addressBook->contacts[j] = addressBook->contacts[j + 1];
                    addressBook->contacts[j + 1] = temp;
                }
            }

            else if(choice == 'c')
            {
                if(strcmp(addressBook->contacts[j].email,addressBook->contacts[j + 1].email) > 0)
                {
                    temp = addressBook->contacts[j];
                    addressBook->contacts[j] = addressBook->contacts[j + 1];
                    addressBook->contacts[j + 1] = temp;
                }
            }
        }
    }

    printf("\nList of Contacts:\n");

    for(i = 0; i < addressBook->contactCount; i++)
    {
        printf("\nContact %d\n", i + 1);

        printf("Name  : %s\n",addressBook->contacts[i].name);

        printf("Phone : %s\n",addressBook->contacts[i].phone);

        printf("Email : %s\n",addressBook->contacts[i].email);
    }
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}

void createContact(AddressBook *addressBook)
{
    int i, count = 0;

    printf("Enter the name: ");

    scanf(" %[^\n]",addressBook->contacts[addressBook->contactCount].name);

    for(i = 0;addressBook->contacts[addressBook->contactCount].name[i] != '\0';i++)
    {
        if(addressBook->contacts[addressBook->contactCount].name[i] != ' ')
        {
            count++;
        }
    }

    if(count < 2)
    {
        printf("Name must contain at least 2 characters\n");
        return;
    }

    for(i = 0;addressBook->contacts[addressBook->contactCount].name[i] != '\0';i++)
    {
        if(!((addressBook->contacts[addressBook->contactCount].name[i] >= 'A' && 
             addressBook->contacts[addressBook->contactCount].name[i] <= 'Z') ||
             (addressBook->contacts[addressBook->contactCount].name[i] >= 'a' &&
              addressBook->contacts[addressBook->contactCount].name[i] <= 'z') ||
             addressBook->contacts[addressBook->contactCount].name[i] == ' '))
        {
            printf("Name must contain only alphabets and spaces\n");
            return;
        }
    }

    printf("Name is valid: %s\n",addressBook->contacts[addressBook->contactCount].name);

    printf("Enter phone number: ");

    scanf(" %[^\n]",addressBook->contacts[addressBook->contactCount].phone);

    count = 0;

    for(i = 0;addressBook->contacts[addressBook->contactCount].phone[i] != '\0';i++)
    {
        if(addressBook->contacts[addressBook->contactCount].phone[i] >= '0' &&
           addressBook->contacts[addressBook->contactCount].phone[i] <= '9')
        {
            count++;
        }
        else
        {
            printf("Phone number must contain only digits\n");
            return;
        }
    }

    if(count != 10)
    {
        printf("Phone number must contain exactly 10 digits\n");
        return;
    }

    printf("Phone number is valid: %s\n",addressBook->contacts[addressBook->contactCount].phone);

    printf("Enter email: ");

    scanf(" %[^\n]",addressBook->contacts[addressBook->contactCount].email);

    int at = 0;
    int dot = 0;

    for(i = 0;addressBook->contacts[addressBook->contactCount].email[i] != '\0';i++)
    {
        if(addressBook->contacts[addressBook->contactCount].email[i] == '@')
        {
            at++;
        }

        if(addressBook->contacts[addressBook->contactCount].email[i] == '.')
        {
            dot++;
        }
    }

    if(at != 1)
    {
        printf("Email must contain exactly one @\n");
        return;
    }

    if(dot == 0)
    {
        printf("Email must contain a dot\n");
        return;
    }

    printf("Email is valid: %s\n", addressBook->contacts[addressBook->contactCount].email);

    addressBook->contactCount++;

    saveContactsToFile(addressBook);

    printf("Contact added successfully!\n");

}

int searchname(AddressBook *addressBook, char search[], int matchingIndex[])
{
    int i;
    int count = 0;

    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strstr(addressBook->contacts[i].name, search)
           == addressBook->contacts[i].name)
        {
            matchingIndex[count] = i;
            count++;
        }
    }

    return count;
}

int searchphone(AddressBook *addressBook, char search[], int matchingIndex[])
{
    int i;
    int count = 0;

    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strstr(addressBook->contacts[i].phone, search)
           == addressBook->contacts[i].phone)
        {
            matchingIndex[count] = i;
            count++;
        }
    }

    return count;
}

int searchemail(AddressBook *addressBook, char search[], int matchingIndex[])
{
    int i;
    int count = 0;

    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(strstr(addressBook->contacts[i].email, search)
           == addressBook->contacts[i].email)
        {
            matchingIndex[count] = i;
            count++;
        }
    }

    return count;
}

void searchContact(AddressBook *addressBook)
{
    int choice;
    char search[50];
    int matchingIndex[100];
    int count = 0;
    int i;
    int selected;

    printf("Search by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);

    printf("Enter  search: ");
    scanf(" %[^\n]", search);

    if(choice == 1)
    {
        count = searchname(addressBook, search, matchingIndex);
    }
    else if(choice == 2)
    {
        count = searchphone(addressBook, search, matchingIndex);
    }
    else
    {
        printf("Invalid choice\n");
        return;
    }

    if(count == 0)
    {
        printf("No matching contacts found\n");
        return;
    }

    printf("\nMatching contacts:\n");

    for(i = 0; i < count; i++)
    {
        printf("\n%d. %s | %s \n",
               i + 1,
               addressBook->contacts[matchingIndex[i]].name,
               addressBook->contacts[matchingIndex[i]].phone);
    }

    printf("\nEnter contact number: ");
    scanf("%d", &selected);

    if(selected < 1 || selected > count)
    {
        printf("Invalid contact number\n");
        return;
    }

    i = matchingIndex[selected - 1];

    printf("\nSelected Contact:\n");
    printf("Name  : %s\n", addressBook->contacts[i].name);
    printf("Phone : %s\n", addressBook->contacts[i].phone);
}

void editName(AddressBook *addressBook, int index)
{
    char newName[50];
    int i;
    int count = 0;

    printf("Enter new name: ");
    scanf(" %[^\n]", newName);

    for(i = 0; newName[i] != '\0'; i++)
    {
        if(newName[i] != ' ')
        {
            count++;
        }
    }

    if(count < 2)
    {
        printf("Name must contain at least 2 characters\n");
        return;
    }

    for(i = 0; newName[i] != '\0'; i++)
    {
        if(!((newName[i] >= 'A' && newName[i] <= 'Z') ||
             (newName[i] >= 'a' && newName[i] <= 'z') ||
             newName[i] == ' '))
        {
            printf("Name must contain only alphabets and spaces\n");
            return;
        }
    }

    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(i != index && strcmp(addressBook->contacts[i].name, newName) == 0)
        {
            printf("Name already exists\n");
            return;
        }
    }

    strcpy(addressBook->contacts[index].name, newName);

    saveContactsToFile(addressBook);

    printf("Name updated successfully\n");
}

void editPhone(AddressBook *addressBook, int index)
{
    char newPhone[20];
    int i;
    int count = 0;

    printf("Enter new phone number: ");
    scanf(" %[^\n]", newPhone);

    // Check digits
    for(i = 0; newPhone[i] != '\0'; i++)
    {
        if(newPhone[i] >= '0' && newPhone[i] <= '9')
        {
            count++;
        }
        else
        {
            printf("Phone number must contain only digits\n");
            return;
        }
    }

    if(count != 10)
    {
        printf("Phone number must contain exactly 10 digits\n");
        return;
    }

    for(i = 0; i < addressBook->contactCount; i++)
    {
        if(i != index && strcmp(addressBook->contacts[i].phone, newPhone) == 0)
        {
            printf("Phone number already exists\n");
            return;
        }
    }

    strcpy(addressBook->contacts[index].phone, newPhone);

    saveContactsToFile(addressBook);

    printf("Phone number updated successfully\n");
}

void editContact(AddressBook *addressBook)
{
    int choice;
    char search[50];
    int matchingIndex[100];
    int count = 0;
    int i;
    int selected;
    int fieldChoice;

    printf("Edit contact by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);

    printf("Enter what you want to search: ");
    scanf(" %[^\n]", search);

    if(choice == 1)
    {
        count = searchname(addressBook, search, matchingIndex);
    }
    else if(choice == 2)
    {
        count = searchphone(addressBook, search, matchingIndex);
    }
    else
    {
        printf("Invalid choice\n");
        return;
    }

    if(count == 0)
    {
        printf("No matching contacts found\n");
        return;
    }

    printf("\nMatching contacts:\n");

    for(i = 0; i < count; i++)
    {
        printf("\n%d. %s | %s | %s\n",
               i + 1,
               addressBook->contacts[matchingIndex[i]].name,
               addressBook->contacts[matchingIndex[i]].phone,
               addressBook->contacts[matchingIndex[i]].email);
    }

    printf("\nEnter contact number to edit: ");
    scanf("%d", &selected);

    if(selected < 1 || selected > count)
    {
        printf("Invalid contact number\n");
        return;
    }

    i = matchingIndex[selected - 1];

    printf("\nWhat do you want to edit?\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    
    printf("Enter your choice: ");
    scanf("%d", &fieldChoice);

    if(fieldChoice == 1)
    {
        editName(addressBook, i);
    }
    else if(fieldChoice == 2)
    {
        editPhone(addressBook, i);
    }
    else
    {
        printf("Invalid choice\n");
    }
}

void deleteContact(AddressBook *addressBook)
{
    //* Define the logic for deletecontact */
}