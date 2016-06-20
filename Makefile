# Makefile for ST40PCS

all: acq mod pcs

acq:	
	make cleano
	CFLAGS=-DST40_ACQ make $(DRYRUN) acq_sngl acq_dual

mod:	
	make cleano
	make CFLAGS=-DST40_MOD $(DRYRUN) mod_sngl mod_dual
	
pcs:	
	make cleano
	make CFLAGS="-DST40_ACQ -DST40_MOD " $(DRYRUN) pcs_sngl pcs_dual

nul:	
	make cleano
	make $(DRYRUN) nul_sngl nul_dual

acq_sngl: test_acq_sngl.o ST40PCS_stub.o acq.o linux_rt.o 
	$(CC) $(CFLAGS) -o $@ $^
	
acq_dual: test_acq_dual.o ST40PCS_stub.o acq.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^
	
mod_sngl: test_mod_sngl.o ST40PCS.o acq_stub.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^

mod_dual: test_mod_dual.o ST40PCS.o acq_stub.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^
	
pcs_sngl: test_mod_sngl.o ST40PCS.o acq.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^
	
pcs_dual: test_mod_dual.o ST40PCS.o acq.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^
	
nul_sngl: test_acq_sngl.o ST40PCS_stub.o acq.o linux_rt.o
	$(CC) $(CFLAGS) -o $@ $^
	
nul_dual: test_acq_dual.o ST40PCS_stub.o acq.o linux_rt.o

	$(CC) $(CFLAGS) -o $@ $^

test_acq_sngl.o: test_sngl.c
	$(CC) $(CFLAGS) -c -o $@ $^

test_acq_dual.o: test_dual.c
	$(CC) $(CFLAGS) -c -o $@ $^

test_mod_sngl.o: test_sngl.c
	$(CC) $(CFLAGS) -c -o $@ $^

test_mod_dual.o: test_dual.c
	$(CC) $(CFLAGS) -c -o $@ $^

acq_stub.o: acq.c	
	$(CC) $(CFLAGS) -c -o $@ $^

# cleano : remove conditional compile object (for clean ident)
cleano: FORCE 
	rm -Rf test*.o
		
clean: FORCE
	rm -f *.o *_sngl *_dual
	
FORCE:
