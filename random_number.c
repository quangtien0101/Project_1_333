#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/kernel.h>  



#include <linux/uaccess.h>
#include <linux/device.h>         

#include <linux/fs.h>             

#define  DEVICE_NAME "random_number" 
#define  CLASS_NAME  "CS333"

MODULE_LICENSE("GPL");            
MODULE_AUTHOR("Bruh!123");    
MODULE_DESCRIPTION("Project1 CS333 making a character device");  
MODULE_VERSION("1.0");          

static struct class*  Random_number_character_class  = NULL; 
static struct device* Random_number_character_device = NULL;

static char   rand_num[99] = "17589234";  // sending the random number as a string value         
static short  message_lenght;              

static int    major_number;                  
static int    numberOpens = 0;     //count the number of process trying to access and read the device
                                   // could be useful to implement mutex         
 


static int     open_device(struct inode *, struct file *);
static int     release_device(struct inode *, struct file *);
static ssize_t read_device(struct file *, char *, size_t, loff_t *);
static ssize_t write_device(struct file *, const char *, size_t, loff_t *);


static struct file_operations file_ops =
{
   .open = open_device,
   .read = read_device,
   .write = write_device,
   .release = release_device,
};

static int __init random_device_init(void){
   printk(KERN_INFO "Random_num_device: Initializing the Random_num_device LKM\n");

   major_number = register_chrdev(0, DEVICE_NAME, &file_ops);
   if (major_number<0)
   {
      printk(KERN_ALERT "Random_num_device didn't register successfully \n");

      return major_number;
   }

   printk(KERN_INFO "Random_num_device registered successfully: Major Number %d\n", major_number);

//device class registration
   Random_number_character_class = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(Random_number_character_class))
   {                
      unregister_chrdev(major_number, DEVICE_NAME);

      printk(KERN_ALERT "Random_num_device didn't register successfully\n");

      return PTR_ERR(Random_number_character_class);          
   }

   printk(KERN_INFO "Random_num_device device registered successfully\n");

//driver registration
   Random_number_character_device = device_create(Random_number_character_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
   if (IS_ERR(Random_number_character_device))
   {               
      class_destroy(Random_number_character_class);

      unregister_chrdev(major_number, DEVICE_NAME);
      
      printk(KERN_ALERT "UNable to create the device!\n");
      
      return PTR_ERR(Random_number_character_device);
   }
   printk(KERN_INFO "Random_num_device device created successfully!\n");    
   return 0;
}

static void __exit random_device_exit(void){
   device_destroy(Random_number_character_class, MKDEV(major_number, 0));     
   
   class_unregister(Random_number_character_class);                          
   
   class_destroy(Random_number_character_class);                             
   
   unregister_chrdev(major_number, DEVICE_NAME);

   printk(KERN_INFO "Random_num_device destroyed successfully!");
}

static int open_device(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "Random_num_device opens at %d time(s)\n", numberOpens);
   return 0;
}

static int release_device(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "Random_num_device: Device successfully closed\n");
   return 0;
}



static ssize_t write_device(struct file *filep, const char *buffer, size_t len, loff_t *offset){
  // This is a read only device!
   printk(KERN_ALERT "This operation is not supported.\n");
   return -EINVAL;

}

static ssize_t read_device(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
   int err = 0; //number of errors that occur during the copying

   message_lenght = strlen(rand_num);

   // copy the content of the device to the buffer of the process
   err = copy_to_user(buffer, rand_num, message_lenght);
   

   if (err==0)
   {            // if t
      printk(KERN_INFO "Random_num_device sent the magic number the user\n");
      return (message_lenght=0);
   }
   else {
      printk(KERN_INFO "Random_num_device failed to send the magic number to the user\n");
      return -EFAULT;              
   }
}



module_init(random_device_init);
module_exit(random_device_exit);
