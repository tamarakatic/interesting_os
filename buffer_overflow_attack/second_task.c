#include <stdio.h>
#include <string.h>
#define BUFSIZE 10
#define MAX_FILE1 30

void checkString(char s1[], char s2[], char name[], char joker[]) {
   if (strstr(name, joker) != NULL) {
      printf("%s", s1);
   } else
   {
      printf("%s", s2);
   }
}

void bufferOverflowAttack(){
   char B[BUFSIZE];

   char s1[16] = "I owe you $1000";
   char s2[17] = "I don't know you";

   char expectedName[] = "joker";
 
   FILE *f = fopen("test_part1.txt" , "r");
   if(f == NULL){
      perror("Error opening file");
      return(-1); 
   }
   else{ 
      fgets(B, MAX_FILE1, f);
   }
   fclose(f);
   printf("What's your name?\n");
   checkString(s1, s2, B, expectedName);
}

int main() {
   bufferOverflowAttack();
   return 0;
}
