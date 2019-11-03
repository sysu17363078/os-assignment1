#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>
#include<pthread.h>
#include <math.h>

typedef int semaphore;
typedef char buffer_item;
#define N 20
#define sleepTime 1


buffer_item buffer[N];
/*�����ź�������������*/
sem_t* mutex;
sem_t* empty;
sem_t* full;

void *ptr;
float l;
float expntl(float x) 
{ 
  double z;                     // Uniform random number from 0 to 1 
 
  // Pull a uniform RV (0 < z < 1) 
  do 
  { 
    z = ((float) rand() / RAND_MAX); 
  } 
  while ((z == 0) || (z == 1)); 
 
  return(-x * log(z)); 
} 
void remove_item(int num)//�������Ƴ����� 
 {
  printf ("hear\n");
  int n;
  sem_getvalue(full,&n);//��õ�ǰ���������±�ָ��
 	buffer[n]=*((char*)(ptr+n));
  char data = buffer[n];                            
 	printf("Consumer%d comsum %d in buffer[%d]\n",num,data,n);
 	sem_post(empty);//�����߿��ÿռ�-1 
 	sem_post(mutex);//�ͷŻ����� 
 }
 void *consumer(void *arg)//�����ߺ��� 
 {
 	char *a = (char *)arg;
    int num = a[0] - '0';
    while(1)
    {
     sem_wait(mutex);//��û����� 
     sem_wait(full);//һ�������߻�û�����������������߿��ÿռ�+1 
	 remove_item(num);//�Ƴ� 
	 sleep(expntl(l));//˯�� 
    }
 	
 }
 int main (int argc,char*argv[])
 {
  l=atof(argv[1]);
 	const char *name = "os";
 	int shm_fd;
	const int size=20*sizeof(char);
	
	shm_fd = shm_open(name,O_RDWR, 0666);
	ptr=mmap(0,size,PROT_READ,MAP_SHARED,shm_fd,0);
	//ptr=(struct share*)ptr;
	mutex=sem_open("MUTEX",O_CREAT,0666,1);
    full=sem_open("FULL",O_CREAT,0666,0);
    empty=sem_open("EMPTY",O_CREAT,0666,20);
	pthread_t a,b,c;
    
    pthread_create(&a,NULL,consumer,"0");
    pthread_create(&b,NULL,consumer,"1");
    pthread_create(&c,NULL,consumer,"2");

    sleep(10);

    return 0;
}
