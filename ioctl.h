
#ifndef __IOCTL_KM_H__
#define __IOCTL_KM_H__   

#include <linux/ioctl.h>


// _IO - ioctl без параметра;
// _IOW - параметры копируются от пользователя в модуль;
// _IOR - параметры заполняются модулем и передаются пользователю;
// _IOWR - параметры могут передаваться в обе стороны.

#define DEVICE_FILE_NAME "driver_dev"

struct ThreadEvent 
{
    wait_queue_head_t wq;
    atomic_t thread_count;
};

struct KMParam 
{ 
	struct ThreadEvent eventStartThread;
	struct ThreadEvent eventStartTest;
	struct ThreadEvent eventStop;
}; 


#define IOC_MAGIC    'h' 
#define IOCTL_SET_KMParam _IOWR( IOC_MAGIC, 1, struct KMParam* ) 

 
#define DEVPATH "/dev/"DEVICE_FILE_NAME


#endif
