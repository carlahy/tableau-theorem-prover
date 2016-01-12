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
      y++;
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

  for (int i=1; i<length-2; i++) {
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

  if (connectives != 0) { //WTF
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
  else if (isBin(g) == 1) { //WRONG : if there is a binary 
    return ( isfmla(partone(g)) && isfmla(parttwo(g)) );
  }
  else {
    //printf("Not a formula\n");
    return 0;
  }
}

int parse(char *g) /* return 1 if a proposition, 2 if neg, 3 if binary, ow 0*/
{
  printf("First char == %c  ", *g);
  printf("formula is ");
  if (isfmla(g) == 1) {

    if (strlen(g) == 1 && prop(*g) == 1) {
      printf("a proposition\n");
      return 1;
    }
    if (*g == '~') {
      printf("a negation\n");
      return 2;
    }
    if (*g == '(') {
      printf("binary\n");
      return 3;
    }
    else {
      printf("not a fmla\n");
      return 0;
    }

  }
  printf("not a fmla.\n");  
  return 0;
}

char *negate(char *g) {
  char *negated = malloc(sizeof(char) * (strlen(g) + 1));
  negated[0]='~';
  int x=1;
  for (int i = 0; i < strlen(g); i++) {
    negated[x] = *(g+i);
    x++;
  }
  return negated;
}

int main() {
	
  char* formulas[] = { "(qvp)", "~(p>(q>p))", "(pv~q)", "~~p", "~(pv~p)", "(p^~p)", "p", "p~" };
  
  // for (int i = 0; i < 8; ++i)
  // {
  //   printf("%s\t", formulas[i]);
  //   parse(formulas[i]);
  //   printf("\n");
  // }

  char *string;
  string = negate(formulas[1]);
  printf("%s\n", string);


	return 0;
}


// char *firstexp(char *g) /* for alpha and beta formulas*/
// {
//   if (parse(g)==3) /*binary fmla*/
//   switch(bin(g)) {  
//     case('v'): return(??);break; //beta
//     case('^'): return(??);break; //alpha
//     case('>'): return(??);break; //beta
//     default:printf("what the f**k?");return(0);
//   }

//   if ( (parse(g)==2) && (parse(mytail(g))==2)) { /*double neg*/
//     return(mytail(mytail(g))); 
//   }

//   if ( (parse(g)==2) && parse(mytail(g))==3 ) /*negated binary*/ 
//   switch(bin(mytail(g)))
//   {
//     case('v'): return( alpha( negate(partone(g)), negate(parttwo(g))) );break;
//     case('^'): return( add_two( negate(partone(g)), negate(parttwo(g)) ) );break;
//     case('>'): return( alpha( partone(g), negate(parttwo(g)) ) );break;
//   } 
//   return(0);
// }

// char *secondexp(char *g)
// {/* for alpha and beta formulas, but not for double negations, returns the second expansion formula*/
// negated(partone(g), ^ , negated(parttwo))
// }