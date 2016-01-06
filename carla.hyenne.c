#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */

int Fsize=50; /*enough space for the fmlas we use*/
int i;

struct tableau {
  char *root;
  struct  tableau *left;
  struct tableau *right;
  struct tableau *parent;
}*tab, *node, *node1, *kid, *pa;

/* Use p, q, r, s for propositions.  Use ~ for negation.  Use v for OR, use ^ for AND, use > for implies.  Brackets are (, ). */


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


/* The actual parsing methods.  */

int parse(char *g)
{/* return 1 if a proposition, 2 if neg, 3 if binary, ow 0*/
}

char *partone(char *g)
{/* for binary connective formulas, returns first part*/
}

char *parttwo(char *g)
{/* for binary connective formulas, returns second part*/
}

char bin(char *g)
{/*for binary connective formulas, returns binary connective*/
}


int type(char *g)
{/*return 0 if not a formula, 1 for literal, 2 for alpha, 3 for beta, 4 for double negation*/
}

char *firstexp(char *g)
{/* for alpha and beta formulas*/
  if (parse(g)==3)/*binary fmla*/  switch(bin(g))
         {case('v'): return(??);break;
         case('^'): return(??);break;
         case('>'): return(??);break;
         default:printf("what the f**k?");return(0);
         }
  if ((parse(g)==2)&& (parse(mytail(g))==2)/*double neg*/) return(??);/*throw away first two chars*/

  if ((parse(g)==2)&&parse(mytail(g))==3) /*negated binary*/ 
  switch(bin(mytail(g)))
  {
    case('v'):return(??);break;
    case('^'):return(??);break;
    case('>'): return(??);break;
  } 
  return(0);
}        


char *secondexp(char *g)
{/* for alpha and beta formulas, but not for double negations, returns the second expansion formula*/
}        

int find_above(struct tableau *t, char *g) /*Is g label of current node or above?*/
{
}

int closed1(struct tableau *t) /*check if p and not p at or above t*/
{
  if (t==NULL) return(0);
  else
    {
    }
}
      
int closed(struct tableau *t) /*check if either *t is closed 1, or if all children are closed, if so return 1, else 0 */
{
}

void  add_one( struct tableau *t, char *g)/* adds g at every leaf below*/
{
}

void alpha(struct tableau *t, char *g, char *h)/*not for double negs, adds g then h at every leaf below*/
{
}

void  add_two(struct tableau *t, char *g, char *h)/*for beta s, adds g, h on separate branches at every leaf below*/
{
}

void expand(struct tableau *tp)/*must not be null.  Checks the root.  If literal, does nothing.  If beta calls add_two with suitable fmlas, if alpha calls alpha with suitable formulas unless a double negation then � */
{ 
}

void complete(struct tableau *t)/*expands the root then recursively expands any children*/
{ if (t!=NULL)
    { 
      expand(t);
      complete((*t).left);
      complete((*t).right); 
    }
}


int main()
{ /*input a string and check if its a propositional formula */

char *name = malloc(Fsize);
 printf("Enter a formula:");
 scanf("%s", name);
   if switch(parse(name))
   {
   0:printf("Not a formula\n");
   1:printf("A proposition\n");
   2:printf("A negation\n");
   3:printf("A binary formula\n");
   }
   printf("the type is ");
   switch(type(name))
   {
   0:printf("I told you, not a fmla\n");
   1:printf("a literal\n");
   2:printf("alpha\n");
   3:printf("beta\n");
   4:printf("double negation\n");
  } 
  If (type(name)>1)
printf("first expansion fmla is %s and second expansion is %s\n", firstexp(name), secondexp(name));

  /*make new tableau with name at root, no children, no parent*/
  struct tableau t={name, NULL, NULL, NULL};

  /*expand the root, recursively complete the children*/
  complete(&t);

  /*check if closed*/
  if (closed(&t)) printf("%s is not satisfiable", name);
  else printf("%s is satisfiable", name);

  return(0);
}