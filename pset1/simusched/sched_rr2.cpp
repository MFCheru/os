#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	quantum = argn[1];
	cycles = new int[argn[0]];
	fill_n(cycles, argn[0], quantum);
}

SchedRR2::~SchedRR2() {
	delete[] cycles;
}


void SchedRR2::load(int pid) {
	int cpu = 0;
	q.at(cpu).push(pid);
}

void SchedRR2::unblock(int pid) {
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
	if (m == EXIT) {
		// Si el pid actual terminó, sigue el próximo.
		if (q.at(cpu).empty()) return IDLE_TASK;
		else {
			int sig = q.at(cpu).front(); q.at(cpu).pop();
			cycles[cpu] = quantum;
			return sig;
		}
	} else {
		if (current_pid(cpu) == IDLE_TASK && !q.at(cpu).empty()) {
			int sig = q.at(cpu).front(); q.at(cpu).pop();
			cycles[cpu] = quantum;
			return sig;
		} else {
			cycles[cpu]--;

			if (cycles[cpu] == 0) {
				if (q.at(cpu).empty()) {
					cycles[cpu] = quantum;
					return current_pid(cpu);
				} else {
					int sig = q.at(cpu).front(); q.at(cpu).pop();
					q.at(cpu).push(current_pid(cpu)); // re-add to queue
					cycles[cpu] = quantum;
					return sig;
				}
			} else {
				return current_pid(cpu);
			}
		}
	}
}
