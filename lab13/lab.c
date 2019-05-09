#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/bio.h>
#include <linux/bvec.h>
#include <linux/gfp.h>
#include <linux/mm_types.h>
#include <linux/blkdev.h>

static struct block_device *device;

blk_qc_t func(struct request_queue *q, struct bio *bio) {
    struct bio_vec bvec;
    struct bvec_iter iter;

    // struct bio *bio = bio_alloc(GFP_NOIO, 1);
    // bio_set_dev(bio, device);
    // bio->bi_disk = device->bd_disk;
    printk(KERN_INFO "enter\n");
    printk(KERN_INFO "bi_flags %u\n", bio->bi_seg_back_size);
    sector_t sec = bio->bi_iter.bi_sector;
    printk("test: sector number: %llu\n", (unsigned long long) sec);

    bio_for_each_segment(bvec, bio, iter) {
        // sector_t sector = iter.bi_sector;
        // char *buffer = kmap_atomic(bvec.bv_page);
        unsigned long offset = bvec.bv_offset;
        // size_t len = bvec.bv_len;
        printk(KERN_INFO "offset %lu\n", offset);
    }
    return 0;
}

int init_module(void) {
    printk(KERN_INFO "Starting\n");
    device = blkdev_get_by_path("/dev/sda1", FMODE_READ, NULL);
    if (!device) {
    	printk(KERN_ERR "ERROR!\n");
    	return -1;
    }
    printk(KERN_INFO "bd_openers %d\n", device->bd_openers);
    blk_queue_make_request(device->bd_queue, func);
    return 0;
}

void cleanup_module(void) {
   printk(KERN_INFO "Cleanup\n");
}

MODULE_LICENSE("GPL");