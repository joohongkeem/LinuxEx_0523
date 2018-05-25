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
int main()
{
	int data_processed;
	int file_pipes[2];
	const char some_data[] = "1234";
	const char some_data2[] = "5678";
	char buffer[BUFSIZ + 1];
	pid_t fork_result;
	int status; 	// wait함수를 위한 변수


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

		if( fork_result == -1 ) 	// fork가 정상적으로 이루어지지 않았을 때
		{
			fprintf(stderr, "Fork Failuer");
			exit(EXIT_FAILURE);
		}
		
		if( fork_result == 0)		// 자식프로세스
		{
			data_processed = read(file_pipes[0], buffer, BUFSIZ);	// 자식은 read,[0]
			printf("[자식]Read %d bytes : %s\n", data_processed, buffer);

			data_processed = write(file_pipes[1], some_data2, strlen(some_data2));
			printf("[자식]Wrote %d bytes\n", data_processed);
			exit(EXIT_SUCCESS);
			
			// 자식프로세스에선
			// [1]에 write, [0]에 read
	
		}
		else // 부모프로세스
		{
			//sleep(2);
			data_processed = write(file_pipes[1], some_data, strlen(some_data));
										// 부모는 write,[1]
			
			sleep(2);
			printf("[부모]Wrote %d bytes\n", data_processed);
				
	
			data_processed = read(file_pipes[0], buffer, BUFSIZ);	// 부모의 read,[0]
			printf("[부모]Read %d bytes : %s\n", data_processed, buffer);
			
			// wait(&status);
			// 요거 무의미함
		

			// 부모프로세스에선
			// [1]에 write, [0]에 read
		}
		//wait(&status);
		
	}
	

	exit(EXIT_SUCCESS);
}



/* [출력결과]
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./pipe2
 * [자식]Read 4 bytes : 1234
 * [자식]Wrote 4 bytes
 * (2초대기)
 * [부모]Wrote 4 bytes
 * [부모]Read 4 bytes : 5678
 *
 * [해볼 것]
 * - sleep, wait위치를 요래조래 바꿔가면서 출력결과 예상하고 직접 확인해보기
 *
 *
 * [주의사항]
 * 항상 부모프로세스가 먼저 실행되진 않는다, 근데 왜 잘 출력됐을까?
 * read의 IO가 'blocking IO'이기 때문 - write가 있을때까지 대기한다!
 */
