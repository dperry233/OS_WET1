#include <linux/sched.h>

/**
 * sys_set_max_zombies:
 */
int sys_set_max_zombies(int max_z, pid_t pid) {
	task_t* p = find_task_by_pid(pid);
	if(!p){
		return -3; // the value of ESRCH
	}
	p->max_zombies = max_z;
	p->curr_zombies = 0;
	INIT_LIST_HEAD(&((p->zombies)->list));
	return 0;
}

/**
 * sys_get_max_zombies:
 */
int sys_get_max_zombies(void) {
	if(current->max_zombies==-1){
		return -3; // the value of ESRCH
	}
	return current->max_zombies;
}

/**
 * sys_get_zombies_count:
 */
int sys_get_zombies_count(pid_t pid) {
	task_t* p = find_task_by_pid(pid);
	if(!p){
		return -3; // the value of ESRCH
	}
	if(p->max_zombies == -1){
		return 0;
	}
	return p->curr_zombies;
}

/**
 * sys_get_zombie_pid:
 */
pid_t sys_get_zombie_pid(int n) {
	if(n >= current->curr_zombies){
		return -22;		// the value of EINVAL
	}
	if((current->max_zombies)==-1){
		return -22;   	// the value of EINVAL
	}
	struct list_head *pos;
	int i=0;
	list_for_each(pos, &(current->zombies->list)){
		if(i==n){
			break;
		}
		i++;
	}
	return (list_entry(pos, struct zombie_list_t, list))->pid;
}

/**
 * sys_give_up_zombie:
 */
int sys_give_up_zombie(int n, pid_t adopter_pid) {
	if(n > current->curr_zombies){
		return -22;		// the value of EINVAL
	}
	if(adopter_pid<0){
		return -3;		// the value of ESRCH
	}
	task_t* adopter_ptr = find_task_by_pid(adopter_pid);
	if(!adopter_ptr){
		return -3; 		// the value of ESRCH
	}
	if( n + adopter_ptr->curr_zombies > adopter_ptr->max_zombies){
		return -22;		// the value of EINVAL
	}
	if((adopter_ptr->max_zombies)==-1){	
		return -22;   	// the value of EINVAL
	}
	struct list_head *pos, *q;
	int i=0;
	list_for_each_safe(pos,q, &(current->zombies->list)){
		if(i==n){
			break;
		}
		i++;
		list_add_tail(&(pos), &(adopter_ptr->zombies->list));
		list_del(pos);
	}
	adopter_ptr->curr_zombies+=n;
	current->curr_zombies-=n;
	return 0;
}
