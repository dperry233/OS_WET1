#include <linux/sched.h>

/**
 * sys_set_max_zombies:
 */
int sys_set_max_zombies(int max_z, pid_t pid) {
	task_t* p = find_task_by_pid(pid);
	if(!p){
		return -3; // if no procces found the return value sould be -3 (the value of ESRCH)
	}
	p->max_zombies = max_z;
	p->curr_zombies = 0;
	/*not sure about this line. the specific zombie_list should be initialized however*/
	INIT_LIST_HEAD(p->zombies->list);

	return 0;
}

/**
 * sys_get_max_zombies:
 */
int sys_get_max_zombies() {
	return current->max_zombies;
}

/**
 * sys_get_zombies_count:
 */
int sys_get_zombies_count(pid_t pid) {
task_t* p = find_task_by_pid(pid);
	if(!p){
		return -3; // if no procces found the return value sould be -3 (the value of ESRCH)
	}
	return p->curr_zombies;
}

/**
 * sys_get_zombie_pid:
 */
pid_t sys_get_zombie_pid(int n) {
	if(!(current->max_zombies)){	// if max_zombies=-1 that is?
		return -22;   	/*  the value of EINVAL is 22  */
	}
	if(n >= current->curr_zombies){ //if there are n zombies the last one is the (n-1) (starting with 0).
		return -3;		/*  the value of ESRCH is 3  */
	}
	struct list_head *pos=current->zombies->(&list);
	for(int i=0;i<n;i++){
		pos=pos->next;
	}
	/* not sure at all */
	return (list_entry(pos, *(zombie_list), list))->pid;
}


int sys_give_up_zombie(int n, pid_t adopter_pid) {

}
