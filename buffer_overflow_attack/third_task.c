#include <stdio.h>
#include <string.h>
#define BUFSIZE 10
#define MAX_FILE2 47

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

   printf("What's your name?\n");
   gets(B);
   char s3[MAX_FILE2];
   FILE *f1 = fopen("test_part2.txt" , "r");
   if(f1 == NULL){
      perror("Error opening file");
      return(-1); 
   }
   else{ 
      fgets(s3, MAX_FILE2, f1);
      for(int i = strlen(B); i <= MAX_FILE2; i++) {
         B[i] = s3[i];
      }
      checkString(s1, s2, B, expectedName);
   }
   fclose(f1);
}

int main() {
   bufferOverflowAttack();
   return 0;
}
