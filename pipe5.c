/* - pipe file descriptor 의 값을 보통 standard input 0, 또는 standard output 1을 할당한다.
 * - dup() and dup2()
 *
 *   #include <unistd.h>
 *   int dup(int file_descriptor);
 *   int dup2(int file_descriptor_one, int file_descriptor_two);
 *                  파일 디스크립터    원하는 파일 디스크립터 번호
 *   return값 :  원하는 파일 디스크립터 번호, 실패하면 -1이 반환
 *   
 *   dup2
 *   파일 디스크립터 복사본을 만듭니다, dup()는 커널에서 알아서 사용하지 않는 디스크립터 번호 중에
 *   하나가 자동으로 지정되지만 dup2()는 프로그래머가 원하는 번호로 지정할 수 있습니다.
 *   프로그래머가 지정하는 번호가 이미 사용하는 번호라면, 
 *   dup2()는 자동으로 그 파일을 닫고 다시 지정해줍니다.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
int main()
{
	int data_processed;
	int file_pipes[2];
	const char some_data[] = "12345";
	pid_t fork_result;
	int status;
	int fd;

	if( pipe(file_pipes) == 0)
	{
		fork_result = fork();
		
		if( fork_result == -1 )
		{
			fprintf(stderr, "Fork Failure");
			exit(EXIT_FAILURE);
		}

		if( fork_result == 0 )
		{
			close(0);		// 0:stdin을 닫는다			
	
			dup(file_pipes[0]);
			//fd = dup(file_pipes[0]);
			//printf("fd = %d\n", fd);	// 이렇게 하면 fd = 0 출력
			close(file_pipes[0]);
			close(file_pipes[1]);
			execlp("od", "od", "-c", (char*)0); // (char*)0 이니깐 나를 가리키는 것?
			exit(EXIT_SUCCESS);
		}
		else
		{
			close(file_pipes[0]);
			data_processed = write(file_pipes[1], some_data, strlen(some_data));
			close(file_pipes[1]);
			printf("%d - wrote %d bytes\n", (int)getpid(), data_processed);
		}
		wait(&status);		// 이거 넣어야지 출력결과중에 아래꺼(제대로)만 출력된다
	
		/* 부모				자식
		 * [1]입력			[0]출력
		 * ------------->  | ------------->
				   |
				 "12345"
				   |
		   <-------------  | <-------------
		   [0]출력			[1]입력
		 */
	}

	exit(EXIT_SUCCESS);
}


/* [출력결과] (pid값은 2씩 증가한다)
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./pipe5
 * 3361 - wrote 5 bytes
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ 0000000   1   2   3   4   5
 * 0000005
 * (_커서)
 * 
 * // 위의 경우는 부모가 종료되고 자식이 종료된거.
 * // 아래의 경우는 자식이 종료되고 부모가 종료된거.
 * // wait(&status)를 넣어줌으로서 자식이 다 종료되야 부모가 종료되게 해준다.
 *
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./pipe5
 * 3387 - wrote 5 bytes
 * 0000000   1   2   3   4   5
 * 0000005
 *
 *
 */
