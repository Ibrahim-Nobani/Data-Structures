#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#define size 150
#define INF 9999
//Name: Ibrahim Nobani || ID:1190278 || Section:4
int VN=0;
int rFlag=0; // read flag to make sure the file is read
char name[1000][1000][100]; // name graph to store in the towns names.
void readAndflag1Vertices()  //function that reads the first 2 vertex and gets the vertix number.
{
    FILE *in; //creating a file and then opening it.
    in=fopen("segments.txt","r");
    if(in == NULL) //if file not found
    {
        printf("This file ceases to exist, try again. \n");
        rFlag=-1; // read flag set to -1, to use in the menu.
        return;
    }
    char s[200],s1[50][size]; // create an arrays to store values in.
    int x,i=0;
    x=fscanf(in,"%[^\n]%*c",s); // scanning and cutting
    while (x != EOF) // reading till its end
    {
        char* token = strtok(s," ");
        for ( i=0; i<2; i++) // getting the first 2 vertex
        {
            strcpy(s1[i],(token));
            if (i!=1)
            {
                token = strtok(NULL, " ");
            }
        }
        VN=max(max(atoi(s1[1]),atoi(s1[0])),VN); // comparing them to find the bigger.
        x=fscanf(in,"%[^\n]%*c",s);
        i=0;
    }
}
void deleteWhiteSpaces(char* listChar)   // function that takes the list and deletes the white spaces in file.
{
    const char* temp = listChar;
    do
    {
        while (*temp == ' ')
        {
            ++temp;
        }
    }
    while (*listChar++ = *temp++);
}
char *strremove(char *string1, const char *string2) // function that deletes a substring from a string, used in file reading.
{
    char *temp, *temp2, *ch;
    if ((temp2 = ch = strstr(string1, string2)) != NULL)
    {
        size_t len = strlen(string2);
        while ((ch = strstr(temp = ch + len, string2)) != NULL)
        {
            while (temp < ch)
                *temp2++ = *temp++;
        }
        while ((*temp2++ = *temp++) != '\0')
            continue;
    }
    return string1;
}
void init(int distenation, int source,FILE *outcome) // function that takes destination, source and file. Because Dijkstra
{
    // algorithm is used inside.

    FILE *in; //creating a file and then opening it.
    in=fopen("segments.txt","r");
    if(in == NULL) //if file not found
    {
        printf("This file ceases to exist, try again. \n");
        rFlag=-1; // read flag set to -1, to use in the menu.
        return;
    }
    char s[200],s1[50][size]; // create an arrays to store values in.
    int x,i=0;
    x=fscanf(in,"%[^\n]%*c",s); // scanning and cutting
    // using adjacency graph matrix here.
    int graph[VN][VN]; // initializing the graph at -1.
    for (int a=0; a<VN; a++)
    {
        for (int b=0; b<VN; b++)
        {
            graph[a][b]=-1;
        }
    }
    while (x != EOF) // while not end of file.
    {
        char *c=strrchr(s,' '); // getting the last space in the file to get the weight which is after the last space.
        strcpy(s1[3],(c));
        deleteWhiteSpaces(s1[3]);
        deleteWhiteSpaces(c); //deleting the space from the weight.
        char* token = strtok(s," ");
        for ( i=0; i<2; i++) // getting first 2 vertex
        {
            strcpy(s1[i],(token));
            if (i!=1)
            {
                token = strtok(NULL, " ");
            }
        }
        while (token!=NULL) // getting the name
        {
            strcpy(s1[2],(token));
            token = strtok(NULL,"");
        }
        strremove(s1[2],s1[3]); // removing the weight from the name generated
        // adding the array into the Matrix graph
        graph[atoi(s1[0])][atoi(s1[1])]=atoi(s1[3]);
        graph[atoi(s1[1])][atoi(s1[0])]=atoi(s1[3]);
        strcpy(name[atoi(s1[0])][atoi(s1[1])],s1[2]);
        strcpy(name[atoi(s1[1])][atoi(s1[0])],s1[2]);
        //printf("Vertex [1]: %s\nVertex [2]: %s\nTown: %s\nDistance: %s\n--------\n",s1[0],s1[1],s1[2],s1[3]);
        x=fscanf(in,"%[^\n]%*c",s);
        i=0;
    }
    dijkstra(graph,distenation,source,outcome); // here taking the graph, destination , source and file
}
void dijkstra(int Graph[VN][VN],int end,int source, FILE *outcome) // dijkstra algorithm.
{
    int weight[VN][VN],distance[VN],temp2[VN]; // initialized new weight, distance to store in.
    int done[VN],flag1,distance2,temp,i,j;
    int n=VN;
    for(i=0; i<n; i++)
    {
        // if the path between 2 vertices is still -1, = infinity.
        for(j=0; j<n; j++)
        {
            if(Graph[i][j]==-1)
            {
                weight[i][j]=INF;
            }
            else // if its not , add it to the weight.
                weight[i][j]=Graph[i][j];
        }
    }
    for(i=0; i<n; i++)
    {
        distance[i]=weight[end][i]; // distance function takes the weight compared to the destination, would not matter if its des or source, since its an undirected graph.
        temp2[i]=end; //temp2 will store the needed path
        done[i]=0; // done array initialized at 0, to mark the visited nodes.
    }
    distance[end]=0; // distance between the same node is zero of course.
    done[end]=1; // mark the given node as 1.
    flag1=1;
    while(flag1<n-1)
    {
        distance2=INF;
        for(i=0; i<n; i++)
        {
            if(distance[i]<distance2&&!done[i]) // we check the initial distance between the source and other destination.
            {
                distance2=distance[i]; // distance 2 = shortest distance.
                temp=i; // the shortest first path.
            }
        }
//check if a better path exists through temp
        done[temp]=1; // its done
        for(i=0; i<n; i++)
        {
            if(!done[i]) // if its not visited.
            {
                if(distance2+weight[temp][i]<distance[i]) // if the weight is smaller that the previous distance.
                {
                    distance[i]=distance2+weight[temp][i];
                    temp2[i]=temp; //saves the path
                }
            }
        }
        flag1++; // keep looping till it finds the best distance.
    }
    int flag=0;
    for(i=0; i<n; i++) // now a loop to print the results in the console and in file.
    {
        if(i!=end && i==source)
        {
            flag=-1;
            printf("\nShortest Path (distance) from node [%d] to [%d] : %d  ",i,end,distance[i]);
            fprintf(outcome,"\nShortest Path (distance) from node [%d] to [%d] : %d  ",i,end,distance[i]);
            printf("\nPath: %d",i);
            fprintf(outcome,"\nPath: %d",i);
            j=i;
            do
            { // print the path
                j=temp2[j];
                printf("->%d",j);
                fprintf(outcome,"->%d",j);
            }
            while(j!=end);
            printf("\n\n");
            fprintf(outcome,"\n");
            j=i;
            while (j!=end)
            {
                j=temp2[j];
                printf("%s",name[j][i]);
                fprintf(outcome,"%s",name[j][i]);
            }
            j=i;
            while (j!=end)
            {
                j=temp2[j];
                printf("->%s",name[j][temp2[j]]);
                fprintf(outcome,"->%s",name[j][temp2[j]]);
            }

        }
    }
    if (end==source) // if the user entered two same nodes
    {
        printf("Same node, distance is zero");
    }
    else if (flag==0) // if flag 0 which means one of the nodes entered is out of range.
    {
        printf("One of the nodes you entered is false or does not exist!");

    }
}
int main()
{
    printf("**************************************************\n");
    printf("SHORTEST PATH FINDER USING DIJKSTRA'S ALGORITHM\n");
    printf("**************************************************\n");
    readAndflag1Vertices(); //read and get vertices number.
    VN=VN+1;
    int source=1, destination=1; // initialize at 1.
    FILE* outcome;
    outcome =fopen("route.txt","w"); // opening file to save in.
    if (rFlag!=-1)
    {
        while (source != -1 && destination != -1)
        {
            printf("Enter -1 twice to exit.\n");
            printf("Please enter source: ");
            scanf("%d",&source);
            printf("Please enter destination: ");
            scanf("%d",&destination);
            init(destination,source,outcome);
            printf("\n");
        }
    }
    else
        printf("Please Read from file first.\n");
    printf("\n\n************\nYou Can find the Output printed in the Route.txt File !\n*************");
    fclose(outcome);
    return 0;
}
int max(int a,int b) // function that takes 2 integers and return the bigger, used to find the vertex number.
{
    if (a >= b)
    {
        return a;
    }
    else
        return b;
}
