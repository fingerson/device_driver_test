#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/slab.h>

static dev_t first; // Global variable for the first device number
int mode=0;
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class
char* buffer=NULL;
int key=0;
int cifra(char* origin, int shift)
{
    char* result = origin;
    int i;
    char temp;
    if (shift < 0)
    {
        shift = (26-1*((-1*shift)%26))%26;
    }

    for (i = 0; origin[i]!= '\0'; i++)
    {
        if (origin[i] >= 'a' && origin[i] <= 'z')
        {
            temp = origin[i]-'a';
            result[i] = (temp+shift)%26 + 'a';
        }
        else if (origin[i] >= 'A' && origin[i] <= 'Z')
        {
            temp = origin[i]-'A';
            result[i] = (temp+shift)%26 + 'A';
        }
        else
        {
            result[i] = origin[i];
        }
    }
    return 0;
}
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver: read()\n");
    if(buffer==NULL){
        return -EFAULT;
    }
    else{
        if(mode==0)
            cifra(buffer,key);
        else if(mode==1)
            cifra(buffer,-key);
        else
            return 0;
        if (copy_to_user(buf, buffer, len) != 0)
            return -EFAULT;
        else
            return 1;
    }
    return 0;
}
static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    if(mode==0 || mode==1){
        kfree(buffer);
        buffer=kmalloc(len,GFP_KERNEL);
        printk(KERN_INFO "Driver: write buffer\n");
        if (copy_from_user(buffer, buf , len) != 0)
            return -EFAULT;
        else
            return len;
    }
    else if(mode==2){

        printk(KERN_INFO "Driver: write key\n");
        if (copy_from_user(&key, buf , len) != 0)
            return -EFAULT;
        else
            return len;
    }
    else{
        return 0;
    }
}
long ioctl_funcs(struct file *filp,unsigned int cmd, unsigned long arg){
    int ret=0;
    printk(KERN_INFO "Entered ioctl_funcs %d %d %d %d", cmd, (int)_IO(MAJOR(first),0),
    (int)_IO(MAJOR(first),1), (int)_IO(MAJOR(first),2));
    if (cmd == (int)_IO(MAJOR(first),0))
    {
        printk(KERN_INFO "Mode: encode");
        mode=0;
    }
    else if (cmd == (int)_IO(MAJOR(first),1))
    {
        printk(KERN_INFO "Mode: decode");
        mode=1;
    }
    else if (cmd == (int)_IO(MAJOR(first),2))
    {
        printk(KERN_INFO "Mode: key");
        mode=2;
    }

    return ret;
}
static int my_open(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: open()\n");
  return 0;
}
  static int my_close(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Driver: close()\n");
  return 0;
}

static struct file_operations fops ={
    .owner= THIS_MODULE,
    .write= my_write,
    .open = my_open,
    .release = my_close,
    .read= my_read,
    .unlocked_ioctl= ioctl_funcs
};

static int __init cypher_init(void) /* Constructor */
{
  printk(KERN_INFO "Cypher registered");
  if (alloc_chrdev_region(&first, 0, 1, "Cypher") < 0)
  {
    return -1;
  }
  printk(KERN_INFO "A");
    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
  {
    unregister_chrdev_region(first, 1);
    return -1;
  }
  printk(KERN_INFO "B");
    if (device_create(cl, NULL, first, NULL, "mynull") == NULL)
  {
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }
  printk(KERN_INFO "C");
    cdev_init(&c_dev, &fops);
    printk(KERN_INFO "C2");
    if (cdev_add(&c_dev, first, 1) == -1)
  {
      printk(KERN_INFO "Aqui");
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }
  printk(KERN_INFO "D");
  return 0;
}

static void __exit cypher_exit(void) /* Destructor */
{
  cdev_del(&c_dev);
  device_destroy(cl, first);
  class_destroy(cl);
  unregister_chrdev_region(first, 1);
  printk(KERN_INFO "Cypher unregistered");
}

module_init(cypher_init);
module_exit(cypher_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Henrique Finger Zimerman/Felipe Pacheco");
MODULE_DESCRIPTION("Our First Driver");
