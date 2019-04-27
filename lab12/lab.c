#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/fs_struct.h>
#include <linux/fdtable.h>
#include <linux/nsproxy.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>

#include <linux/kthread.h> 
#include <linux/path.h>
#include <linux/mount.h>
#include <linux/mount.h>
#include <linux/dcache.h>
#include <linux/moduleparam.h>
#include <linux/pid.h>
#include <linux/pid_namespace.h>
#include <linux/list.h>

#define TAG "Week 12"

static int pid = -100;
module_param(pid, int, 0660);

static struct task_struct *thread;

void fs1(void) {
    struct task_struct *cur_proc;
    int used_files_cnt = 0;
    if(pid == -100) pid = current->pid;
    cur_proc = pid_task(find_get_pid(pid), PIDTYPE_PID);
    
    printk(KERN_INFO "Process ID is %d\n", cur_proc->pid);
    printk(KERN_INFO "Process name %s\n", cur_proc->comm);

    printk(KERN_INFO "Process fs pointer: %p\n", cur_proc->fs);
    printk(KERN_INFO "Process files pointer: %p\n", cur_proc->files);
    printk(KERN_INFO "Process nsproxy pointer: %p\n", cur_proc->nsproxy);

    used_files_cnt = cur_proc->files->count.counter;

    printk(KERN_INFO "Files used: %d\n", used_files_cnt);
    
    // part 2
    struct path root = cur_proc->fs->root;
    struct path pwd = cur_proc->fs->pwd;
    printk(KERN_INFO "Root path mnt: %p\n", root.mnt);
    printk(KERN_INFO "root of the mounted tree: %p\n", root.mnt->mnt_root);
    printk(KERN_INFO "pointer to superblock: %p\n", root.mnt->mnt_sb);
    printk(KERN_INFO "Pwd path mnt: %p\n", pwd.mnt);
    printk(KERN_INFO "root of the mounted tree: %p\n", pwd.mnt->mnt_root);
    printk(KERN_INFO "pointer to superblock: %p\n", pwd.mnt->mnt_sb); 
    printk(KERN_INFO "*****************\n");
    
    // part 3
    printk(KERN_INFO "Root path dentry: %p\n", root.dentry);
    printk(KERN_INFO "Pwd path dentry: %p\n", pwd.dentry);
    printk(KERN_INFO "Root path dentry name: %s\n", root.dentry->d_iname);
    printk(KERN_INFO "Pwd path dentry name: %s\n", pwd.dentry->d_iname);
    printk(KERN_INFO "*****************\n");
    
    // part 4
    struct dentry *entry;
    if (pwd.dentry != NULL) {
        printk(KERN_INFO "list head subdirs: %p\n", &pwd.dentry->d_subdirs);
        list_for_each_entry(entry, &pwd.dentry->d_subdirs, d_child) {
            printk(KERN_INFO "name of the entry: %s\n", entry->d_name.name);
            if (&entry->d_subdirs || &entry->d_child) {
                printk(KERN_INFO "path dentry name: %s\n", entry->d_iname);
            }
        }
    }
    
/*    struct list_head *cur;
    struct dentry *entryy;
    list_for_each(cur, &(root.dentry->d_subdirs)) {
        entryy = list_entry(cur, struct dentry, d_subdirs);
        printk(KERN_INFO "name of the entry: %s\n", entryy->d_iname);
    }*/
    
    printk(KERN_INFO "*****************\n");
}

int thread_function(void *data) {
    fs1();
    return 0;
}

int init_module(void) {
    printk(KERN_INFO "Starting %s\n", TAG);
    thread = kthread_run(thread_function, NULL, "myThread");

	if (thread) 
		printk(KERN_INFO "Thread is created!");
	else
		printk(KERN_INFO "Thread is Not created!");

    return 0;
}

void cleanup_module(void) {
   printk(KERN_INFO "Cleanup %s\n", TAG);
}

MODULE_LICENSE("GPL");