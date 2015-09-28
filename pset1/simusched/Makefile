export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_rr2.o sched_no_mistery.o
MAIN=simusched

.PHONY: all clean new ejercicio1 ejercicio2 ejercicio3 ejercicio4 ejercicio5 ejercicio6 ejercicio7 ejercicio8
all: $(MAIN)

$(MAIN): $(OBJS) sched_mistery.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h basesched.h
basesched.o: basesched.cpp basesched.h
basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h \
 sched_rr.h sched_rr2.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

clean:
	rm -f $(OBJS) $(MAIN)
	rm -rf ejercicio1 ejercicio2 ejercicio3 sejercicio5 ejercicio6 ejercicio7 ejercicio8

new: clean all

ejercicio1:
	make
	mkdir -p ejercicio1
	./simusched tasks/1.tsk 1 0 0 SchedFCFS | python graphsched.py > ejercicio1/1.png && eog ejercicio1/1.png &

ejercicio2:
	make
	mkdir -p ejercicio2
	./simusched tasks/2.tsk 1 4 0 SchedFCFS | python graphsched.py > ejercicio2/2_1nucleo.png  && eog ejercicio2/2_1nucleo.png &
	./simusched tasks/2.tsk 2 4 0 SchedFCFS | python graphsched.py > ejercicio2/2_2nucleos.png && eog ejercicio2/2_2nucleos.png &

ejercicio3:
	make
	mkdir -p ejercicio3
	./simusched tasks/3.tsk 1 4 0 SchedFCFS | python graphsched.py > ejercicio3/3.png && eog ejercicio3/3.png &

ejercicio4:
	make
	mkdir -p ejercicio4
	./simusched tasks/4.tsk 2 2 0 SchedRR 5 10 | python graphsched.py > ejercicio4/4.png && eog ejercicio4/4.png &

ejercicio5:
	make
	mkdir -p ejercicio5
	./simusched tasks/5.tsk 1 2 0 SchedRR 2 | python graphsched.py > ejercicio5/5_quantum2.png && eog ejercicio5/5_quantum2.png &
	./simusched tasks/5.tsk 1 2 0 SchedRR 10 | python graphsched.py > ejercicio5/5_quantum10.png && eog ejercicio5/5_quantum10.png &
	./simusched tasks/5.tsk 1 2 0 SchedRR 50 | python graphsched.py > ejercicio5/5_quantum50.png && eog ejercicio5/5_quantum50.png &
	./simusched tasks/5a.tsk 1 2 0 SchedRR 2 | python graphsched.py > ejercicio5/5a_quantum2.png && eog ejercicio5/5a_quantum2.png &
	./simusched tasks/5a.tsk 1 2 0 SchedRR 10 | python graphsched.py > ejercicio5/5a_quantum10.png && eog ejercicio5/5a_quantum10.png &
	./simusched tasks/5a.tsk 1 2 0 SchedRR 50 | python graphsched.py > ejercicio5/5a_quantum50.png && eog ejercicio5/5a_quantum50.png &

ejercicio6:
	make
	mkdir -p ejercicio6
	./simusched tasks/5.tsk 1 2 0 SchedFCFS 2 | python graphsched.py > ejercicio6/6_quantum2.png && eog ejercicio6/6_quantum2.png &
	./simusched tasks/5.tsk 1 2 0 SchedFCFS 10 | python graphsched.py > ejercicio6/6_quantum10.png && eog ejercicio6/6_quantum10.png &
	./simusched tasks/5.tsk 1 2 0 SchedFCFS 50 | python graphsched.py > ejercicio6/6_quantum50.png && eog ejercicio6/6_quantum50.png &

ejercicio7:
	make
	mkdir -p ejercicio7
	./simusched tasks/mistery.tsk 1 0 0 SchedMistery 5 6 7 8 9 10 | python graphsched.py > ejercicio7/mist.png && eog ejercicio7/mist.png &
	./simusched tasks/mistery.tsk 1 0 0 SchedNoMistery 5 6 7 8 9 10 | python graphsched.py > ejercicio7/no_mist.png && eog ejercicio7/no_mist.png &
	./simusched tasks/mistery2.tsk 1 0 0 SchedMistery 5 6 7 8 9 10 | python graphsched.py > ejercicio7/mist2.png && eog ejercicio7/mist2.png &
	./simusched tasks/mistery2.tsk 1 0 0 SchedNoMistery 5 6 7 8 9 10 | python graphsched.py > ejercicio7/no_mist2.png && eog ejercicio7/no_mist2.png &
	./simusched tasks/mistery3.tsk 1 0 0 SchedMistery 5 6 7 8 9 10 | python graphsched.py > ejercicio7/mist3.png && eog ejercicio7/mist3.png &
	./simusched tasks/mistery3.tsk 1 0 0 SchedNoMistery 5 6 7 8 9 10 | python graphsched.py > ejercicio7/no_mist3.png && eog ejercicio7/no_mist3.png &

ejercicio8:
	make
	mkdir -p ejercicio8
	./simusched tasks/8.tsk 2 2 0 SchedRR2 20 40 | python graphsched.py > ejercicio8/8_quantumRR2.png && eog ejercicio8/8_quantumRR2.png &
	./simusched tasks/8.tsk 2 2 0 SchedRR 20 40 | python graphsched.py > ejercicio8/8_quantumRR.png && eog ejercicio8/8_quantumRR.png &
	./simusched tasks/8a.tsk 2 2 0 SchedRR2 20 40 | python graphsched.py > ejercicio8/8a_quantumRR2.png && eog ejercicio8/8a_quantumRR2.png &
	./simusched tasks/8a.tsk 2 2 0 SchedRR 20 40 | python graphsched.py > ejercicio8/8a_quantumRR.png && eog ejercicio8/8a_quantumRR.png &

