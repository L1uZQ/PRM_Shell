
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include "process_resource_management.hpp"
#include <list>
using namespace std;

void test_shell()
{
    string command, line;

	string defaultpath = "./";
	string path = "";
    //  path = defaultpath;
	ifstream infile(path + "input.txt");
	if (!infile.is_open()) {
		cout << "wrong file" << endl;
	}

	ofstream outfile;
	outfile.open(path + "output.txt", ios::out);

	string name;
	int priority, unit;
	PRM prm;
	cout << "init" <<endl;
	outfile << "init" << " ";
	while (getline(infile, line)) {
		istringstream linestream(line);

		if (!line.empty() && line.length() >= 2) {

			linestream >> command;

			if (command == "init") {
				prm.restore();
				cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
				outfile << prm.pcb[prm.current_running].pid << " ";
			}
			else if (command == "quit") {
				break;
			}
			else if (command == "cr") {
				linestream >> name >> priority;
				if (prm.contain(name) != -1) {
					cout << "error (duplicate name)" << " ";
					outfile << "error (duplicate name)" << " ";
				}
				else if (priority > 2 or priority <= 0) {
					cout << "error" << " ";
				}
				else {
					prm.create(name, priority);
					cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
					outfile << prm.pcb[prm.current_running].pid << " ";
				}
			}
			else if (command == "de") {
				linestream >> name;
				int t = prm.contain(name);
				if (t == -1) {
					cout << "error (process not existed)" << " ";
					outfile << "error (process not existed)" << " ";
				}
				else {
					prm.destroy(t);
					cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
					outfile << prm.pcb[prm.current_running].pid << " ";
				}
			}
			else if (command == "req") {
				linestream >> name >> unit;
				if (name == "R1" && unit == 1) {
					prm.request(0, unit);
					cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
					outfile << prm.pcb[prm.current_running].pid << " ";
				} else if (name == "R2" && (0 < unit && unit <= 2)) {
					prm.request(1, unit);
					cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
					outfile << prm.pcb[prm.current_running].pid << " ";
				} else if (name == "R3" && (0 < unit && unit <= 3)) {
					prm.request(2, unit);
					cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
					outfile << prm.pcb[prm.current_running].pid << " ";
				} else if (name == "R4" && (0 < unit && unit <= 4)) {
					prm.request(3, unit);
					cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
					outfile << prm.pcb[prm.current_running].pid << " ";
				} else {
					cout << "error (invalid request)" << " ";
					outfile << "error (invalid request)" << " ";
				}
			}
			else if (command == "rel") {
				linestream >> name >> unit;
				if (name == "R1" && unit == 1) {
					if (prm.pcb[prm.current_running].other_resource[0].used >= unit) {
						prm.release_scheduler(0, unit);
						cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
						outfile << prm.pcb[prm.current_running].pid << " ";
					} else { cout << "error (release exceed actual R1 units)" << " ";
							outfile << "error (release exceed actual R1 units)" << " ";}
				} else if (name == "R2" && (0 < unit && unit <= 2)) {
					if (prm.pcb[prm.current_running].other_resource[1].used >= unit) {
						prm.release_scheduler(1, unit);
						cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
						outfile << prm.pcb[prm.current_running].pid << " ";
					} else { cout << "error (release exceed actual R2 units)" << " ";
							outfile << "error (release exceed actual R2 units)" << " ";}
				} else if (name == "R3" && (0 < unit && unit <= 3)) {
					if (prm.pcb[prm.current_running].other_resource[2].used >= unit) {
						prm.release_scheduler(2, unit);
						cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
						outfile << prm.pcb[prm.current_running].pid << " ";
					} else { cout << "error (release exceed actual R3 units)" << " ";
							outfile << "error (release exceed actual R3 units)" << " ";}
				} else if (name == "R4" && (0 < unit && unit <= 4)) {
					if (prm.pcb[prm.current_running].other_resource[3].used >= unit) {
						prm.release_scheduler(3, unit);
						cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
						outfile << prm.pcb[prm.current_running].pid << " ";
					} else { cout << "error (release exceed actual R4 units)" << " ";
							outfile << "error (release exceed actual R4 units)" << " ";}
				}
				else {
					cout << "error (invalid release)" << " ";
					outfile << "error (invalid release)" << " ";
				}

			}
			else if (command == "to") {
				prm.timeout();
				cout <<"运行进程 "<< prm.pcb[prm.current_running].pid <<endl;
				outfile << prm.pcb[prm.current_running].pid << " ";
			}
			else {
				cout << "error (else error)" << " ";
				outfile << "error (else error)" << " ";
			}
		} else {
			cout << endl;
			outfile << endl;
		}
	}

	cout << "end" << endl;

	infile.close();
	outfile.close();
}