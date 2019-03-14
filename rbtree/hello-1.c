#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */	
#include <linux/rbtree.h>
#include <linux/slab.h>

struct person {
    char* name;
    int age;
    struct rb_node node;
};

int init_module(void)
{ 
    struct person *parent = kmalloc(sizeof(struct person), GFP_KERNEL);
    parent->name = "hello";
    parent->age = 20;

    struct rb_root root = RB_ROOT;

    
    struct person *val = kmalloc(sizeof(struct person), GFP_KERNEL);
    val->name = "world"
    val->age = 23;
    

    struct person *rb_search(struct rb_root *root, int age)
    {
        struct rb_node *node = root->rb_node;  

        while (node)
        {
            struct person *p = rb_entry(node, struct person, node);

            if (p->age > age)
	            node = node->rb_left;
            else if (p->age < age)
		        node = node->rb_right;
            else
    	        return p;  
  	    }
	    return NULL;
    }

    void rb_insert(struct rb_root *root, struct person *new)
    {
        struct rb_node **link = &root->rb_node, *parent;
	    int value = new->age;

	    while (*link)
	    {
	        parent = *link;
	        struct person *p = rb_entry(parent, struct person, parent);

	        if (p->age > value)
		        link = &(*link)->rb_left;   
	        else
		        link = &(*link)->rb_right;
	    }

	    rb_link_node(new, parent, link);
	    rb_insert_color(new, root);
    }

    printk(KERN_INFO "My age: %d\n", val->age);

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
