
#include <iostream>
#include <fcntl.h>   
#include <unistd.h> 
#include <sys/ioctl.h>

#include <ioctl.h>

int main(int argc, char *argv[])
{

    ha::scoped_resource<int, char*, int> file_desc(::open, DEVPATH, O_RDONLY, ::close);

    if (file_desc < 0)
	{
		throw std::runtime_error(std::string("open() failed: ") + ::strerror(errno));
	}
        
	KMParam param;
	param.countThread = 3;
	param.calls = 22;
	param.bufSize = 33;

	int ret_val;
	ret_val = ioctl(file_desc, IOCTL_SET_KMParam, &param);
	if (ret_val < 0) 
	{
		std::cout << "ioctl_set_msg failed: " << ret_val << std::endl;
		return -1;
	}

	// cat /proc/devices | grep driver_dev 
	// 251 driver_dev
	//sudo mknod -m0666 /dev/qqq_123 c 251 0 
	return 0;
}
