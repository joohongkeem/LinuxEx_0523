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

/* - exec call를 사용하면 자식 프로세스와 부모 프로세스와 다른 프로그램이 되게 한다.
 * - After an exec call, old process는 child process로 대체된다
 *   >> child process가 file_pipes 데이터의 사본을 사용할 수 없다
 *   >> 그러므로 file descriptor를 newly executed program의 파라미터로 전달함으로 해결 가능하다.
 */

int main(int argc, char *argv[])
{
	int data_processed;
	int file_descriptor;
	char buffer[BUFSIZ + 1];

	memset(buffer, '\0', sizeof(buffer));
/* void *memset(void *ptr, int value, size_t num);
 * >> 어떤 메모리의 시작점부터 연속된 범위를 어떤 값으로(바이트 단위) 모두 지정하고 싶을 때 사용
 *    특정 범위에 있는 연속된 메모리에 값을 지정하고 싶을 때 사용, for문보다 더 빠른속도 가능
 *
 * ptr : 채우고자 하는 메모리의 시작 포인터(시작 주소)
 * value : 메모리에 채우고자 하는 값, int형이지만 unsigned char(1byte)로 변환되어서 저장됨
 * num : 채우고자 하는 바이트 수, 즉 채우고자 하는 메모리의 크기
 */

	sscanf(argv[1], "%d", &file_descriptor);
/* int sscanf(const char *buffer, const char *format, argument-list);
 *
 * sscanf 함수는 buffer에서 argument-list가 제공하는 위치로 데이터를 읽는다.
 * 각 argument는 format-string에서 유형지정자에 대응하는 유형의 변수에 대한 포인터여야 한다.
 *
 * sscanf 함수는 성공적으로 변환 및 지정된 필드 수를 리턴한다
 * 리턴값은 읽었지만 지정되지 않은 필드는 포함되지 않는다. 
 * 리턴값은 변환 전에 스트링이 끝이 난 경우 EOF이다.
 *
 * ex) char *string = "ABC Z";
 *     char s[5];
 *     char c;
 *     sscanf(string, "%s %c", s, c); 하면 s에 ABC, c에 Z가 들어간다.
 */	
	data_processed = read(file_descriptor, buffer, BUFSIZ);

	printf("%d - read %d bytes : %s\n", getpid(), data_processed, buffer);

	
	exit(EXIT_SUCCESS);
}
	




