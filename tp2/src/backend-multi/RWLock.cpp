#include "RWLock.h"

/* IMPORTANTE: Se brinda una implementación básica del Read-Write Locks
que hace uso de la implementación provista por pthreads. Está dada para
que puedan utilizarla durante la adaptación del backend de mono a multi
jugador de modo de poder concentrarse en la resolución de un problema
a la vez. Una vez que su adaptación esté andando DEBEN hacer su propia
implementación de Read-Write Locks utilizando únicamente Variables de
Condición. */

RWLock :: RWLock() {

	pthread_mutex_init(&(this->lock_mutex), NULL);
	pthread_mutex_init(&(this->lock_writer), NULL);
	pthread_mutex_init(&(this->lock_reader), NULL);
	pthread_cond_init (&(this->condition), NULL);
	writer = false;
	readers = 0;
}

void RWLock :: rlock() {

	pthread_mutex_lock(&(this->lock_mutex));

	while (writer) 
		pthread_cond_wait(&(this->condition), &(this->lock_mutex));

	pthread_mutex_lock(&(this->lock_reader));
	readers++;
	pthread_mutex_unlock(&(this->lock_reader));
	
	pthread_mutex_unlock(&(this->lock_mutex));
}

void RWLock :: wlock() {

	pthread_mutex_lock(&(this->lock_mutex));
	
	while (writer)
		pthread_cond_wait(&(this->condition), &(this->lock_mutex));

	pthread_mutex_lock(&(this->lock_writer));
	writer = true;
	pthread_mutex_unlock(&(this->lock_writer));

	while (readers > 0) 
		pthread_cond_wait(&(this->condition), &(this->lock_mutex));
	
	
	pthread_mutex_unlock(&(this->lock_mutex));
}

void RWLock :: runlock() {

	pthread_mutex_lock(&(this->lock_mutex));
	
	pthread_mutex_lock(&(this->lock_reader));
	readers--;
	pthread_mutex_unlock(&(this->lock_reader));

	if (readers == 0)
		pthread_cond_broadcast(&(this->condition));
	
	pthread_mutex_unlock(&(this->lock_mutex));
}

void RWLock :: wunlock() {

	pthread_mutex_lock(&(this->lock_writer));
	writer = false;
	pthread_mutex_unlock(&(this->lock_writer));

	pthread_cond_broadcast(&(this->condition));
}
