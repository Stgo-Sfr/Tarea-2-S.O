#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct node{
    int data;
    struct node* next;
} node;

pthread_mutex_t crlock;
pthread_mutex_t aplock;
pthread_mutex_t rflock;
pthread_mutex_t rblock;


void print_list(node* head)
{
    node* cursor = head;
    while(cursor != NULL)
    {
        printf("%d ", cursor->data);
        cursor = cursor->next;
    }
}

node* create(int data,node* next)
{
    pthread_mutex_lock(&crlock);
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;
    pthread_mutex_unlock(&crlock);
 
    return new_node;
}

node* append(node* head, int data)
{ 
    pthread_mutex_lock(&aplock);
    /* go to the last node */
    node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;
 
    /* create a new node */
    node* new_node =  create(data,NULL);
    cursor->next = new_node;
    pthread_mutex_unlock(&aplock);
 
    return head;
}

node* remove_front(node* head)
{
    pthread_mutex_lock(&rflock);
    if(head == NULL)
        return NULL;
    node *front = head;
    head = head->next;
    front->next = NULL;
    /* is this the last node in the list */
    if(front == head)
        head = NULL;
    free(front);
    pthread_mutex_unlock(&rflock);
    return head;
}

node* remove_back(node* head)
{
    pthread_mutex_lock(&rblock);

    if(head == NULL)
        return NULL;
 
    node *cursor = head;
    node *back = NULL;
    while(cursor->next != NULL)
    {
        back = cursor;
        cursor = cursor->next;
    }
    if(back != NULL)
        back->next = NULL;
 
    /* if this is the last node in the list*/
    if(cursor == head)
        head = NULL;
 
    free(cursor);

    pthread_mutex_lock(&rblock);
 
    return head;
}

int main(void) {
  pthread_mutex_init(&crlock,NULL);
  pthread_mutex_init(&aplock,NULL);
  pthread_mutex_init(&rflock,NULL);
  pthread_mutex_init(&rblock,NULL);

  node* head = create(0, NULL);
  // inserte código para agregar 100 nodos
  // primero secuencialmente (usando un ciclo)+
  for (int i = 1; i < 100; i++){
    append(head, i);
  }
  // luego en paralelo
  print_list(head);
  printf("\nListo!\n");

  pthread_mutex_destroy(&crlock);
  pthread_mutex_destroy(&aplock);
  pthread_mutex_destroy(&rflock);
  pthread_mutex_destroy(&rblock);

  return 0;
}
