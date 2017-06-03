#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 200

typedef struct sequnce{
	float value;
	float avrg;
	unsigned char size;
}SEQUENCE;

float average(SEQUENCE *sequence){
	float sum;
	int i;
	
	sum=0;
	for(i=0;i<sequence->size;i++){
		sum=sum+sequence->value;
	}
	
	return sum/sequence->size;
}

SEQUENCE *analyze_sequence(int filedesc){
	float value;
	int i,filesize;
	unsigned char size;
	SEQUENCE *sequence;
	
	read(filedesc, &size, 1);
	sequence->size=size;
	sequence->avrg=0.;
	sequence=malloc(size*sizeof(float));
	
	for(i=0;i<size;i++){
		read(filedesc, &value, sizeof(value));
		sequence->value=value;
		sequence->avrg=sequence->avrg+value;
	}
	
	sequence->avrg=sequence->avrg/sequence->size;
	close(filedesc);
	return sequence;
}

void analyze(char *filename){
	int i, filedesc, filesize, *avrgs;
	float sum_avgs;
	SEQUENCE *sequence, **floats;
	
	sum_avgs=0.;
	filedesc=open(filename, O_RDONLY);
	if (filedesc<0){
		return;
	}
	
	filesize=lseek(filedesc,0,SEEK_SET);
	sequence=malloc(filesize*sizeof(sequence));
	avrgs=malloc(filesize*sizeof(float));
	
	for(i=0; i<filesize;i++){
		floats[i]=analyze_sequence(filedesc);
	}
	
	for(i=0;i<filedesc;i++){
		avrgs[i]=average(floats[i]);
		sum_avgs=sum_avgs+avrgs[i];
	}
	
	
	
}
int main(int argc, char **argv){
	char *file;
	int n;
	
	srand(time(NULL));
	if (argc!=2){
		printf("Zly pocet argumentov");
		return 1;
	}
	file=argv[1];
	n=atoi(argv[2]);
	
	analyze(file);	
}
