#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */

#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/io.h>

static struct tasklet_struct my_tasklet;
static int irqn = 16;

irqreturn_t my_interrupt(int irq, void *dev_id) {
    tasklet_schedule(&my_tasklet);
    return IRQ_NONE;
}

static void my_tasklet_handler(unsigned long t_arg) {
    if(printk_ratelimit())
        printk(KERN_INFO "Entering %ld", t_arg);
}

int init_module(void) {
    tasklet_init(&my_tasklet, my_tasklet_handler, 123);

    if (request_irq(1, (irq_handler_t) my_interrupt, IRQF_SHARED, "usb", (void *)(my_interrupt))) {
        printk(KERN_ERR "print_key: cannot register IRQ %d\n", irqn);
        return -EIO;
    }
    else {
        printk(KERN_INFO "success!");
    	return 0;
    }
}

void cleanup_module(void)
{
    free_irq(irqn, (void *)(my_interrupt));
    tasklet_kill(&my_tasklet);

	printk(KERN_INFO "Hello: goodbye.\n");
}

