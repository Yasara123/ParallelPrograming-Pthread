 #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define N1 2
#define N2 65536

struct list_node_s
{
    int data;
    struct list_node_s* next;
};
void Display(struct list_node_s* head_p );
int Member( int value, struct  list_node_s* head_p );
int Insert(int value, struct list_node_s** head_pp);
int Delete(int value, struct list_node_s** head_pp);
void freeMemory(struct list_node_s** head_pp);
void FisherYatesRandNumbers(int* number, int n);
void shuffle(int *array, size_t n);
float mMember, mInsert, mDelete;

int main(int argc, char* argv[])
{
    	struct list_node_s* head = NULL;
    	int memberCount = 0, insertCount = 0, deleteCount =0;
    	double startTime, finishTime, elapsedTime;
    	int n, m,i;
		if(argc!=6){
			 printf("Invalid number of arguments.\n please provide arguments as <n> <m> <probMember> <probInsert> <probDelete> \n");
			 exit(-1);
		}
    	n = (int)atof( argv[1]);
		m = (int)atof( argv[2]);	
		float probMember= (float)atof( argv[3]) ;
		float probInsert= (float)atof( argv[4]) ;
		float probDelete= (float)atof( argv[5]);
		/*
		* we need n random, but unique values for initiallized the linked list as well as since requirement specified as
		*"a new value inserted into the list cannot be a value already in the list"  we are taking another n uniques wich are apart from previous n values.
		*So that alt gether 2*n unique values we are generated. 
		*/		
        int *ptr = malloc(2*n* sizeof *ptr);
		time_t t;
		/*
		*To make 2n random numbers which are between N1 and N2 we choose Fisher–Yates shuffle
		*/
        FisherYatesRandNumbers(ptr, 2*n);
		/*
		*Here our logic is to make requirement "Then perform m random Member, Insert, and Delete operations". 
		*Here we make known number of array that later we are going to shufle it and make it random but since we know the numbers whithing 
		* the array we can control probMember, probInsert and probDelete
		*/
    	int *ptr2 = malloc(m*sizeof *ptr2);
        for(i=0; i<m; i++)
            {
                ptr2[i]=i;
            }
		/*
		*Shuffle the known number array to make operations in random order
		*/
        shuffle(ptr2,m);
       
		//Populate the linked list with n random, but unique values. Each value should be between 0 and 2^16 – 1
    	for(i=0; i<n; i++)
    	{
    	    Insert(ptr[i],&head);
    	}
        //printf("-----Initial Linked List---------\n");
		//Display(head); //Display the initial linked list

		//perform m random Member, Insert, and Delete operations. Assumption "use any values" indicates should not be unique values
		clock_t begin = clock();
    	for(i=0; i<m; i++)
    	{

			 if(ptr2[i]<round(probDelete*m))
    	     {
    	    	 Delete(ptr[(int)(n+i)],&head);
    	         deleteCount ++;
    	     }
            else if(ptr2[i]<round(probInsert*m+probDelete*m))
    	     {	
    	     	 Insert(ptr[(int)(n+i)],&head);
    	        insertCount++;

    	     }
			 else if(ptr2[i]<round(probMember*m+probInsert*m+probDelete*m))
    	    {
    	    	Member(ptr[(int)(n+i)],head);
    	        memberCount++;
    	     }

    	 }
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		//printf("\n\n-----Final Linked List---------\n");
		//Display(head);//Display the linked list after operations
		
    	 freeMemory(&head);
        printf("\nFind Member Executed Count: %d\n",memberCount);
		printf("\nInsert Member Executed Count: %d\n",insertCount);
		printf("\nDelete Member Executed Count: %d\n",deleteCount);   
		printf("Execution Time = %f seconds\n", time_spent);

    	 return 0;
		 
}
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}
void FisherYatesRandNumbers(int* number, int n){
    int len = N2-N1+1,i , r , temp;
    int num[len];
    time_t t;
	srand((unsigned) time(&t));
    //Fill array with desired numbers
    for( temp=0,i=N1; temp<len; i++,temp++ )
        num[temp] = i;

    //Fisher–Yates shuffle algorithm
    for( i=len-1; i>0; i-- ){
        r = rand()%i;   //pop random number
        //swaping using temp
        temp = num[i];
        num[i] = num[r];
        num[r] = temp;
    }
    for(i=0;i<n;i++)
        {
            number[i]=num[i];
        }
}

int Member( int value, struct  list_node_s* head_p )
{
    struct list_node_s* curr_p = head_p;

    while( curr_p != NULL && curr_p->data < value )
    {
        curr_p = curr_p->next;
    }

    if(curr_p == NULL || curr_p->data > value)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
void Display(struct list_node_s* head_p )
{
    struct list_node_s* curr_p = head_p;

    while( curr_p != NULL  )
    {    printf("%d\n",curr_p->data);
        curr_p = curr_p->next;

    }


}
int Insert(int value, struct list_node_s** head_pp)
{
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p = NULL;

    while(curr_p !=NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if(curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;

        if(pred_p == NULL)
        {
            *head_pp = temp_p;
        }
        else
        {
            pred_p->next = temp_p;
        }
        return 1;

    }
    else
    {
        return 0;
    }
}

int Delete (int value, struct list_node_s** head_pp)
{
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while(curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if(curr_p != NULL && curr_p -> data == value)
    {
        if(pred_p == NULL){
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else
        {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;

    }
    else
    {
        return 0;
    }

}

void freeMemory(struct list_node_s** head_pp) {
	struct list_node_s* curr_p;
	struct list_node_s* succ_p;

	if (head_pp == NULL)
		return;

	curr_p = *head_pp;
	succ_p = curr_p->next;

	while (succ_p != NULL) {
		free(curr_p);
		curr_p = succ_p;
		succ_p = curr_p->next;
	}

	free(curr_p);
	*head_pp = NULL;
}
