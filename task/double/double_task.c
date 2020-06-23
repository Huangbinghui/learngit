#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/msg.h>
#include<semaphore.h>
pthread_t s1,s2,r;//创建线程sender1,sender2,receiver 
sem_t Write;//创建信号量变量 
struct msgbuff{//消息缓冲区 
	long mtype ;//消息类型 
	char mtext[100];//消息内容 
};
void *receive(void *arg){//arg即传入的msgid,下同 
	struct msgbuf buf;
	int ret;
	int flag1,flag2;//用于判断sender1、sender2是否结束发送 
	flag1=1;flag2=1;
	while(1){
		//消息队列初始化，将buf的sizeof(buf)字节置为0 
		memset(&buf,0,sizeof(buf));
		sem_wait(&Write);// 申请写的权限 
		//msgrcv即messagereceive,ret为返回值
		//函数原型 ssize_t msgrcv(int msgid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
		//arg即传入的msgid,msgp是指向消息缓冲区的指针,msgsz即messagesize
		//msgtyp即消息类型(可以自定义),msgflg即messageflag
		//具体参数含义参见https://baike.baidu.com/item/msgsnd/msgrcv
		ret=msgrcv(*(int *)arg,&buf,sizeof(buf.mtext),2,0);
		if (ret==-1){//返回值为-1表示接收失败 
			perror("msgrcv error");
			sem_post(&Write);//释放写的权限 
			exit(1);//异常退出程序 
		}
		else{//接收成功 
			printf("receive:%s\n",buf.mtext);//打印接收到的内容 
			sem_post(&Write);//释放写的权限 
			sleep(1);
		} 
		if(flag1&&!strncmp(buf.mtext,"end1",4)){
			pthread_cancel(s1);
			flag1--;//sender1发送结束 
		}
		else if(flag2&&!strncmp(buf.mtext,"end2",4)){
			pthread_cancel(s2);
			flag2--;//sender2结束发送 
		}
		if((flag1+flag2)==0){
			//sender1和sender2均发送结束 
			printf("---------end---------\n"); 
			exit(0); //正常退出程序 
		}
	}
}
 
void *sender1(void *arg){
	struct msgbuf buf;
	int ret;
	int x=1;
	while(x){
		memset(&buf,0,sizeof(buf));//初始化buf 
		sem_wait(&Write);//申请写的权限 
		scanf("%s",buf.mtext);//从键盘读入字符串 
		buf.mtype=2;//定义消息类型 
		//buf.mtext为exit,则将buf.mtext置为end1,并将x置0 
		if(!strncmp(buf.mtext,"exit",4)){	
			strcpy(buf.mtext,"end1");
			x=0;
		}
		//否则,在buf.mtext后加上//from s1 用以区分 
		else strcat(buf.mtext,"//from s1");
		//函数原型 int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg); 
		ret=msgsnd(*(int *)arg,&buf,sizeof(buf.mtext),0);
		if(ret==-1){//返回值为-1，发送失败 
			perror("msgsnd error");
			sem_post(&Write);//释放写的权限 
			exit(1);//异常退出 
		}
		sem_post(&Write);//释放写的权限
		sleep(1);
	}
}
//sender1与sender2类似
void *sender2(void *arg){
	struct msgbuf buf;
	int ret;
	int x=1;
	while(x){
		memset(&buf,0,sizeof(buf));
		sem_wait(&Write);
		scanf("%s",buf.mtext);
		buf.mtype=2;
		if(!strncmp(buf.mtext,"exit",4)){	
			strcpy(buf.mtext,"end1");
			x=0;
		}
		else strcat(buf.mtext,"//from s2");
		ret=msgsnd(*(int *)arg,&buf,sizeof(buf.mtext),0);
		if(ret==-1){
			perror("msgsnd error");
			sem_post(&Write);
			exit(1);
		}
		sem_post(&Write);
		sleep(1);
	}
}
int main(){
	//ret即return的缩写,用来储存返回值 
	int ret,msgid,x;
	//int sem_init(sem_t *sem, int pshared, unsigned int value); 
	//sem为初始化的信号量名,pshare为0表示只在该进程的所有线程间共享,value为信号量的大小 
	//具体参数含义参见https://baike.baidu.com/item/sem_init 
	sem_init(&Write,0,1);
	
	//以下两行代码用以确保Write的初值为1,因为笔者在调试代码期间,有时Write的初值不为1 
	//如有需要可以使用这两行代码 
	/*sem_getvalue(&Write,&x);
	if(x==0) sem_post(&Write);*/ 
	
	key_ t key;
	key=100;//可以自定义key的大小
	//函数原型int msgget(key_t key, int msgflg);
	//具体参数含义参见https://baike.baidu.com/item/msgget
	msgid=msgget(key,IPC_CREAT|0666);
	if(msgid==-1){//msgid为-1表示创建消息队列失败 
		perror("msgid error");
		exit(1);
	}
	//创建线程,并将msgid传入
	//函数原型int pthread_create(pthread_t *tidp,const pthread_attr_t *attr
	//,(void*)(*start_rtn)(void*),void *arg);
	//*tidp为线程标识符的指针,*attr用以设置线程属性
	//第三个参数是线程运行函数的起始地址,*arg为传入运行函数的参数 
	//具体参数含义参见https://baike.baidu.com/item/pthread_create 
	ret=pthread_create(&r,NULL,receive,(void *)&msgid); 
	if (ret!=0){
		perror("receiver create error");
		exit(1);
	}
	
	ret=pthread_create(&s1,NULL,sender1,(void *)&msgid);
	if (ret!=0){
		perror("sender1 create error");
		exit(1);
	}
	
	ret=pthread_create(&s2,NULL,sender2,(void *)&msgid);
	if (ret!=0){
		perror("sender2 create error");
		exit(1);
	}
	//pthread_join等待一个线程结束
	//用法参见https://baike.baidu.com/item/pthread_join
	pthread_join(r,NULL);
	pthread_join(s1,NULL);
	pthread_join(s2,NULL);
	//删除消息队列
	//用法参见https://baike.baidu.com/item/msgctl
	msgctl(msgid,IPC_RMID,NULL);//ctl为control 
	return 0;
}