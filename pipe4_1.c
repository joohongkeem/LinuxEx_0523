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
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	int data_processed;
	int file_descriptor;
	char buffer[BUFSIZ + 1];

	fd = open("pipe1.c", O_RDONLY);
	printf("fd = %d\n", fd);	//fd가 5번으로 뜬다!? XXXX 3으로뜬다.

	memset(buffer, '\0', sizeof(buffer));
	sscanf(argv[1], "%d", &file_descriptor);
	
	data_processed = read(file_descriptor, buffer, BUFSIZ);

	printf("%d - read %d bytes : %s\n", getpid(), data_processed, buffer);

	
	exit(EXIT_SUCCESS);
}
	




