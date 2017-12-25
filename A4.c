/* Vicky Mohammad
 * March 23, 2017
 * This program uses linklist to input contacts*/

/*import libraries*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

/*create struct for the linklist*/
struct contact {
    unsigned long phone_number;
    long first_name_posn;
    long last_name_posn;
    long company_name_posn;
    long email_posn;
    long next;
};
typedef struct contact list;

/*struct buffer to store the string value*/ 
struct stringBuffer {
    char firstName[100];
    char lastName[100];
    char companyName[100];
    char email[100];
};

/*display struct pos for debug*/
void displayContact(struct contact *structArray, int element){
    /*display the reading for debugging purpose*/
    printf("Phone: %lu \n", structArray[element].phone_number);
    printf("First Name: %lu \n", structArray[element].first_name_posn);
    printf("Last Name: %lu \n", structArray[element].last_name_posn);
    printf("Company Name: %lu \n", structArray[element].company_name_posn);
    printf("Email: %lu \n", structArray[element].email_posn);
    printf("Next: %lu \n", structArray[element].next);
}/*end display function*/

/*retrieve contact*/
void retreiveContact(long structArray, char string[], FILE *filePointer){
    /*go to the position in the file*/
    fseek(filePointer, structArray, SEEK_SET);
    /*read until the null*/
    int x = 1;
    if(structArray != 0){
        do {
            fread(&string[x-1], sizeof(char), 1, filePointer);
            x++;
        } while(string[x-2] != '\0');
    } else {
        string[0] = '\0';
    }/*end if*/
}/*end receive contact function*/

/*write contact*/
void writeContact(char phoneNumber[], char firstName[], char lastName[], char companyName[], char email[], struct contact contactArray[], int contactElement, FILE *filePointer){
    /*variable for writing and asking the users*/
    char *stringBuffer = malloc(sizeof(char));
    char *ptrForPhoneConvert;
    long phonePos;

    /*set filepointer at the begining*/
    fseek(filePointer, 0, SEEK_END);
    /*set up the struct*/
    phonePos = ftell(filePointer);
    
    /*set up struct pos to zero so there is no random character appear*/
    contactArray[contactElement].first_name_posn = 0;
    contactArray[contactElement].last_name_posn = 0;
    contactArray[contactElement].company_name_posn = 0;
    contactArray[contactElement].email_posn = 0;
    contactArray[contactElement].next = 0;

    /*write the struct*/
    contactArray[contactElement].phone_number = strtoul(phoneNumber, &ptrForPhoneConvert, 10);
    fwrite(&contactArray[contactElement], sizeof(struct contact), 1 , filePointer);
    phoneNumber[0] = '\0';
    /*write thr other sub struct*/
    if (firstName[0] != '\0'){
        /*copy the file to the string buffer to be write*/
        contactArray[contactElement].first_name_posn = ftell(filePointer);
        stringBuffer = realloc(stringBuffer, sizeof(char)*(strlen(firstName)+1));
        strncpy(stringBuffer, firstName, strlen(firstName)+1);
        fwrite(stringBuffer, sizeof(char), strlen(stringBuffer)+1, filePointer);
        firstName[0] = '\0';
        stringBuffer[0] = '\0';
    }/*end if*/
    if (lastName[0] != '\0'){
        /*copy the file to the string buffer to be write*/
        contactArray[contactElement].last_name_posn = ftell(filePointer);
        stringBuffer = realloc(stringBuffer, sizeof(char)*(strlen(lastName)+1));
        strncpy(stringBuffer, lastName, strlen(lastName)+1);
        fwrite(stringBuffer, sizeof(char), strlen(stringBuffer)+1 , filePointer);
        lastName[0] = '\0';
        stringBuffer[0] = '\0';
    }/*end if*/
    if (companyName[0] != '\0'){
        /*copy the file to the string buffer to be write*/
        contactArray[contactElement].company_name_posn = ftell(filePointer);
        stringBuffer = realloc(stringBuffer, sizeof(char)*(strlen(companyName)+1));
        strncpy(stringBuffer, companyName, strlen(companyName)+1);
        fwrite(stringBuffer, sizeof(char), strlen(stringBuffer)+1 , filePointer);
        companyName[0] = '\0';
        stringBuffer[0] = '\0';
    }/*end if*/
    if (email[0] != '\0'){
        /*copy the file to the string buffer to be write*/
        contactArray[contactElement].email_posn = ftell(filePointer);
        stringBuffer = realloc(stringBuffer, sizeof(char)*(strlen(email)+1));
        strncpy(stringBuffer, email, strlen(email)+1);
        fwrite(stringBuffer, sizeof(char), strlen(stringBuffer)+1 , filePointer);
        email[0] = '\0';
        stringBuffer[0] = '\0';
    }/*end if*/
    /*copy the file to the string buffer to be write*/
    contactArray[contactElement].next = ftell(filePointer);

    /*seek at the main pointer*/
    fseek(filePointer, phonePos, SEEK_SET);
    /*set and update the position*/
    fwrite(&contactArray[contactElement], sizeof(struct contact), 1 , filePointer);
    /*set back to the default pointer*/
    fseek(filePointer, 0, SEEK_END);
}/*end write contact*/

/*sorting in ascending order function*/
void sortContact(struct contact data[], int element){
    /*declare a variable*/
    int i, j, x = 0;
    struct contact temp;
    struct stringBuffer string[element];
    struct stringBuffer tempString;
    char phoneNumber[100];
    FILE *file;
    FILE *file2;
    /*read the data and save it to the string to the struct*/
    file = fopen("myContactList.db", "rb");
    file2 = fopen("temp.db", "wb");
    rewind(file);
    for (x=0;x<element;x++){
        fread(&data[x], sizeof(data), 1, file);
        /*retrive the data*/
        retreiveContact(data[x].first_name_posn, string[x].firstName, file);
        retreiveContact(data[x].last_name_posn, string[x].lastName, file);
        retreiveContact(data[x].company_name_posn, string[x].companyName, file);
        retreiveContact(data[x].email_posn, string[x].email, file);
        /*print for debug
        printf("Reading... String: %s\n", string[x].firstName);*/
    }/*end while*/

    /*rank the value from smallest to largest*/
    /*including the company and the last name*/
    for(i=1;i<=element-1;i++){
        for(j=1;j<=element-i;j++){
            if ((string[j-1].lastName[0] != '\0') && (string[j].lastName[0] != '\0')){
                if(strcmp(string[j-1].lastName, string[j].lastName) >= 0){
                    /*sort the string*/
                    strcpy(tempString.firstName, string[j-1].firstName);
                    strcpy(string[j-1].firstName, string[j].firstName);
                    strcpy(string[j].firstName, tempString.firstName);
                    /*sort the string*/
                    strcpy(tempString.lastName, string[j-1].lastName);
                    strcpy(string[j-1].lastName, string[j].lastName);
                    strcpy(string[j].lastName, tempString.lastName);
                    /*sort the string*/
                    strcpy(tempString.companyName, string[j-1].companyName);
                    strcpy(string[j-1].companyName, string[j].companyName);
                    strcpy(string[j].companyName, tempString.companyName);
                    /*sort the string*/
                    strcpy(tempString.email, string[j-1].email);
                    strcpy(string[j-1].email, string[j].email);
                    strcpy(string[j].email, tempString.email);
                    /*sort the struct pos*/
                    temp = data[j-1];
                    data[j-1] = data[j];
                    data[j] = temp;
                }/*end if*/
            } else if ((string[j-1].lastName[0] == '\0') && (string[j].lastName[0] != '\0')){
                if(strcmp(string[j-1].companyName, string[j].lastName) >= 0){
                    /*sort the string*/
                    strcpy(tempString.firstName, string[j-1].firstName);
                    strcpy(string[j-1].firstName, string[j].firstName);
                    strcpy(string[j].firstName, tempString.firstName);
                    /*sort the string*/
                    strcpy(tempString.lastName, string[j-1].lastName);
                    strcpy(string[j-1].lastName, string[j].lastName);
                    strcpy(string[j].lastName, tempString.lastName);
                    /*sort the string*/
                    strcpy(tempString.companyName, string[j-1].companyName);
                    strcpy(string[j-1].companyName, string[j].companyName);
                    strcpy(string[j].companyName, tempString.companyName);
                    /*sort the string*/
                    strcpy(tempString.email, string[j-1].email);
                    strcpy(string[j-1].email, string[j].email);
                    strcpy(string[j].email, tempString.email);
                    /*sort the struct pos*/
                    temp = data[j-1];
                    data[j-1] = data[j];
                    data[j] = temp;
                }/*end if*/
            } else if ((string[j-1].lastName[0] != '\0') && (string[j].lastName[0] == '\0')){
                if(strcmp(string[j-1].lastName, string[j].companyName) >= 0){
                    /*sort the string*/
                    strcpy(tempString.firstName, string[j-1].firstName);
                    strcpy(string[j-1].firstName, string[j].firstName);
                    strcpy(string[j].firstName, tempString.firstName);
                    /*sort the string*/
                    strcpy(tempString.lastName, string[j-1].lastName);
                    strcpy(string[j-1].lastName, string[j].lastName);
                    strcpy(string[j].lastName, tempString.lastName);
                    /*sort the string*/
                    strcpy(tempString.companyName, string[j-1].companyName);
                    strcpy(string[j-1].companyName, string[j].companyName);
                    strcpy(string[j].companyName, tempString.companyName);
                    /*sort the string*/
                    strcpy(tempString.email, string[j-1].email);
                    strcpy(string[j-1].email, string[j].email);
                    strcpy(string[j].email, tempString.email);
                    /*sort the struct pos*/
                    temp = data[j-1];
                    data[j-1] = data[j];
                    data[j] = temp;
                }/*end if*/
            } else if ((string[j-1].lastName[0] == '\0') && (string[j].lastName[0] == '\0')){
                if(strcmp(string[j-1].companyName, string[j].companyName) >= 0){
                    /*sort the string*/
                    strcpy(tempString.firstName, string[j-1].firstName);
                    strcpy(string[j-1].firstName, string[j].firstName);
                    strcpy(string[j].firstName, tempString.firstName);
                    /*sort the string*/
                    strcpy(tempString.lastName, string[j-1].lastName);
                    strcpy(string[j-1].lastName, string[j].lastName);
                    strcpy(string[j].lastName, tempString.lastName);
                    /*sort the string*/
                    strcpy(tempString.companyName, string[j-1].companyName);
                    strcpy(string[j-1].companyName, string[j].companyName);
                    strcpy(string[j].companyName, tempString.companyName);
                    /*sort the string*/
                    strcpy(tempString.email, string[j-1].email);
                    strcpy(string[j-1].email, string[j].email);
                    strcpy(string[j].email, tempString.email);
                    /*sort the struct pos*/
                    temp = data[j-1];
                    data[j-1] = data[j];
                    data[j] = temp;
                }/*end if*/
            } /*end if*/
        }/*end for*/
    }/*end for*/
    /*write the sorted data*/
    rewind(file);
    rewind(file2);

    for (x=0;x<element;x++){
        /*display what is it writing for debug
        printf("Writing...\n");
        printf("Phone Nmmber: %lu\n", data[x].phone_number);
        printf("First Name: %s\n", string[x].firstName);
        printf("Last Name: %s\n", string[x].lastName);
        printf("Company Name: %s\n", string[x].companyName);
        printf("Email: %s\n", string[x].email);*/
        /*the each data*/
        sprintf(phoneNumber, "%lu", data[x].phone_number); 
        writeContact(phoneNumber, string[x].firstName, string[x].lastName, string[x].companyName, string[x].email, data, x, file2);
    }/*end while*/
    /*close the file*/
    fclose(file);
    fclose(file2);
    /*rename the file*/
    remove("myContactList.db");
    rename("temp.db", "myContactList.db");
}/*end sorting function*/

/*delete record at position*/
void deleteContact(struct contact data[], long pos, int element){
    /*declare variable*/
    char firstName[100];
    char lastName[100];
    char companyName[100];
    char phoneNumber[100];
    char email[100];
    FILE *file;
    FILE *file2;
    int x = 0;
    /*print and open file
    printf("Deleting Record..\n");*/
    file = fopen("myContactList.db", "rb");
    file2 = fopen("temp.db", "wb");
    /*set pointer at the start*/
    rewind(file);
    for (x=0;x<element;x++){
        fread(&data[x], sizeof(data), 1, file);
        /*retrive the data*/
        retreiveContact(data[x].first_name_posn, firstName, file);
        retreiveContact(data[x].last_name_posn, lastName, file);
        retreiveContact(data[x].company_name_posn, companyName, file);
        retreiveContact(data[x].email_posn, email, file);
        /*write it to a new file except the pos to delete*/
        if (data[x].next != pos){
            sprintf(phoneNumber, "%lu", data[x].phone_number); 
            writeContact(phoneNumber, firstName, lastName, companyName, email, data, x, file2);
        } else {
            /*display for debug
            printf("Phone Nmmber: %lu\n", data[x].phone_number);
            printf("First Name: %s\n", firstName);
            printf("Last Name: %s\n", lastName);
            printf("Company Name: %s\n", companyName);
            printf("Email: %s\n", email);*/
        }/*end if*/
    }/*end while*/
    /*close file*/
    fclose(file);
    fclose(file2);
    /*rename the file*/
    remove("myContactList.db");
    rename("temp.db", "myContactList.db");
    /*display for debug purpose
    printf("Record deleted.\n");
    printf("\n");*/
}/*end delete function*/

/*enter contact attriubute*/
void enterContact(char firstName[], char lastName[], char companyName[], char phoneNumber[], char email[]){
    /*declare variables*/
    int constraintPhoneNumber;
    int stringLength;
    int atChar = 0;
    int lastNameIsEntered;
    int x = 0;
    /*ask the user*/
    printf("First Name: ");
    fgets(firstName, 100, stdin);
    firstName[strlen(firstName)-1] = '\0';
    /*ask the user*/
    printf("Last Name: ");
    fgets(lastName, 100, stdin);
    lastName[strlen(lastName)-1] = '\0';
    lastNameIsEntered = 1;
    /*if last name is null then company name must be entered*/
    if(lastName[0] == '\0'){
        lastNameIsEntered = 0;
    }/*end if*/

    /*ask the user*/
    printf("Company Name: ");
    fgets(companyName, 100, stdin);
    companyName[strlen(companyName)-1] = '\0';
    /*constraint if user does not enter last name*/
    if(lastNameIsEntered == 0){
        /*re-eneter company name again*/
        while(1){
            if (companyName[0] == '\0'){
                /*ask the user again*/
                printf("Company Name: ");
                fgets(companyName, 100, stdin);
                companyName[strlen(companyName)-1] = '\0';
            } else {
                /*exit the loop*/
                break;
            }/*end if*/
        }/*end while*/
    }/*end if*/
    /*ask the user*/
    printf("Phone Number (enter only numbers): ");
    fgets(phoneNumber, 100, stdin);
    stringLength = strlen(phoneNumber);
    phoneNumber[strlen(phoneNumber)-1] = '\0';
    /*contraint until user input only number*/
    constraintPhoneNumber = 0;
    while (constraintPhoneNumber == 0){
        if (phoneNumber[0] == '\0') {
            /*if it's not a number do not exit*/
            constraintPhoneNumber = 0;
            x = 100;
            /*empty the buffers*/
            phoneNumber[0] = '\0';
            stringLength = 0;
            /*enter the inputs*/
            printf("Phone Number (enter only numbers): ");
            fgets(phoneNumber, 100, stdin);
            stringLength = strlen(phoneNumber);
            phoneNumber[strlen(phoneNumber)-1] = '\0';
        }/*end if*/
        for (x=0;x<stringLength-1;x++){
            /*printf("looped\n");*/
            if ((phoneNumber[x] >= 48) && (phoneNumber[x] <= 57) && 
                ((stringLength == 8) || (stringLength == 11))){
                /*if its a number exit the for loop*/
                constraintPhoneNumber = 1;
            } else {
                /*if it's not a number do not exit*/
                constraintPhoneNumber = 0;
                x = 100;
                /*empty the buffers*/
                phoneNumber[0] = '\0';
                stringLength = 0;
                /*enter the inputs*/
                printf("Phone Number (enter only numbers): ");
                fgets(phoneNumber, 100, stdin);
                stringLength = strlen(phoneNumber);
                phoneNumber[strlen(phoneNumber)-1] = '\0';
            }/*end if*/
        }/*end for loop*/
    }/*end while loop*/
    /*ask the user*/
    printf("Email: ");
    fgets(email, 100, stdin);
    email[strlen(email)-1] = '\0';
    /*contraint thr email out line*/
    stringLength = 0;
    stringLength = strlen(email);
    for(x=0;x<=stringLength;x++){
        if(email[x] == '@'){
            atChar++;
        }else if (email[x] == ' '){
            atChar = 0;
            break;
        }/*end if*/
    }/*end for*/
    /*ask email again if user didn't put anything*/
    while(1){
        if ((email[stringLength-4] == '.') && (email[stringLength-4] == '.') && 
            (email[stringLength-5] >= 'A') && (email[stringLength-5] <= 'z') && 
            (email[0] >= 'A') && (email[0] <= 'z') && (atChar == 1)){
            /*exit the loop*/
            break;
        } else {
            /*ask the user again*/
            printf("Email: ");
            fgets(email, 100, stdin);
            email[strlen(email)-1] = '\0';
            /*contraint thr email out line*/
            atChar = 0;
            stringLength = strlen(email);
            for(x=0;x<=stringLength;x++){
                if(email[x] == '@'){
                    atChar++;
                } else if (email[x] == ' '){
                    atChar = 0;
                    break;
                }/*end if*/
            }/*end for*/
        }/*end if*/
    }/*end if*/
}/*end function*/

void enterEditContact(char phoneNumber[], char email[]){
    /*declare variables*/
    int constraintPhoneNumber;
    int stringLength;
    int atChar = 0;
    int x = 0;
    /*ask the user*/
    printf("Phone Number (enter only numbers): ");
    fgets(phoneNumber, 100, stdin);
    stringLength = strlen(phoneNumber);
    phoneNumber[strlen(phoneNumber)-1] = '\0';
    /*contraint until user input only number*/
    constraintPhoneNumber = 0;
    while (constraintPhoneNumber == 0){
        if (phoneNumber[0] == '\0') {
            /*if it's not a number do not exit*/
            constraintPhoneNumber = 0;
            x = 100;
            /*empty the buffers*/
            phoneNumber[0] = '\0';
            stringLength = 0;
            /*enter the inputs*/
            printf("Phone Number (enter only numbers): ");
            fgets(phoneNumber, 100, stdin);
            stringLength = strlen(phoneNumber);
            phoneNumber[strlen(phoneNumber)-1] = '\0';
        }/*end if*/
        for (x=0;x<stringLength-1;x++){
            /*printf("looped\n");*/
            if ((phoneNumber[x] >= 48) && (phoneNumber[x] <= 57) && 
                ((stringLength == 8) || (stringLength == 11))){
                /*if its a number exit the for loop*/
                constraintPhoneNumber = 1;
            } else {
                /*if it's not a number do not exit*/
                constraintPhoneNumber = 0;
                x = 100;
                /*empty the buffers*/
                phoneNumber[0] = '\0';
                stringLength = 0;
                /*enter the inputs*/
                printf("Phone Number (enter only numbers): ");
                fgets(phoneNumber, 100, stdin);
                stringLength = strlen(phoneNumber);
                phoneNumber[strlen(phoneNumber)-1] = '\0';
            }/*end if*/
        }/*end for loop*/
    }/*end while loop*/
    /*ask the user*/
    printf("Email: ");
    fgets(email, 100, stdin);
    email[strlen(email)-1] = '\0';
    /*contraint thr email out line*/
    stringLength = 0;
    stringLength = strlen(email);
    for(x=0;x<=stringLength;x++){
        if(email[x] == '@'){
            atChar++;
        }else if (email[x] == ' '){
            atChar = 0;
            break;
        }/*end if*/
    }/*end for*/
    /*ask email again if user didn't put anything*/
    while(1){
        if ((email[stringLength-4] == '.') && (email[stringLength-4] == '.') && 
            (email[stringLength-5] >= 'A') && (email[stringLength-5] <= 'z') && 
            (email[0] >= 'A') && (email[0] <= 'z') && (atChar == 1)){
            /*exit the loop*/
            break;
        } else {
            /*ask the user again*/
            printf("Email: ");
            fgets(email, 100, stdin);
            email[strlen(email)-1] = '\0';
            /*contraint thr email out line*/
            atChar = 0;
            stringLength = strlen(email);
            for(x=0;x<=stringLength;x++){
                if(email[x] == '@'){
                    atChar++;
                } else if (email[x] == ' '){
                    atChar = 0;
                    break;
                }/*end if*/
            }/*end for*/
        }/*end if*/
    }/*end if*/
}/*end function*/

/*main*/
int main(){
    /*declare input buffer variables*/
    char firstName[100];
    char lastName[100];
    char companyName[100];
    char phoneNumber[100];
    char email[100];
    /*declare variables for retriving a file*/
    char stringRetrieve[100];
    long endOfFile = 0;
    /*var for contact array and file*/
    char menu[10];
    char secondMenu[10];
    int exit = 0, z = 0;
    int secondExit = 0;
    int contactElement = 0;
    int scroll = 0;
    int scrollToShow;
    char scrollBuffer[100];
    /*variables for struct and arraay*/
    list *contactArray = malloc(sizeof(list));
    FILE *filePointer;

    /*create a file if there isnt any*/
    filePointer = fopen("myContactList.db", "ab");
    fclose(filePointer);
    printf("\n");

    /*Infinite loop the program until exit*/
    while(exit == 0){
        /******************************************************************************************/
        /*read the file and put the pos on struct*/
        filePointer = fopen("myContactList.db", "rb");
        /*get file pointer at the end of a file*/
        fseek(filePointer, 0, SEEK_END);
        endOfFile = ftell(filePointer);
        fseek(filePointer, 0, SEEK_SET);
        contactElement = 0;
        /*read the data to the struct array*/
        while(endOfFile != ftell(filePointer)){
            if(filePointer){
                /*read the struct and put the value on the position*/
                fread(&contactArray[contactElement], sizeof(struct contact), 1, filePointer);
                /*move pointer to the next data*/
                fseek(filePointer, contactArray[contactElement].next, SEEK_SET);
                /*create element and move on to the next element*/
                contactElement = contactElement + 1;
                contactArray = realloc(contactArray, sizeof(list)*(contactElement+1));
            }/*end if*/
        }/*end while*/
        /*close for reading*/
        fclose(filePointer);
        /******************************************************************************************/

        /*sort the file in ascending order*/
        sortContact(contactArray, contactElement);

        /******************************************************************************************/
        /*read the file and put the pos on struct*/
        filePointer = fopen("myContactList.db", "rb");
        /*get file pointer at the end of a file*/
        fseek(filePointer, 0, SEEK_END);
        endOfFile = ftell(filePointer);
        fseek(filePointer, 0, SEEK_SET);
        contactElement = 0;
        /*read the data to the struct array*/
        while(endOfFile != ftell(filePointer)){
            if(filePointer){
                /*read the struct and put the value on the position*/
                fread(&contactArray[contactElement], sizeof(struct contact), 1, filePointer);
                /*display the contact from function
                printf("Reading...\n");
                displayContact(contactArray, contactElement);*/
                /*move pointer to the next data*/
                fseek(filePointer, contactArray[contactElement].next, SEEK_SET);
                /*create element and move on to the next element*/
                contactElement = contactElement + 1;
                contactArray = realloc(contactArray, sizeof(list)*(contactElement+1));
            }/*end if*/
        }/*end while*/
        /*close for reading
        printf("\n");*/
        fclose(filePointer);
        /******************************************************************************************/

        /*ask the user for new contact*/
        printf("Number of Contacts = %d \n", contactElement);
        /*set menu to null*/
        filePointer = fopen("myContactList.db", "rb+");
        fseek(filePointer, SEEK_SET, 0);
        
        /*change the scroll out put value so it wont seg fault*/
        if(contactElement <= 5){
            /*constraint the scroll so it only shows what you have*/
            scroll = 0;
            scrollToShow = contactElement-1;
        } else {
            /*constraint the scroll so it only shows what you have*/
            if (scroll < 0){
                scroll = 0;
            } else if (scroll > contactElement-5){
                scroll = contactElement-5;
            }/*end if*/
            scrollToShow = 4;
        }/*end if*/
        /*show the names that has been inputed*/
        scrollBuffer[0] = '\0';
        for(z = scroll; z <= scroll+scrollToShow; z++){
            /*display the the contact list*/
            retreiveContact(contactArray[z].first_name_posn, firstName, filePointer);
            retreiveContact(contactArray[z].last_name_posn, lastName, filePointer);
            retreiveContact(contactArray[z].company_name_posn, companyName, filePointer);
            /*display last name or company name depending of the situation*/
            if ((firstName[0] != '\0') && (lastName[0] != '\0')){
                /*display the letter if it doesnt have any*/
                if (scrollBuffer[0] != lastName[0]){
                    printf("%c\n", lastName[0]);
                    scrollBuffer[0] = lastName[0];
                }/*end if*/
                printf("    %d. %s %s\n", z+1, firstName, lastName);
            } else if ((companyName[0] != '\0') && (lastName[0] == '\0')) {
                /*display the letter if it doesnt have any*/
                if (scrollBuffer[0] != companyName[0]){
                    printf("%c\n", companyName[0]);
                    scrollBuffer[0] = companyName[0];
                }/*end if*/
                printf("    %d. %s\n", z+1, companyName);
            } else if ((firstName[0] == '\0') && (lastName[0] != '\0')){
                /*display the letter if it doesnt have any*/
                if (scrollBuffer[0] != lastName[0]){
                    printf("%c\n", lastName[0]);
                    scrollBuffer[0] = lastName[0];
                }/*end if*/
                printf("    %d. %s\n", z+1, lastName);
            }/*end if*/
        }/*end if*/
        /*close the file*/
        fclose(filePointer);

        /*enter the menu*/
        printf("Action(+-#AX): ");
        fgets(menu, 10, stdin);
        printf("\n");
        /*if user say yes or no*/
        if(menu[0] == 'A'){
            /*set menu to null and exit equals true*/
            *secondMenu = '\0';
            secondExit = 1;
            *menu = '\0';
            /*enter the contacts*/
            enterContact(firstName, lastName, companyName, phoneNumber, email);
            /*enter the menu*/
            printf("Action(RS): ");
            fgets(secondMenu, 10, stdin);
            printf("\n");
            if(secondMenu[0] == 'R'){
                /*set menu to null and exit equals true*/
                *secondMenu = '\0';
                secondExit = 1;
            } else if(secondMenu[0] == 'S'){
                /*display the pointer value for debuging purpose
                printf("Writting...\n");
                printf("Phone Nmmber: %s\n", phoneNumber);
                printf("First Name: %s\n", firstName);
                printf("Last Name: %s\n", lastName);
                printf("Company Name: %s\n", companyName);
                printf("Email: %s\n", email);
                printf("\n");*/
                /*add a new contact*/
                filePointer = fopen("myContactList.db", "rb+");
                writeContact(phoneNumber, firstName, lastName, companyName, email, contactArray, contactElement, filePointer);
                contactElement = contactElement + 1;
                contactArray = realloc(contactArray, sizeof(list)*(contactElement+1));
                fclose(filePointer);
                /*delete the old contact*/
                deleteContact(contactArray, contactArray[atoi(menu)-1].next, contactElement);
                contactElement = contactElement - 1;
                /*set menu to null and exit equals true*/
                *secondMenu = '\0';
                secondExit = 1;
            } else {
                /*set menu to null and exit equals true*/
                *secondMenu = '\0';
                secondExit = 1;
            }/*end if*/
        } else if (menu[0] == 'X'){
            /*set menu to null*/
            *menu = '\0';
            /*exit the program*/
            return(0);
        } else if ((atoi(menu) > 0) && (atoi(menu) <= contactElement)){
            /*set the exit to false so it will infinite loop again*/
            secondExit = 0;
            while(secondExit == 0){
                /*set menu to null*/
                filePointer = fopen("myContactList.db", "rb+");
                /*set the filePointer to the begining of the file*/
                fseek(filePointer, SEEK_SET, 0);
                /*display the selected data*/
                printf("Contact #%d\n", atoi(menu));
                /*retrive the data*/
                retreiveContact(contactArray[atoi(menu)-1].first_name_posn, stringRetrieve, filePointer);
                printf("first Name: %s\n", stringRetrieve);
                stringRetrieve[0] = ' ';
                /*retrive the data*/
                retreiveContact(contactArray[atoi(menu)-1].last_name_posn, stringRetrieve, filePointer);
                printf("last Name: %s\n", stringRetrieve);
                stringRetrieve[0] = ' ';
                /*retrive the data*/
                retreiveContact(contactArray[atoi(menu)-1].company_name_posn, stringRetrieve, filePointer);
                printf("Company Name: %s\n", stringRetrieve);
                stringRetrieve[0] = ' ';
                /*retrive the data*/
                printf("Phone Number: %lu\n", contactArray[atoi(menu)-1].phone_number);
                stringRetrieve[0] = ' ';
                /*retrive the data*/
                retreiveContact(contactArray[atoi(menu)-1].email_posn, stringRetrieve, filePointer);
                printf("Email: %s\n", stringRetrieve);
                stringRetrieve[0] = ' ';
                /*close the file*/
                fclose(filePointer);

                /*enter the menu*/
                printf("Action(RED): ");
                fgets(secondMenu, 10, stdin);
                printf("\n");
                if(secondMenu[0] == 'R'){
                    /*set menu to null and exit equals true*/
                    *secondMenu = '\0';
                    secondExit = 1;
                }else if(secondMenu[0] == 'E'){
                    /*set menu to null and exit equals true*/
                    *secondMenu = '\0';
                    secondExit = 1;
                    /*retreive the data and display whats needed*/
                    filePointer = fopen("myContactList.db", "rb+");
                    retreiveContact(contactArray[atoi(menu)-1].first_name_posn, firstName, filePointer);
                    retreiveContact(contactArray[atoi(menu)-1].last_name_posn, lastName, filePointer);
                    retreiveContact(contactArray[atoi(menu)-1].company_name_posn, companyName, filePointer);
                    printf("Contact #%d\n", atoi(menu));
                    printf("First Name: %s\n", firstName);
                    printf("Last Name: %s\n", lastName);
                    printf("Company Name: %s\n", companyName);
                    fclose(filePointer);
                    /*ask the user for information to edit*/
                    enterEditContact(phoneNumber, email);
                    /*enter the menu*/
                    printf("Action(RSD): ");
                    fgets(secondMenu, 10, stdin);
                    printf("\n");
                    /*ask to delete, save or return*/
                    if(secondMenu[0] == 'R'){
                        /*set menu to null and exit equals true*/
                        *secondMenu = '\0';
                        secondExit = 1;
                    } else if(secondMenu[0] == 'S'){
                        /*display the pointer value for debuging purpose
                        printf("Writting...\n");
                        printf("Phone Nmmber: %s\n", phoneNumber);
                        printf("First Name: %s\n", firstName);
                        printf("Last Name: %s\n", lastName);
                        printf("Company Name: %s\n", companyName);
                        printf("Email: %s\n", email);
                        printf("\n");*/
                        /*add a new contact*/
                        filePointer = fopen("myContactList.db", "rb+");
                        writeContact(phoneNumber, firstName, lastName, companyName, email, contactArray, contactElement, filePointer);
                        contactElement = contactElement + 1;
                        contactArray = realloc(contactArray, sizeof(list)*(contactElement+1));
                        fclose(filePointer);
                        /*delete the old contact*/
                        deleteContact(contactArray, contactArray[atoi(menu)-1].next, contactElement);
                        contactElement = contactElement - 1;
                        /*set menu to null and exit equals true*/
                        *secondMenu = '\0';
                        secondExit = 1;
                    } else if(secondMenu[0] == 'D'){
                        /*delete contact*/
                        deleteContact(contactArray, contactArray[atoi(menu)-1].next, contactElement);
                        contactElement = contactElement - 1;
                        /*set menu to null and exit equals true*/
                        *secondMenu = '\0';
                        secondExit = 1;
                    } else {
                        /*set menu to null and exit equals true*/
                        *secondMenu = '\0';
                        secondExit = 1;
                    }/*end if*/
                }else if(secondMenu[0] == 'D'){
                    /*delete contact*/
                    deleteContact(contactArray, contactArray[atoi(menu)-1].next, contactElement);
                    contactElement = contactElement - 1;
                    /*set menu to null and exit equals true*/
                    *secondMenu = '\0';
                    secondExit = 1;
                } else {
                    /*set menu to null and exit equals true*/
                    *secondMenu = '\0';
                    secondExit = 1;
                }/*end if*/
                /*close the file*/
                *menu = '\0';
            }/*end while loop*/
        } else if (menu[0] == '+'){
            /*scroll down*/
            scroll = scroll + 1;
            *menu = '\0';
        } else if (menu[0] == '-'){
            /*scroll up*/
            scroll = scroll - 1;
            *menu = '\0';
        }/*end if*/
    }/*end while*/

    /*end program*/
    fseek(filePointer, 0, SEEK_SET);
    fclose(filePointer);
    return(0);
}/*end of main*/