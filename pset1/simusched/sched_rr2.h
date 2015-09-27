#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include <map>
#include "basesched.h"

class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        ~SchedRR2();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
	private:
		int* quantum;
		int* cycles;
		std::vector<std::queue<int> > q;
		std::vector<int> totalLoad; // buscar otra estructura?
		std::map<int,int> CPUBlockedTask;

		int getCPU();
};

#endif
