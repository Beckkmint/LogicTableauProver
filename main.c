//
//  TableauProver.c
//  
//
//  Created by Becky on 06/01/2018.
//
//

#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */
#include <tclDecls.h>

int Fsize=50;
int cases=10;


int i;
int j;

int leaf = 0;
char ***allBranches[10][20];

/*typedef struct tableau tableau;*/

struct tableau {
    char *root;
    struct tableau *left;
    struct tableau *right;
    struct tableau *parent;
}*tab, *node, *node1, *kid, *pa;



/*put all your functions here.  You will need
 1.
 int parse(char *g) which returns 1 if a proposition, 2 if neg, 3 if binary, ow 0
 2.
 void complete(struct tableau *t)
 which expands the root of the tableau and recursively completes any child tableaus.
 3.
 int closed(struct tableau *t)
 which checks if the whole tableau is closed.
 Of course you will almost certainly need many other functions.
 
 How you implement these functions is up to you.  Best not to change the main method, though.
 */

int findBC(char *g)
{
  //find the position of binary connective in the formula
    int k,h;
    int c = 0;
    int length = strlen(g);
    char bc[] = {'v','>','^'};

    int pos_BC = 0;
    for (k = 0; k < length; k++) {
        if (c == 1) {
            for (h = 0; h < 3; h++) {
                if (g[k] == bc[h]) {
                    pos_BC = k;
                    return pos_BC;
                }
            }
        }
        if (g[k] == '(') {
            c++;
        }
        else if (g[k] == ')') {
            c--;
        }
    }
    return 0;
}


int parse(char *g)
{
/* return 1 if proposition, 2 if negation, 3 if binary, ow 0*/
    int i = 0;
    int j = 0;
    int length = strlen(g);
  
    //proposition
    if(length == 1){
        if(g[0] == 'p' || g[0] == 'q' || g[0] == 'r'){
            return 1;
        }
    }
    
    //negation
    else if(g[0] == '-'){
        if(parse(g + 1) > 0)
            return 2;
    }
    
    //binary
    else if(g[0] == '(' && g[length - 1] == ')'){
        char formulaOne[100];
        char formulaTwo[100];
        int a = findBC(g);
        
        while(i < a - 1){
            formulaOne[i] = g[i + 1];
            i++;
        }
        formulaOne[a - 1] = '\0';
  
        while(j < length - (a + 1) - 1){
            formulaTwo[j] = g[j + a + 1];
            j++;
        }
        formulaTwo[length - (a + 1) - 1] = '\0';
        
        if(parse(formulaOne) > 0 && parse(formulaTwo) > 0)
            return 3;
        
    }

    return 0;
}


void complete(struct tableau *t){

}


int closed(struct tableau *t){
    // if a branch contains both p/q/r and -p/-q/-r then returns ture
    //char *branch[] = (char *[]) {};
    char *branch[20];
    char ***allBranch = findAllPaths(t,branch,0);
    return allContainContradiction(allBranch);
}


char*** findAllPaths(struct tableau *t, char *branch[], int pathLength){
    if (t == NULL)
        return NULL;

    branch[pathLength] = t->root;
    pathLength ++;

    if (t->left == NULL && t->right == NULL){
        **allBranches[leaf] = branch;
        leaf ++;
        if (leaf == countLeaves(t))
            return **allBranches;
        return NULL;
    }

    else{
        char ***left = findAllPaths(t->left,branch,pathLength);
        char ***right = findAllPaths(t->right,branch,pathLength);
        if (left != NULL)
            return left;
        return right;
    }
}


int countLeaves(struct tableau *t){
    if(t == NULL)
        return 0;
    if(t->left == NULL && t->right==NULL)
        return 1;
    else
        return countLeaves(t->left) + countLeaves(t->right);
}


int allContainContradiction(char ***branch){ //max 10 branches approx
    int noOfBranches = sizeof(branch) / sizeof(**branch);
    for (int a = 0 ; a < noOfBranches ; a ++){
        if (!containContradiction(branch[a]))
            return 0;
    }
    return 1;
}


int containContradiction(char *branch[]){
    int nodesInBranch = sizeof(branch) / sizeof(branch[0]);
    for(int a = 0; a < nodesInBranch; a ++){
        if (parse(branch[a]) == 1){
            for(int b = 0; b < nodesInBranch; b ++){
                if (branch[b] == "-" + branch[a][0])
                    return 1;
            }
        }
    }
    return 0;
}


int main()

{ /*input a string and check if its a propositional formula */
    
    
    char *name = malloc(Fsize);
    FILE *fp, *fpout;
    
    /* reads from input.txt, writes to output.txt*/
    if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
    if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}
    
    int j;
    for(j=0;j<cases;j++)
    {
        fscanf(fp, "%s",name);/*read formula*/
        switch (parse(name))
        {case(0): fprintf(fpout, "%s is not a formula.  ", name);break;
            case(1): fprintf(fpout, "%s is a proposition.  ", name);break;
            case(2): fprintf(fpout, "%s is a negation.  ", name);break;
            case(3):fprintf(fpout, "%s is a binary.  ", name);break;
            default:fprintf(fpout, "What the f***!  ");
        }
        
        /*make new tableau with name at root, no children, no parent*/
        
        struct tableau t={name, NULL, NULL, NULL};
        
        /*expand the root, recursively complete the children*/
        if (parse(name)!=0)
        {
            complete(&t);
            if (closed(&t))
                fprintf(fpout, "%s is not satisfiable.\n", name);
            else fprintf(fpout, "%s is satisfiable.\n", name);
        }
        else fprintf(fpout, "I told you, %s is not a formula.\n", name);
    }
    
    fclose(fp);
    fclose(fpout);
    free(name);
    
    return(0);
}