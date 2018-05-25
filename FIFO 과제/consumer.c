#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE (5000)
#define FIFO_NAME "/tmp/fifo"

int main()
{
	int pipe_fd, res;
	int open_mode = O_RDONLY;
	char buffer[2][BUFFER_SIZE];
	int bytes_read = 0;
	FILE* fwp;
	int i=0;

	//memset(buffer, '\0', sizeof(buffer)*2);
	printf("Process %d opening FIFO O_RDONLY\n", getpid());

	
	
	
	if (access(FIFO_NAME, F_OK) == -1) 
	{
		res = mkfifo(FIFO_NAME, 0777);
		if (res != 0) 
		{
			fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
			exit(EXIT_FAILURE);
		}
	}

	
	// txt파일 전송받기
	fwp = fopen("doc2.txt", "w+");
	printf("[Text전송]Process %d\n", getpid()); 		// fwp는 주소값이므로 값이 무의미

	if(fwp==NULL)
	{
		puts("File open Failuer");
		exit(EXIT_FAILURE);
	}

	pipe_fd = open(FIFO_NAME, open_mode);
	printf("Data전송]Process %d result %d\n", getpid(), pipe_fd);
	
	
	//puts(buffer2);
	if (pipe_fd != -1) 
	{
		i=0;
		do 
		{
			res = read(pipe_fd, buffer, sizeof(buffer)); // sizeof(buffer)자리에 1넣으면 절대 안된다.
			bytes_read += res;
		} while (res > 0);


	
		

	
		(void)close(pipe_fd);
	}
	else 
	{
		exit(EXIT_FAILURE);
	}

	// 데이터가 잘 전송되었는지 확인해보자.
	puts("");
	puts("BUFFER TEST");
	puts("");
	puts("buffer[0]");
	puts(buffer[0]);
	puts("buffer[1]");
	puts(buffer[1]);

	// txt파일의 내용이 잘 전달되었는지 확인해보자.
	fwrite(buffer[1],sizeof(char),BUFFER_SIZE,fwp);
	fclose(fwp);
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
