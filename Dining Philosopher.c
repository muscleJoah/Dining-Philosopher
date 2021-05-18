#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h> 
#include <semaphore.h> 

#define SIZE 5 
#define LEFT (philNumber + 4) % SIZE 
#define RIGHT (philNumber + 1) % SIZE  

int EATING = 0; int HUNGRY = 1; int THINKING = 2;
int state[SIZE]; 
int philosophers[SIZE] = { 0, 1, 2, 3, 4 }; 

sem_t mutex; // 세마포어 선언
sem_t S[SIZE]; 

void canTakeChops(int philNumber); // 젓가락을 현재 들수 있는지 check 
void takeChopsticks(int philNumber); // 젓가락을 집기
void putdownChopsticks(int philNumber); // 젓가락 내려놓기
void* philospher(void* num); // 분기시켜 실행할 쓰레드 함수
  
int main() 
{ 
    int i; 
    pthread_t thread_id[SIZE]; // 쓰레드 초기화
    sem_init(&mutex, 0, 1); 
    for (i = 0; i < SIZE; i++) 
      sem_init(&S[i], 0, 0); //THINKING 상태로 semaphore 초기화
    for (i = 0; i < SIZE; i++) { 
        pthread_create(&thread_id[i], NULL, philospher, &philosophers[i]); //philosopher porcess 만들기.
        printf("Philosopher %d is thinking\n", i + 1); 
    } 

    for (i = 0; i < SIZE; i++) 
       pthread_join(thread_id[i], NULL); //쓰레드가 종료되길 기다림.
} 

void canTakeChops(int philNumber) 
{ 
    if (state[philNumber] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) { 
        //젓가락을 집을 수 있는 상태
        state[philNumber] = EATING; 
        sleep(2); 
        printf("Philosopher%d takes chopsticks %d & %d\n", philNumber + 1, LEFT + 1, philNumber + 1); 
        printf("Philosopher%d starts to eat\n", philNumber + 1); 
        sem_post(&S[philNumber]); //세마포어의 잠금을 해제
    } 
} 

void takeChopsticks(int philNumber) //젓가락들기
{ 
    sem_wait(&mutex); 
    tate[philNumber] = HUNGRY; 
    printf("Philosopher%d is Hungry\n", philNumber + 1); 
    canTakeChops(philNumber); //이웃 철학자가 안먹고있으면 들기
    sem_post(&mutex); 
    sem_wait(&S[philNumber]); //먹을수 없다면 signal 올때까지 대기
    sleep(1); 
} 
  

void putdownChopsticks(int philNumber) //젓가락 내려놓기
{   
    sem_wait(&mutex); 
    state[philNumber] = THINKING; 
    printf("Philosopher%d puts down chopsticks %d & %d\n",  philNumber + 1, LEFT + 1, philNumber + 1); 
    printf("Philosopher%d is thinking\n", philNumber + 1); 
    canTakeChops(LEFT); 
    canTakeChops(RIGHT);  
    sem_post(&mutex); 
} 
  
void* philospher(void* num) 
{ 
    while (1) { 
        int* i = num; 
        sleep(1); 
        takeChopsticks(*i); 
        sleep(0); 
        putdownChopsticks(*i); 
    } 
} 