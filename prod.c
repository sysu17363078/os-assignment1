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

typedef char buffer_item;//���ش洢������ 
#define N 20
#define sleepTime 1


buffer_item buffer[N];
/*�����ź�������������*/ 
sem_t* mutex;//������ 
sem_t* empty;
sem_t* full;

void *ptr;//��������ָ�� 
float l;
float expntl(float x) //��ָ���������� 
{ 
  double z;                     // Uniform random number from 0 to 1 
 
  // Pull a uniform RV (0 < z < 1) 
  do 
  { 
    z = ((float) rand() / RAND_MAX); 
  } 
  while ((z == 0) || (z == 1)); 
  printf ("%f\n",-x * log(z));
  return(-x * log(z)); 
} 
void insert(int num)//�����߲��뺯�� 
 {
  printf ("hear\n");//��� 
  int n;
  sem_getvalue(full,&n);//��õ�ǰ���������±�ָ�� 
 	buffer[n]=(char)(rand()%256);//����������� 
 	char data=buffer[n];
  memcpy(ptr+n,&data,1);//�����ݷŽ������� 
 	printf("Producer%d produce %d in buffer[%d]\n",num,data,n);
 	sem_post(full);//�����߿��ÿռ�-1 
 	sem_post(mutex);//�ͷŻ����� 
 }
 void *producer(void *arg)//�����ߺ��� 
 {
 	char *a = (char *)arg;
    int num = a[0] - '0';//��ȡ�����߱�� 
    while(1)
    {
     sem_wait(mutex);//��û����� 
     sem_wait(empty);//һ�������߻�û�����������������߿��ÿռ�+1 
	 insert(num);//���� 
	 sleep(expntl(l));//˯�� 
    }
 	
 }
 int main (int argc,char*argv[])
 {
  l=atof(argv[1]);
 	const char *name = "os";
 	int shm_fd;
	const int size=20*sizeof(char);
	
	/*���ɹ�������*/ 
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd,size);
	ptr=mmap(0,size,PROT_WRITE,MAP_SHARED,shm_fd,0);
	
	/*���������ź�������������*/ 
	mutex=sem_open("MUTEX",O_CREAT,0666,1);
    full=sem_open("FULL",O_CREAT,0666,0);
    empty=sem_open("EMPTY",O_CREAT,0666,20);
	pthread_t a,b,c;
    
    pthread_create(&a,NULL,producer,"0");
    pthread_create(&b,NULL,producer,"1");
    pthread_create(&c,NULL,producer,"2");

    sleep(10);
    
    /*�ر���*/ 
    sem_close(mutex);
    sem_close(full);
    sem_close(empty);
    
    /*ע����*/ 
    shm_unlink(name);
    sem_unlink("MUTEX");
    sem_unlink("FULL");
    sem_unlink("EMPTY");
    return 0;
}
