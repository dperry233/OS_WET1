#include <errno.h>
#include <termios.h>


/**
 * set_max_zombies: Sets a limit for the allowed number of zombies a process
 * with the given pid can have.
 *
 * @param max_z - the limit to set.
 * @param pid	- the pid of the process.
 *
 * @return:
 * 	0 if the limit was set successfully or -1 otherwise.
 * 	- if max_z<0 errno will get 'EINVAL'
 * 	- if pid<0 errno will get 'ESRCH'
 */
int set_max_zombies(int  max_z, pid_t pid) {
	if(max_z < 0){
		errno = EINVAL;
		return -1;
	}
	if(pid<0){
		errno = ESRCH;
		return -1;
	}
	int res = -1;
	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (243), "b" (max_z), "c"(pid)
		:"memory"
		);
	if (res < 0) {
		errno = (-res);
		return -1;
	}
	return res;
}

/**
 * get_max_zombies: returns the limit for the allowed number of zombies of the
 * process with the given pid .
 *
 * @return:
 * 	the limit for the allowed number of zombies,
 * 	or -1 if the limit is undefined
 * 	- if the limit is undefined errno will get 'ESRCH'.
 */
int get_max_zombies() {
	int res = -1;
	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (244)
		:"memory"
		);
	if (res < 0) {
		errno = (-res);
		return -1;
	}
	return res;
}

/**
 * get_zombies_count: returns the current number of zombies that the process
 * with the given pid has.
 *
 * @param pid	- the pid of the process.
 *
 * @return:
 * 	the number of zombies or -1 if the pid<0.
 * 	- if the pid<0.d errno will get 'ESRCH'.
 */
int get_zombies_count(pid_t pid) {
	if(pid < 0){
		errno = ESRCH;
		return -1;
	}
	int res = -1;
	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (245), "b"(pid)
		:"memory"
		);
	if (res < 0) {
		errno = (-res);
		return -1;
	}
	return res;
}

/**
 * get_zombie_pid: returns the pid of the n-th zombie among the children of the
 * calling process.
 *
 * @param n	- the serial number of the zombie process.
 *
 * @return:
 * 	the pid of the n-th zombie or -1 if the n is bigger then the current number
 * 	of zombies the procces has.
 * 	- if n is invalid errno will get 'ESRCH'.
 * 	- if no limit was set errno will get 'EINVAL'.
 */
pid_t get_zombie_pid(int n) {
	if (n < 0){
		errno = EINVAL ;
		return -1;
	}
	pid_t res = -1;
	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (246), "b"(n)
		:"memory"
		);
	if (res < 0){
		errno = (-res);
		return -1;
	}
	return res;
}

/**
 * give_up_zombie: moves the first n zombies from the calling process to the
 * process with the given pid.
 *
 * @param n				- the number of zombie to give.
 * @param adopter_pid	- the pid of the adopting process.
 *
 * @return:
 * 	0 if the zombies were moves successfully or -1 otherwise.
 * 	- if n is bigget than the zombies number of the calling process
 * 		errno will get 'EINVAL'.
 * 	- if the total number of zombies the adopter will have after the adoption is
 * 		bigger the his limit errno will get 'EINVAL'.
 * 	- if adopter_pid<0 errno will get 'ESRCH'.
 */
int give_up_zombie(int n, pid_t adopter_pid) {
	if (n < 0){
		errno = EINVAL ;
		return -1;
	}
	int res = -1;
	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (247), "b" (n), "c"(adopter_pid)
		:"memory"
		);
	if ( res < 0) {
		errno = (-res);
		return -1;
	}
	return res;
}
