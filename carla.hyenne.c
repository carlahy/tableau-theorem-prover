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
  // printf("Error: not a formula\n");
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
  // printf("Error: not a formula\n");
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
  // printf("Error: not a formula\n");
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
    return 0;
  }
}

int parse(char *g) /* return 1 if a proposition, 2 if neg, 3 if binary, ow 0*/
{
  if (isfmla(g) == 1) {

    if (strlen(g) == 1 && prop(*g) == 1) {
      return 1;
    }
    if (*g == '~') {
      return 2;
    }
    if (*g == '(') {
      return 3;
    }
    else {
      return 0;
    }

  }
  return 0;
}

//////////////////////////// TABLEAU IMPLENTATION ////////////////////////////

int type(char *g)
{/*return 0 if not a formula, 1 for literal, 2 for alpha, 3 for beta, 4 for double negation*/
}

char *firstexp(char *g) /* for alpha and beta formulas*/
{
  if (parse(g)==3) /*binary fmla*/
  switch(bin(g)) {  
    case('v'): return(??);break;
    case('^'): return(??);break;
    case('>'): return(??);break;
    default:printf("what the f**k?");return(0);
  }

  if ( (parse(g)==2) && (parse(mytail(g))==2)) { /*double neg*/
    return(mytail(mytail(g))); /*throw away first two chars*/
  }

  if ( (parse(g)==2) && parse(mytail(g))==3 ) /*negated binary*/ 
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

int find_above(struct tableau *t, char *g) /*Is g label of current node or above? USED WHEN LOOKING IF SATISFIABLE*/
{
  if ( tableau.root == g ) {
    return 1
  }
  else {
    return find_above(tableau.parent, g);
  }
  // printf("Could not find specified node above.\n");
  return 0;
}

int closed1(struct tableau *t) /*check if p and not p at or above t*/
{
  if (tableau == NULL) {
    return 0; //OPEN
  }
  else {
    if (tableau.parent.root[0] == '~') {
      if (tableau.root == mytail(tableau.parent.root)) {
        return 1;
      }
    }
    else {
      return (closed1(tableau.parent));
    }
  }
  return 0;
}
      
int closed(struct tableau *t) /*check if either *t is closed1, or if all children are closed, if so return 1, else 0 */
{
  if (closed1(tableau) == 1) {
    return 1;
  }
  else {
    return ( closed1(tableau.left) && closed1(tableau.right) );
  }
  // printf("Tableau is open\n");
  return 0;
}

void add_one(struct tableau *t, char *g)/* adds g at every leaf below*/
{
  if (tableau.left == NULL && tableau.right == NULL) {

    kid->root = g;
    kid->left = NULL;
    kid->right = NULL;
    kid->parent = tableau;

    tableau.left = kid; //WTF AM I DOING?
  }
  else {
    if (tableau.left != NULL) {
      add_one(tableau.left, g);  
    }
    if (tableau.right != NULL) {
      add_one(tableau.right, g);
    }
  }
}

void alpha(struct tableau *t, char *g, char *h)/*not for double negs, adds g then h at every leaf below*/
{
}

void add_two(struct tableau *t, char *g, char *h)/*for beta s, adds g, h on separate branches at every leaf below*/
{
  if (tableau.left == NULL && tableau.right == NULL) {

    node->root = g;
    node->left = NULL;
    node->right = NULL;
    node->parent = tableau;
    tableau.left = node; //WTF

    node1->root = h;
    node1->left = NULL;
    node1->right = NULL;
    node1->parent = tableau;
    tableau.right = node1; //WTF

  }
  else {
    if (tableau.left != NULL) {
      add_two(tableau.left, g, h);  
    }
    if (tableau.right != NULL) {
      add_two(tableau.right, g, h);
    }
  }
}

void expand(struct tableau *tp)
/*must not be null.  Checks the root.  
If literal, does nothing.  
If beta calls add_two with suitable fmlas, 
if alpha calls alpha with suitable formulas unless a double negation then � */
{
  //must not be NULL
  //if literal, do nothing
  //if beta, calls add_two with suitable fmlas
  //if alpha, calls add_one with suitable fmlas (unless double neg)
  //if double negation 
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
{ /*input 6 strings from "input.txt" */
  char *names[inputs];/*to store each of the input strings*/

  for (i=0;i<inputs;i++) 
  {
    names[i]=malloc(Fsize);/*create enough space*/
  }

  FILE *fp, *fpout, *fopen();

  if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}/*ouputs to be sent to "output.txt"*/

  fscanf(fp,"%s %s %s %s %s %s",names[0],names[1], names[2], names[3],names[4],names[5]);/*read input strings from "input.txt"*/
 
  /*lets check your parser*/
  for(i=0;i<inputs;i++) {
    j = parse(names[i]);
    switch(j)
    {
      case(0):fprintf(fpout,"%s is not a formula", names[i]);break;
      case(1):fprintf(fpout,"%s is a proposition",names[i]);break;
      case(2):fprintf(fpout,"%s is a negation",names[i]);break;
      case(3):fprintf(fpout,"%s is a binary formula",names[i]);break;
      default:fprintf(fpout,"%s is not a formula",names[i]);break;
    }
  }
 
  /*make 6 new tableaus each with name at root, no children, no parent*/

  struct tableau tabs[inputs];

  for(i=0;i<inputs;i++)
  {
    tabs[i].root=names[i];
    tabs[i].parent=NULL;
    tabs[i].left=NULL;
    tabs[i].right=NULL;

    /*expand each tableau until complete, then see if closed */ 

    complete(&tabs[i]);
    if (closed(&tabs[i])) fprintf(fpout,"%s is not satisfiable\n", names[i]);
    else fprintf(fpout,"%s is satisfiable\n", names[i]);
  }
 
  fclose(fp);
  fclose(fpout);
 
  return(0);
}



