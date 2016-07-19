#include <mdsobjects.h>
using namespace MDSplus;
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* usage mdsput tree node [fname] */

int fileLen(const char* fname)
{
	struct stat buf;
	if (stat(fname, &buf) == 0){
		return buf.st_size;
	}else{
		return 0;
	}
}
template <class T, class M> 
class MdsPutThing {
	T* mydata;
	int lenw;
public:
	MdsPutThing() : mydata(0) {}
	virtual ~MdsPutThing() {
		if (mydata) delete [] mydata;
	}
private:	
	void getData(const char* fname) {
		FILE* fp = fopen(fname, "r");
		if (!fp){
			fprintf(stderr, "ERROR failed to open \"%s\"\n", fname);
			exit(1);
		}
		lenw = fileLen(fname)/sizeof(T);
		if (lenw == 0){
			fprintf(stderr, "ERROR zero len\n");
			exit(1);
		}
		mydata = new T[lenw];
		if (fread(mydata, sizeof(T), lenw, fp) != lenw){
			fprintf(stderr, "ERROR failed to read %d items\n", lenw);
			exit(1);
		}
	}
public:
	MdsPutThing(const char* fname) {
		getData(fname);
	}

	int operator() (const char* treename, const char* nodename) {
		Tree *tree = new Tree(treename, 0);
		TreeNode *node = tree->getNode(nodename);
		Array *array = new M(mydata, lenw);
		node->putData(array);
		return 0;
	}
};


int main(int argc, char* argv[])
{
	if (argc < 4){
		fprintf(stderr, "USAGE: mdsput tree node [fname]\n");
	}else{
		MdsPutThing<unsigned, Uint32Array> mds(argv[3]);
		return mds(argv[1], argv[2]);
	}
}
