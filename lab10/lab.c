#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/time.h>

static struct timer_list mytimer;
static struct tm res;
static struct timeval now;

void exampleTimerFunc(struct timer_list *t) {
	int ret;
	//printk(KERN_INFO "%d %d\n", sys_tz.tz_minuteswest, sys_tz.tz_dsttime);

	do_gettimeofday(&now);
	time_to_tm(now.tv_sec, 0, &res);
	printk(KERN_INFO "%02d:%02d:%02d", res.tm_hour, res.tm_min, res.tm_sec);

	ret = mod_timer(&mytimer, jiffies + msecs_to_jiffies(1000));
	if (ret) {
		printk("Error in mod_timer\n");
	}
}

int init_module(void) {
   printk(KERN_INFO "Starting\n");
   printk("Starting timer to fire in 1000ms (%ld)\n", jiffies);
   mytimer.expires = jiffies + msecs_to_jiffies(1000);
   timer_setup(&mytimer, exampleTimerFunc, 0);

   add_timer(&mytimer);
   return 0;
}

void cleanup_module(void) {
   printk(KERN_INFO "Cleanup\n");
   del_timer(&mytimer);
}
