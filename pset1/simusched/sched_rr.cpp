#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>


#include <unistd.h>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	quantum = new int[argn.size()-1];

	for (int i = 1; i < (int) argn.size(); i++) {
		quantum[i-1] = argn[i];
	}

	cycles = new int[argn[0]];
}

SchedRR::~SchedRR() {
	delete[] cycles;
	delete[] quantum;
}


void SchedRR::load(int pid) {
	q.push(pid);
}

void SchedRR::unblock(int pid) {
	q.push(pid);
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	if (m == EXIT || m == BLOCK) {
		// Si el pid actual terminó, sigue el próximo.
		if (q.empty()) return IDLE_TASK;
		else {
			int sig = q.front(); q.pop();
			cycles[cpu] = quantum[cpu];
			return sig;
		}
	} else {
		if (current_pid(cpu) == IDLE_TASK && !q.empty()) {
			int sig = q.front(); q.pop();
			cycles[cpu] = quantum[cpu];
			return sig;
		} else {
			cycles[cpu]--;

			if (cycles[cpu] == 0) {
				if (q.empty()) {
					cycles[cpu] = quantum[cpu];
					return current_pid(cpu);
				} else {
					int sig = q.front(); q.pop();
					q.push(current_pid(cpu)); // re-add to queue
					cycles[cpu] = quantum[cpu];
					return sig;
				}
			} else {
				return current_pid(cpu);
			}
		}
	}
}