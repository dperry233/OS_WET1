#include <errno.h>
#include <termios.h>

typedef enum {
    ALLOW_FORK = 0,
    BLOCK_FORK = 1
} plevel;

int set_privilege(plevel priv, pid_t pid) {
	int __res;
	__asm__(
		"int $0x80;"
		: "=a" (__res)
		: "0" (243), "b" (priv), "c"(pid)
		:"memory"
		);
	if ((__res) < 0) {
		errno = (-__res); 
		return -1;
	}
	return __res;
}

plevel get_privilege() {
    int __res;
	__asm__(
		"int $0x80;"
		: "=a" (__res)
		: "0" (244)
		:"memory"
		);
	if ((__res) < 0) {
		errno = (-__res); 
		return -1;
	}
	return __res;
}
