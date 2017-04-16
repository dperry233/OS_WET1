#include <errno.h>
#include <termios.h>



int set_max_zombies(int  max_z, pid_t pid) {
	unsigned int res(-1);

	if(max_z<0){
		errno = EINVAL;
		return -1
	}

	if(pid<0){
		errno = ESRCH;
		return -1
	}

	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (243), "b" (max_z), "c"(pid)
		:"memory"
		);

	if ( res < 0) {
		errno = (-res);	
		return -1;
	}
	return res;
}

int get_max_zombies() {
	unsigned int res(-1);


	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (244)
		:"memory"
		);

	if ( res < 0) {
		errno = EINVAL;	
		return -1;
	}
	return res;
}

int get_zombies_count(pid_t pid) {
	unsigned int res(-1);

	if(pid<0){
		errno = ESRCH;
		return -1
	}

	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (245), "b"(pid)
		:"memory"
		);

	if ( res < 0) {
		errno = (-res);	
		return -1;
	}
	return res;
}

pid_t get_zombie_pid(int n) {
	
	pid_t res(-1)


	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (246), "b"(n)
		:"memory"
		);


	if(res==-1){
		errno = ESRCH;
		return -1
	}

	if ( res == -2) {
		errno = EINVAL;	
		return -1;
	}
	return res;
}

int set_max_zombies(int  n, pid_t adopter_pid) {
	unsigned int res(-1);



	if(adopter_pid<0){
		errno = ESRCH;
		return -1
	}

	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (247), "b" (n), "c"(adopter_pid)
		:"memory"
		);

	if ( res == -1) {
		errno = EINVAL);	
		return -1;
	}

	return res;
}