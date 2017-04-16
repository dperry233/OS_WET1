#include <linux/sched.h>

int sys_set_privilege(plevel priv, pid_t pid) {
    if (pid <= 1) {
        return -EINVAL;
    }
    if (priv != ALLOW_FORK && priv != BLOCK_FORK) {
        return -EINVAL;
    }
    if (current->priv != ALLOW_FORK) {
        return -EINVAL;
    }
    task_t* p = find_task_by_pid(pid);
    p->priv = priv;
    return 0;
}

plevel sys_get_privilege() {
    return current->priv;
}