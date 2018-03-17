/*************************************************************************
    > File Name: mydriver.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: 2018年03月17日 星期六 12时50分49秒
 ************************************************************************/

#include <linux/module.h>
#include <linux/init.h>

#include <linux/fs.h>  /* For chrdev_region*/
#include <linux/cdev.h> /* For cdev */

static char * whom = "world";
static int howmany = 1;

module_param(whom, charp, S_IRUGO);
module_param(howmany, int, S_IRUGO);

/* the dev_t structure to store the device */
static dev_t mydriver_devt;
#define MAX_MYDRIVER 2

static const struct file_operations mydriver_ops = {
	.owner  = THIS_MODULE,
};

struct mydriver_dev {
	struct cdev cdev;
};

struct mydriver_dev mydriver_device;

static int __init mydriver_init(void) 
{
	int ret = 0;
/*
	struct cdev * mycdev = NULL;
*/
	printk("daixianze %s \n", __func__);

	ret = alloc_chrdev_region(&mydriver_devt, 0, MAX_MYDRIVER, "mydriver");
	if (ret < 0)
		pr_err("mydriver: unable to allocate char dev region \n");
/*
	mycdev = cdev_alloc();
	if (!mycdev) {
		pr_err("mydriver: unable to alloc cdev \n");
		goto fail;
	}
	mycdev->owner = mydriver_ops.owner;
	mycdev->ops = &mydriver_ops;
*/
	cdev_init(&mydriver_device.cdev, &mydriver_ops);
	mydriver_device.cdev.owner = mydriver_ops.owner;

	ret = cdev_add(&mydriver_device.cdev, mydriver_devt, MAX_MYDRIVER);
	if (ret < 0)
		goto fail;

	if (ret)
		goto fail;
fail:
	return ret;
}

static void __exit mydriver_exit(void)
{
	unregister_chrdev_region(mydriver_devt, MAX_MYDRIVER);
}

module_init(mydriver_init);
module_exit(mydriver_exit);

MODULE_LICENSE("GPL");

