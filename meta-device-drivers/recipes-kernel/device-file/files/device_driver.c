#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/err.h>




/* Meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdallah Ahmed Mohammed ");
MODULE_DESCRIPTION("test parameters");


dev_t device_number=0;
struct cdev st_character_device;
struct class *device_class;
struct device *device_file;
#define SIZE    10
//static unsigned char buffer[SIZE] = " Hello , linux device drivers ";
static unsigned char buffer[SIZE] = "";
static int Driver_Open (struct inode *device_file, struct file *instance)
{
    printk("%s device_file open was called \n",__FUNCTION__);
    return 0;
}
static int Driver_release (struct inode *device_file, struct file *instance)
{
    printk("%s device_file release was called \n",__FUNCTION__);
    return 0;
}
ssize_t Driver_Read (struct file *file, char __user *user_buffer, size_t count, loff_t *offset)
{
    int not_copied=0;
    printk("%s device_file read was called , count to read = %ld , offset = %lld \n",__FUNCTION__,count,*offset);
    if(count + (*offset) > SIZE)
    {
        count = SIZE - (*offset);
    }
    not_copied = copy_to_user(user_buffer,&buffer[*offset],count);
    if(not_copied)
    {
        printk("the count of not copied bytes is %d \n",not_copied);
        printk("error during the copieing\n");
        return -1;
    }
    // if the count less than size the function called more than one time 
    // start from the end reading index
    *offset = count;
   // printk("the count of not copied bytes is %d \n",not_copied);
   //printk("%s: message: %s \n", __func__, user_buffer);
    //printk("%s: message: %s \n", __func__, buffer);
   
    return count;
}


ssize_t Driver_Write (struct file *file,const char __user *user_buffer, size_t count, loff_t *offset)
{
    int not_copied=0;
    printk("%s device_file read was called , count to write = %ld , offset = %lld \n",__FUNCTION__,count,*offset);
    if(count + (*offset) > SIZE)
    {
        count = SIZE - (*offset);
    }
    if(!count)
    {
        printk("no memory space left \n");
        return -1;
    }
    
    not_copied = copy_from_user(&buffer[*offset],user_buffer,count);
    if(not_copied)
    {
        printk("the count of not copied bytes is %d \n",not_copied);
        printk("error during the copieing\n");
        return -1;
    }
    // if the count less than size the function called more than one time 
    // start from the end reading index
    *offset = count;
    printk("the count of not copied bytes is %d \n",not_copied);
    printk("the copied message is : %s \n",buffer);
    return count;
}


const struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = Driver_Open,
    .release = Driver_release,
    .read = Driver_Read,
    .write = Driver_Write
};









static int __init Module_init(void)
{
    int retval;
    
    
    printk("hello Kernel this is the init module function \n");
    retval = alloc_chrdev_region(&device_number,0,1,"test_device");
    if (retval==0 )
    {
        printk("%s retval = %d , registered device major number = %d and minor number = %d \n",__FUNCTION__,retval,MAJOR(device_number),MINOR(device_number));
    }else
    {
        printk("cann't register device major number \n");
        return -1;
    }
    cdev_init(&st_character_device,&fops);
    retval = cdev_add(&st_character_device,device_number,1);
    if (retval != 0)
    {
        printk("Registration of the devie failed \n");
       goto ERROR_ALLOCATE;
    }
    device_class = class_create(THIS_MODULE,"test_class");
    if(device_class == NULL)
    {
        
       printk("Creating device class failed\n");
       goto ERROR_CLASS_CREATE;
    }

    device_file = device_create(device_class,NULL,device_number,NULL,"test_device_file");
    if(device_file == NULL)
    {
        printk("Creating device file failed\n");
        goto ERROR_DEVICE_FILE_CREATE;
    }
   
    return 0;




    ERROR_DEVICE_FILE_CREATE:
        class_destroy(device_class);
    ERROR_CLASS_CREATE:
        cdev_del(&st_character_device);
    ERROR_ALLOCATE:
        unregister_chrdev_region(device_number,1);
        return -1;

      
}

static void __exit Module_module(void)
{
        device_destroy(device_class,device_number);
        class_destroy(device_class);
        cdev_del(&st_character_device);
        unregister_chrdev_region(device_number,1);
        printk("Goodbye Kernel this is the exit module function \n");
}

module_init(Module_init);
module_exit(Module_module);









