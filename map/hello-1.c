/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */	
#include <linux/idr.h>
#include <linux/slab.h>
#include <linux/radix-tree.h>
#include <linux/gfp.h>
#include <linux/percpu.h>

struct person {
        char* name;
        int age;
};

int init_module(void)
{ 
    struct person *parent = kmalloc(sizeof(struct person), GFP_KERNEL);
    parent->name = "hello";
    parent->age = 20;

    struct idr idr_huh;
    idr_init(&idr_huh);
    
    int id;
    do {
        if (!ida_pre_get(&idr_huh, GFP_KERNEL))
            return -ENOSPC;
        ret = ida_get_new(&idr_huh, parent, &id);
        } while (ret == -EAGAIN);
	        return 0;
    }

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
