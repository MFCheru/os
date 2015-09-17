#ifndef __SCHED_NOMIS__
#define __SCHED_NOMIS__

#include <vector>
#include <deque>
#include <map>
#include "basesched.h"

class SchedNoMistery : public SchedBase {
  public:
    SchedNoMistery(std::vector<int> argn);
    virtual void load(int pid);
    virtual void unblock(int pid);
    virtual int tick(int cpu, const enum Motivo m);
  private:
	std::vector<int> quantum_list;
	std::vector<std::queue<int> > q;
  std::map<int, int> blockedQueue;
	int cycles_left, current_queue, tasks;
	int next_pid(void);
};

#endif
