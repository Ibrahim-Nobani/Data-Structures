#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define maximum 100
// Name: Ibrahim Nobani | ID: 1190278 | section:4
// defined the struct node type first
struct node {
    int coef; // coeffiecient and exponent (power), that we are going to store our data inside.
    int exp;
    struct node* next; // a next to point at the next value.
};
struct node* poly[maximum]; // defined all the needed linked lists (global) and initialized them with null.
struct node *resultA=NULL;
struct node *resultS=NULL;
struct node *resultM=NULL;
struct node *tempA=NULL;
struct node *tempS=NULL;
struct node *tempM=NULL;
int count, flag2=0; // count is the number of lines in the text (number of equations).
struct node* makeEmpty(struct node*);
void insB(struct node** , int , int );
void insA(struct node** , int , int );
void DeleteList(struct node*);
void OpenFileAndRead();
void deleteWhiteSpaces(char*);
void delete1(char*);
void deleteNL(char*);
void verse(struct node *);
void verseFile(struct node *,FILE*);
void AdjustM(struct node*);
int isLast (struct node*, struct node*);
int findValue(double, struct node* );
int findX(struct node*);
struct node* polyAdd(struct node *, struct node *);
struct node* polySub(struct node *, struct node *);
struct node* AdjustAdd(struct node*,struct node*);
struct node* AdjustSub(struct node*, struct node*);
struct node* PolyMultiply(struct node *,struct node *);
struct node* FindPrevious(int , struct node* );
int main()
{
    for (int i=0; i<maximum; i++){
     poly[i]=NULL; // here the array linked list which the equations are to be stored in, initialized all of it to null.
}
int flag1=0; // a flag that makes sure the file is read.
    int choice; //user's choice to the menu.
    do { // a simple menu interface to let the user choose from.
        printf("1- Read Equations from file. \n");
        printf("2- Display equations. \n");
        printf("3- Preform addition on equations. \n");
        printf("4- Preform subtraction on equations. \n");
        printf("5- preform multiplication on equations. \n");
        printf("6- Save all results in a text file. \n");
        printf("**Press any other NUMBER to exit** \n");
        scanf("%d",&choice);
        if(choice == 1){
                if (flag2==-1){
                    printf("Put equations in file in order to read correctly!!\n");
                }
                else {
            printf("The equations file has been read Successfully! \n");
            OpenFileAndRead(); // calling the read function.
            flag1=1; // setting up the first flag value to 1, to make sure the equations are read on the next steps.
                }
        }
        else if (choice == 2){
                if(flag1==1){ // if equations are read.
            printf("Equations are displayed below: \n");
                for (int i=0; i<count; i++){
                verse(poly[i]);
            }
        }
    else printf("You should read the equations from file first!! \n"); // a message in case the file has not been read.
        }
        else if (choice == 3){
                if (flag1==1){
                int j=0;
                insA(&resultA,0,0); // addition result is given an initial value of 0, which is the neutral component of the addition operation.
              while (j<count){
                  resultA= polyAdd(resultA,poly[j]); // adding all the equations together.
                  j++;
            }
            tempA=resultA;
            verse(tempA); // printing result.
            resultA=NULL; // setting the result back to null, in case the user wants to do an addition on other equations.
            findX(tempA); // takes the X element the user wishes to insert.
                }
                else printf("You should read the equations from file first!! \n");
        }
        else if (choice==4){
            if (flag1==1){
             resultS=poly[0]; // setting the result to the first linked list equation.
             for (int j=1; j<count; j++){
                resultS=polySub(resultS,poly[j]); // subtracting first equation from the second one.
             }
             tempS=resultS;
             verse(tempS);
             resultS=NULL;
             findX(tempS);
            }
            else printf("You should read the equations from file first!! \n");
        }
        else if (choice ==5){
            if (flag1==1){
                insA(&resultM,1,0); // initializing the result to the neutral component of the multiplication operation.
                for (int i=0; i<count; i++){
                   resultM=PolyMultiply(resultM,poly[i]);
                }
                tempM=resultM;
                resultM=NULL;
                verse(tempM);
                findX(tempM);
            }
            else printf("You should read the equations from file first!! \n");
        }
        else if (choice ==6){
            if (flag1==1){
                    printf("Check the results file\n");
              FILE* outcome;
              outcome =fopen("results.txt","w");
              verseFile(tempA,outcome);
              verseFile(tempS,outcome);
              verseFile(tempM,outcome);
              fclose(outcome);
            }
            else printf("You should read the equations from file first!! \n");
        }
    } while (choice < 7 && choice >=1);
    return 0;
}
struct node* makeEmpty(struct node* L){ //make Empty function.
    if(L != NULL)
         DeleteList(L);
    L = (struct node*) malloc(sizeof(struct node));
    if (L == NULL)
        printf("out of memory");
    L->next = NULL;
    return L;
};
void DeleteList(struct node* L){ //delete list function.
    if(L == NULL)
        return;
   struct node* p;
   struct node* temp;
   p= L->next;
   L->next= NULL;
    while(p != NULL){
    temp=p->next;
    free(p);
    p=temp;
}
}
// 2 insert functions, to enter values
void insB(struct node** list, int c, int e){
    struct node *temp;
    temp = makeEmpty(NULL);
    temp->coef=c;
    temp->exp=e;
    temp->next=*list;
    *list=temp;
    //printf("INS B\n");
}
void insA(struct node** list, int c, int e){
struct node *temp, *p;
temp = *list;
if(*list == NULL){
    insB(&(*list),c,e);
    //printf("NULL");
}
    else{
while (temp->next != NULL){
    temp=temp->next;
    //printf("INS E\n");
}

p=makeEmpty(NULL);
p->coef=c;
p->exp=e;
p->next=NULL;
temp->next=p;
    }
}
// verse function that prints the equations using the coefficient and the power obtained from the file.
void verse(struct node *list){
struct node *temp;
temp=list;
while (temp != NULL){// if coef != 0. if exp != 0
    if (temp->coef !=0 && temp->exp!=0){
    printf("%dx^%d + ",temp->coef,temp->exp);
    temp=temp->next;
    }
    else if(temp->coef==0){
        temp=temp->next;
    }
    else{
         printf("%d + ",temp->coef);
    temp=temp->next;
    }
}
printf("0\n");
}
// verseFile functions that prints equations results into a file.
void verseFile(struct node *list,FILE *outcome){
struct node *temp;
temp=list;
while (temp != NULL){// if coef != 0. if exp != 0
    if (temp->coef !=0 && temp->exp!=0){
    fprintf(outcome,"%dx^%d + ",temp->coef,temp->exp);
    temp=temp->next;
    }
    else if(temp->coef==0){
        temp=temp->next;
    }
    else{
         fprintf(outcome,"%d + ",temp->coef);
    temp=temp->next;
    }
}
fprintf(outcome,"0\n");
if(outcome==NULL){
    printf("This File is Empty, please evaluate the polynomial operations first. \n");
    flag2=1;
}
//fclose(outcome);
}
// addition of polynomial function.
struct node* polyAdd(struct node *poly1, struct node *poly2){
struct node *result=NULL, *fresult=NULL;
struct node *tp, *tq;
tp=poly1;
tq=poly2;
while (tp!=NULL && tq!=NULL){ //making sure both polynomials are not null
    if(tp->exp==tq->exp){ // add coefficients if the exponents are the same, and insert them in the result linked list.
        insA(&result,tp->coef + tq->coef, tp->exp);
        tp=tp->next;
        tq=tq->next;
    }
    else {
             if(tp->exp > tq->exp){ // if the first exponent is bigger than the second insert the first to the linked list.
        insA(&result,tp->coef,tp->exp);
        tp=tp->next;
    }
          else { // if the second exponent is bigger than the first insert the second to the linked list.
            insA(&result,tq->coef,tq->exp);
            tq=tq->next;
          }
       }
    }
    while (tp!=NULL){
        insA(&result,tp->coef,tp->exp);
        tp=tp->next;
}
     while (tq!=NULL){
        insA(&result,tq->coef,tq->exp);
        tq=tq->next;
    }
   //verse(result);
   struct node* result1=NULL;
   result1=AdjustAdd(result,fresult); // an adjust function that makes sure there are no 2 polynomials that share the same power are not added.
 return result1;
};
struct node* polySub(struct node *poly1, struct node *poly2){ // polynomial subtraction function.
struct node *result=NULL, *fresult=NULL;
struct node *tp, *tq;
tp=poly1;
tq=poly2;
while (tp!=NULL && tq!=NULL){
    if(tp->exp==tq->exp){ // if exponents are equal, subtract first coefficient from the second.
        insA(&result,tp->coef - tq->coef, tp->exp);
        tp=tp->next;
        tq=tq->next;
    }
    else {
             if(tp->exp > tq->exp){ // if the first exponent is bigger than the second insert the first to the linked list.
        insA(&result,tp->coef,tp->exp);
        tp=tp->next;
    }
          else { // if the second exponent is bigger than the first insert the second to the linked list.
            insA(&result,tq->coef,tq->exp);
            tq=tq->next;
          }
       }
    }
    while (tp!=NULL){
        insA(&result,tp->coef,tp->exp);
        tp=tp->next;
}
     while (tq!=NULL){
        insA(&result,tq->coef,tq->exp);
        tq=tq->next;
    }
  struct node* result1=NULL;
   result1=AdjustSub(result,fresult);// an adjust function that makes sure there are no 2 polynomials that share the same power are not subtracted.
 return result1;
};
struct node* AdjustAdd(struct node* result, struct node* fresult){ // the addition adjust function.
 int flag = 0;
    while ((result!=NULL && result->next!=NULL) || isLast(result,result)){ // first it checks if both the result head and the next are not null, or its not the last node.
        if (isLast(result,result) && flag ==-1){ // if its the last and flag=-1(means its not equal the power before it)
            insA(&fresult,result->coef,result->exp);
            break;
            }
            else if (isLast(result,result)){
                break;
            }
    else if(result->exp == result->next->exp){  // if the power is equal the next node power, add them together.
        insA(&fresult,result->coef + result->next->coef,result->exp);
        result=result->next;
        flag =1;
    }
    else{ // if they are not equal just insert the node to the new final result linked list and proceed to the next node.
        insA(&fresult,result->coef,result->exp);
        result=result->next;
        flag =-1;
    }
}
return fresult;
};
struct node* AdjustSub(struct node* result, struct node* fresult){ // adjust subtraction result, works like the adjust addition.
 int flag = 0;
    while ((result!=NULL && result->next!=NULL) || isLast(result,result)){
        if (isLast(result,result) && flag ==-1){
            insA(&fresult,result->coef,result->exp);
            break;
            }
            else if (isLast(result,result)){
                break;
            }
    else if(result->exp == result->next->exp){
        insA(&fresult,result->coef - result->next->coef,result->exp); // subtracts if similar exponents.
        result=result->next;
        flag =1;
    }
    else{
        insA(&fresult,result->coef,result->exp);
        result=result->next;
        flag =-1;
    }
}
return fresult;
};

struct node* PolyMultiply(struct node* poly1, struct node* poly2){ // multiplication function
    struct node *tq, *tp, *product=NULL;
    tp=poly1;
   tq=poly2;
    while (tp!=NULL){
        tq=poly2;
        while (tq!=NULL){ // runs throw the equation when its not null and multiple every coefficient and adds the exponents with the first node of the first equation.
            insA(&product,tp->coef * tq->coef, tp->exp+tq->exp); // insert the results.
            tq=tq->next; // go next till the equation is over.
        }
        tq=poly2;
        tp=tp->next; // go to the second node and does the same till the equation is null.
    }
    AdjustM(product); // function that adjusts the  product result.
    return product;
};
int isLast(struct node* p, struct node* LL){ // is last function, checks if the entered node is the last one.
return p->next==NULL;
}
struct node* FindPrevious(int X, struct node* L){ // find the previous node of a given one.
struct node* P;
P = L;
while(P->next != NULL && P->next->exp != X)
P = P->next;
return P;
}
void AdjustM(struct node* poly){ // multiplication adjust that runs throw the product and fixes the results.
    struct node *temp1, *temp2, *temp3;
    temp1 = poly;
    while (temp1 != NULL && temp1->next != NULL) {
        temp2 = temp1;
        while (temp2->next != NULL) {
            if (temp1->exp == temp2->next->exp) {
                temp1->coef = temp1->coef + temp2->next->coef;
                temp3 = temp2->next;
                temp2->next = temp2->next->next;
                free(temp3);
            }
            else
                temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
}
int findValue (double x,struct node* reload){ // findValue function to enter an alternative value for X in the equation and get the result.
struct node* twd;
twd = reload;
int result =0, fresult=0,a, b;
  while (twd!=NULL){
        a=twd->coef;
        b=twd->exp;
        result= a * (int)pow((double)x,b);
        fresult=fresult+result;
        twd=twd->next;
  }
return fresult;
}
int findX(struct node* reload){ // works almost like the findValue after entering a substitute of X, but this function takes the X value from the user
char ch;                        // and prints the result in an appropriate way.
int result,xV;
            printf("Do you wish to enter a substitute variable value for X? (y for YES) ");
            scanf("%s",&ch);
            if (ch =='y'){
                    printf("Enter the value that wish to substitute X for: ");
                    scanf("%d",&xV);
                    result=findValue(xV,reload);
                    printf("The result after substituting X with %d is: %d \n",xV,result);
            }
            return result;
}
void OpenFileAndRead(){ // the read from file function.
FILE *inside; // opening a file
inside=fopen("equations.txt","r");
char storeArray[maximum]; // the array that the will store the equations.
int flag,power,coefficient,j=0;
count=0; // global count value that generates number of equations.
if (inside==NULL){ // checking if the file is NULL.
    printf("This file ceases to exist!\n");
    flag2=-1;
    return;
}
 inside = fopen("equations.txt", "r");
while (fgets(storeArray,maximum,inside)!=NULL){ // getting values from the file.
        //printf("%s\n",storeArray);
        deleteWhiteSpaces(storeArray); // deletes all white spaces in file.
        delete1(storeArray); // deletes all "^" in the file (because we are reading character by character.
        deleteNL(storeArray); // delete all new lines, because the string length function counts it in the array length, which effects the loop below and reads an extra value.
    power=0;
    coefficient=1;
    flag=1;
    j=0;
    while (j<strlen(storeArray)){
        if(storeArray[j]=='+'){ // if the character is +
            insA(&poly[count],coefficient,power);
            power=0;
            flag=1; // flag to 1
           // printf("A");
        }
        else if(storeArray[j]=='-'){ // if the character is +
            if (j != 0){ // making sure its not at the start of the line, because if it is that means the number followed is negative.
                insA(&poly[count],coefficient,power);;
               // printf("S");
            }
            power=0;
                flag =-1; // flag to minus one.
        }
        else if(storeArray[j]=='x'){ // if the character is X
            power=1;
        //printf("X");
            if(!isdigit(storeArray[j-1])){
                coefficient=1;
            }
        }
        else { // else if its a number (the only thing left). does the following operation to evaluate the coefficient and power.
                //printf("N");
                if (power==0){
             if(isdigit(storeArray[j-1])){
                   // printf("C");
                coefficient=coefficient*10 + (storeArray[j]-'0')*flag; // the flag is -1 if the integer previous character is -.
             }
             else {
                 //printf("J");
                coefficient=(storeArray[j]-'0')*(flag);
             }
        }
        else if (isdigit(storeArray[j-1])){
                //printf("C");
             power=power*10 + (storeArray[j]-'0')*(flag);
             }
             else {
                 //printf("K");
                power=(storeArray[j]-'0');
             }
        }
        if (j==((strlen(storeArray))-1)){ // if the loop is coming to an end insert the last.
           insA(&poly[count],coefficient,power);
            //printf("W");
        }
        j++;

        }
count++;

    }
fclose(inside);
}
void delete1(char* listChar) { // the function that deletes the power sign "^".
    const char* temp = listChar;
    do {
        while (*temp == '^') {
            ++temp;
        }
    } while (*listChar++ = *temp++);
}
void deleteWhiteSpaces(char* listChar) { // function that deletes the white spaces in file.
    const char* temp = listChar;
    do {
        while (*temp == ' ') {
            ++temp;
        }
    } while (*listChar++ = *temp++);
}
void deleteNL(char* listChar) { // function that removes the new line when counting the string length inside a file.
     const char* temp = listChar;
    do {
        while (*temp == '\n') {
            ++temp;
        }
    } while (*listChar++ = *temp++);
}
