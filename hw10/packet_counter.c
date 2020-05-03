#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

// 5 * 60sec/min * 1000ms/sec
// 5 minute recording period
static const unsigned counting_period_in_ms = 5 * 60 * 1000;

extern atomic64_t packet_counter_out;
extern atomic64_t packet_counter_in;
extern atomic64_t packet_counter_merges;
extern atomic64_t packet_counter_enq;
extern atomic64_t packet_counter_deq;
extern atomic64_t packet_counter_req;

#define PACKET_COUNTER_BUFFER_SIZE (1 << 12)
static char str_buf[PACKET_COUNTER_BUFFER_SIZE] = {0};

static int packet_counter_show(struct seq_file *seq_fp, void *p) {
	seq_puts(seq_fp, str_buf);
	return 0;
}

static int packet_counter_open(struct inode *inode, struct file *fp) {
	try_module_get(THIS_MODULE);
	return single_open(fp, packet_counter_show, NULL);
}

static int packet_counter_release(struct inode *inode, struct file *fp) {
	module_put(THIS_MODULE);
	return single_release(inode, fp);
}

// timer for every 5 minutes as defined above in counting_period_in_ms
struct timer_list packet_counter_timer;
// struct for /proc/ entry
static struct proc_dir_entry *packet_counter_fp;
// filename for /proc/ entry
static const char *const packet_counter_file_name = "packet_counter";
// file_operations fstruct for packet_counter_fp
static const struct file_operations packet_counter_fops = {
	.owner		= THIS_MODULE,
	.open		= packet_counter_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= packet_counter_release,
};

static void packet_counter_flush(struct timer_list *timer) {
	int n;
	mod_timer(timer, jiffies + msecs_to_jiffies(counting_period_in_ms));
	n = snprintf(str_buf, PACKET_COUNTER_BUFFER_SIZE,
		     "Internet Packet Counting Statistics (updated per 5 minutes):\n"
		     "# outgoing packets	: %ld\n"
		     "# incoming packets	: %ld\n"
		     "# queue merges		: %ld\n"
		     "# enqueue ops		: %ld\n"
		     "# dequeue ops		: %ld\n"
		     "# requeue ops		: %ld\n",
		     atomic64_read(&packet_counter_out), atomic64_read(&packet_counter_in),
		     atomic64_read(&packet_counter_merges), atomic64_read(&packet_counter_enq),
		     atomic64_read(&packet_counter_deq), atomic64_read(&packet_counter_req));
	atomic64_set(&packet_counter_out, 0);
	atomic64_set(&packet_counter_in, 0);
	atomic64_set(&packet_counter_merges, 0);
	atomic64_set(&packet_counter_enq, 0);
	atomic64_set(&packet_counter_deq, 0);
	atomic64_set(&packet_counter_req, 0);
	if (n < 0) {
		printk(KERN_INFO "Matthew Kobilas pkt_cnt: failed to add string to buffer\n");
		proc_set_size(packet_counter_fp, 0);
	} else if (n > PACKET_COUNTER_BUFFER_SIZE) {
		printk(KERN_INFO "Matthew Kobilas pkt_cnt: string too large for buffer\n");
		proc_set_size(packet_counter_fp, PACKET_COUNTER_BUFFER_SIZE);
	} else {
		prinkt(KERN_INFO "Matthew Kobilas pkt_cnt: updated /proc/ file with Internet packet statistics\n");
		proc_set_size(packet_counter_fp, n);
	}
}

static int __init packet_counter_init(void) {
	packet_counter_file_entry = proc_create(packet_counter_fp, 0, NULL, &packet_counter_fops);
	if (!packet_counter_fp) {
		printk(KERN_INFO "Matthew Kobilas pkt_cnt: error when opening packet_counter\n");
		return -1;
	}
	proc_set_user(packet_counter_fp, KUIDT_INIT(0), KGIDT_INIT(0));
	proc_set_size(packet_counter_fp, 0);
	timer_setup(&packet_counter_timer, packet_counter_flush, 0);
	packet_counter_flush(&packet_counter_timer);
	printk(KERN_INFO "Matthew Kobilas pkt_cnt: packet_counter module installed\n");
	return 0;
}

static void __exit packet_counter_exit(void) {
	del_timer(&packet_counter_timer);
	proc_remove(packet_counter_fp);
	prinkt(KERN_INFO "Matthew Kobilas pkt_cnt: packet_counter module removed\n");
}

module_init(packet_counter_init);
module_exit(packet_counter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matthew Kobilas <mattkobi789@gmail.com>");
MODULE_DESCRIPTION("CS680 Assignment 10 -- Prints Internet packet statistics of upload and download over set time period.");
