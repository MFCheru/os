#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

#include <stdio.h>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	quantum = new int[argn.size()-1];

	for (int i = 0; i < argn[0]; ++i) {
		q.push_back(queue<int>());
		totalLoad.push_back(0);
	}

	for (int i = 1; i < (int) argn.size(); i++) {
		quantum[i-1] = argn[i];
	}

	cycles = new int[argn[0]];
}

SchedRR2::~SchedRR2() {
	delete[] cycles;
	delete[] quantum;
}


void SchedRR2::load(int pid) {
	int cpu = getCPU();
	q.at(cpu).push(pid);
	totalLoad[cpu]++;
}

void SchedRR2::unblock(int pid) {
	q.at(CPUBlockedTask[pid]).push(pid);
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
	if (m == EXIT || m == BLOCK) {
		// Si la tarea termino, se la quita de la carga total.
		// Si se bloqueo, se toma nota de la CPU actual para poder
		// agregarla nuevamente cuando se desbloquee.
		if (m == EXIT) {
			totalLoad[cpu]--;
		} else {
			CPUBlockedTask[current_pid(cpu)] = cpu;
		}

		// Si el pid actual terminó, sigue el próximo.
		if (q.at(cpu).empty()) return IDLE_TASK;
		else {
			int sig = q.at(cpu).front(); q.at(cpu).pop();
			cycles[cpu] = quantum[cpu];
			return sig;
		}
	} else {
		if (current_pid(cpu) == IDLE_TASK && !q.at(cpu).empty()) {
			int sig = q.at(cpu).front(); q.at(cpu).pop();
			cycles[cpu] = quantum[cpu];
			return sig;
		} else {
			cycles[cpu]--;

			if (cycles[cpu] == 0) {
				if (q.at(cpu).empty()) {
					cycles[cpu] = quantum[cpu];
					return current_pid(cpu);
				} else {
					int sig = q.at(cpu).front(); q.at(cpu).pop();
					q.at(cpu).push(current_pid(cpu)); // re-add to queue
					cycles[cpu] = quantum[cpu];
					return sig;
				}
			} else {
				return current_pid(cpu);
			}
		}
	}
}

/* This could probably be improved with a more efficent
 * data structure.
 */
int SchedRR2::getCPU() {
	int cpu = 0;
	int i = 1;
	for (vector<int>::iterator it = ++totalLoad.begin() ; it != totalLoad.end(); ++it) {
		if (*it < totalLoad.at(cpu)) {
			cpu = i;
		}
		i++;
	}
	return cpu;
}
