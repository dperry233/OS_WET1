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
	INIT_LIST_HEAD(&((p->zombies)->list));

	return 0;
}

/**
 * sys_get_max_zombies:
 */
int sys_get_max_zombies(void) {
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
	struct list_head *pos;
		int i=1;
list_for_each(pos, &(current->zombies)->list){
		if(i>n){
			goto end_of_for1;
		}
		i=i+1;
		
		
	}
end_of_for1: 
	/* not sure at all */
	return (list_entry(pos, struct zombie_list_t, list))->pid;
}


int sys_give_up_zombie(int n, pid_t adopter_pid) {
	if((current->max_zombies)==-1){	
		return -22;   	/*  the value of EINVAL is 22  */
	}
	if(n >= current->curr_zombies){ //if there are n zombies the last one is the (n-1) (starting with 0).
		return -22;		/*  the value of ESRCH is 3  */
	}
	task_t* adopter_ptr = find_task_by_pid(adopter_pid);
	if(!adopter_ptr){
		return -3; // if no procces found the return value sould be -3 (the value of ESRCH)
	}
	if((adopter_ptr->max_zombies)==-1){	
		return -22;   	/*  the value of EINVAL is 22  */
	}
	if(n+adopter_ptr->curr_zombies >= adopter_ptr->max_zombies){ 
		return -22;		
	}
	int i=1;
	struct list_head *pos, *q;  /* q is used specifically for list_for_each_safe   */
	list_for_each_safe(pos,q, &(current->zombies)->list){
		if(i>n){
			goto end_of_for;
		}
		i=i+1;
		list_add_tail(&(pos), &(adopter_ptr->zombies->list));
		list_del(pos);
	}
end_of_for: 
	adopter_ptr->curr_zombies=adopter_ptr->curr_zombies+n;
	current->curr_zombies=current->curr_zombies -n;
	return 0;

}
