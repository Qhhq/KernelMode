#include <linux/kernel.h>  
#include <linux/module.h>  
#include <linux/cdev.h> 
#include <linux/fs.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <asm/atomic.h>
#include <linux/slab.h>

#include <ioctl.h>


#define DEVICE_FIRST  0 
#define DEVICE_COUNT  1 
static struct cdev hcdev; 
static int major = 0; 
static int device_open = 0; 


static int km_open(struct inode *n, struct file *f) 
{ 
	if( device_open )
	{
		return -EBUSY;
	}  
	device_open++; 
	return 0; 
} 
	
static int km_release(struct inode *n, struct file *f) 
{ 
	device_open--; 
	return 0; 
} 

static int thread_func(void *data) 
{
	struct KMParam *param;
	param = (struct KMParam *)data;

    atomic_dec(&param->eventStartThread.thread_count);
    wake_up_interruptible(&param->eventStartThread.wq);

	wait_event_interruptible(param->eventStartTest.wq, 0 == atomic_read(&param->eventStartTest.thread_count));

	printk("thread_func start\n");
	ssleep(2);
	printk("thread_func end\n");


    atomic_dec(&param->eventStop.thread_count);
    wake_up_interruptible(&param->eventStop.wq);

	return 0; 
}

static long device_ioctl (struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	int i;
	struct KMParam *param;
	struct task_struct *tsk;
	param = 0;



	switch(ioctl_num)
		{
			case IOCTL_SET_KMParam:
			{
				param = kmalloc(sizeof(struct KMParam), GFP_KERNEL);
				if (!param)
				{
					return -ENOMEM;
				}


    			init_waitqueue_head(&param->eventStartThread.wq);
				init_waitqueue_head(&param->eventStartTest.wq);
				init_waitqueue_head(&param->eventStop.wq);

				atomic_set(&param->eventStartThread.thread_count, 0);
				atomic_set(&param->eventStartTest.thread_count, 1);
				atomic_set(&param->eventStop.thread_count, 0);

				for (i = 0; i < 3; ++i)
				{
					tsk = kthread_run( thread_func, (void *)param, "thread_%d", i );
					if (!IS_ERR(tsk))
					{
						atomic_inc(&param->eventStartThread.thread_count);
						atomic_inc(&param->eventStop.thread_count);
					}
					

				}
				wait_event_interruptible(param->eventStartThread.wq, 0 == atomic_read(&param->eventStartThread.thread_count));			
				printk("All threads started\n");

				atomic_dec(&param->eventStartTest.thread_count);
				wake_up_interruptible(&param->eventStartTest.wq);
	
				wait_event_interruptible(param->eventStop.wq, 0 == atomic_read(&param->eventStop.thread_count));
				printk("Main thread end\n");

				kfree(param);
			}
			break;
		}
	return 0;
}	

static const struct file_operations km_fops = { 
	.owner = THIS_MODULE, 
	.open = km_open, 
	.release = km_release, 
	.unlocked_ioctl = device_ioctl,
}; 

static int __init module_start(void)
{ 
	int ret; 
	dev_t dev; 
	printk(KERN_ERR "module_start\n"); 
	ret = alloc_chrdev_region(&dev, DEVICE_FIRST, DEVICE_COUNT, DEVICE_FILE_NAME);
	if(ret < 0) 
	{ 
		printk(KERN_ERR "Can not register char device region\n"); 
		return ret; 
	} 
	major = MAJOR(dev);  

	cdev_init(&hcdev, &km_fops); 
	hcdev.owner = THIS_MODULE; 
	hcdev.ops = &km_fops;  

	ret = cdev_add(&hcdev, dev, DEVICE_COUNT); 
	if(ret < 0) 
	{ 
		unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT); 
		printk( KERN_ERR "Can not add char device\n" ); 
		return ret; 
	} 
	printk(KERN_INFO "module installed %d:%d\n", MAJOR(dev), MINOR(dev) );
	return 0;
} 

static void __exit module_stop (void)
{ 
	cdev_del(&hcdev); 
	unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT); 
	printk(KERN_INFO "km_exit\n");
}

MODULE_LICENSE("GPL"); 
module_init(module_start);
module_exit(module_stop);

