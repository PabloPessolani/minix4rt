# Makefile for the tests.

CC =	exec cc
CFLAGS= -O -D_MINIX -D_POSIX_SOURCE

OBJ=	test1  test2  test3  test4  test5  test6  test7  test8  test9  \
	test10        test12 test13 test14 test15 test16 test17 test18 test19 \
	       test21 test22 test23        test25 test26 test27 test28 test29 \
	test30 test31 test32        test34 test35 test36 test37 test38 test39 \
	test40 t10a t11a t11b 

MRT=  mrtstart mrtstop mrtflags \
	mrttest1 mrttest1b mrttest1c mrttest2 mrttest3 mrttest3b mrttest3c \
	mrttest4 mrttest4b mrttest5 mrttest5b mrttest5c mrttest6 mrttest6b \
  	mrttest6c mrttest6d mrttest6e mrttest7 mrttest7b mrttest8 mrttest8b \
	mrttest8c mrttest8d mrttest8e mrttest8f mrttest8g mrttest9 mrttest10 \
	mrttest11 mrttest12 mrttest12b mrttest12c mrttest12d mrttest12e mrttest12f\
      mrttest13 \
      mrtperf1 mrtperf2 mrtperf3 mrtperf4 \
      mrtperf1t mrtperf2t mrtperf3t mrtperf4t \
      mrtperf1tp mrtperf2tp mrtperf3tp \
      mrtperf1tph mrtperf2tph mrtperf3tph \
	mrtperf5 mrtperf5t 


BIGOBJ=  test20 test24

ROOTOBJ= test11 test33

all:	$(OBJ) $(MRT) $(BIGOBJ) $(ROOTOBJ)

mrt:	$(MRT)

$(OBJ):
	$(CC) $(CFLAGS) -o $@ $@.c
	install -S 10kw $@

$(MRT):
	$(CC) $(CFLAGS) -o $@ $@.c
	install -S 10kw $@

$(BIGOBJ):
	$(CC) $(CFLAGS) -o $@ $@.c
	install -S 32kw $@

$(ROOTOBJ):
	$(CC) $(CFLAGS) $@.c
	install -c -S 10kw -o root -m 4755 a.out $@
	rm a.out

clean:	
	@rm -f *.o *.s *.bak test? test?? t10a t11a t11b DIR* \
            mrttest[1-9] mrttest[1-9][a-z] mrtperf[1-9]\
		mrtperf[1-9]t mrtperf[1-9]tp mrtperf[1-9]tph \
		mrtstart mrtstop

test1:	test1.c
test2:	test2.c
test3:	test3.c
test4:	test4.c
test5:	test5.c
test6:	test6.c
test7:	test7.c
test8:	test8.c
test9:	test9.c
test10:	test10.c
t10a:	t10a.c
test11:	test11.c
t11a:	t11a.c
t11b:	t11b.c
test12:	test12.c
test13:	test13.c
test14:	test14.c
test15:	test15.c
test16:	test16.c
test17:	test17.c
test18:	test18.c
test19:	test19.c
test20:	test20.c
test21:	test21.c
test22:	test22.c
test23:	test23.c
test24:	test24.c
test25:	test25.c
test26:	test26.c
test27:	test27.c
test28:	test28.c
test29:	test29.c
test30:	test30.c
test31:	test31.c
test32:	test32.c
test33:	test33.c
test34:	test34.c
test35:	test35.c
test36:	test36.c
test37:	test37.c
test38:	test38.c
test39:	test39.c
test40:	test40.c
mrttest1:	mrttest1.c
mrttest1b:	mrttest1b.c
mrttest1c:	mrttest1c.c
mrttest2:	mrttest2.c
mrttest3:	mrttest3.c
mrttest3b:	mrttest3b.c
mrttest3c:	mrttest3c.c
mrttest4:	mrttest4.c
mrttest4b:	mrttest4b.c
mrttest5:	mrttest5.c
mrttest5b:	mrttest5b.c
mrttest5c:	mrttest5c.c
mrttest6:	mrttest6.c
mrttest6b:	mrttest6b.c
mrttest6c:	mrttest6c.c
mrttest6d:	mrttest6d.c
mrttest6e:	mrttest6e.c
mrttest7:	mrttest7.c
mrttest7b:	mrttest7b.c
mrttest8:	mrttest8.c
mrttest8b:	mrttest8b.c
mrttest8c:	mrttest8c.c
mrttest8d:	mrttest8d.c
mrttest8e:	mrttest8e.c
mrttest8e:	mrttest8f.c
mrttest8g:	mrttest8g.c
mrttest9:	mrttest9.c
mrttest10:	mrttest10.c
mrttest11:	mrttest11.c
mrttest12:	mrttest12.c
mrttest12b:	mrttest12b.c
mrttest12c:	mrttest12c.c
mrttest12d:	mrttest12d.c
mrttest12e:	mrttest12e.c
mrttest12f:	mrttest12f.c
mrttest13:	mrttest13.c
mrttest14:	mrttest14.c
mrttest15:	mrttest15.c
mrttest16:	mrttest16.c
mrttest17:	mrttest17.c
mrtstart:	mrtstart.c
mrtflags:	mrtflags.c
mrtstop:	mrtstop.c

mrtperf1:	mrtperf1.c
mrtperf1t:	mrtperf1t.c
mrtperf1tp:	mrtperf1tp.c
mrtperf1tph: mrtperf1tph.c
mrtperf2:	mrtperf2.c
mrtperf2t:	mrtperf2t.c
mrtperf2tp:	mrtperf2tp.c
mrtperf2tph: mrtperf2tph.c
mrtperf3:	mrtperf3.c
mrtperf3t:	mrtperf3t.c
mrtperf3tp:	mrtperf3tp.c
mrtperf3tph: mrtperf3tph.c
mrtperf4:	mrtperf4.c
mrtperf4t:	mrtperf4t.c
mrtperf5:	mrtperf5.c
mrtperf5t:	mrtperf5t.c

