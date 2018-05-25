/* - pipe는 서로 관련이 없는 프로세스들의 데이터 교환에 불편하다
 * - command line program
 *   $mknod filename p
 *   $mkfifo filename
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	int res = mkfifo("/tmp/my_fifo", 0777);
	if(res == 0) printf("FIFO created\n");
	exit(EXIT_SUCCESS);
}

/* [출력결과]
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./fifo1
 * FIFO created
 *
 * 이다음부턴 실행해도 아무변화 X
 *
 * 무슨 의미인가?
 * - /tmp/에 my_fifo라는 fifo가 생성되었다!!
 *
 * /
