#include "stdlib.h"
#include "stdio.h"
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct _sequence{
	unsigned char size;
	float * values;
} SEQUENCE;

void analyze_sequence(int filedesc, SEQUENCE *sequence, int index){
	unsigned char size;
	int i;
	float val;

	read(filedesc, &size, 1);
	sequence[index].size=size;
	sequence[index].values=(float*)malloc(size * sizeof(float));

	for (i=0; i<size; i++){
		read(filedesc, &val, sizeof(val));
		sequence[index].values[i]=val;
	}
}

float average(SEQUENCE sequence){
	float sum;
	int i;
	
	sum=0;
	for(i=0; i<sequence.size; i++){
		sum=sum+sequence.values[i];
	}
	
	return sum/sequence.size;
}

void analyze(char *fileName){
	int filedesc,i,j,filesize;
	float sum_of_avgs, *avgs, n;
	unsigned char dlzka;
	SEQUENCE *sequences;
	
	sum_of_avgs=0;
	filedesc=open(fileName, O_RDONLY);
	if (filedesc<0){
		return;
	}
	
	filesize=lseek(filedesc,0,SEEK_END);
	lseek(filedesc,0,SEEK_SET);
	for(i=0;i<filesize;i++){
		read(filedesc,&dlzka,sizeof(unsigned char));
		lseek(filedesc,sizeof(float)*dlzka,SEEK_CUR);
		n++;
		i=i+sizeof(float)*dlzka;
	}
	printf("pocet: %d\n",n);
	lseek(filedesc,0,SEEK_SET);
	sequences=malloc(n*sizeof(SEQUENCE));
	avgs=malloc(n*sizeof(float));
	for (i=0; i<n; i++){
		analyze_sequence(filedesc, sequences, i);
	}
	
	for (i=0; i<n; i++){
		avgs[i]=average(sequences[i]);
		printf("avgs: %.2lf\n",avgs[i]);
		sum_of_avgs=sum_of_avgs+avgs[i];
	}
	printf("sum of avgs: %.2lf\n",sum_of_avgs);

	printf("Priemer priemerov: %.2lf\n", sum_of_avgs / n);
	printf("Priemery:\n");
	for (i=0; i<n; i++){
		printf("%.2lf\n", avgs[i]);
	}

	printf("\n");
	for (i=0; i<n; i++){
		SEQUENCE sequence=sequences[i];
		for (j=0; j<sequence.size; j++){
			printf("%.2lf ", sequence.values[j]);
		}
	}

	close(filedesc);
}

int main(int argc, char **argv){
	char *file;
	
	file=argv[1];
	analyze(file);

	return 0;
}
