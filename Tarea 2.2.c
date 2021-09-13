#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

// Tamaño del arreglo
#define MAX 16
  
// Numero MAXIMO de threads
#define MAX_THREAD 4

//Mutex
pthread_mutex_t crlock;

// arreglo a utilizar
int a[] = {6,8,11,13,14,23,28,32,57,78,100,104,157,180,192,201};
  //         2           6           10
// Llave (key) a buscar en el arreglo
int key =180;
bool found = false;
int part = 0;
  
void* binary_search(void* arg)
{

	// Cada thread revisa una (MAX / MAX_THREAD) cuarta parte del arreglo
	
	//Poner mutex 
	pthread_mutex_lock(&crlock);
  int thread_part = part++;
  int mid;
  
  int low = thread_part * (MAX / MAX_THREAD);
  int high = (thread_part + 1) * (MAX / MAX_THREAD);
  

  // Busque la clave hasta low <high
   // la clave se encuentra en cualquier parte del arreglo
   while (low < high && !found && low>0)  {
		 // busqueda binaria iterativa
     mid = (high - low) / 2 + low;
		 
	//	  printf("low %d \n",low);
	//	  printf("mid %d \n",mid);
	//	  printf("high %d \n",high);


  
     if (a[mid] == key)  {
			 found = true;
			 break;
      }
			else if (a[mid] > key)
			high = mid;
			
			else
           low = mid+1;
    }
		pthread_mutex_unlock(&crlock);
		return NULL;
}
  

int main()
{
  pthread_t threads[MAX_THREAD];
	//Mutex
	pthread_mutex_init(&crlock,NULL);

  for (int i = 0; i < MAX_THREAD; i++)
       pthread_create(&threads[i], NULL, binary_search, (void*)NULL);
  
  for (int i = 0; i < MAX_THREAD; i++)
      pthread_join(threads[i], NULL);
  
  // Llave encontrada
  if (found)
			printf("La llave %d fue encontrada en el arreglo \n",key);
        
  
   // Llave no encontrada
   else
			printf("La llave %d no fue encontrada \n",key);

   pthread_mutex_destroy(&crlock); 
  return 0;

}
