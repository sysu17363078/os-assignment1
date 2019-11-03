#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define N 5     //哲学家数量

#define LEFT(i)    (i+N-1)%N  //左手边哲学家编号
#define RIGHT(i)   (i+1)%N    //右手边哲家编号

enum{THINKING,HUNGRY,EATING}state[5]; //记录五个哲学家的状态 
sem_t chopsticks[N];//五支筷子五把锁 

//每个哲学家的思考时间，吃饭时间，思考开始时间，吃饭开始时间
clock_t thinking_time[N], eating_time[N], start_eating_time[N], start_thinking_time[N];  
//线程函数
void *philosopher(void *arg);

int main()
{
	for (int i=0; i<N; i++) {
        sem_init(&chopsticks[i], 0, 1);
    }

      
    pthread_t a,b,c,d,e;
    //为每一个哲学家开启一个线程，传递哲学家编号
    pthread_create(&a,NULL,philosopher,"0");
    pthread_create(&b,NULL,philosopher,"1");
    pthread_create(&c,NULL,philosopher,"2");
    pthread_create(&d,NULL,philosopher,"3");
    pthread_create(&e,NULL,philosopher,"4");
    
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    pthread_join(c, NULL);
    pthread_join(d, NULL);
    pthread_join(e, NULL);
    
    
    //初始化随机数种子
    srand(time(NULL));
    while(1)
    {
        ;
    }
}

void pickup_forks(int num)
{
	int left = num;//左手筷子 
    int right = (num + 1) % N;//左右手筷子
	//pthread_mutex_lock(&state[i]);
	if(state[LEFT(num)] != EATING && state[RIGHT(num)] != EATING)
	{
		//pthread_cond_wait(&mutex,&cond_var);
		sem_wait(&chopsticks[right]);
        printf("%d pick up %d\n", num, right);
		sem_wait(&chopsticks[left]);
        printf("%d pick up %d,star to eat\n", num, left);
        state[num] = EATING;
        start_eating_time[num] = clock(); //记录开始吃饭时间
        eating_time[num] = rand() % 5 ;   //随机生成吃饭时间
        //printf ("%ld\n",eating_time[num]);//打印吃饭的时间 
        //输出状态
        printf("state: %d %d %d %d %d\n",state[0],state[1],state[2],state[3],state[4]);
        printf("%d is eating\n",num);
        //pthread_mutex_unlock(&state[i]); 
	}
 } 
 void return_forks(int num)
 {
 	int left = num;
    int right = (num + 1) % N;
 	//pthread_mutex_lock(&state[i]);
 	sem_post(&chopsticks[left]);
    printf("%d put down %d\n", num, left);
    sem_post(&chopsticks[right]);
    printf("%d put down %d\n", num, right);
 	state[num] = THINKING;
 	//pthread_cond_signal(&cond_var) ;
    printf("%d finshi eating\n",num);
    printf("state: %d %d %d %d %d\n",state[0],state[1],state[2],state[3],state[4]);
    start_thinking_time[num] = clock();  //记录开始思考时间
    thinking_time[num] = rand() % 10 ;  //随机生成思考时间
    printf("%d is thinking\n",num);
    //pthread_mutex_unlock(&state[i]); 
 }
void *philosopher(void *arg)
{
    char *a = (char *)arg;
    int num = a[0] - '0';  //根据传递参数获取哲学家编号
    int rand_time; 
    while(1)
    {
        //如果该哲学家饿了，则准备拿起筷子吃饭 
        if(state[num] == HUNGRY)
        {
        	pickup_forks(num);
		}
        else if(state[num] == EATING)
        {
            //吃饭时间已到 ，放下筷子
            if(clock() - start_eating_time[num] >= eating_time[num])  //
            {
                return_forks(num);
            }
        }
        else if(state[num] == THINKING)
        {
        	//printf("%d is thinking\n");
            //思考一定时间后，哲学家饿了
            if(clock() - start_thinking_time[num] >= thinking_time[num])
            {
                state[num] = HUNGRY;
                printf("state: %d %d %d %d %d\n",state[0],state[1],state[2],state[3],state[4]);
				printf("%d is hungry\n",num);
            }
        }
              
    }
}
