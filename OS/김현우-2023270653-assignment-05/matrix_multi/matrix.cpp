#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int argc, char** argv)
{
	int row, column, i, j;
	char file[50];
	FILE *fp = stdout; 

	printf("Number of Rows  : ");
	scanf("%d", &row);
	printf("Number of Column: ");
	scanf("%d", &column);
	printf("Out file name   : ");
	scanf("%s", &file);	

	fp = fopen(file, "w");
	srand( (unsigned int)time(NULL) );
	for(i=0; i<row; i++)
	{
		for(j=0; j<column; j++)
		{

			fprintf(fp, "%5d", rand() % 7 - 3 );

		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	printf("]]###########: Program Terminated Successfully\n");		


	return 0;
}

