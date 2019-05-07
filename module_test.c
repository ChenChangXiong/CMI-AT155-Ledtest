#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include  <linux/fs.h>
#include  <linux/miscdevice.h>
#include  <asm/io.h>
#include <linux/timer.h>

#define GPIO_TO_PIN(bank, gpio) (32 * (bank) + (gpio))
#define  LED_ON   1
#define  LED_OFF  0

static  struct   timer_list    leds_timer;
static char led_status = 0 ;
struct misc_leds{
      unsigned int gpio;
	  char* name;
	  int value; 
};

static struct misc_leds _leds[]={
         	{GPIO_TO_PIN(3,16),"led3",0},
			{GPIO_TO_PIN(3,17),"led4",0},
};
//timer handler
static  void   leds_timer_handler(unsigned long  data)
{
    if(!led_status){
		gpio_set_value(_leds[0].gpio,LED_ON) ;
		gpio_set_value(_leds[1].gpio,LED_OFF) ;
		led_status = LED_ON ;
    }else{
		gpio_set_value(_leds[0].gpio,LED_OFF) ;
		gpio_set_value(_leds[1].gpio,LED_ON)  ;
		led_status = LED_OFF;
    }
    
	mod_timer(&leds_timer, jiffies + HZ/1) ;
}
static struct file_operations ledfops={
   .owner = THIS_MODULE,
   	//.write = misc_led_write ,
};
static struct miscdevice  led_misc ={
     .minor = 255 ,
	 	.name = "led_misc",
	 	.fops = &ledfops,
};

static  int  __init   led_module_init(void)
{
    int i ;
	for(i=0;i<ARRAY_SIZE(_leds);i++){
	      gpio_request(_leds[i].gpio, _leds[i].name) ;
		  gpio_direction_output(_leds[i].gpio, _leds[i].value) ;
		}
	setup_timer(&leds_timer,  leds_timer_handler,  (unsigned long)"led_timer");
	mod_timer(&leds_timer, jiffies + HZ/1) ;  //start
	misc_register(&led_misc) ;
	return  0;
}

static  void  __exit   led_module_exit(void)
{
	int i ;
    printk("---rmmod success---\n") ;  

	for(i=0;i<ARRAY_SIZE(_leds);i++){
         gpio_free(_leds[i].gpio) ;
	}

	misc_deregister(&led_misc) ;
	del_timer(&leds_timer);
}

module_init(led_module_init);
module_exit(led_module_exit);

MODULE_LICENSE("GPL");

















