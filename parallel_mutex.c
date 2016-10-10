#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#define N1 2
#define N2 65536
int *ptrNumberForOperations; 
int *ptrOperationsTurns; 
struct list_node_s
{
    int data;
    struct list_node_s* next;
};
 
const int MAX_THREADS = 1024; 
long thread_count;
pthread_mutex_t mutex;
struct list_node_s* head = NULL;    
int n, m, k=0;
float probMember;
float probInsert;
float probDelete ;
int memberCount=0;
int insertCount=0;
int deleteCount=0;
int Member(int value, struct  list_node_s* head_p);
int Insert(int value, struct list_node_s** head_pp);
int Delete (int value, struct list_node_s** head_pp);
void Display(struct list_node_s* head_p );
void FisherYatesRandNumbers(int* number, int n);
void shuffle(int *array, size_t n);
void* thread_function(void* rank);
void freeMemory(struct list_node_s** head_pp);

int main(int argc, char* argv[])
{
    int i=0;
    long thread;
    pthread_t* thread_handles;
    double start, finish, elapsed;   
	if(argc!=7){
			 printf("Invalid number of arguments.\n please provide arguments as <n> <m> <thread_count> <probMember> <probInsert> <probDelete> \n");
			 exit(-1);
		}
    n = (int)atof( argv[1]);
    m = (int)atof( argv[2]);
	thread_count = (int)atof( argv[3]);
	if(thread_count <= 0 || thread_count > MAX_THREADS){
			 printf("Number of threads exceeded the maximum\n");
			 exit(-1);
		}
    probMember= (float)atof( argv[4]) ;
    probInsert= (float)atof( argv[5]) ;
    probDelete= (float)atof( argv[6])	;
	
	/*
	* we need n random, but unique values for initiallized the linked list as well as since requirement specified as
	*"a new value inserted into the list cannot be a value already in the list"  we are taking another n uniques wich are apart from previous n values.
	*So that alt gether 2*n unique values we are generated. 
	*/
	int *ptr = malloc(2*n * sizeof *ptr);
	/*
	*To make 2n random numbers which are between N1 and N2 we choose Fisher–Yates shuffle
	*/
    FisherYatesRandNumbers(ptr, (int)ceil(2*n));
	ptrNumberForOperations=ptr;//assign to globle array
	/*
	*Here our logic is to make requirement "Then perform m random Member, Insert, and Delete operations". 
	*Here we make known number of array that later we are going to shufle it and make it random but since we know the numbers whithing 
	* the array we can control probMember, probInsert and probDelete
	*/
    int *ptr2 = malloc(m *sizeof *ptr2);
    for(i=0; i<m; i++)
    {
        ptr2[i]=i;
	}
	/*
	*Shuffle the known number array to make operations in random order
	*/
    shuffle(ptr2,m);
	ptrOperationsTurns=ptr2;
	//initial insertions to the array
	for(i=0; i<n; i++)
    {
    	    Insert(ptr[i],&head);
    }
 
    //printf("-----Initial Linked List---------\n");
    //Display(head); //Display the initial linked list
	
    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));  
     
    clock_t begin = clock();
    pthread_mutex_init(&mutex, NULL);
     
    for (thread = 0; thread < thread_count; thread++)  
    {
        pthread_create(&thread_handles[thread], NULL,thread_function, (void*)thread);  
    }
     
    for (thread = 0; thread < thread_count; thread++) 
    {
        pthread_join(thread_handles[thread], NULL); 
    }
     
    pthread_mutex_destroy(&mutex);
    
    clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	//printf("\n\n-----Final Linked List---------\n");
    //Display(head);//Display the linked list after operations
	
    freeMemory(&head);
	/*
	*This part ensure the correctness of perations which we can see and verify operations are happed properly in given proberbilities
	*/
    printf("\nFind Member Executed Count: %d\n",memberCount);
    printf("\nInsert Member Executed Count: %d\n",insertCount);
    printf("\nDelete Member Executed Count: %d\n",deleteCount);   
    printf("Execution Time = %f seconds\n", time_spent);
	
	free(ptr);
	free(ptr2);
    return 0;
}
int p=0;
void* thread_function(void* rank) 
{
    int i;
    long long new_m = m/thread_count;
    for( i=0; i < new_m; i++ )
    { 
        
		//Then perform m random Member, Insert, and Delete operations
     
         if(ptrOperationsTurns[p]<round(probDelete*m))
        {	p++;
			pthread_mutex_lock(&mutex);
            Delete(ptrNumberForOperations[(int)(n+k)],&head);  
				k++;
            pthread_mutex_unlock(&mutex);
			deleteCount++;
			
			
        }
        else if(ptrOperationsTurns[p]<round(probInsert*m+probDelete*m))
        {	p++;
            pthread_mutex_lock(&mutex);
			Insert(ptrNumberForOperations[(int)(n+k)],&head);
			k++;
			insertCount++;
            pthread_mutex_unlock(&mutex);
			
        }
        else if(ptrOperationsTurns[p]<round(probMember*m+probInsert*m+probDelete*m))
        {           
           p++;
            pthread_mutex_lock(&mutex);
            Member(ptrNumberForOperations[(int)(n+k)],head);  
				k++;
            pthread_mutex_unlock(&mutex);
			memberCount++;
        }   
    }  
 
   return NULL;
} 
void shuffle(int *array, size_t n)
{
    if (n > 1)
    {	time_t t;
		srand((unsigned) time(&t));
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

 
void Display(struct list_node_s* head_p )
{
    struct list_node_s* curr_p = head_p;

    while( curr_p != NULL  )
    {    printf("%d\n",curr_p->data);
        curr_p = curr_p->next;

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
