#ifndef __SCHED_NOMIS__
#define __SCHED_NOMIS__

#include <vector>
#include <queue>
#include <list>
#include "basesched.h"

class SchedNoMistery : public SchedBase {
  public:
    SchedNoMistery(std::vector<int> argn);
    virtual void load(int pid);
    virtual void unblock(int pid);
    virtual int tick(int cpu, const enum Motivo m);
  private:
	std::vector<int> quantum_list;
	std::vector<int> quantum_curr;
	std::list<int> q;
	int cycles_left;
	int not_executed;

	// int next_pid();
};

#endif
