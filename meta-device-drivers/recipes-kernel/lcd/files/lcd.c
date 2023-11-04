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
#include<linux/delay.h>

/* Meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abdallah Ahmed Mohammed ");
MODULE_DESCRIPTION("LCD driver");


dev_t device_number=0;
struct cdev st_character_device;
struct class *device_class;
struct device *device_file;
#define LCD_Buffer_Size    17
static unsigned char LCD_Buffer[LCD_Buffer_Size] = "";
/* LCD pins*/
static unsigned int gpio_lcd_pins[] = {
    2,      /*Enable pin*/
    3,      /* Resgister select pin R/W*/
    4,      /* Data pin 0*/
    17,     /* Data pin 1*/
    27,     /* Data pin 2*/
    22,     /* Data pin 3*/
    10,     /* Data pin 4*/
    9,      /* Data pin 5*/
    11,     /* Data pin 6*/
    5       /* Data pin 7*/
};

static unsigned char *lcd_pins_label[] ={
    "Enale_pin",
    "Resgidter_Select",
    "Data_pin_0",
    "Data_pin_1",
    "Data_pin_2",
    "Data_pin_3",
    "Data_pin_4",
    "Data_pin_5",
    "Data_pin_6",
    "Data_pin_7"
};

#define Enable_Pin                  gpio_lcd_pins[0]
#define Register_select_Pin         gpio_lcd_pins[1]

void LCD_Enable(void)
{
    gpio_set_value(Enable_Pin,1);
    msleep(5);
    gpio_set_value(Enable_Pin,0);
}

void LCD_Send_Byte(uint8_t Byte)
{
    int i;
    for(i=2;i<10;i++)
    {
        gpio_set_value(gpio_lcd_pins[i],((Byte) & (1<<i)) >> i);
    }
    LCD_Enable();
    msleep(5);
}

void LCD_Send_Command(uint8_t Command)
{
    gpio_set_value(Register_select_Pin,0);
    LCD_Send_Byte(Command);
}

void LCD_Send_Data(uint8_t Data)
{
    gpio_set_value(Register_select_Pin,1);
    LCD_Send_Byte(Data);
}

void lcd_clean(void)
{
    LCD_Send_Command(0x1);
}
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
    if(count + (*offset) > LCD_Buffer_Size)
    {
        count = LCD_Buffer_Size - (*offset);
    }
    not_copied = copy_to_user(user_buffer,&LCD_Buffer[*offset],count);
    if(not_copied)
    {
        printk("the count of not copied bytes is %d \n",not_copied);
        printk("error during the copieing\n");
        return -1;
    }
    // if the count less than size the function called more than one time 
    // start from the end reading index
    *offset = count;

   
    return count;
}


ssize_t Driver_Write (struct file *file,const char __user *user_buffer, size_t count, loff_t *offset)
{
    int not_copied=0;
    int i;
    printk("%s device_file read was called , count to write = %ld , offset = %lld \n",__FUNCTION__,count,*offset);
    if(count + (*offset) > LCD_Buffer_Size)
    {
        count = LCD_Buffer_Size - (*offset);
    }
    if(!count)
    {
        printk("no memory space left \n");
        return -1;
    }
    
    not_copied = copy_from_user(&LCD_Buffer[*offset],user_buffer,count);
    if(not_copied)
    {
        printk("the count of not copied bytes is %d \n",not_copied);
        printk("error during the copieing\n");
        return -1;
    }
    *offset = count;


    lcd_clean();
    for(i=0;i<count;i++)
    {
        LCD_Send_Data(LCD_Buffer[i]);
    }
    printk("the count of not copied bytes is %d \n",not_copied);
    printk("the copied message is : %s \n",LCD_Buffer);
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
    int counter=0;
    
    printk("hello Kernel this is the init module function \n");
    retval = alloc_chrdev_region(&device_number,0,1,"test_device_name");
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

    for(;counter<10;counter++)
    {
        retval = gpio_request(gpio_lcd_pins[counter],lcd_pins_label[counter]);
        if(retval)
        {
            printk("Failed to request GPIO pin %d\n", gpio_lcd_pins[counter]);
            counter = 9;
            goto ERROR_GPIO_REQUEST; 
        }
    }

    for(counter = 0;counter<10;counter++)
    {
        retval =  gpio_direction_output(gpio_lcd_pins[counter],0);
        if(retval)
        {
            printk("Failed to set GPIO direction\n");
            counter = 9;
            goto ERROR_GPIO_SET_DIR;
        }
    }

    /* init the display*/
    LCD_Send_Command(0x30);       /* 8 bit mode */
    LCD_Send_Command(0xF) ;       /*turn display on , turn cursor on , set cursor to blinking*/
    LCD_Send_Command(0x1) ;       /* clear the LCD*/

    return 0;




    ERROR_GPIO_SET_DIR:
    for(;counter >=0;counter --)
        {
            gpio_free(gpio_lcd_pins[counter]);
        }
        
    ERROR_GPIO_REQUEST:
        device_destroy(device_class,device_number);
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
    int i = 9;
    lcd_clean();
    for(;i>=0;i--)
    {
        gpio_set_value(gpio_lcd_pins[i],0);
        gpio_free(gpio_lcd_pins[i]);
    }
    device_destroy(device_class,device_number);
    device_destroy(device_class,device_number);
    class_destroy(device_class);
    cdev_del(&st_character_device);
    unregister_chrdev_region(device_number,1);
    printk("Goodbye Kernel this is the exit module function \n");
}

module_init(Module_init);
module_exit(Module_module);









