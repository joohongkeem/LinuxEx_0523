/*
 * FILE *popen(const char *command, const char *open_mode);
 * int pclose(FILE *stream_to_close);
 *
 * [popen]
 * -한 프로그램이 새로운 프로세스로 다른 프로그램을 호출하고,
 *  데이터를 전달하거나 받아들이게 해준다
 * -open_mode : "r" or "w"
 *  		r - fread와 같은 stdio 라이브러리 함수로 읽어들일 수 있음
 * 		w - fwrite에 대한 호출을 통해 호출된 명령으로 데이터를 보낼 수 있다.
 *
 * [pclose]
 * -popen으로 시작된 프로세스를 pclose를 사용하여 파일 스트림을 닫을 수 있다
 * -pclose가 호출될 때 프로세스가 여전히 실행중이면 pclose 호출은 프로세스가
 *  끝날 때까지 대기할 것이다.
 * -pclose 호출은 일반적으로 파일 스트림을 닫는 프로세스의 exit code를 반환한다.
 */


/* - popen call은 먼저 쉘인 sh를 호출하고,
 *   command string을 인수로 전달하여 요청한 프로그램을 실행한다.
 * - 시스템 리소스 측면에서 popen 함수는 비효율적이다.
 *   Why? popen을 호출할 때마다 요청된 프로그램 뿐 아니라 쉘도 실행된다.
 *        그래서 popen에 대한 호출은 두 개의 extra process가 실행된다.
 */

#include <stdio.h>	// 필요한 헤더
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
	FILE *write_fp;
	char buffer[BUFSIZ + 1];	// BUFSIZ는 이미 선언되어 있다?!?! 사이즈가 몇일까?!?!?
	
	sprintf(buffer,"Once upon a time, there was...\n");
	write_fp = popen("od -c", "w");
	// od -c 명령어는 파일을 열어서 하나씩 char단위로 끊어서 보여주는 명령어
	// 터미널창에서 od -c ~~~.c 치면 대충 알 수 있다.
	
	// 먼저 od -c라는 명령어를 실행할 수 있는 프로그램을 호출해주고,
	// 그 file discriptor를 write_fp에 넣는다.

	if(write_fp != NULL)
	{
		fwrite(buffer, sizeof(char), strlen(buffer), write_fp);
		// buffer의 내용을 write_fp에 쓴다.
		//  >> 이 과정을 통해 od -c라는 프로세서에 입력값이 들어가므로 명령어가 실행된다.
		//
		// 이거 주석처리하면 buffer에는 잘들어가지만, od -c는 실행 X

		puts("");
		puts("Q.buffer엔 뭐가 들었을까?");
		puts(buffer);

		pclose(write_fp);
		exit(EXIT_SUCCESS);
	}

	exit(EXIT_FAILURE);
}

/* [출력결과]
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./popen2
 * 0000000   O   n   c   e       u   p   o   n       a       t   i   m   e
 * 0000020   ,       t   h   e   r   e       w   a   s   .   .   .  \n
 * 0000037
 *
 * [참고]
 * $ echo "Once upon a time, there was..."|od -c
 * 라고 입력해도 동일한 결과 출력!
 */

