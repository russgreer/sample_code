#include "schedule.h"
#include <stdlib.h>
#include <stdio.h>

struct Node {
    int pid;      
    struct Node* next;
    struct Node* prev;
};

struct Node *gPriority[4]; // 4 linked lists for priorities
int gcount;        // number of processes
int gCurrPriority; // current priority linked list


/**
 * Function to initialize any global variables for the scheduler.
 */

void init(){
 
    int i;
    for(i=0; i<4;i++)
        gPriority[i] = NULL;

    gcount = 0;
    gCurrPriority = 0; // start at priority 1 (index 0)
   
}


struct Node* createNode (int newPid) {

    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));

    if( ! newNode) return NULL; // failed to allocate mem

    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->pid = newPid;

    return newNode;
}


/**
 * Function to add a process to the scheduler
 * @Param pid - the ID for the process/thread to be added to the
 *      scheduler queue
 * @Param priority - priority of the process being added
 * @return true/false response for if the addition was successful
 */
int addProcess(int pid, int priority){

        struct Node* newNode = createNode(pid);
     
        priority--;  // fix for 1 based priority passed

        if(! newNode)
        {
          printf("addProcess() failed to add newNode");
          return 0; // fail        
        }

   
        // if there are no nodes in this priority list add 1st one (list head)
        if (gPriority[priority] == NULL) {
            gPriority[priority] = newNode;
            gcount++; // add to process count
            return 1;
        }

   // adding node to end of this priority list
  
   // find end of this list
   struct Node* ptr = gPriority[priority];  
   while (ptr->next != NULL) {
          ptr = ptr->next;
         }

   newNode->prev = ptr;
   newNode->next = NULL;
   ptr->next = newNode;
   gcount++; // add to process count
   
   return 1;
}


/**
 * Function to remove a process from the scheduler queue
 * @Param pid - the ID for the process/thread to be removed from the
 *      scheduler queue
 * @Return true/false response for if the removal was successful
 */
int removeProcess(int incomingPid){

  if( (! gPriority[0]) && (! gPriority[1]) && (! gPriority[2]) && (! gPriority[3]) )
     return 0; // there are no nodes

  // BUG. incomingPid might be -1
//  if( incomingPid < 0 ) printf("\nBUG: incomingPid was -1\n");

   int i =0;
   for(i=0; i< 4;i++) // go through each linked list looking for the pid node
   {
     struct Node *TempList = gPriority[i];

     if( ! TempList) // no nodes in this list, try the next one
         continue;

     while(TempList) // go through each node in this list
     {

      if(TempList->pid == incomingPid) // found the node to remove
       {

        if(!TempList ->next && !TempList ->prev) // only node in list
        {
          struct Node *tmp = TempList;
          free(tmp);
          tmp = NULL;
          gPriority[i] = NULL;
          gcount--;
        }
       else if(TempList ->prev && !TempList ->next) // Last node in list
              {
                TempList ->prev ->next = NULL;
                struct Node *tmp = TempList;
                free(tmp);
                tmp = NULL;
                gcount--;
              }
            else if(!TempList ->prev && TempList ->next) // Head node with more in list
             {    
              gPriority[i] = TempList ->next; // new head of list
              struct Node *tmp = TempList;
              free(tmp);
              tmp = NULL;
              gcount--;
             }
            else if (TempList->prev && TempList ->next) // remove a middle node
             {              
                if(TempList->prev)           
                   TempList->prev->next = TempList->next;
                
                if(TempList->next)
                   TempList->next->prev = TempList->prev;

                struct Node *tmp = TempList;
                free(tmp);
                tmp = NULL;
                gcount--;                
             }  

      }
 
     TempList = TempList ->next; // next node in this list...

    } // end while nodes in this list

   } // end for loop (4 priorities)


  return 1;    
}


// returns the length of a linked list (how many nodes)
int getListLen( struct Node *list )
{
 struct Node *workPtr = list;

 int len;
 len = 0;

 while(workPtr)
 {
  len++;
  workPtr = workPtr ->next;
 }

 return len;
}

  


// pull a node off the head of the list, make next node the list head
struct Node *dequeue( struct Node *node, int priority )
{

  if( ! node ) 
      return NULL;

  if( ! node ->prev && ! node ->next )  // sigle node - remove it from the list, NULL list head and return the node
    {
      gPriority[priority] = NULL; // NULL list head
      node ->next = NULL; // unwire node (will be re-queue'd later by sending to enqueue() )
      node ->prev = NULL;
      gcount--;
      return node;
    }

  
  // remove from list, make next node list head.  
  if(node ->prev)
     node ->prev ->next = node ->next; 

  if(node ->next) // make this node the new list head...
    {
      gPriority[priority] = node ->next;
      node ->next ->prev = node ->prev;
    }

  // remove from list (will be re-queue'd later by sending to enqueue() )
  node ->prev = NULL;  
  node ->next = NULL;

  gcount--;

  return node;
}


// push node on to the tail of the list 
struct Node *enqueue( int priority, struct Node *node )
{

  if(! node)
     return NULL;        
        
  // if there are no nodes in this priority list add 1st one (list head)
  if(gPriority[priority] == NULL) 
    {      

      if( node )
        {
          node ->prev = NULL;
          node ->next = NULL;
        }

      gPriority[priority] = node;
      gcount++;
      return node;
    }

  // adding node to end of this priority list
  struct Node *workPtr = gPriority[priority];  
  if( workPtr )
    {
      while(workPtr->next) 
            workPtr = workPtr->next;   
   
      node->prev = workPtr;
      node->next = NULL;

      workPtr->next = node;
      gcount++;
      return node;
     }
  
   return NULL; // should never reach here
}


/**
 * Function to get the next process from the scheduler
 * @Param time - pass by pointer variable to store the quanta of time
 * 		the scheduled process should run for
 * @Return returns the process id of the next process that should be
 *      executed, returns -1 if there are no processes (BUG: will underflow an array in calling code)
 */
int nextProcess(int *time)
{

  int i;
  for( i = 0; i < 4; i++ )
  {
      if( gCurrPriority == i && gPriority[i] ) // this priority list is not empty
        {
          *time = (4 - i);      

          struct Node *node = gPriority[i];

          node = dequeue( node, gCurrPriority ); // pull a node off the head of the list (an re-adjust ptrs)

          node = enqueue( gCurrPriority, node);  // push node on to the tail of the list 

          gCurrPriority++;  // set to next priority list
        
          if( gCurrPriority > 3 )
              gCurrPriority = 0;

          return node ->pid; // return pid
        }

     if( gCurrPriority == i && (! gPriority[i])) // current priority list is empty, set to next priority list
     {
        gCurrPriority++;                  

        if( gCurrPriority > 3 )
            gCurrPriority = 0;
     }

  } // end for


 return -1;  // BUG for some reason returning -1 sometimes and being processed as a PID in sim2.c
} 


// DEBUG
void ShowAllLists()
{
  int i;
  
  for( i=0; i < 4; i++)
     {
       struct Node *workPtr = gPriority[i];

       printf("for list prioity[%d] - ", i);

       int x = 0;
       while( workPtr ) // show each pid in this list.
            {
              printf("%d,", workPtr ->pid);
             
              if(x++ > 10) break;  

              workPtr = workPtr ->next;
            }

       printf("\n"); 
     }
}


 



/**
 * Function that returns a boolean 1 True/0 False based on if there are any
 * processes still scheduled
 * @Return 1 if there are processes still scheduled 0 if there are no more
 *		scheduled processes
 */
int hasProcess(){
    if (gcount > 0) {
        return 1;
    }
	return 0;
}


 

void printList()
{
    int i;
    for(i=0;i<4;i++) // each linked list (priorities)
    {

      if (gPriority[i])
      {
        struct Node* t = gPriority[i];

        if (t->next == NULL) // last node in the list
           {
            printf("%d - priority %d (last node)\n", t->pid, i);            
           }        
        
         while (t->next)
         {
            printf("%d - priority %d > ", t->pid, i);
            t = t->next;

            if (! t->next)  // last node in list
                printf("%d - priority %d (last node)\n", t->pid, i);
         } 
  

     } // end if

   }

}

