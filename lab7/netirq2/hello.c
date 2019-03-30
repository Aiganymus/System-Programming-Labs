#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */

#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <linux/workqueue.h>

#include <linux/ip.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/tcp.h>

MODULE_LICENSE("GPL");

struct packet_info {
    struct work_struct work;

    u32 *source_address;
    uint16_t source_port;
};

static struct nf_hook_ops hook_ops;
static struct workqueue_struct *wq;

static void wq_func(struct work_struct *work) {
    struct packet_info *packet_info = (struct packet_info*) work;
    if(printk_ratelimit()) {
        printk(KERN_INFO "Source IP addres: %pI4\nSource port: %hu", packet_info->source_address, packet_info->source_port);
    }
}

static unsigned int hook_func(void* priv, struct sk_buff *skb, const struct nf_hook_state* state)
{
    struct iphdr *ip_header = (struct iphdr *)skb_network_header(skb);
    struct tcphdr *tcp_header;

    struct packet_info *packet_info = kmalloc(sizeof(struct packet_info), GFP_KERNEL);

    if (ip_header->protocol == 6) //TCP protocol
    {
        tcp_header = (struct tcphdr *)((__u32 *)ip_header+ ip_header->ihl);

        INIT_WORK((struct work_struct*) packet_info, wq_func);       
        packet_info->source_port = ntohs(tcp_header->source);
        packet_info->source_address = &ip_header->saddr;
        queue_work(wq, (struct work_struct*) packet_info);       
    }
    return NF_ACCEPT; //accept the packet
}

int init_module(void) {
    int retval;

    wq = create_workqueue("my work queue");

    hook_ops.hook = hook_func;
    hook_ops.hooknum = NF_INET_LOCAL_IN;
    hook_ops.pf = PF_INET;
    hook_ops.priority = NF_IP_PRI_FIRST;
    retval = nf_register_net_hook(&init_net, &hook_ops);
    printk("nf_register_net_hook returned %d\n", retval);

    return retval;
}

void cleanup_module(void)
{
    flush_workqueue(wq);
    destroy_workqueue(wq);
    nf_unregister_net_hook(&init_net, &hook_ops);
    printk("Unregistered the net hook.\n");
}

