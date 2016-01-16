#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h> 

int Fsize=50;
int inputs=6;

int i;
int j;

/*typedef struct tableau tableau;*/

struct tableau {
  char *root;
  struct tableau *left;
  struct tableau *right;
  struct tableau *parent;
}*tab, *node, *node1, *kid, *pa;

/* List processing methods  */

char *mytail(char *list)  /*given non-empty string, returns string without the first char*/
{
  int length = strlen(list);
  char *tail = malloc(sizeof(char) * (length-1));
  int y=0;
  for(int i=1; i<length; i++) {
    tail[y] = list[i]; 
    y++;
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
  return NULL;
}

char bin(char *g) /*for binary connective formulas, returns binary connective*/
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
    else if (brackets == 1 && bc(*(g+i)) == 1) {
      return *(g+i);
    }
  }
  return 0;
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

  if (connectives != 0) {
    return 1;
  }

  return 0;
}

int isfmla(char *g) { 
  if (strlen(g) == 1 && prop(*g) == 1) {
    return 1;
  }
  else if (*g == '-') {
    return isfmla(mytail(g));
  } 
  else if (isBin(g) == 1) {
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
    if (*g == '-') {
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
  if (parse(g) == 1) {
    return 1;
  }

  else if (parse(g) == 3) { //binary
    char connective = bin(g);
    if (connective == '^') { //(p^q), alpha
      return 2;
    }
    else if (connective == 'v') { //(pvq), beta
      return 3;
    }
    else if (connective == '>') { //(p>q), beta
      return 3;
    }
  }

  else if (parse(g) == 2 && parse(mytail(g)) != 2) { //single negation
    if (type(mytail(g)) == 2) { //-(p^q)
      return 3;
    }
    else if (type(mytail(g)) == 3) { //-(pvq), -(p>q)
      return 2;
    }
  }

  else if (parse(g) == 2 && parse(mytail(g)) == 2) { //double negation
    return 4;
  }

  return 0;
}

char *negate(char *g) {
  if (parse(g) == 2) {
    return mytail(g);
  }
  char *negated = malloc(sizeof(char) * (strlen(g) + 1));
  negated[0]='-';
  int x=1;
  for (int i = 0; i < strlen(g); i++) {
    negated[x] = *(g+i);
    x++;
  }
  return negated;
}        

void add_one(struct tableau *t, char *g)/* adds g at every leaf below*/
{
  if ((*t).left == NULL && (*t).right == NULL) {
    struct tableau *newtab = malloc(sizeof(struct tableau));
    newtab->root = g;
    newtab->left = NULL;
    newtab->right = NULL;
    newtab->parent = t;

    t->left = newtab;
  }
  else {
    if ((*t).left != NULL) {
      add_one((*t).left, g);  
    }
    if ((*t).right != NULL) {
      add_one((*t).right, g);
    }
  }
}

void alpha(struct tableau *t, char *g, char *h)/*not for double negs, adds g then h at every leaf below*/
{
  add_one(t, g);
  add_one(t, h);
}

void add_two(struct tableau *t, char *g, char *h)/*for beta s, adds g, h on separate branches at every leaf below*/
{
  if ((*t).left == NULL && (*t).right == NULL) {

    struct tableau *lefttab = malloc(sizeof(struct tableau));
    
    lefttab->root = g;
    lefttab->left = NULL;
    lefttab->right = NULL;
    lefttab->parent = t;
    t->left = lefttab;

    struct tableau *righttab = malloc(sizeof(struct tableau));

    righttab->root = h;
    righttab->left = NULL;
    righttab->right = NULL;
    righttab->parent = t;
    t->right = righttab;

  }
  else {
    if ((*t).left != NULL) {
      add_two((*t).left, g, h);  
    }
    if ((*t).right != NULL) {
      add_two((*t).right, g, h);
    }
  }
}

void expand(struct tableau *tp)
/*must not be null.  Checks the root.  
If literal, does nothing.  
If beta calls add_two with suitable fmlas, 
if alpha calls alpha with suitable formulas unless a double negation then � */
{
  if((*tp).root != NULL) {
    //if literal, do nothing
    //if binary formula
    if (parse((*tp).root) == 3) { //if binary (not negated)
      if(type((*tp).root) == 2) { //if alpha
        alpha(tp, partone((*tp).root), parttwo((*tp).root));
      }
      else if(type((*tp).root) == 3) { //if beta
        add_two(tp, partone((*tp).root), parttwo((*tp).root));
      }
    }

    else if ( parse((*tp).root) == 2 && type((*tp).root) != 4) { //if negated, but not double negated
      if (type(mytail((*tp).root)) == 2) { // if alpha
        add_two( tp, negate(partone((*tp).root)), negate(parttwo((*tp).root)) );
      }
      else if (type(mytail((*tp).root)) == 3) { //if beta
        if (bin((*tp).root) == 'v') {
          alpha( tp, negate(partone(mytail((*tp).root))), negate(parttwo(mytail((*tp).root))));
        }
        else if (bin((*tp).root) == '>') {
          alpha( tp, partone(mytail((*tp).root)), negate(parttwo(mytail((*tp).root)))) ;
        }
      }
    }

    else if (type((*tp).root) == 4 ) { //double negated
      add_one(tp, mytail(mytail((*tp).root)));
    }
  }
}

void complete(struct tableau *t) /*expands the root then recursively expands any children*/
{ 

  if (t!=NULL)
  { 
    expand(t);
    complete((*t).left);
    complete((*t).right); 
  }
}

int find_above(struct tableau *t, char *g) /*Is g label of current node or above?*/
{
  if (t != NULL) 
  {
    if ( strcmp((*t).root, g) == 0 ) {
      return 1;
    }
    else {
      return find_above((*t).parent, g);
    }
  }
  return 0;
}

int closed1(struct tableau *t) /*check if p and not p at or above t*/
{
  if (type((*t).root) == 1) {
    if (find_above(t, negate((*t).root)) == 1) {
      return 1;
    }
    else {
      return closed1((*t).parent);      
    }
  }
  return 0;
}

int closed(struct tableau *t) /*check if either *t is closed1, or if all children are closed, if so return 1, else 0 */
{
  if (type((*t).root) == 4 && type(mytail(mytail((*t).root))) == 1) {
    return 0;
  }
  if (closed1(t) == 1) { 
    return 1;
  }
  else {
    if ( (*t).left == NULL ) {
      return 1;
    } else {
      closed((*t).left);  
    }
    
    if ( (*t).right == NULL ) {
      return 1;
    }
    else {
      closed((*t).right);  
    }
  }
  return 0;
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
      case(0):fprintf(fpout,"%s is not a formula\n", names[i]);break;
      case(1):fprintf(fpout,"%s is a proposition\n",names[i]);break;
      case(2):fprintf(fpout,"%s is a negation\n",names[i]);break;
      case(3):fprintf(fpout,"%s is a binary formula\n",names[i]);break;
      default:fprintf(fpout,"%s is not a formula\n",names[i]);break;
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

    complete(&tabs[i]);

    if (closed(&tabs[i]) == 1) fprintf(fpout,"%s is not satisfiable\n", names[i]);
    else fprintf(fpout,"%s is satisfiable\n", names[i]);
  }

  fclose(fp);
  fclose(fpout);
 
  return(0);
}