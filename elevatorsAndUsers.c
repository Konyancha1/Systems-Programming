#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
 
int users_waiting;
pthread_mutex_t lock;
 
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
 
 printf("How many people are waiting outside elevator %d\n", elevator);
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
 
// function to control elevator movement
void userInput(){
 
 pthread_mutex_lock(&lock);
 
 printf("users waiting = %d",users_waiting); //checking code.
 for(int i = 0; i < users_waiting; i++){
   printf( "\nWhich floor is user %d going to?\n", i+1);
   scanf("%d", &users[i].destination_floor);

   //make sure that the floor choice is not greater 8 floors
   while(users[i].destination_floor > 8 || users[i].destination_floor < 0){
    printf("We dont have that floor in this building only 0- 8 \n");
    printf("Try again");
    printf( "\nWhich floor is user %d going to?\n", i+1);
   scanf("%d", &users[i].destination_floor);

   }
 }
 int size = 9;
  //aranging the destination floor in ascending order
  for (int i = 0; i < users_waiting; ++i)
        {
            for (int j = i + 1; j < users_waiting; ++j)
            {
                if (users[i].destination_floor > users[j].destination_floor)
                {
                    int a =  users[i].destination_floor;
                    users[i].destination_floor = users[j].destination_floor;
                    users[j].destination_floor = a;
                }
            }
            //removing duplicate values in the elevator
            for( int j = i + 1;  j < users_waiting; j++){
              if(users[i].destination_floor == users[j].destination_floor){
                for(int k = j; k < size - 1; k++){
                  users[k].destination_floor = users[k + 1].destination_floor;
                }
                users_waiting--;
                j--;
              }
            }
        }
//   // testing to see if the floor destination are arranged in ascending order.
//  for (int i = 0; i < 9; i++)
//  printf("the floor is %d\n", users[i].destination_floor);



 pthread_mutex_unlock(&lock);
 }



// function to control user movement
void elevatorMvmt(int elevator){
 pthread_mutex_lock(&lock);
 for(int i = 0; i < 2; i++)
 printf("elevator %d is  currently on floor %d\n", elevator, elevators[i].current_floor);


// Going to specific floor
for( int i ; i < 5; i++){
 printf("Elevator going to floor %d\n", users[i].destination_floor);
 sleep(2*users[i].destination_floor);
 printf("Elevator has arrived at floor %d\n", users[i].destination_floor);
 printf("Elevator has stopped for 2 seconds for user to alight\n");
 sleep(2);
 if(users[i].destination_floor == 0){
  printf("All users have alighted");
  exit(0);
 }
}
// pthread_mutex_unlock(&lock);
}

 
int main(int argc, char** argv){
 
 init_user(users_waiting);
 
 // Creating a thread for each user
 pthread_t user_t[users_waiting];
 for(size_t i=0;i < 1;i++)  {
   pthread_create(&user_t[i],NULL,userInput,(void*)i);
 }
 
 init_elevator();
 
 pthread_t elevator_t[2];
 for(size_t i=0;i<2;i++) {
   pthread_create(&elevator_t[i],NULL,elevatorMvmt,(void*)i);
 }
 
 for(int i = 0; i < 1; i++)
   {
     pthread_join(user_t, NULL);
   }
 
 for(int i = 0; i < 2; i++)
   {
     pthread_join(elevator_t[i], NULL);
   }
}
