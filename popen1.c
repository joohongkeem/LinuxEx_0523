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
	FILE *read_fp;
	char buffer[BUFSIZ + 1];
	// BUFSIZ는 '메모리 효율에 나쁘지 않은 정도의 크기'로서 선언되어 있다.
	int chars_read;


	memset(buffer, '\0', sizeof(buffer));

	read_fp = popen("uname -a", "r"); 
		// process open
		// uname -a 명령어의 주소를 read_fp에 넣는다?!
		// uname -a 명령어는 현재 사용중인 커널 버전 확인
	if(read_fp!=NULL)
	{
		chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
		printf("fread 반환값 : %d\n",chars_read);
		if(chars_read>0){
			printf("Output was : -\n%s\n", buffer);
		}
		pclose(read_fp);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}

/* [출력결과]
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./popen1
 * Output was : -
 * Linux Hongsberry 4.14.34-v7+ #1110 SMP Mon Apr 16 15:18:51 BST 2018 armv7l GNU/Linux
 *
 */
