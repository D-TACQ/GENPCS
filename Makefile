# Makefile for GENPCS

CFLAGS+=-Wall
ifdef $(DEBUG)
CFLAGS+=-g
endif
CFLAGS+=-O1

all: acq
#all: acq mod pcs tests apps

acq:	
	$(MAKE) cleano
	$(MAKE) $(DRYRUN) CFLAGS="$(CFLAGS) -DGEN_ACQ" acq_sngl acq_dual acq_multi

mod:	
	make cleano
	$(MAKE) $(DRYRUN) CFLAGS="$(CFLAGS) -DGEN_MOD" mod_sngl mod_dual mod_multi
	
pcs:	
	$(MAKE) cleano
	$(MAKE) $(DRYRUN) CFLAGS="$(CFLAGS) -DGEN_ACQ -DGEN_MOD " $pcs_sngl pcs_dual

nul:	
	$(MAKE) cleano
	$(MAKE) $(DRYRUN) nul_sngl nul_dual

apps: mdsput splitu32 LOG


# do NOT make these products singly, use top level commands above
acq_sngl: test_acq_sngl.o GENPCS_stub.o acq.o linux_rt.o 
	$(CC) $(CFLAGS) -o $@ $^
	
acq_dual: test_acq_dual.o GENPCS_stub.o acq.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^
	
acq_multi: test_acq_multi.o GENPCS_stub.o acq.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^
	
mod_sngl: test_mod_sngl.o GENPCS.o acq_stub.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^

mod_dual: test_mod_dual.o GENPCS.o acq_stub.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^
	
pcs_sngl: test_mod_sngl.o GENPCS.o acq.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^
	
pcs_dual: test_mod_dual.o GENPCS.o acq.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^
	
nul_sngl: test_acq_sngl.o GENPCS_stub.o acq_stub.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^
	
nul_dual: test_acq_dual.o GENPCS_stub.o acq_stub.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^

tests: pig GENPCS_print_consts
	
pig: pig.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^

test_acq_sngl.o: test_sngl.c
	$(CC) $(CFLAGS) -c -o $@ $^

test_acq_dual.o: test_dual.c
	$(CC) $(CFLAGS) -c -o $@ $^
	
test_acq_multi.o: test_multi.c
	$(CC) $(CFLAGS) -c -o $@ $^	

test_mod_sngl.o: test_sngl.c
	$(CC) $(CFLAGS) -c -o $@ $^

test_mod_dual.o: test_dual.c
	$(CC) $(CFLAGS) -c -o $@ $^

acq_stub.o: acq.c	
	$(CC) $(CFLAGS) -c -o $@ $^

CXXFLAGS+= -I/usr/local/mdsplus/include -g
mdsput:	 mdsput.o 
	$(CXX) $(CFLAGS) -o mdsput mdsput.o \
	-L/usr/local/mdsplus/lib/ -lMdsObjectsCppShr

mdsput.o : mdsput.cpp

LOG:
	mkdir -p LOG

# cleano : remove conditional compile object (for clean ident)
cleano: FORCE 
	rm -Rf test*.o
		
clean: FORCE
	rm -f *.o *_sngl *_dual *_multi pig GENPCS_print_consts
	
FORCE:
