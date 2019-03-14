#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <sys/syscall.h> /* For SYSCALL_DEFINE* macros */

SYSCALL_DEFINE2(sys_helloworld, char *, buff, size_t, buff_sz)
{
        snprintf(buff, buff_sz, "Hello, world\n");
        return 0;
}

/*
int init_module(void)
{
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Hello: goodbye.\n");
}
*/
