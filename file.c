#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) {
    {
  FILE *file = fopen("contacts.csv","w");
  if(file == NULL)
  {
    printf("Error: unable to open file for saving\n");
  }
  fprintf(file,"%d\n",addressBook->contactCount);
  for(int i=0;i<addressBook->contactCount;i++)
  {
    fprintf(file,"%s,%s,%s\n",addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
  }
    fclose(file);
    printf("successfully contacts saved to 'contacts.csv\n\n");
}
  
}

void loadContactsFromFile(AddressBook *addressBook) {
    
}
