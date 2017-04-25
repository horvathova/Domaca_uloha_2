#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

float get_random_float(){
	return (float)rand() / (float)(RAND_MAX / 1024);
}

unsigned char get_random_range(unsigned char min, unsigned char max){
	float scaled_random;
	char return_value;
	
	scaled_random=(float)rand() / RAND_MAX;
	return_value=(max-min+1)*scaled_random+min;
	return return_value;
}

void generate_sequence(int filedesc, unsigned char size){
	int i;
	float random_value;
	
    printf("%d\n", size);
	write(filedesc, &size, 1);
	for (i=0; i<size; i++){
		random_value=get_random_float();
		write(filedesc, &random_value, sizeof(random_value));
	}
}

void generate(char * fileName, int n){
	int i,filedesc;
	unsigned char size;
	
	filedesc=open(fileName, O_WRONLY | O_CREAT);
    printf("%d\n", filedesc);
	if (filedesc<0){
		return;
	}

	for (i=0; i<n; i++){
		size=get_random_range(10, 20);
		generate_sequence(filedesc, size);
	}
	close(filedesc);
}

int main(int argc, char **argv){
	char *file;
	int n;
	
	srand(time(NULL));
	if (argc<3){
		printf("Malo argumentov");
		return 1;
	}
	file=argv[1];
	n=atoi(argv[2]);
	
	generate(file, n);

	return 0;
}
