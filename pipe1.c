/* The Pipe Call
 * - popen은 high-level function, pipe는 lower-level function
 * - 쉘을 호출하지 않고도 요청된 명령을 변환하며, 두 프로그램 간에 전달하는 방법을 제공한다.
 *
 *   #include <unistd.h>
 *   int pipe(int file_descriptor[2]);
 *   >> 파이프는 단방향이므로 쓰는방향, 읽는방향의 2가지 디스크립터를 선언해 주어야 한다.
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

int main()
{
	int data_processed;
	int file_pipes[2];
	const char some_data[] = "1234";
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


	if( pipe(file_pipes) == 0 ) // 파이프가 정상적으로 만들어 졌다면?
	{
		data_processed = write(file_pipes[1], some_data, strlen(some_data));
		printf("Wrote %d bytes\n", data_processed);
		data_processed = read(file_pipes[0], buffer, BUFSIZ);
		printf("Read %d bytes : %s\n", data_processed, buffer);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}


/* [출력결과]
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./pipe1
 * Wrote 4 bytes
 * Read 4 bytes : 1234
 *
 */

