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
	q.push_back(pid);
 //   	list<int>::iterator it = q.begin();
 //    advance(it, not_executed);
	// q.insert(it,1,pid);

	quantum_curr.push_back(0);
	not_executed++;
}

void SchedNoMistery::unblock(int pid) {
	q.push_front(pid);
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {

	if (m == EXIT) {
		// current pid ended, get next
		if (q.empty()) return IDLE_TASK;
		else { // get task from list
			int pid = q.front(); q.pop_front();
			return pid;
		}
	} else if (m == BLOCK) {
		int pid = q.front();
		q.front() = current_pid(cpu);
		cycles_left = quantum_list.at(quantum_curr.at(pid));
		return pid;
	} else {
		if (current_pid(cpu) == IDLE_TASK && !q.empty()) {
			int pid = q.front(); q.pop_front();
			cycles_left = quantum_list.at(quantum_curr.at(pid));
			// not_executed = max(not_executed - 1, 0);
			return pid;
		} else {
			cycles_left--;

			if (cycles_left == 0) {
				quantum_curr[current_pid(cpu)] = min(quantum_curr.at(current_pid(cpu))+1,
												((int)quantum_list.size())-1);
				if (q.empty()) {
					cycles_left = quantum_list.at(quantum_curr.at(current_pid(cpu)));
					return current_pid(cpu);
				} else {
					// if (not_executed == 0) q.push_back(current_pid(cpu));
					q.push_back(current_pid(cpu));
					int pid = q.front(); q.pop_front();
					cycles_left = quantum_list.at(quantum_curr.at(pid));
					return pid;
				}
			} else {
				return current_pid(cpu);
			}
		}
	}
}

// int SchedNoMistery::next_pid() {
// 	int pid;
// 	if (not_executed > 0) { // new task
// 		list<int>::iterator it = q.begin();
// 		advance(it, q.size()-not_executed);
// 		pid = *it;
// 		cycles_left = 1;
// 		not_executed = max(not_executed - 1, 0);
// 	} else {
// 		pid = q.front(); q.pop_front();
// 		cycles_left = quantum_list.at(quantum_curr.at(pid));
// 	}

// 	return pid;
// }
		// q.remove(current_pid(cpu)); // in case of an empty task