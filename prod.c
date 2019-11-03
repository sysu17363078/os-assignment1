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

typedef char buffer_item;//本地存储用数组 
#define N 20
#define sleepTime 1


buffer_item buffer[N];
/*有名信号量（三把锁）*/ 
sem_t* mutex;//互斥锁 
sem_t* empty;
sem_t* full;

void *ptr;//共享缓存区指针 
float l;
float expntl(float x) //负指生成数函数 
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
void insert(int num)//生产者插入函数 
 {
  printf ("hear\n");//标记 
  int n;
  sem_getvalue(full,&n);//获得当前缓冲数组下标指针 
 	buffer[n]=(char)(rand()%256);//随机生成数据 
 	char data=buffer[n];
  memcpy(ptr+n,&data,1);//将数据放进共享区 
 	printf("Producer%d produce %d in buffer[%d]\n",num,data,n);
 	sem_post(full);//生产者可用空间-1 
 	sem_post(mutex);//释放互斥锁 
 }
 void *producer(void *arg)//生产者函数 
 {
 	char *a = (char *)arg;
    int num = a[0] - '0';//获取生产者编号 
    while(1)
    {
     sem_wait(mutex);//获得互斥锁 
     sem_wait(empty);//一旦生产者获得互斥锁，则代表消费者可用空间+1 
	 insert(num);//插入 
	 sleep(expntl(l));//睡眠 
    }
 	
 }
 int main (int argc,char*argv[])
 {
  l=atof(argv[1]);
 	const char *name = "os";
 	int shm_fd;
	const int size=20*sizeof(char);
	
	/*生成共享缓冲区*/ 
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd,size);
	ptr=mmap(0,size,PROT_WRITE,MAP_SHARED,shm_fd,0);
	
	/*生成有名信号量（三把锁）*/ 
	mutex=sem_open("MUTEX",O_CREAT,0666,1);
    full=sem_open("FULL",O_CREAT,0666,0);
    empty=sem_open("EMPTY",O_CREAT,0666,20);
	pthread_t a,b,c;
    
    pthread_create(&a,NULL,producer,"0");
    pthread_create(&b,NULL,producer,"1");
    pthread_create(&c,NULL,producer,"2");

    sleep(10);
    
    /*关闭锁*/ 
    sem_close(mutex);
    sem_close(full);
    sem_close(empty);
    
    /*注销锁*/ 
    shm_unlink(name);
    sem_unlink("MUTEX");
    sem_unlink("FULL");
    sem_unlink("EMPTY");
    return 0;
}
