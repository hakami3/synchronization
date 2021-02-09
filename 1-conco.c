#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>


// \B9\C2\C5ؽ\BA \B0\B4ü \BC\B1\BE\F0
pthread_mutex_t mutex_lock;
int cmd_counter (int argc, char *argv[]);
int cmdProcessing(void);
void *t_function(void *data);
unsigned int g_count = 0;  // \BE\B2\B7\B9\B5\E5 \B0\F8\C0\AF\C0ڿ\F8.
unsigned int loop =0;
unsigned int totalcount = 0;
struct sigaction act;
void handler(int ignum)
{
	printf("\n");
	kill(0, SIGINT);
}

struct CMD {
	char *name;
	int (*cmd)(int argc, char *argv[]);
};
struct CMD builtin[] = {		// 내장 명령 정보
    { "counter", cmd_counter },
};


void *t_function(void *data)
{
	int i;
	pthread_mutex_lock(&mutex_lock);

	// critical section
	g_count = 0;   
	printf("Thread %d started...\n",*((int*)data));	
	for (i = 0; i < loop; i++)
	{
		g_count++;  // \BE\B2\B7\B9\B5\E5 \B0\F8\C0\AF\C0ڿ\F8
		totalcount++;
	}
	pthread_mutex_unlock(&mutex_lock);
}
#define MAX_TOKENS 128
int main()
{
	int isExit = 0;
	act.sa_handler = handler;
	sigaction(SIGINT, &act, NULL);	

	while (!isExit) {
		isExit = cmdProcessing();

	}

	return 0;
}

int cmdProcessing (void)
{
    char cmdLine[1024];		// 입력 명령 전체를 저장하는 배열
    char *cmdTokens[MAX_TOKENS];	// 입력 명령을 공백으로 분리하여 저장하는 배열
    char delim[] = " \t\n\r";		// 토큰 구분자 - strtok에서 사용
    char *token;			// 하나의 토큰을 분리하는데 사용
    int tokenNum;			// 입력 명령에 저장된 토큰 수


    /* 기타 필요한 변수 선언 */


    int exitCode = 0;			// 종료 코드 (default = 0)
	fputs("$ ", stdout);
	fgets(cmdLine, 1024, stdin);
    
    tokenNum = 0;
    token = strtok (cmdLine, delim);	// 입력 명령의 문자열 하나 분리
    while (token) {			// 문자열이 있을 동안 반복
         cmdTokens[tokenNum++] = token;	// 분리된 문자열을 배열에 저장
         token = strtok(NULL, delim);     	// 연속하여 입력 명령의 문자열 하나 분리
    }
    cmdTokens[tokenNum] = NULL;
    if (tokenNum == 0)
        return exitCode;

    if (strcmp(cmdTokens[0], builtin[0].name) == 0)
	return builtin[0].cmd(tokenNum, cmdTokens);



    return exitCode;
}

int cmd_counter (int argc, char *argv[])
{
    if(argc==1)
    {
	printf("Usage: counter num_threads num_loops \n");
	return 0;
    }
    
	
    pthread_t p_thread[1000];
    int id[1000];
    int status;
    int i;
    loop = atoi(argv[2]);
    unsigned int sh;
    sh = atoi(argv[1])*atoi(argv[2]);
    clock_t start, end;
    totalcount =0;
    // 뮤텍스 객체 초기화, 기본 특성으로 초기화 했음
    pthread_mutex_init(&mutex_lock, NULL);
 
    start=clock();
    for(i=0; i<atoi(argv[1]); i++)
    {
    id[i]=i;
    pthread_create(&p_thread[i], NULL, t_function, (void *)&id[i]);
    }
    for(i=0; i<atoi(argv[1]); i++)
    {
    pthread_join(p_thread[i],(void *)&status);
    }
    end=clock();	
    printf("Last counter: %d, should be %d.\n",totalcount,sh);	
    printf("Elapsed time: %.3lf\n", (end-start)/(double)1000000); 
 
    

    return 0;
}


