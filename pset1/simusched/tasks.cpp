#include "tasks.h"

#include <stdlib.h>
#include <time.h>
#include <algorithm>

using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

// my tasks

void TaskConsola(int pid, vector<int> params) {

	srand(1); // set seed

	for (int i = 0; i < params[0]; ++i) {
		int t = params[1] + rand() % (params[2] - params[1] + 1);
		uso_IO(pid, t);
	}

}

void TaskBatch(int pid, vector<int> params) {

	srand(1); // set seed

	int total_cpu = params[0];
	int cant_bloqueos = params[1];

	bool config[total_cpu];
	fill_n(config, total_cpu, false);
	for (int i = 0; i < cant_bloqueos; i++) config[i] = true;
	random_shuffle(&config[0], &config[total_cpu-1]);

	for (int i = 0; i < total_cpu; ++i) {
		if (config[i] == true) { // block
			uso_IO(pid, 1);
		} else {
			uso_CPU(pid, 1);
		}
	}
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(TaskBatch, 2);
}
