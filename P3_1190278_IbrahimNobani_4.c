#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define size 150
#define TABLE_SIZE 31
// Name: Ibrahim Nobani | ID: 1190278 | section:4
typedef struct AVLnode *AVLNode; // creating the tree struct
struct AVLnode { // adding the attributes needed for the patient info.
    char patientName[size];
    char gender[size];
    int admissionDate;
    int birthDate;
    char illness [size];
    char address[size];
    char bloodType[size];
    AVLNode Left; // left and right AVL nodes
    AVLNode Right;
    int Height; // height integer.

};
typedef struct hashT { // hash table struct along with the patient's attributes.
    char *patientName;
    char *gender;
    int admissionDate;
    int birthDate;
    char *illness ;
    char *address;
    char *bloodType;
    struct hashT *next;
} hashT;

typedef struct {
    hashT **patients; // represents the entry to the hash table, array of pointers to the entry.(entry= key and values).
} hashTT;
hashTT *ht; // our hash table
AVLNode tree; // the AVL tree.
// the flags are used to holden the menu, in order for the code to make more sense.
int rFlag=0,hrFlag=0, hFlag=0, dFlag=0, illFlag=0, tFlag=0, flag1=0; // read flag, hash flag, delete flag, illness flag, tree menu flag and hash flag
AVLNode MakeEmpty(AVLNode);
AVLNode Find(char [], AVLNode);
AVLNode FindIllness(char [], AVLNode);
AVLNode FindMax(AVLNode);
AVLNode FindMin(AVLNode);
AVLNode SingleRotateWithLeft(AVLNode);
AVLNode SingleRotateWithRight(AVLNode);
AVLNode DoubleRotateWithLeft(AVLNode);
AVLNode DoubleRotateWithRight(AVLNode);
AVLNode Delete(char [],AVLNode);
AVLNode Insert(char[],char [],int ,int ,char [],char [] ,char [],AVLNode);
int Max(int,int);
int Balance(AVLNode);
int Height(AVLNode);
int hash(const char*);
void PrintInOrder(AVLNode);
void PrintInOrderF(AVLNode,FILE*);
void readFromFileH(FILE *);
void readFromFile();
void TreeMenu(FILE *);
void hashMenu(FILE *);
void insertH(hashTT *, const char*, const char*, int ,int ,char*,char*,char*);
void findHash(hashTT *, char *);
void deleteHash(hashTT *,char *);
void printHash(hashTT *);
void printHashF(hashTT *,FILE*);
hashT *pairHT(const char*, const char*, int ,int ,char*,char*,char*);
hashTT *makeHash();
int main(){
    ht = makeHash();
    tree=MakeEmpty(tree);
    FILE* outcome;
    outcome =fopen("patients_hash.data","w"); // creating the file that is gonna connect the hash and the tree.
    TreeMenu(outcome); //calling the tree menu function.
    return 0;
}
AVLNode MakeEmpty( AVLNode T ){ // function that makes the tree empty.
    if( T != NULL ){
        MakeEmpty( T->Left ); // calling the same function using recursion on both its left and right side.
        MakeEmpty( T->Right );
        free( T );
    }
    return NULL;
}
AVLNode Find(char s[], AVLNode T ){ // function to find the desired patient based on its name.
    if( T == NULL ){
        return NULL;
    }
    if( strcasecmp(T->patientName,s) > 0 ) // if on the left side to a recursion.
        return Find( s, T->Left );
    else if( strcasecmp(T->patientName,s) < 0 ) // if on the right side.
        return Find( s, T->Right ); // recursion on right.
    else
        return T;
}
AVLNode FindIllness(char s[], AVLNode T ){ // a function that finds the illness, and prints all the patients info of that illness.
    if( T == NULL ){ // if the tree node is null returns NULL.
        return NULL;
    }
        FindIllness( s, T->Left ); // using recursion for the left side of the tree.
     if(strcasecmp(T->illness,s) == 0){ // if the illness is found, print.
         illFlag=1;
        printf("The searched for illness is listed below: \n");
        printf("Patient Name: %s | Gender:%s | Admission Date: %d | Birth Date: %d | Illness: %s |Address: %s | Blood Type: %s\n",T->patientName,T->gender,T->admissionDate,T->birthDate,T->address,T->illness,T->bloodType);
     }
        FindIllness( s, T->Right );
        return T;
}

AVLNode FindMin( AVLNode T )
{
    if( T == NULL )
        return NULL;
    else if( T->Left == NULL )
        return T;
    else
        return FindMin( T->Left );
}
AVLNode FindMax( AVLNode T )
{
    if( T != NULL )
        while( T->Right != NULL )
            T = T->Right;
    return T;
}
//simple funtion that return the height variable
int Height( AVLNode P )
{
    if( P == NULL )
        return -1;
    else
        return P->Height;
}
//used in rotation functions
int Max( int Lhs, int Rhs )
{
    return Lhs > Rhs ? Lhs : Rhs;
}
int Balance(AVLNode t) { // function that finds the balance, balance = height of the left child - height of the right one.
    if (t == NULL)
        return 0;
    return Height(t->Left) - Height(t->Right);
}
AVLNode SingleRotateWithLeft(AVLNode K2) // a function that gets called when k2 has a left child, a rotate is preformed between those two nodes and their heights get updated.
{   // when this shape is acquired, this function is called to fix this imbalance.
    /*.........(node1).....  node1=K2 were the imbalance occurred
     .....(node2).........
    .(node3).............*/
    AVLNode K1; // create a new node.
    K1 = K2->Left; // K2->Left is node2, k1 is pointing at node2 for now.
    K2->Left = K1->Right; // now K2 left is set to be K1 right.
    K1->Right = K2; // right of k1 (node2), will point at node1, the figure becomes this:
     /*........(node2)...........
    ...(node3).......(node1)......*/
    //now both node1 and node2 height are updated, while node3 is the newly added node and will be updated in the insert function
    K2->Height = Max( Height( K2->Left ), Height( K2->Right ) ) + 1;
    K1->Height = Max( Height( K1->Left ), K2->Height ) + 1;
    return K1;
}
AVLNode SingleRotateWithRight( AVLNode K1 ) // a function that gets called when k2 has a right child, a rotate is preformed between those two nodes and their heights get updated.
{   // when this shape is acquired, this function is called to fix this imbalance.
    /*..(node1).........   node1=K1 were the imbalance occurred
     .......(node2).......
    ...........(node3)....*/
    AVLNode K2; // new node called K2
    K2 = K1->Right; // K1->Right is node2, k2 is pointing at node2 for now.
    K1->Right = K2->Left; //node 2 original position will become null,where k2->left points to null
    K2->Left = K1; // left of k2 (node2), will point at node1, the figure becomes this:
     /*........(node2)...........
    ...(node1).......(node3)......*/
    //now both node1 and node2 height are updated, while node3 is the newly added node and will be updated in the insert function
    K1->Height = Max( Height( K1->Left ), Height( K1->Right ) ) + 1;
    K2->Height = Max( Height( K2->Right ), K1->Height ) + 1;

    return K2;
}

AVLNode DoubleRotateWithLeft( AVLNode K3 )//LR-double rotation, when k3 has a left child, and this left child has a right child, a left- right rotation is preformed.
{    // initial shape.
      /*......(node1).......
    ....(node2)...........
    ........(node3)......*/
    // rotate between node2 and node3
    K3->Left = SingleRotateWithRight( K3->Left );
        //the new shape will be
    /*.........(node1).....
     .....(node2).........
    .(node3).............*/
    return SingleRotateWithLeft( K3 );
}

AVLNode DoubleRotateWithRight( AVLNode K1 )//RL-double rotation, when k1 has a right child, and this right child has a right child, a left- right rotation is preformed
{
    /*..(node1).......
    ..........(node2)..
    ....(node3)......*/
    // Rotate between node2 and node3
    K1->Right = SingleRotateWithLeft( K1->Right );
    /*..(node1).......
    ..........(node3)..
    ................(node2)*/
    return SingleRotateWithRight( K1 );
}
AVLNode Insert(char name[],char gender[],int aDate,int bDate,char ill[],char address[] ,char blood[],AVLNode T ){
    if( T == NULL ){
        T = malloc( sizeof( struct AVLnode ) );
        if( T == NULL )
            printf( "Out of space!!!" );
        else{
            // copying the entires into the tree
            strcpy(T->patientName,name);
            strcpy(T->gender,gender);
            strcpy(T->illness,ill);
            strcpy(T->address,address);
            strcpy(T->bloodType,blood);
            T->admissionDate=aDate;
            T->birthDate=bDate;
            T->Height = 0; // setting the node's initial height
            T->Left = T->Right = NULL; // right and left nodes are null initially as well.
        }
    }
    else if( strcasecmp(T->patientName,name) > 0 ) // patient on the left node based on its ASCII, the below lines are implemented which they intend to add it to the left branch.
    {
        //the first step will be the recursive call, so that the first node to the left will act as the root
        //this process is important because we might have multiple tree nodes in the left side

        T->Left = Insert(name,gender,aDate,bDate,ill,address,blood,T->Left);
        //this part will be entered only when the multiple recursive calls is finished and we have an issue with the balance after adding the new node
        //note the AVL trees have special condition called balance, it ensures the complexity of O(logN)
        //the balance states that the hight of the left and right subtrees can differ by 1,0,-1 only
        if( Balance(T) == 2 ) // this checks if we have an imbalance, and the balance is set to two, the AVL tree can only accept
            if(strcasecmp(T->Left->patientName,name)>0) // now we need to figure the shape of this imbalance.
                //if the imbalance is line shaped we only need a single rotate to left
                T = SingleRotateWithLeft( T );
            else //if the it had a zig-zag shape we will do a double rotate
                T = DoubleRotateWithLeft( T );
    }
    //a condition to put the user in the right node
    else if( strcasecmp(T->patientName,name) < 0  )
    { //comparison process then the recursive insertion process
        T->Right = Insert(name,gender,aDate,bDate,ill,address,blood,T->Right);
        if( Balance(T) == -2 )
            if(strcasecmp(T->Right->patientName,name) < 0 )
                T = SingleRotateWithRight( T ); //if the imbalance is line shaped we only need a single rotate to right
            else //if the it had a zig-zag shape we will do a double rotate
                T = DoubleRotateWithRight( T );
    }
     // else if the key already exist, nothing is done.
    T->Height = Max( Height( T->Left ), Height( T->Right ) ) + 1;//The tree variable gets updated for the new node
    //for each recursive call, this process is done.
    return T;
}
void PrintInOrder(AVLNode t){ // print function
    if( t != NULL){ // making sure its not null.
        PrintInOrder( t->Left ); // recursion on the left side.
        printf("Patient Name: %s | Gender:%s | Admission Date: %d | Birth Date: %d | Illness: %s |Address: %s | Blood Type: %s\n",t->patientName,t->gender,t->admissionDate,t->birthDate,t->illness,t->address,t->bloodType);
        PrintInOrder( t->Right ); // recursion on the right side.
    }
}
void PrintInOrderF(AVLNode t, FILE *outcome){ // function to print in file.
    if( t != NULL){
        PrintInOrderF(t->Left,outcome );
        fprintf(outcome,"%s#%s#%d#%d#%s#%s#%s\n",t->patientName,t->gender,t->admissionDate,t->birthDate,t->illness,t->address,t->bloodType);
        PrintInOrderF(t->Right,outcome );
    }
}
AVLNode Delete(char s[],AVLNode t){
    AVLNode temp;
    if(t==NULL){ // if the patient is not found.
        dFlag=-1; // delete flag which will be used in the menu
        printf("***The user you wish to delete is no where to be found!***\n\n");
    }
    else if( strcasecmp(t->patientName,s) > 0 ) // if value is less go left side of the tree
        t->Left=Delete(s,t->Left); // call a recursive for the left side if its less
    else if( strcasecmp(t->patientName,s) < 0 ) // if value is more go right side of the tree
        t->Right=Delete(s,t->Right); // call a recursive for the right side if its more
    else {
        if(t->Left != NULL && t->Right != NULL){ // if the node has two children (right and left).
                temp=FindMin(t->Right); // find lowest key node in the right side
                //copy the content of MIN->ToRight in the place of the found node
                strcpy(t->patientName,temp->patientName);
                strcpy(t->gender,temp->gender);
                strcpy(t->illness,temp->illness);
                strcpy(t->address,temp->address);
                strcpy(t->bloodType,temp->bloodType);
                t->admissionDate=temp->admissionDate;
                t->birthDate=temp->birthDate;
                t->Right=Delete(t->patientName,t->Right);
        }
        else { // if it has one child or none.
             temp=t;
             if(t->Left==NULL) // if the node has only a right side, this right side will take it place.
                t=t->Right;
             else if(t->Right==NULL) // if the node has only a left side, this left side will take it place.
                t=t->Left;
             free(temp); // free the node.
        }
    }
    if(t==NULL) // after deleting the node of a one root tree
        return t;
    else if( Balance(t) >= 2 ){ // now we adjust (balance) the tree, if H->left-H->right>2
            if( strcasecmp(t->Left->patientName,s)>0 ){ // check the shape for the desired rotation.
                t = SingleRotateWithLeft( t ); //if the imbalance is line shaped we only need a single rotate to left
            }
            else{ //if the it had a zig-zag shape we will do a double rotate
                t = DoubleRotateWithLeft( t );
            }
        }
    else if( Balance(t) <= -2 ){
            if( strcasecmp(t->Left->patientName,s)<0 ){ // check the shape
                t = SingleRotateWithRight( t ); //if the imbalance is line shaped we only need a single rotate to right
            }
            else{ //if the it had a zig-zag shape we will do a double rotate
                t = DoubleRotateWithRight( t );
            }
    }

    return t;
}
void readFromFile(){ // read the tree attributes from file function.
FILE *in; //creating a file and then opening it.
    in=fopen("patients.txt","r");
    if(in == NULL){//if file not found
        printf("This file ceases to exist, try again. \n");
        rFlag=-1; // read flag set to -1, to use in the menu.
             return;
             }
    char s[200],s1[50][size]; // create an arrays to store values in.
    int x,i=0;
    x=fscanf(in,"%[^\n]%*c",s); // scanning and cutting
        while (x != EOF){
            //using strtok to cut s
         char* token = strtok(s,"#");
         while (token != NULL) {
           strcpy(s1[i],(token)); //i will represent index of s1
           token = strtok(NULL, "#");
           i++;
           }
           //we will use integer var for category to ease usage
           tree = Insert(s1[0],s1[1],atoi(s1[2]),atoi(s1[3]),s1[4],s1[5],s1[6],tree); // inserting inside of the tree.
           x=fscanf(in,"%[^\n]%*c",s);
           i=0;
    }
}
void TreeMenu(FILE *outcome){ // a function for tree menu, i made this as a function to call it more than once, and to move between hash an tree menu easier.
int choice;
do { // statements for the menu
    printf("AVL Tree Options Menu:\n");
    printf("1- Read the patients information from file and store them in an AVL tree.\n");
    printf("2- Print the information read from file.\n");
    printf("3- Enter another patient's information.\n");
    printf("4- Search/find a specific patient to check the patient's data or to update it.\n");
    printf("5- Delete a specific patient.\n");
    printf("6- List all patients with the same illness\n");
    printf("7- Save in file.\n");
    printf("8- Move to the hash Table.\n");
    printf("*- Press any other key to exit the Table.****\n");
    printf("\n**Choose one of the previous options based on number**:\t");
    //choice= choice1 -'0'; // turning that string into an integer.
    int intCheck1 =scanf("%d",&choice);
                if(intCheck1 <1){ //if the entered value is not an integer, set choice to 0 which means break out.
                char dummy;
                scanf("%c",&dummy);
                choice=0;
                }
                printf("\n\n");
    if (choice ==1){
        if (rFlag==-1){ // if read flag is -1, means that there is no file.
           printf("\nThe file has not been read, put the information in file to read correctly!\n\n");
        }
            else {
                    tFlag=1; // tree flag set to 1, this will be used in the next choices, if the tFlag is not 1, they cannot be run as there is nothing in the Tree to work with.
                    readFromFile();
                    printf("\n----------The patients data have been successfully read!!----------\n\n");
            }
        }
    else if (choice ==2){ // printing if choice is 2.
        if (tFlag==1){ // making sure that there is data in the tree.
            printf("\n");
            PrintInOrder(tree);
            printf("\n");
        }
        else printf("\nThe file has not been read, put the information in file or press number 3 to insert in order to read correctly!\n\n");
    }
    else if (choice==3){ // insert in the tree.
            char pn[size],g[size],ill[size],ad[size],bt[size]; // create char arrays and integers to take the values
            int adD,bd;
            printf("\nPlease enter the data as ordered below: \n");
            printf("Insert the patient's name: ");
            scanf(" %[^\n]%*c",pn);
            printf("\nInsert the patient's gender (m for male, f for female): ");
            scanf(" %[^\n]%*c",g);
            // check if the gender entry is correct.
            while (strcasecmp(g,"m")!=0 && strcasecmp(g,"f")!=0 && strcasecmp(g,"male")!=0 && strcasecmp(g,"female")!=0){
                printf("ERROR, Please enter a valid gender (male(M) or female(F)): ");
                scanf(" %[^\n]%*c",g);
            }
            printf("\nInsert the admission date of the patient: ");
            int intCheck =scanf("%d",&adD);
            while(intCheck <1){ //check if the entered value is an integer, if not re-enter again.
                printf("Error, this entry has to be an integer!!");
                printf("\nPlease enter a valid admission date for the patient: ");
                char dummy;
                scanf("%c",&dummy);
                intCheck =scanf("%d",&adD);
            }
            printf("\nInsert the birth date of the patient: ");
            int intCheck1 =scanf("%d",&bd);
            while(intCheck1 <1){ //check if the entered value is an integer, if not re-enter again.
                printf("Error, this entry has to be an integer!!");
                printf("\nPlease enter a valid birth date for the patient: ");
                char dummy;
                scanf("%c",&dummy);
                intCheck1 =scanf("%d",&bd);
            }
            printf("\nInsert the patient's illness or disease: ");
            scanf(" %[^\n]%*c",ill);
            printf("\nInsert the patient's address: ");
            scanf(" %[^\n]%*c",ad);
            printf("\nInsert the patient's blood type: ");
            scanf(" %[^\n]%*c",bt);
            tree=Insert(pn,g,adD,bd,ill,ad,bt,tree); // insert the values.
            tFlag=1;
    }
    if (choice ==4){ // search for a patient and give the choice to update him.
            if (tFlag==1){
        int sFlag=0;
        AVLNode t=NULL;
        t = MakeEmpty(t);
        char name[size];
        printf("\nEnter the name of the patient you wish to find: ");
        scanf(" %[^\n]%*c",name);
        t=Find(name,tree); // a tree that has the found patient.
        if(t==NULL) // if the patient is not found.
               printf("The Patient you looked for is no where to be found.\n");
            else{
                sFlag=1; // search flag set to 1 if the patient is actually found.
                printf("\nThe searched for patient: \n");
                printf("Patient Name: %s | Gender:%s | Admission Date: %d | Birth Date: %d | Illness: %s |Address: %s | Blood Type: %s\n",t->patientName,t->gender,t->admissionDate,t->birthDate,t->illness,t->address,t->bloodType);
        }
        if (sFlag==1){ // if the search flag is 1, give the option to update.
                char ch;
                printf("\nDo you wish to update the found user? (y for yes): ");
                scanf("%s",&ch);
            if (ch=='y'){
                printf("\n----Enter the new information for the patient you are updating:---- \n");
                tree=Delete(name,tree); // delete the user and then let the user insert it attributes again.
                char pn[size],g[size],ill[size],ad[size],bt[size];
                int adD,bd;
                printf("\nPlease enter the data as ordered below: \n");
                printf("Insert the patient's name: ");
                scanf(" %[^\n]%*c",pn);
                printf("\nInsert the patient's gender (m for male, f for female): ");
                scanf(" %[^\n]%*c",g);
                // check if the gender entry is correct.
                while (strcasecmp(g,"m")!=0 && strcasecmp(g,"f")!=0 && strcasecmp(g,"male")!=0 && strcasecmp(g,"female")!=0){
                printf("ERROR, Please enter a valid gender (male(M) or female(F)): ");
                scanf(" %[^\n]%*c",g);
                }
                printf("\nInsert the admission date of the patient: ");
                int intCheck =scanf("%d",&adD);
                while(intCheck <1){ //check if the entered value is an integer, if not re-enter again.
                printf("Error, this entry has to be an integer!!");
                printf("\nPlease enter a valid admission date for the patient: ");
                char dummy;
                scanf("%c",&dummy);
                intCheck =scanf("%d",&adD);
                }
                printf("\nInsert the birth date of the patient: ");
                int intCheck1 =scanf("%d",&bd);
                while(intCheck1 <1){ //check if the entered value is an integer, if not re-enter again.
                printf("Error, this entry has to be an integer!!");
                printf("\nPlease enter a valid birth date for the patient: ");
                char dummy;
                scanf("%c",&dummy);
                intCheck1 =scanf("%d",&bd);
                }
                printf("\nInsert the patient's illness or disease: ");
                scanf(" %[^\n]%*c",ill);
                printf("\nInsert the patient's address: ");
                scanf(" %[^\n]%*c",ad);
                printf("\nInsert the patient's blood type: ");
                scanf(" %[^\n]%*c",bt);
                tree=Insert(pn,g,adD,bd,ill,ad,bt,tree);
            }
        }
        else printf("Cannot update because the user you entered was not found.\n\n"); // if search flag is not 1, cannot update the user.
    }
     else printf("\nThe file has not been read, put the information in file or press number 3 to insert in order to read correctly!\n\n");
    }
    else if (choice == 5){
        if (tFlag==1){
            char name[size];
            printf("\nEnter the name of the patient you wish to delete: ");
            scanf(" %[^\n]%*c",name);
            tree=Delete(name,tree);
            if (dFlag==0){ // display a message that the user is deleted if the delete Flag is zero.
                printf("The user '%s' has been removed!\n\n",name);
            }
            dFlag=0; // delete flag set to zero again.
        }
        else printf("\nNothing to delete, please fill the tree from file (press 1) or press 3 to insert.\n\n");
    }
    else if (choice==6){
            if (tFlag==1){ // function to list by illness.
        illFlag=0;
        char illName[size];
        printf("\nEnter the illness or disease name: ");
        scanf(" %[^\n]%*c",illName);
        FindIllness(illName,tree);
        if (illFlag==0){ // illness flag is zero if the disease is not found.
            printf("\nThe disease you are looking for is not registered in our system\n\n");
        }
    }
    else printf("\nThe file has not been read, put the information in file or press number 3 to insert in order to read correctly!\n\n");
}
     else if(choice==7){
                if (tFlag==1){
                 hFlag=1; // hash flag set to 1, the hash menu cannot run if this choice is not called and the data is not stored in a file.
                PrintInOrderF(tree,outcome);
                fclose(outcome);
                printf("\nCheck the results in the patient_hash.data file\n\n");
                char ch;
                printf("\nDo you wish to proceed to the hash table? (y for yes): ");
                scanf("%s",&ch);
            if (ch=='y'){ // give the option to move into the hash menu after saving in file.
                    hashMenu(outcome);
            }
    }
    else printf("NOTHING TO SAVE IN FILE, Read from the file (press 1) or insert from console by pressing 3\n\n");
    }
    else if (choice==8){ // proceed to the hash menu
        hashMenu(outcome);
    }
} while (choice >=1 && choice <9);
}
void hashMenu(FILE *outcome){
    int choice;
if (hFlag==1){ // if the data is stored in file.
    do {
        printf("\n\n********Hash Table options menu:\n");
        printf("1- Read hash table from file.\n");
        printf("2- Print the hash table information.\n");
        printf("3- Print the hash table size.\n");
        printf("4- Print out the function and the method used.\n");
        printf("5- Insert a new patient to the hash table.\n");
        printf("6- Search for a specific patient in the hash Table.\n");
        printf("7- Delete a specific patient from the hash Table.\n");
        printf("8- Save all progress and patients in file.\n");
        printf("9- Go back to the AVL tree menu.\n");
        printf("*- Press any other key to exit the Table.****\n");
        printf("\n**Choose one of the previous options based on number**:\t");
        int intCheck1 =scanf("%d",&choice);
                if(intCheck1 <1){ //if the entered value is not an integer, set choice to 0 which means break out.
                char dummy;
                scanf("%c",&dummy);
                choice=0;
                }
                printf("\n\n");
        if (choice==1){
                 if (rFlag==-1){ // if read flag is -1, means that there is no file.
                    printf("\nThe file has not been read, put the information in file to read correctly!\n\n");
        }
                else {
                    flag1=1; // flag that makes sure the values are read and the hash has data.
                    readFromFileH(outcome);
                    printf("\n----------The patients data have been successfully read!!----------\n\n");
        }
        }
        else if(choice==2){
                if (flag1==1){ // print hash if data is entered
            printHash(ht);
                }
                else printf("\nThe file has not been read, put the information in file or press number 5 to insert in order to read correctly!\n\n");
        }
        else if (choice==3){ // display table size.
            printf("The hash table size is : %d\n",TABLE_SIZE);
        }
        else if(choice==4){ // the function and the method used.
            printf("In this hash Table, collision was solved using the open addressing method, Quadratic Hashing.\n");
            printf("The function used is: \n H(x) = (function + i^2) % Table_Size\n");
            printf("Where the function here is hash(key=(patientName))");
        }
        else if(choice==5){ // creating char arrays and variables to take from user and insert in the hash.
            char pn[size],g[size],ill[size],ad[size],bt[size];
            int adD,bd;
            printf("\nPlease enter the data as ordered below: \n");
            printf("Insert the patient's name: ");
            scanf(" %[^\n]%*c",pn);
            printf("\nInsert the patient's gender (m for male, f for female): ");
            scanf(" %[^\n]%*c",g);
            // check if the gender entry is correct.
            while (strcasecmp(g,"m")!=0 && strcasecmp(g,"f")!=0 && strcasecmp(g,"male")!=0 && strcasecmp(g,"female")!=0){
                printf("ERROR, Please enter a valid gender (male(M) or female(F)): ");
                scanf(" %[^\n]%*c",g);
            }
            printf("\nInsert the admission date of the patient: ");
            int intCheck =scanf("%d",&adD);
                while(intCheck <1){ //check if the entered value is an integer, if not re-enter again.
                printf("Error, this entry has to be an integer!!");
                printf("\nPlease enter a valid admission date for the patient: ");
                char dummy;
                scanf("%c",&dummy);
                intCheck =scanf("%d",&adD);
                }
            printf("\nInsert the birth date of the patient: ");
            int intCheck1 =scanf("%d",&bd);
                while(intCheck1 <1){ //check if the entered value is an integer, if not re-enter again.
                printf("Error, this entry has to be an integer!!");
                printf("\nPlease enter a valid birth date for the patient: ");
                char dummy;
                scanf("%c",&dummy);
                intCheck1 =scanf("%d",&bd);
                }
            printf("\nInsert the patient's illness or disease: ");
            scanf(" %[^\n]%*c",ill);
            printf("\nInsert the patient's address: ");
            scanf(" %[^\n]%*c",ad);
            printf("\nInsert the patient's blood type: ");
            scanf(" %[^\n]%*c",bt);
            insertH(ht,pn,g,adD,bd,ill,ad,bt); // insert in hash after taking the values.
            flag1=1; // flag set to 1.
        }
        else if (choice==6){
                if (flag1==1){ // a patient search function.
            char name[size];
            printf("Enter the name of the patient you wish to find: ");
            scanf(" %[^\n]%*c",name);
            findHash(ht,name);
        }
        else printf("\nThe file has not been read, put the information in file or press number 5 to insert in order to read correctly!\n\n");
        }
        else if(choice==7){  // deleting a user from the hash.
                if (flag1==1){
            char name[size];
            printf("Enter the name of the patient you wish to delete: ");
            scanf(" %[^\n]%*c",name);
            deleteHash(ht,name);
        }
        else printf("\nThe file has not been read, put the information in file or press number 5 to insert in order to read correctly!\n\n");
        }
        else if(choice==8){
                if (flag1==1){
            printf("Check the output.txt file\n");
            FILE* out;
              out =fopen("output.txt","w");
              printHashF(ht,out);
              fclose(out);
                }
                else printf("Please insert some entires to the hash table to put them in file, press 1 or 5 \n");
        }
        else if (choice==9){ // go back to the tree menu.
            TreeMenu(outcome);
        }
    } while (choice >=1 && choice <10);


}
else { // if the data is not stored in the file patient_hash.data, go back to the AVL menu and save it.
    printf("\n****\nYou should save all tree in file, we are getting you back to the AVL tree screen\n\n****\n");
    TreeMenu(outcome);
}
}
int hash(const char *key) { // string hash function for the key.
    int hashValue=0;
 while( *key != '\0' ){
hashValue += *key++;
 }
return (hashValue % TABLE_SIZE);
}
hashTT *makeHash() {
    // allocate table
    hashTT *hashtable = malloc(sizeof(hashTT) * 1);
    // allocate table entries
    hashtable->patients = malloc(sizeof(hashT*) * TABLE_SIZE); // make size for every pointer entry.
    // set each to null (needed for proper operation)
    for (int i = 0; i < TABLE_SIZE; ++i) {
        hashtable->patients[i] = NULL;
    }
    return hashtable;
}
hashT *pairHT(const char *patientName, const char *gender, int admissionDated,int bDate,char* ill,char* address ,char* blood) {
    // allocate the H
    hashT *T = malloc(sizeof(hashT));
    T->patientName = malloc(strlen(patientName));
    T->gender = malloc(strlen(gender));
    T->illness = malloc(strlen(ill));
    T->address = malloc(strlen(address));
    T->bloodType = malloc(strlen(blood));
    // copy the patientName and gender in place
    strcpy(T->patientName, patientName);
    strcpy(T->gender, gender);
    strcpy(T->illness,ill);
    strcpy(T->address,address);
    strcpy(T->bloodType,blood);
    T->birthDate=bDate;
    T->admissionDate=admissionDated;
    // next starts out null but may be set later on
    T->next = NULL;
    return T;
}
void insertH(hashTT *hashtable, const char *patientName, const char *gender, int admissionDate,int bDate,char* ill,char* address ,char* blood) {
   int j=0;
   int key=hash(patientName);
   //printf("Key:%d\n",key); // i used this print to track what the key was, and the J print in the loop to what it become if a collision occurred.
    // try to look up an H set
    int i;
    for (i=0; i<TABLE_SIZE; i++){
             j=(key+i*i)%TABLE_SIZE; // using the quadratic function to add the data.
             //printf("j:%d\n",j);
    if (hashtable->patients[j]==NULL){
        hashtable->patients[j] = pairHT(patientName, gender, admissionDate,bDate,ill,address,blood);
        return;
    }
    else if (strcmp(hashtable->patients[j]->patientName, patientName) == 0){ // check if the key already exists
    break;
    }
    }
    if(i==TABLE_SIZE){ // if i reached the table size, it means the element has no place.
        printf("\nElement has no place\n\n");
    }
}
void printHash(hashTT *hashtable) { // hash print function.
    for (int i = 0; i < TABLE_SIZE; ++i) { // a loop that goes through the hash and prints it all
        hashT *H = hashtable->patients[i];
        if (H == NULL) {
                printf("Slot[%d]: --\n",i); // printing empty buckets
            continue;
        }
        printf("Slot[%d]: ", i);
        for(int i = 0; i < TABLE_SIZE; ++i) {
            printf("Patient Name: %s | Gender:%s | Admission Date: %d | Birth Date: %d | Illness: %s |Address: %s | Blood Type: %s", H->patientName, H->gender,H->admissionDate,H->birthDate,H->illness,H->address,H->bloodType);
            if (H->next == NULL) {
                break;
            }
            H = H->next;
        }
        printf("\n");
    }
}
void printHashF(hashTT *hashtable, FILE* out) { // print hash in file function.
    for (int i = 0; i < TABLE_SIZE; ++i) { // a loop that goes through the hash and prints it all
        hashT *H = hashtable->patients[i];
        if (H == NULL) {
                fprintf(out,"Slot[%d]: --\n",i); // printing empty buckets
            continue;
        }
        fprintf(out,"Slot[%d]: ", i);
        for(int i = 0; i < TABLE_SIZE; ++i) {
            fprintf(out,"Patient Name: %s | Gender:%s | Admission Date: %d | Birth Date: %d | Illness: %s |Address: %s | Blood Type: %s", H->patientName, H->gender,H->admissionDate,H->birthDate,H->illness,H->address,H->bloodType);
            if (H->next == NULL) {
                break;
            }
            H = H->next;
        }
        fprintf(out,"\n");
    }
    if(out==NULL){
    printf("This File is Empty. \n");
    fprintf(out,"This File is Empty. \n");
}
}
void findHash(hashTT *hashtable,char *patientName) {
     int j = hash(patientName); // j is the key hash value
    hashT *H = hashtable->patients[j];
    // no j means no H
    if (H == NULL) {
            printf("\n The User does not exist\n");
        return;
    }
    // walk through each H in the j, which could just be a single thing
    while (H != NULL) {
        // return if found
        if (strcmp(H->patientName, patientName) == 0) {
        printf("\nThe Searched for patient is: \n");
        printf("Patient Name: %s | Gender:%s | Admission Date: %d | Birth Date: %d | Illness: %s |Address: %s | Blood Type: %s\n", H->patientName, H->gender,H->admissionDate,H->birthDate,H->illness,H->address,H->bloodType);
            return;
        }
        // proceed to next patientName if available
        H = H->next;
    }
    // reaching here means there were >= 1 entries but no patientName match
    printf("\nMatch Not found, Make sure the name you entered is correct and try again.\n");
    return;
}
void deleteHash(hashTT *hashtable, char *patientName) {
     int j = hash(patientName); // j is the key hash
    hashT *H = hashtable->patients[j]; // new hash
    // no j means no H
    if (H == NULL) {
            printf("\nThe User does not exist.\n");
        return;
    }
    hashT *prev;
    int flag = 0; // flag here counts till the last entry
    // walk through each H until either the end is reached or a matching patientName is found
    while (H != NULL) {
        // check patientName
        if (strcasecmp(H->patientName, patientName) == 0) {
            // first item and no next H
            if (H->next == NULL && flag == 0) {
                hashtable->patients[j] = NULL;
            }
            // first item with a next H
            if (H->next != NULL && flag == 0) {
                hashtable->patients[j] = H->next;
            }
            // last item
            if (H->next == NULL && flag != 0) {
                prev->next = NULL;
            }
            if (H->next != NULL && flag != 0) {
                prev->next = H->next;
            }
            // free the deleted H
            free(H->patientName);
            free(H->gender);
            free(H->admissionDate);
            free(H->birthDate);
            free(H->illness);
            free(H->address);
            free(H->bloodType);
            free(H);
            printf("\n---Patient '%s' has been successfully deleted---\n",patientName);
            return;
        }
        prev = H;
        H = prev->next;
        ++flag;
    }
    //return;
}
void readFromFileH(FILE *in){
    in =fopen("patients_hash.data","r"); // re-open the hash data file to read from.
    if(in == NULL){//if file not found
        printf("This file ceases to exist, try again. \n");
        hrFlag=-1; // hash read flag used in the menu
             return;
             }
    char s[200],s1[50][150];
    int x,i=0;
    x=fscanf(in,"%[^\n]%*c",s);
        while (x != EOF){
            //using strtok to cut s
         char* token = strtok(s,"#");
         while (token != NULL) {
           strcpy(s1[i],(token)); //i will represent index of s1
           token = strtok(NULL, "#");
           i++;
           }
           //we will use integer var for category to ease usage
           insertH(ht,s1[0],s1[1],atoi(s1[2]),atoi(s1[3]),s1[4],s1[5],s1[6]);
           x=fscanf(in,"%[^\n]%*c",s);
           i=0;
    }
}

