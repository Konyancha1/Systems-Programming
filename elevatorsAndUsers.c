#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// creating necessary variables
int users_waiting;
int users_allowed;
int extra_users;
pthread_mutex_t lock;

//Users alighting in a specific floor
static struct floors{
  int userno;
}floors[15];

// A user struct to hold user information
static struct user{
  pthread_barrier_t barrier;
  int id;
  int current;
  int destination_floor;
  pthread_mutex_t lock;
} users[50];

// An elevator struct to hold elevator information
static struct elevator{
  int id;
  int floor;
  int current_floor;
  int passengers;
  int occupancy;
  struct user *u;
  pthread_mutex_t lock;
} elevators[2];

int elevator;
//A function containing elevator information
void init_elevator(){
   pthread_mutex_init(&elevators[elevator].lock, NULL);
   elevators[elevator].current_floor=0;
   elevators[elevator].occupancy=0;
   elevators[elevator].u=NULL;
}

// A function to get the number of users who want to use the elevator
void init_user(int elevator){
  int i;

  //Getting the number of people waiting outside and storing it in the user_waiting variable we
  printf("How many people are waiting outside elevator A :\n");

  printf("How many people are waiting outside elevator A\n");

  scanf("%d", &users_waiting);

  // A loop where the barrier for each user
  for (int i = 0; i < users_waiting; i++)
    {
      pthread_barrier_init(&users[i].barrier, NULL, 2);
      elevators[elevator].passengers++;
    }
}

// Function to check the elevator status
void check_elevator(int elevator){
  if(elevators[elevator].passengers > 5 && elevators[elevator].passengers <= 10){
    extra_users = (elevators[elevator].passengers)- 5;
    users_allowed = users_waiting - extra_users;
  }
  else if(elevators[elevator].passengers <= 5){
    users_allowed = users_waiting;
  }
  if(elevators[elevator].passengers > 10){
    printf("Both elevators have a combined capacity of 10\n");
    exit(1);
  }
}

//function to remove duplicate and arrange in ascending order
void ascendingAndDuplicate(){
  int size = 9;
  //aranging the destination floor in ascending order
  for (int i = 0; i < users_allowed; ++i)
    {
      for (int j = i + 1; j < users_allowed; ++j)
        {
          if (users[i].destination_floor > users[j].destination_floor)
            {
              int a =  users[i].destination_floor;
              users[i].destination_floor = users[j].destination_floor;
              users[j].destination_floor = a;
            }
        }
      //removing duplicate values in the elevator
      int increment = 1;
      for( int j = i + 1;  j < users_allowed + 1; j++){
        if(users[i].destination_floor == users[j].destination_floor){

            floors[users[i].destination_floor].userno = ++increment; // recording the number of users in a particular floor

          for(int k = j; k < size + 1; k++){
            users[k].destination_floor = users[k + 1].destination_floor;
          }
          users_allowed--;
          j--;
        }
        else{
            
              floors[users[i].destination_floor].userno = increment;

           }
      }
    }

}


// function to control elevator movement
void userInput(){
  pthread_mutex_lock(&lock);
  check_elevator(elevator);

  printf("users waiting outside elevator A = %d\n",users_allowed); //checking code.
  printf("Extra users will automatically use elevator B\n");

  for(int i = 0; i < users_allowed; i++){
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
  

    //function to remove duplicate and arrange in ascending order
    ascendingAndDuplicate();


  //removing the mutex lock
  pthread_mutex_unlock(&lock);
  if(extra_users > 0){
    pthread_mutex_lock(&lock);
    printf("\n\nUsers waiting outside elevator B: %d \n", extra_users);
    for(int i = 0; i < extra_users; i++){
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
    //function to remove duplicate and arrange in ascending order
    ascendingAndDuplicate();

    pthread_mutex_unlock(&lock);
  }
}

// function to control user movement
void elevatorMvmt(int elevator){
  int count = 0;
  
  pthread_mutex_lock(&lock);
  printf("\nelevator A is  currently on floor %d\n", elevators[elevator].current_floor);
  // Going to specific floor
  for( int i ; i < 5; i++){
    if(users[i].destination_floor == 0){
      printf("There is nobody in the elevator\n");
      printf("Elevator A in standby mode\n");
      exit(0);
    }
    //Outputting elevator status to the user
    printf("Elevator A going to floor %d \n", users[i].destination_floor);
    

    sleep(2*users[i].destination_floor);

    printf("Elevator A has arrived at floor %d and %d users have alighted\n", 
    users[i].destination_floor,floors[users[i].destination_floor].userno);
    
    printf("Elevator A has stopped for 2 seconds for user to alight\n\n");
    sleep(2);
    
    count++;
    //Making sure all users have alighted from the elevator
    if(count == users_allowed){
      printf("All users have alighted \n");
    }
  }
  //removing the lock after the thread
  pthread_mutex_unlock(&lock);
  if(extra_users > 0){
    int count = 0;
    pthread_mutex_lock(&lock);
    printf("elevator B is  currently on floor %d\n", elevators[elevator].current_floor);
    // Going to specific floor
    for( int i ; i < 5; i++){
      if(users[i].destination_floor == 0){
      printf("There is nobody in the elevator\n");
      printf("Elevator B in standby mode\n");
      exit(0);
    }
      printf("Elevator B going to floor %d\n", users[i].destination_floor);
      sleep(2*users[i].destination_floor);
      printf("Elevator B has arrived at floor %d and %d users have alighted\n", 
    users[i].destination_floor,floors[users[i].destination_floor].userno);
      
      printf("Elevator B has stopped for 2 seconds for user to alight\n");
      sleep(2);
      
      count++;
      // Making sure all users have alighted
      if(count == extra_users){
        printf("All users have alighted \n");
        exit(0);
      }
    }
    pthread_mutex_unlock(&lock);
  }
  else{
    exit(0);
  }
}

// The main function
int main(int argc, char** argv){

  init_user(users_allowed);

  // Creating a thread for each user
  pthread_t user_t;
  for(size_t i=0;i < 1;i++)  {
    pthread_create(&user_t,NULL,userInput,(void*)i);
  }

  init_elevator();
  // Creating the elevators
  pthread_t elevator_t[2];
  for(size_t i=0;i<1;i++) {
    pthread_create(&elevator_t[i],NULL,elevatorMvmt,(void*)i);
  }
  //joining the threads
  pthread_join(user_t, NULL);
  for(int i = 0; i < 2; i++)
    {
      pthread_join(elevator_t[i], NULL);
    }
}
