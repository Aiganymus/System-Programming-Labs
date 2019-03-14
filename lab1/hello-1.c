/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/sched.h>
#include <asm/current.h>

int init_module(void)
{
	struct task_struct *my_parent = current->parent;
	pid_t task_pid = my_parent->pid;

	printk(KERN_INFO "!!!!PID is %d", task_pid);

   // struct list_head children = current->parent->children;
    struct list_head *cur;
    struct task_struct *child;
    list_for_each(cur, my_parent->children) {
       child = list_entry(cur, struct task_struct, sibling);
       printk(KERN_INFO "Child pid: %d.", child->pid);
    }


	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Hello: goodbye.\n");
}
