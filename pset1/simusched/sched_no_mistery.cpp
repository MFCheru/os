#include <vector>
#include <queue>
#include <stdio.h>
#include "sched_no_mistery.h"
#include "basesched.h"
#include <iostream>

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {
	// cpu cores, rest of params
	cycles_left = 1;
	current_queue = 0;
	tasks = 0;

	quantum_list.push_back(1);
	q.push_back(queue<int>());

	if (argn.size() > 1) {
		for (vector<int>::iterator it = ++argn.begin(); it != argn.end(); ++it) {
			quantum_list.push_back(*it);
			q.push_back(queue<int>());
		}
	}
}

void SchedNoMistery::load(int pid) {

	q.at(0).push(pid);
	tasks++;
}

void SchedNoMistery::unblock(int pid) {
	q.at(max(0,blockedQueue[pid]-1)).push(pid);
	blockedQueue.erase(pid);
	tasks++;
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {
	if (m == EXIT || m == BLOCK) {
		if (m == BLOCK) {
			blockedQueue[current_pid(cpu)] = current_queue;
		}
		
		tasks--;
		// current pid ended, get next
		if (tasks == 0) return IDLE_TASK;
		else { // get task from list
			return next_pid();
		}
	} else {
		if (current_pid(cpu) == IDLE_TASK && tasks > 0) {
			return next_pid();
		} else {
			cycles_left--;
			if (current_pid(cpu) != IDLE_TASK && cycles_left == 0) {

				current_queue = min(current_queue + 1, ((int) q.size()) - 1);
				if (tasks == 0) {
					cycles_left = quantum_list.at(current_queue);
					return current_pid(cpu);
				} else {
					q.at(current_queue).push(current_pid(cpu));
					return next_pid();
				}
			}
			return current_pid(cpu);
		}
	}
}

/* Requires some queue not to be empty */
int SchedNoMistery::next_pid() {
	int selectQueue = 0;
	for (vector<queue<int> >::iterator it = q.begin(); it != q.end(); ++it) {
		if ((*it).size() > 0) break;
		selectQueue++;
	}
	int pid = q.at(selectQueue).front();
	q.at(selectQueue).pop();
	current_queue = selectQueue;
	cycles_left = quantum_list.at(selectQueue);
	return pid;
}