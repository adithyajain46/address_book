#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook addressBook) 
{
 FILEfile = fopen("contacts.csv","w");
    if(file==NULL)
    {
        printf("Error: Unable to open file for saving\n");
        return;
    }

    fprintf(file,"%d\n",addressBook->contactCount);

    for(int i=0;i<addressBook->contactCount;i++)
    {
        fprintf(file,"%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    }
    fclose(file);
    printf("successfully contacts saved to 'contacts.csv\n\n");
}

void loadContactsFromFile(AddressBook addressBook) 
{
    FILEfile = fopen("contacts.csv", "r");

    if(file == NULL)
    {
        printf("No contacts file found\n");
        return;
    }

    fscanf(file, "%d\n", &addressBook->contactCount);

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        fscanf(file, "%[^,],%[^,],%[^\n]\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    fclose(file);
}