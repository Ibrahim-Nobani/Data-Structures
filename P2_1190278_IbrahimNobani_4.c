#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maximum 100
// Name: Ibrahim Nobani | ID: 1190278 | section:4
struct Stack* createStack();
void push(struct Stack*, char[]);
void stackPrinting();
void popLost(struct Stack* , struct Stack*);
void saveStackPrinting();
int isEmpty(struct Stack*);
void operationCommands();
struct Stack{ // creating the Stack.
    char line[maximum];
    struct Stack* next;
};
struct Stack* S1; // the 2 used stacks are set to global.
struct Stack* S2;
int flag=0; // flag for the print messages in the undo/redo to enhance the code.
int main()
{
   S1 = createStack(); // creating both stacks.
   S2 = createStack();
   operationCommands();
    return 0;
}
struct Stack* createStack(){ // create stack function.
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
    if(stack == NULL){
        printf("Out of memory!\n");
        return NULL;
    }
    stack -> next = NULL;
    return stack;
}

void push(struct Stack* take, char s[maximum]){ // push function used.
    struct Stack* reload = (struct Stack*) malloc(sizeof(struct Stack));
    if(reload == NULL){
        printf("Out of memory!\n");
        return;
    }
    strcpy(reload->line, s);
    reload->next=take->next;
    take->next=reload;
}

int isEmpty(struct Stack* stack){ // a function to check if a stack is empty.
    return stack->next == NULL;

}
void operationCommands(){
    printf("MyCommand > "); // asks what the command you wish to enter.
    char choice[maximum*maximum];
    gets(choice); // get the command.
   // printf("%s\n",choice);
    if (strlen(choice)>100){ // choice sized at 100.
        printf("Poor choice of words, Length not accepted. \n");
        operationCommands(); // recalling the function again so the user can enter a valid choice.
    }
    if(strcmp(choice,"print")==0){ // if choice is print, call the print function.
            printf("Results stored >\n");
        stackPrinting();
        //printf("D");
    }
    else if(strcmp(choice,"undo")==0){ // if choice is undo, call the pop function.
             if (flag!=-1){
             printf("The last statement has been deleted. \n");
        }
        popLost(S1,S2);
    }
    else if(strcmp(choice,"redo")==0){ // if choice is redo, call the print function, with it parameters flipped.
            if(flag!=-1){
            printf("The last deleted statement has been restored.\n");
        }
        popLost(S2,S1);
    }
    else if(strcmp(choice,"save")==0){ // if choice is save, call the save function to save in file.
        saveStackPrinting();
        printf("Check the output file\n");
    }
    else if(strcmp(choice,"quit")==0){ // if choice is quit call the quit to exit.
        saveStackPrinting();
        printf("See you later! \n");
        exit(0);
    }
    else {
        push(S1,choice); // if a statement is entered, push it to the first stack.
    }
    operationCommands(); // call the operation function again till the user quits.
 }
 void stackPrinting(){
 if(isEmpty(S1)){ // check if anything is entered in the stack.
    printf("You have not entered any values, or you have undone all of them, please enter to proceed.\n");
    return;
 }
 struct Stack* temp; // two temporary stacks.
 struct Stack* temp2 = createStack();
 temp=S1; // first temp takes the first stack.
 while (temp->next!=NULL){ // checks that the next is not null.
        temp=temp->next; // if not, pushes the line to the second temp.
    push(temp2,temp->line);
 }
 temp=temp2; // after pushing we print the results.
 while (temp->next!=NULL){
    temp=temp->next;
    printf("%s\n",temp->line);
 }

}
void saveStackPrinting(){ // same as the above function, the difference is that it saves all in file,
    FILE *result;
    result=fopen("output.txt","w");
if(isEmpty(S1)){
    fprintf(result,"Stack is empty\n");
    return;
 }
 struct Stack* temp;
 struct Stack*temp2 = createStack();
 temp=S1;
 while (temp->next!=NULL){
        temp=temp->next;
    push(temp2,temp->line);
 }
 temp=temp2;
 while (temp->next!=NULL){
    temp=temp->next;
    fprintf(result,"%s\n",temp->line);
 }
 fclose(result);
 }
 void popLost (struct Stack* s1, struct Stack* s2){ // a pop function in charge of the redo / undo commands.
 if(isEmpty(s1)){ // checks if empty.
    printf("Nothing to undo or redo, try inserting a few stuff!\n");
    flag=-1;
    return;
 }
struct Stack* temp; // a temp stack.
 temp=s1->next; // temp is equal the next value of the first stack.
 push(s2,s1->next->line); // pushes the second value to the second stack.
 s1->next=s1->next->next; // continues
 free(temp); // frees the stack.
}
