#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int users_waiting;

static struct user{
  pthread_barrier_t barrier;
  int id;
  int current;
  int destination_floor;
  pthread_mutex_t lock;
} users[50];

static struct elevator{
  int id;
  int in_elevator;
  int floor;
  int current_floor;
  int passengers;
  int occupancy;
  struct user *u;
  pthread_mutex_t lock;
} elevators[2];

void init_elevator(){
  int i;

  for(int i = 0; i < 2; i++)
    {
      pthread_mutex_init(&elevators[i].lock, NULL);
      elevators[i].current_floor=0;
      elevators[i].occupancy=0;
      elevators[i].u=NULL;
    }
}

void init_user(int elevator){
  int i;

  printf("How many people are waiting outside elevator %d", elevator);
  scanf("%d", &users_waiting);

  for (int i = 0; i < users_waiting; i++)
    {
      pthread_barrier_init(&users[i].barrier, NULL, 2);
    }
}

void check_elevator(int elevator){
  if(elevators[elevator].passengers > 5 || elevators[elevator].passengers < 0){
    printf("Elevator %d over capacity or negative passenger count %d!\n", elevator, elevators[elevator].passengers);
    exit(1);
  }
}

int main(int argc, char** argv){

  init_user(users_waiting);

  pthread_t user_t[users_waiting];
  for(size_t i=0;i<users_waiting;i++)  {
    pthread_create(&user_t[i],NULL,function_to_control_user_movement,(void*)i);
  }

  init_elevator();

  pthread_t elevator_t[2];
  for(size_t i=0;i<2;i++) {
    pthread_create(&elevator_t[i],NULL,function_to_control_elevator_movement,(void*)i);
  }

  for(int i = 0; i < users_waiting; i++)
    {
      pthread_join(user_t[i], NULL);
    }

  for(int i = 0; i < 2; i++)
    {
      pthread_join(elevator_t[i], NULL);
    }
}
