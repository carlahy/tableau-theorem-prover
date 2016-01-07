#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h> 

/* List processing methods  */

char *mytail(char *list)  /*given non-empty string, returns string without the first char*/
{
  int length = strlen(list);
  char *tail = malloc(sizeof(char) * (length-1));
  int y=0;
  for(int i=1; i< length; i++) {
    tail[0] = list[i];
  }
  return tail;
}

char *segment(char *list, int i, int j)/* characters from pos i up to j-1, provided i<=j*/
{
  if (i<=j) {
    char *result = malloc(sizeof(char) * (j-i));
    int y = 0;
    for (int x = i; x < j; x++){
      result[y] = list[x];
    }
    return result;
  }
  else {
    printf("Invalid segment parameters\n");
    return NULL;
  }
}

/* Basics.  Recognise propositions and connectives.  */

int prop(char x)
{if((x='p')||(x='q')||(x='r')||(x='s')) return(1);else return(0);}

int bc(char x)
{if ((x=='v')||(x=='^')||(x=='>')) return(1);else return(0);}


//////////////////////////// PARSING ////////////////////////////

/* The actual parsing methods.  */

char *partone(char *g) /* for binary connective formulas, returns first part*/
{
  int length = strlen(g);
  int brackets = 0;
  for (int i=1; i<length; i++) {
    if (*(g+i) == '(') {
      brackets++;
    }
    else if (*(g+i) ==')') {
      brackets--;
    }
    else if (brackets == 0 && bc(*(g+i)) == 1) {
      return segment(g, 1, i);
    }
  }
  printf("Error: not a formula\n");
  return NULL;
}

char *parttwo(char *g) /* for binary connective formulas, returns second part*/
{
  int length = strlen(g);
  int brackets = 0;
  for (int i=1; i<length; i++) {
    if (*(g+i) == '(') {
      brackets++;
    }
    else if (*(g+i) ==')') {
      brackets--;
    }
    else if (brackets == 0 && bc(*(g+i)) == 1) {
      return segment(g, i+1, length-1);
    }
  }
  printf("Error: not a formula\n");
  return NULL;
}

char bin(char *g) /*for binary connective formulas, returns binary connective (USE IN TABLEAU) */
{
  int length = strlen(g);
  int brackets = 0;
  for (int i=0; i<length; i++) {
    if (*(g+i) == '(') {
      brackets++;
    }
    else if (*(g+i) == ')') {
      brackets--;
    }
    else if (brackets == 0 && bc(*(g+i)) == 1) {
      return *(g+i);
    }
  }
  printf("Error: not a formula\n");
  return '\0';
}

int isBin(char *g) //is fmla a binary formula
{
  int brackets = 0;
  int connectives = 0;
  int length = strlen(g);

  if(*g != '(' || *(g+length-1) != ')') {
    return 0;
  }

  for (int i=1; i<length-1; i++) {
    if (*(g+i) == '(') {
      brackets++;
    }
    else if (*(g+i) == ')') {
      brackets--;
    }
    else if (bc(*(g+i)) == 1 && brackets == 0) {
      connectives++;
    }
  }

  if (connectives == 0) {
    return 1;
  }

  return 0;
}

int isfmla(char *g) { 
  if (strlen(g) == 1 && prop(*g) == 1) {
    return 1;
  }
  else if (*g == '~') {
    return isfmla(mytail(g));
  }
  else if (isBin(g)) {
    return ( isfmla(partone(g)) && isfmla(parttwo(g)) );
  }
  else {
    return 0;
  }
}

int parse(char *g) /* return 1 if a proposition, 2 if neg, 3 if binary, ow 0*/
{
  if(prop(*g)) {
    return 1;
  }
  else if (*g == '~') {
    return 2;
  }
  else if (*g == '(') {
    return 3;
  }
  else {
    return 0;
  }
}

int main() {
	char string[] = "hello world!";
	int length = strlen(string);
	printf("length is %i", length);
	//printf("%s\n", );



	return 0;
}