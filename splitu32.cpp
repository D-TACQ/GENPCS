#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int fileLen(const char* fname)
{
        struct stat buf;
        if (stat(fname, &buf) == 0){
                return buf.st_size;
        }else{
                return 0;
        }
}


int _splitu32(unsigned *raw, int lenw, FILE **fpout, int nsplit)
{
	for(int ii = 0; lenw--; ++raw, (ii = ii >= nsplit? 0: ii) ){
		fwrite(raw, sizeof(unsigned), 1, fpout[ii++]);
	}
}

int splitu32(const char* fname, int nsplit)
{
	int lenw = fileLen(fname)/sizeof(unsigned);
        if (lenw == 0){
                fprintf(stderr, "ERROR zero len\n");
                exit(1);
        }
	unsigned *raw = new unsigned[lenw];
	FILE *fp = fopen(fname, "r");
        if (!fp){
               fprintf(stderr, "ERROR failed to open \"%s\"\n", fname);
               exit(1);
        }
	if (fread(raw, sizeof(unsigned), lenw, fp) != lenw){
		fprintf(stderr, "ERROR failed to read %d items\n", lenw);
		exit(1);
	}
	fclose(fp);
	FILE** fpout = new FILE* [nsplit];
	for (int ii = 0; ii < nsplit; ++ii){
		char fno[128];
		sprintf(fno, "%s.%d", fname, ii);
		fpout[ii] = fopen(fno, "w");
		if (fpout[ii] == 0){
			fprintf(stderr, "ERROR failed to create \"%s\"\n", fno);
		}
	}
	return _splitu32(raw, lenw, fpout, nsplit);
}

int main(int argc, char* argv[])
{
	if (argc < 3){
		fprintf(stderr, "USAGE:splitu32 fname nsplit\n");
		exit(1);
	}
	return splitu32(argv[1], atoi(argv[2]));
}
