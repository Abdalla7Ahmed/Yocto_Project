#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include<linux/gpio.h>
#include<linux/interrupt.h>

/* Meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdallah Ahmed Mohammed ");
MODULE_DESCRIPTION("Interrupt driver");




unsigned int irq_number;

static irq_handler_t gpio_irq_handler(unsigned int irq,void *dev_id,struct pt_regs regs)
{
    printk("%s gpio_irq was called /n",__FUNCTION__);
    return (irq_handler_t)IRQ_HANDLED;
}

static int __init Module_init(void)
{
    int retval;
    
    printk("hello Kernel this is the init module function \n");
   

    retval = gpio_request(17,"Interrupt_label");
    if(retval)
    {
        printk("Failed to request GPIO pin %d\n", 17);
        return -1;
    }


    retval =  gpio_direction_input(17);
    if(retval)
    {
        printk("Failed to set GPIO direction\n");
        gpio_free(17);
        return -1;

    }

    irq_number = gpio_to_irq(17);

    retval = request_irq(irq_number,(irq_handler_t) gpio_irq_handler,IRQF_TRIGGER_RISING,"irq_test",NULL);
    if(retval)
    {
        printk("Failed to request interrupt number %d\n",irq_number);
        return -1;
    }

    printk("GPIO number 17 is mapped to %d\n",irq_number);


    return 0;




      
}

static void __exit Module_module(void)
{
    free_irq(irq_number,NULL);
    gpio_free(17);
    printk("Goodbye Kernel this is the exit module function \n");
}

module_init(Module_init);
module_exit(Module_module);









