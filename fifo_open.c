/* open(const char *path, O_RDONLY);
 * open(const char *path, O_RDONLY | O_NONBLOCK);
 * open(const char *path, O_WRONLY);
 * open(const char *path, O_WRONLY | O_NONBLOCK);
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/my_fifo"

int main(int argc, char *argv[])	// 0에 ./어쩌구저쩌구 1에 다음인자, 2에 다음인자 ...
{
	int res;
	int open_mode = 0;
	int i;

	if(argc < 2)
	{
		fprintf(stderr, "Usage : %s <some combination of O_RDONLY O_WRONLY O_NONBLOCK>\n", *argv);
		exit(EXIT_FAILURE);
	}

	for(i=1; i<argc; i++)
	{
		if(strncmp(*++argv, "O_RDONLY", 8) == 0)
			open_mode |= O_RDONLY;
		if(strncmp(*argv, "O_WRONLY", 8) ==0)
			open_mode |= O_WRONLY;
		if(strncmp(*argv, "O_NONBLOCK", 10) ==0)
			open_mode |= O_NONBLOCK;
	}

	if(access(FIFO_NAME,F_OK) == -1)
	{
		res = mkfifo(FIFO_NAME, 0777);
		if(res != 0)
		{
			fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
			exit(EXIT_FAILURE);
		}
	}
	
	printf("Process %d opening FIFO\n", getpid());
	res = open(FIFO_NAME, open_mode);
	printf("Process %d result %d \n", getpid(),res);
	sleep(5);

	if(res!=-1)
		(void)close(res);
	printf("Process %d finished \n", getpid());
	exit(EXIT_SUCCESS);
}

/* [출력결과]
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./fifo_open O_RDONLY &
 * [1] 4665
 * Process 4665 opening FIFO
 * pi@Hongsberry:~/RaspiEx/LinuxEx_Ras/Execute $ ./fifo_open O_WRONLY
 * Process 4668 opening FIFO
 * Process 4665 result 3 
 * Process 4668 result 3 
 * Process 4665 finished 
 * Process 4668 finished 
 * [1]+  Done                    ./fifo_open O_RDONLY
 *
 *
 */
