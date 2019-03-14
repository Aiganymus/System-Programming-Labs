/*  
 *  hello-1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */	
#include <linux/kfifo.h>
#include <linux/slab.h>

struct person {
        char* name;
        int age;
};

int init_module(void)
{ 
    struct person *parent = kmalloc(sizeof(struct person), GFP_KERNEL);
    parent->name = "hello";
    parent->age = 20;

    struct kfifo fifo;
    int ret;
    ret = kfifo_alloc(&fifo, PAGE_SIZE, GFP_KERNEL);
    if (ret)
        return ret;

    kfifo_in(&fifo, &parent, sizeof(parent));

    /*unsigned int val = kfifo_is_full(&fifo);
    printk(KERN_INFO "Fifo full: %d.", val);

    unsigned int val_1 = kfifo_is_empty(&fifo);
    printk(KERN_INFO "Fifo empty: %d.", val_1);*/
    
    struct person *val = kmalloc(sizeof(struct person), GFP_KERNEL);
    ret = kfifo_out_peek(&fifo, &val, sizeof(val));
    if (ret != sizeof(val))
        return -EINVAL;
    printk(KERN_INFO "My age: %d\n", val->age);

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
