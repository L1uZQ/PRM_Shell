#ifndef PROCESS_RESOURCE_MANAGEMENT_HPP_
#define PROCESS_RESOURCE_MANAGEMENT_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <fstream>
#include <string.h>
#include <queue>
#include <list>

struct PCB;

class LN {
public:
	LN() : next(nullptr) {}
	LN(const LN& ln) : value(ln.value), next(ln.next) {}
	LN(PCB* v, LN* n = nullptr) : value(v), next(n) {}
	PCB* value;
	LN* next;
};

struct RCB {
	std::string rid = "Ri";
	int initial = 0;
	int remaining = 0;
	std::list<int> wait_list;
};

struct resource {
	int rid = -1;
	int used = 0;
	int wait_request = 0;
};
struct PCB {
	std::string pid = " ";
	std::string  type = "ready";
	int id = -1;
	int parent = -1;
	int children = -1;
	int younger = -1;
	int older = -1;
	int priority = -1;
	resource other_resource[4];
};


class PRM {

private:
	RCB R1;
	RCB R2;
	RCB R3;
	RCB R4;
	std::list<int> readylist[3];

	int delete_number = -1;

public:
	PRM();
	~PRM();
	void restore();
	void create(std::string name, int p);
	int scheduler();
	void destroy(int n);

	int contain(std::string);
	bool is(const int& value);
	void request(int n, int unit);
	void release_scheduler(int n, int unit);
	void release(int n, int unit);

	void timeout();

	int current_running;
	RCB rcb[4];
	PCB pcb[20];

};

void PRM::timeout() {
	int p = pcb[current_running].priority;
	readylist[p].remove(current_running);
	pcb[current_running].type = "ready";
	readylist[p].push_back(current_running);
	scheduler();
}

void PRM::release(int n, int unit) {
	pcb[current_running].other_resource[n].used -= unit;
	rcb[n].remaining+= unit;
	int temp_pcb = rcb[n].wait_list.front();
	while (temp_pcb != 0 && pcb[temp_pcb].other_resource[n].wait_request <= rcb[n].remaining) {
		rcb[n].remaining -= pcb[temp_pcb].other_resource[n].wait_request;
		rcb[n].wait_list.remove(temp_pcb);
		pcb[temp_pcb].type = "ready";
		pcb[temp_pcb].other_resource[n].used += pcb[temp_pcb].other_resource[n].wait_request;
		readylist[pcb[temp_pcb].priority].push_back(temp_pcb);
		temp_pcb = rcb[n].wait_list.front();
	}
}

void PRM::release_scheduler(int n, int unit) {
	release(n, unit);
	scheduler();
}

void PRM::request(int n, int unit) {
	if (rcb[n].remaining >= unit) {
		rcb[n].remaining = rcb[n].remaining - unit;
		pcb[current_running].other_resource[n].rid = n;
		pcb[current_running].other_resource[n].used += unit;
	}
	else {
		pcb[current_running].type = "blocked";
		pcb[current_running].other_resource[n].wait_request += unit;
		rcb[n].wait_list.push_back(current_running);
		readylist[pcb[current_running].priority].remove(current_running);
	}
	scheduler();
}
bool PRM::is (const int& value) { return (value == delete_number); }

void PRM::destroy(int n) {
	for (int i = 0; i < 4; i++) {
		if (pcb[n].other_resource[i].used != 0) {
			release(i ,pcb[n].other_resource[i].used);
			//释放进程占用的资源
			if (rcb[i].remaining > rcb[i].initial) {
				std::cout << "error in destroy: delete resources exit initial units" << std::endl;
			}
			pcb[n].other_resource[i].rid = -1;
			pcb[n].other_resource[i].used = 0;
		}
	}
	//如果进程处于就绪状态或运行中，则移出就绪队列
	if (pcb[n].type == "ready" or pcb[n].type == "running") {
		int p = pcb[n].priority;
		readylist[p].remove(n);
	}
	//如果进程在阻塞状态，移出阻塞队列
	else if ((pcb[n].type).compare("blocked") == 0) {
		for (int i = 0; i < 4; i++) {
			rcb[i].wait_list.remove(n);
		}
	}
	for (int i = 0; i < 20; i++) {
		if (pcb[i].parent == n) {
			destroy(pcb[i].id);
		}
		if (pcb[i].id == n) {
			pcb[i].pid = " ";
			pcb[i].type = "ready";
			pcb[i].parent = -1;
			pcb[i].children = -1;
			pcb[i].older = -1;
			pcb[i].younger = -1;
			pcb[i].priority = -1;
			pcb[i].other_resource[0].rid = -1;
			pcb[i].other_resource[1].rid = -1;
			pcb[i].other_resource[2].rid = -1;
			pcb[i].other_resource[3].rid = -1;
			pcb[i].other_resource[0].used = 0;
			pcb[i].other_resource[1].used = 0;
			pcb[i].other_resource[2].used = 0;
			pcb[i].other_resource[3].used = 0;
			pcb[i].other_resource[0].wait_request = 0;
			pcb[i].other_resource[1].wait_request = 0;
			pcb[i].other_resource[2].wait_request = 0;
			pcb[i].other_resource[3].wait_request = 0;
		}
		if (pcb[i].older == n) {
			pcb[i].older = -1;
		}
		if (pcb[i].younger == n) {
			pcb[i].younger = -1;
		}
	}
	scheduler();
}
int PRM::contain(std::string name) {
	for (int i = 0; i < 20; i++) {
		if (name.compare(pcb[i].pid) == 0) {
			return i;
		}
	}
	return -1;
}

int PRM::scheduler() {
	if (!readylist[2].empty()) { //先调度就绪队列2
		current_running = readylist[2].front();
		pcb[current_running].type = "running";
		return readylist[2].front();
	} else if (!readylist[1].empty()) { //再调度就绪队列1
		current_running = readylist[1].front();
		pcb[current_running].type = "running";
		return readylist[1].front();
	} else {
		current_running = 0;
		pcb[current_running].type = "running";
		return 0;
	}
}
void PRM::create(std::string name, int p) {
	for (int i = 1; i < 20; i++) {
		if (pcb[i].pid == " ") {
			pcb[i].pid = name;
			pcb[i].priority = p;
			readylist[p].push_back(pcb[i].id);
			pcb[i].parent = current_running;
			for (int j = 1; j < 20; j++) {
				if (j < i && pcb[j].parent == pcb[i].parent) {
					pcb[j].younger = i;
					pcb[i].older = j;
				}
			}
			break;
		}
	}
	scheduler();
}

void PRM::restore() {
	for (int i = 1; i < 20; i++) {
		pcb[i].pid = " ";
		pcb[i].type = "ready";
		pcb[i].parent = -1;
		pcb[i].children = -1;
		pcb[i].older = -1;
		pcb[i].younger = -1;
		pcb[i].priority = -1;
		pcb[i].other_resource[0].rid = -1;
		pcb[i].other_resource[1].rid = -1;
		pcb[i].other_resource[2].rid = -1;
		pcb[i].other_resource[3].rid = -1;
		pcb[i].other_resource[0].used = 0;
		pcb[i].other_resource[1].used = 0;
		pcb[i].other_resource[2].used = 0;
		pcb[i].other_resource[3].used = 0;
		pcb[i].other_resource[0].wait_request = 0;
		pcb[i].other_resource[1].wait_request = 0;
		pcb[i].other_resource[2].wait_request = 0;
		pcb[i].other_resource[3].wait_request = 0;
	}
	current_running = 0;

	for (int i = 0; i < 4; i++) {
		rcb[i].initial = i+1;
		rcb[i].remaining = i+1;
		while(!rcb[i].wait_list.empty()) {
			rcb[i].wait_list.pop_front();
		}
	}
	for (int i = 0; i < 3; i++) {
		while (!readylist[i].empty()) {
			readylist[i].pop_back();
		}
	}
}

PRM::PRM() {
	for (int i = 0; i < 20; i++) {
		pcb[i].id = i;
	}
	//std::cout << "init" << " ";
	pcb[0].pid = "init";
	pcb[0].priority = 0;
	readylist[0].push_back(0);
	current_running = 0;

	R1.rid = "R1";
	R1.initial = 1;
	R1.remaining = 1;

	R2.rid = "R2";
	R2.initial = 2;
	R2.remaining = 2;

	R3.rid = "R3";
	R3.initial = 3;
	R3.remaining = 3;

	R4.rid = "R4";
	R4.initial = 4;
	R4.remaining = 4;

	rcb[0] = R1;
	rcb[1] = R2;
	rcb[2] = R3;
	rcb[3] = R4;

}

PRM::~PRM() {}



#endif /* PROCESS_RESOURCE_MANAGEMENT_HPP_ */
