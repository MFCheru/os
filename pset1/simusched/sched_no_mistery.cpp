#include <vector>
#include <queue>
#include <stdio.h>
#include "sched_no_mistery.h"
#include "basesched.h"

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {
	// cpu cores, rest of params
	cycles_left = 1;
	not_executed = 0;

	quantum_list.push_back(1);

	if (argn.size() > 1) {
		for (vector<int>::iterator it = ++argn.begin(); it != argn.end(); ++it) {
			quantum_list.push_back(*it);
		}
	}
}

void SchedNoMistery::load(int pid) {
	list<int>::iterator it = q.begin();
	advance(it, not_executed);
	q.insert(it,1,pid);
	not_executed++;

	quantum_curr.push_back(0);
}

void SchedNoMistery::unblock(int pid) {
	list<int>::iterator it = q.begin();
	advance(it, unblocks_left);
	q.insert(it,1,pid);
	unblocks_left++;
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {
	if (m == EXIT || m == BLOCK) {
		// current pid ended, get next
		if (q.empty()) return IDLE_TASK;
		else { // get task from list
			return next_pid();
		}
	} else {
		if (current_pid(cpu) == IDLE_TASK && !q.empty()) {
			return next_pid();
		} else {
			cycles_left--;

			if (current_pid(cpu) != IDLE_TASK && cycles_left == 0) {
				quantum_curr[current_pid(cpu)] = min(quantum_curr.at(current_pid(cpu))+1,
												((int)quantum_list.size())-1);
				if (q.empty()) {
					cycles_left = quantum_list.at(quantum_curr.at(current_pid(cpu)));
					not_executed = max(not_executed - 1, 0);
					return current_pid(cpu);
				} else {
					q.push_back(current_pid(cpu));
					return next_pid();
				}
			} else {
				return current_pid(cpu);
			}
		}
	}
}

int SchedNoMistery::next_pid() {
	int pid = q.front(); q.pop_front();
	if (unblocks_left > 0) {
		cycles_left = 1;
		unblocks_left--;
	} else {
		cycles_left = quantum_list.at(quantum_curr.at(pid));
	}
	not_executed = max(not_executed - 1, 0);

	return pid;
}