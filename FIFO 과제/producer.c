#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 5000
#define TEN_MEG (10000)
#define FIFO_NAME "/tmp/fifo"

int main()
{
	int pipe_fd;
	int bytes_sent = 0;
	int res;
	char buffer[2][BUFFER_SIZE];
	int i;
	FILE *frp;

	memset(buffer, '\0', sizeof(buffer));
	
	printf("Process %d opening FIFO O_WRONLY\n", getpid());

	if (access(FIFO_NAME, F_OK) == -1) 
	{
		res = mkfifo(FIFO_NAME, 0777);
		if (res != 0) 
		{
			fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
			exit(EXIT_FAILURE);
		}
	}

	// 의미있는 데이터 전송을 buffer[0]에 넣는다.
	for(i = 32; i<=126; i++) 
	{
		buffer[0][i-32] = i; 
		
	}
	buffer[0][127-32] = '\n';
	buffer[0][128-32] = '\0';
	
	
	printf("[Data전송]Process %d result %d\n", getpid(), pipe_fd);	
						// FIFO를 정상적으로 열었다? 3출력
	
	//txt파일을 전송해보자
	frp = fopen("doc.txt","r");
	printf("[Text전송]Process %d\n", getpid());
	
	if(frp==NULL)
	{
		puts("File open Failure");
		exit(EXIT_FAILURE);
	}
	else fread(buffer[1],sizeof(char),BUFFER_SIZE, frp);
	
	//puts(buffer[0]);
	//puts(buffer[1]);

	pipe_fd = open(FIFO_NAME, O_WRONLY);

	if (pipe_fd != -1) 
	{
		
		while(bytes_sent < TEN_MEG) 
		{
			res = write(pipe_fd, buffer, sizeof(buffer));
			if (res == -1) 
			{
				
				fprintf(stderr, "Write error on pipe\n");
				exit(EXIT_FAILURE);
			}
			bytes_sent += res;
		}
		
		
		(void)close(pipe_fd);
	}
	

	
	return 0;
}


/* [출력결과]
 * (터미널1)
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./producer
 * Process 5535 opening FIFO O_WRONLY
 * Process 5535 result 3
 * (터미널2)
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./consumer
 * Process 5534 opening FIFO O_RDONLY
 * Process 5534 result 3
 * Data가 잘 전송되는지 확인해보자
 *
 */ 
