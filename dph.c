#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define N 5     //��ѧ������

#define LEFT(i)    (i+N-1)%N  //���ֱ���ѧ�ұ��
#define RIGHT(i)   (i+1)%N    //���ֱ��ܼұ��

enum{THINKING,HUNGRY,EATING}state[5]; //��¼�����ѧ�ҵ�״̬ 
sem_t chopsticks[N];//��֧��������� 

//ÿ����ѧ�ҵ�˼��ʱ�䣬�Է�ʱ�䣬˼����ʼʱ�䣬�Է���ʼʱ��
clock_t thinking_time[N], eating_time[N], start_eating_time[N], start_thinking_time[N];  
//�̺߳���
void *philosopher(void *arg);

int main()
{
	for (int i=0; i<N; i++) {
        sem_init(&chopsticks[i], 0, 1);
    }

      
    pthread_t a,b,c,d,e;
    //Ϊÿһ����ѧ�ҿ���һ���̣߳�������ѧ�ұ��
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
    
    
    //��ʼ�����������
    srand(time(NULL));
    while(1)
    {
        ;
    }
}

void pickup_forks(int num)
{
	int left = num;//���ֿ��� 
    int right = (num + 1) % N;//�����ֿ���
	//pthread_mutex_lock(&state[i]);
	if(state[LEFT(num)] != EATING && state[RIGHT(num)] != EATING)
	{
		//pthread_cond_wait(&mutex,&cond_var);
		sem_wait(&chopsticks[right]);
        printf("%d pick up %d\n", num, right);
		sem_wait(&chopsticks[left]);
        printf("%d pick up %d,star to eat\n", num, left);
        state[num] = EATING;
        start_eating_time[num] = clock(); //��¼��ʼ�Է�ʱ��
        eating_time[num] = rand() % 5 ;   //������ɳԷ�ʱ��
        //printf ("%ld\n",eating_time[num]);//��ӡ�Է���ʱ�� 
        //���״̬
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
    start_thinking_time[num] = clock();  //��¼��ʼ˼��ʱ��
    thinking_time[num] = rand() % 10 ;  //�������˼��ʱ��
    printf("%d is thinking\n",num);
    //pthread_mutex_unlock(&state[i]); 
 }
void *philosopher(void *arg)
{
    char *a = (char *)arg;
    int num = a[0] - '0';  //���ݴ��ݲ�����ȡ��ѧ�ұ��
    int rand_time; 
    while(1)
    {
        //�������ѧ�Ҷ��ˣ���׼��������ӳԷ� 
        if(state[num] == HUNGRY)
        {
        	pickup_forks(num);
		}
        else if(state[num] == EATING)
        {
            //�Է�ʱ���ѵ� �����¿���
            if(clock() - start_eating_time[num] >= eating_time[num])  //
            {
                return_forks(num);
            }
        }
        else if(state[num] == THINKING)
        {
        	//printf("%d is thinking\n");
            //˼��һ��ʱ�����ѧ�Ҷ���
            if(clock() - start_thinking_time[num] >= thinking_time[num])
            {
                state[num] = HUNGRY;
                printf("state: %d %d %d %d %d\n",state[0],state[1],state[2],state[3],state[4]);
				printf("%d is hungry\n",num);
            }
        }
              
    }
}
