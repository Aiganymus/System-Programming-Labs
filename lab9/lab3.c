#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/random.h>
#include <linux/flex_array.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/kthread.h>  
#include <linux/mutex.h>

#define ARR_SIZE 100

static int from = 0;
static int to = 0;
static int *arr;
static struct task_struct *thread1;
static struct task_struct *thread2;
static struct task_struct *thread3;
static struct mutex my_mutex;

void fillArray(void) {
    int rand, i;
    for(i = 0; i < ARR_SIZE; i++) {
        get_random_bytes(&rand, sizeof(rand));
        rand &= 0x7FFFFFFF;
        rand %= 200;
        arr[i] = rand;
    }
}

void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    int L[n1], R[n2]; 
  
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
void mergeSort(int arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        int m = l+(r-l)/2; 
  
        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
} 

int threadSort(void *data) {
    while(!kthread_should_stop()) {
        while(to < ARR_SIZE) {
            printk(KERN_INFO "I'm %s", current->comm);
            mutex_lock(&my_mutex);
            from = to;
            to += 10;
            mutex_unlock(&my_mutex);
            mergeSort(arr, from, to); 
        }
    }
    return 0;
}

int init_module(void) {
    printk(KERN_INFO "Hello :)");
    arr = kmalloc(sizeof(int)*ARR_SIZE, GFP_KERNEL);
    fillArray();

    mutex_init(&my_mutex);

	thread1 = kthread_run(threadSort, NULL, "myThread1");
	thread2 = kthread_run(threadSort, NULL, "myThread2");
	thread3 = kthread_run(threadSort, NULL, "myThread3");

	if (thread1 && thread2 && thread3) 
		printk(KERN_INFO "Threads ARE created!");
	else
		printk(KERN_INFO "Threads are Not created!");
	return 0;
}

void cleanup_module(void)
{
	kthread_stop(thread1);
	kthread_stop(thread2);
	kthread_stop(thread3);
    int j;
    for(j = 0; j < 10; j++) {
      	printk(KERN_INFO "Goodbye :( %d\n", arr[j]);    
    }

}
