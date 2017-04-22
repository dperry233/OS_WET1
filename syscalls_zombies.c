#include <linux/sched.h>

int sys_set_max_zombies(int  max_z, pid_t pid) {
	task_t* p = find_task_by_pid(pid);
	if(p==NULL){
		return -1;
	}
	p->max_zombies= max_z;
	p->curr_zombies= 0;
	INIT_LIST_HEAD(&zombie_list.list); 

	return 0;
}


int sys_get_max_zombies() {
	return current->max_zombies;
}




int sys_get_zombies_count(pid_t pid) {
task_t* p = find_task_by_pid(pid);
	if(p==NULL){
		return -1;
	}
	return p->curr_zombies;
}


pid_t sys_get_zombie_pid(int n) {
	if(current->max_zombies){
		return -2;   /*  wrapper handles actual return values from pdf   */
	}
	if(n> current->curr_zombies){
		return -1;
	}


	/* continue here , function not done!!!!!!!!  */

}


int sys_give_up_zombie(int n, pid_t adopter_pid) {

}