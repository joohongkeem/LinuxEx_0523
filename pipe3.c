/* The Pipe Call
 * - popen은 high-level function, pipe는 lower-level function
 * - 쉘을 호출하지 않고도 요청된 명령을 변환하며, 두 프로그램 간에 전달하는 방법을 제공한다.
 *
 *   #include <unistd.h>
 *   int pipe(int file_descriptor[2]);
 *   >> 파이프는 단방향이므로 쓰는방향, 읽는방향의 2가지 디스크립터를 선언해 주어야 한다.
 *	1을 write, 0을 read로 자주 사용하나...? 
 *
 *   인수 : array of two integer file descriptors
 *   return : two new file descriptors 배열을 채우면 0을 반환, 실패하면 -1을 반환하고 errno를 Set
 *                                                                       (EMFILE, ENFILE, EFAULT)
 *   FIFO : file_descriptor[1]에 data를 쓰고, file_descriptor[0]에서 읽어들인다.
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>	// wait을 위한 헤더
#include <sys/types.h>	// wait을 위한 헤더

/* - exec call를 사용하면 자식 프로세스와 부모 프로세스와 다른 프로그램이 되게 한다. [프로세스의 대체]
 *   ex) 프로세스 A엔 없고, 프로세스 B에 있는 기능이 A에게 필요하다면, A를 B로 대체하면 된다!
 *   	 즉, 프로세스 A를 종료시키고 프로세스 B를 생성시키는 행위! 
 *
 * - After an exec call, old process는 child process로 대체된다
 *   >> child process가 file_pipes 데이터의 사본을 사용할 수 없다
 *   >> 그러므로 file descriptor를 newly executed program의 파라미터로 전달함으로 해결 가능하다.
 *
 *   #include <unistd.h>
 *   [execl계열]
 *   int execl(const char *path, const char *arg, ..., (char*)0);
 *         대체할 프로세스 경로|대체할 프로세스의 인수
 *   int execlp(const char *file, const char *arg, ..., (char*)0);
 *        대체할 프로세스의 이름| 
 *   int execle(const char *path, const char *arg, ..., (char*)0, char* const envp[]);
 *   	    대체할 프로세스 경로| 대체할 프로세스 인수|           임의 지정할 환경변수
 *   	    							- 새로 대체될 프로그램의 환경변수를 임의로 설정
 *   	    							  (없으면, 대체되기 전의 프로세스 환경정보 그대로)
 *
 *   [execv계열]
 *   int execv(const char *path, char *const argv[]);
 *   int execvp(const char *file, char *const argv[]);
 *   int execve(const char *path, char *const argv[], char* const envp[]);
 *
 *   execl계열과 execv의 가장 큰 차이는
 *   	execl 계열에선 char형 포인터인 arg로, execv계열에선 char형 포인터 배열인 argv로 선언된다
 *   	즉, 포인터로 받는 것은 리스트를 통해 받는 것이고, 포인터 배열로 받는단 것은 배열을 통해 인수를 받는단 것.
 *
 *   execl계열에서 (char*)0은 더 이상 인수가 없다는 것을 의미하는 용도로 무조건 써줘야 한다.
 *
 */

int main()
{
	int data_processed;
	int file_pipes[2];
	const char some_data[] = "1234";
	char buffer[BUFSIZ + 1];
	pid_t fork_result;


	memset(buffer, '\0', sizeof(buffer));
/* void *memset(void *ptr, int value, size_t num);
 * >> 어떤 메모리의 시작점부터 연속된 범위를 어떤 값으로(바이트 단위) 모두 지정하고 싶을 때 사용
 *    특정 범위에 있는 연속된 메모리에 값을 지정하고 싶을 때 사용, for문보다 더 빠른속도 가능
 *
 * ptr : 채우고자 하는 메모리의 시작 포인터(시작 주소)
 * value : 메모리에 채우고자 하는 값, int형이지만 unsigned char(1byte)로 변환되어서 저장됨
 * num : 채우고자 하는 바이트 수, 즉 채우고자 하는 메모리의 크기
 */


	if( pipe(file_pipes) == 0 ) // 파이프가 정상적으로 만들어 졌다면?
	{
		fork_result = fork();
		// [질문]fork를 먼저하고 pipe를 하면 어떻게 될까?
		// pipe를 먼저하면 fork시에 pipe도 자식프로세스에 같이 복사되지만
		// fork를 먼저하면 부모, 자식에 각각 pipe를 만들어줘야 한다!!

		if( fork_result == -1 ) 	// fork가 정상적으로 이루어지지 않았	을 때
		{
			fprintf(stderr, "Fork Failuer");
			exit(EXIT_FAILURE);
		}
		
		if( fork_result == 0)		// 자식프로세스
		{
			printf("자식)file_pipes[0] = %d\n", file_pipes[0]);
			printf("자식)file_pipes[1] = %d\n", file_pipes[1]);

			sprintf(buffer, "%d", file_pipes[0]);
			
			// buffer에 "%d", file_pipes[0]이 저장된다. (출력은 따로 X, printf(buffer))
/* sprintf()함수
 *	>> 리턴값은 끝 널문자를 계산하지 않고 배열에 작성된 바이트 수를 리턴한다.
 * Ex)
 * char buffer[200];
 * int i,j;
 * double fp;
 * char *s = "Baltimore"
 * char c;
 *
 * int main()
 * c = 'l';
 * i = 35;
 * fp = 3.141582;
 *
 * j = sprintf(buffer, "%s\n", s);
 * j += sprintf(buffer+j, "%c\n", c);
 * j += sprintf(buffer+j, "%d\n", i);
 * j += sprintf(buffer+j, "%f\n", fp);
 * printf("String: \n%s\ncharacter count = %d\n", buffer, j);
 *
 * [출력결과]
 * string:
 * Baltimore
 * l
 * 35
 * 1.732051
 *
 *
 * character count = 24
 */
			execl("pipe4", "pipe4", buffer, (char *)0);
			exit(EXIT_SUCCESS);
			
		}
		else // 부모프로세스
		{
			printf("부모)file_pipes[0] = %d\n", file_pipes[0]);
			printf("부모)file_pipes[1] = %d\n", file_pipes[1]);

			data_processed = write(file_pipes[1], some_data, strlen(some_data));
			printf("%d - wrote %d bytes\n", getpid(), data_processed);
		}
		
	}
	

	exit(EXIT_SUCCESS);
}


	
/* [출력결과]
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./pipe3
 * 2527 - wrote 4 bytes
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./pipe3
 * 2529 - wrote 4 bytes
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./pipe3
 * 2531 - wrote 4 bytes
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./pipe3
 * 2533 - wrote 4 bytes
 * (실행할 때 마다 pid가 2씩 증가하는구나!!)
 *
 *
 * [???????] pipe4를 만들었더니 출력결과가 달라졌다!!
 *
 */
