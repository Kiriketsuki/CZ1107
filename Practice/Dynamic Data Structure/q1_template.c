////////////////////////////////////////////////////////////

#include "stdlib.h"
#include "stdio.h"

////////////////////////////////////////////////////////////

void insert(char *s,int n);
void removes(char *s, int n);

//////////////////////////////////////////////////////////
void main ()
{
	int n;
    char *s;
	int choice;


	printf("How many characters do you want to input:");
	scanf("%d", &n);

	s=(char *)malloc((n+1)*sizeof(char));

    if (s==NULL)
	{
		printf("can't allocate the memory!");
		return;
	}

	printf("Input the string:");
	scanf("%s",s);

	printf("The string is %s\n", s);

	do
	{
		printf("Do you want to 1-insert or 2-remove or 3-quit?:");
		scanf("%d",&choice);
		getchar(); //to clear the return carriage in the buffer

		if (choice==1){
			insert(s,n);
            printf("The string is now %s\n", s);}
		else if (choice==2) {
			removes(s,n);
            printf("%s \n", s);}
	}while (choice!=3);


}

//////////////////////////////////////////
void removes(char *s, int n)
{
    printf("%c\n", *s);
    for (int i = 0; i < n-1; i++) {
        *s = *(s+1);
        s += 1;
    }
    *s = '\0';
    printf("The string is now ");
}

//////////////////////////////////////////
void insert(char *s, int n)
{   
    char to_insert;
    printf("What is the character you want to insert?: \n");
    scanf("%c", &to_insert);
    getchar();

    s = s+n-1;

    for (int i = 0; i < n-1; i++) {
        *s = *(s-1);
        s--;
    }

    *s = to_insert;
}
