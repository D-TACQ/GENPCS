#include <mdsobjects.h>
using namespace MDSplus;
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string.h>

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

class MdsPutter {
public:
	virtual int store (const char* fname, const char* treename, const char* nodename) = 0;

	static MdsPutter* create(const char* dtype);
};

template <class T, class M> 
class MdsPutThing: public MdsPutter {
	T* mydata;
	int lenw;
protected:
	MdsPutThing() : mydata(0) {}
	virtual ~MdsPutThing() {
		if (mydata) delete [] mydata;
	}
	friend class MdsPutter;
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

	int store (const char* fname, const char* treename, const char* nodename) {
		getData(fname);
		Tree *tree = new Tree(treename, 0);
		TreeNode *node = tree->getNode(nodename);
		Array *array = new M(mydata, lenw);
		node->putData(array);
		return 0;
	}
};

MdsPutter * MdsPutter::create(const char* dtype)
{
	if (strcmp(dtype, "s16") == 0){
		return new MdsPutThing<short, Int16Array>();
	}
	/* could be many more .. */

	return new MdsPutThing<unsigned, Uint32Array>();
}
int main(int argc, char* argv[])
{
	if (argc < 4){
		fprintf(stderr, "USAGE: mdsput tree node [fname] [dtype]\n");
	}else{
		const char* tree = argv[1];
		const char* node = argv[2];
		const char* fname = argv[3];
		const char* dtype = argc>4? argv[4]: "u32";

		MdsPutter * putter = MdsPutter::create(dtype);
		return putter->store(fname, tree, node);
	}
}
