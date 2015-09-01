#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

//#include "usbdevice.h"
//#include "usbutils.h"
#include <libudev.h>

struct udev *udev;
struct udev_monitor *udev_monitor;
int udev_monitor_fd;
struct udev_device  *udev_device = NULL;

int udev_handle_init()
{
	udev = udev_new();
	if (udev == NULL)
	{
		printf("\033[31m jolin log   : \033[0m" "%s %s %4d error:%s\n",
				__FILE__, __func__, __LINE__, strerror(errno));
		return -1;
	}

	udev_monitor = udev_monitor_new_from_netlink(udev, "udev");
	if (udev_monitor == NULL)
	{
		printf("\033[31m jolin log   : \033[0m" "%s %s %4d error:%s\n",
				__FILE__, __func__, __LINE__, strerror(errno));
		return -1;
	}

	if (udev_monitor_filter_add_match_subsystem_devtype(
				udev_monitor, "usb", NULL))
	{
		printf("\033[31m jolin log   : \033[0m" "%s %s %4d error:%s\n",
				__FILE__, __func__, __LINE__, strerror(errno));
		return -1;
	}

	if (udev_monitor_enable_receiving(udev_monitor))
	{
		printf("\033[31m jolin log   : \033[0m" "%s %s %4d error:%s\n",
				__FILE__, __func__, __LINE__, strerror(errno));
		return -1;
	}

	udev_monitor_fd = udev_monitor_get_fd(udev_monitor);
	if (udev_monitor_fd < 0)
	{
		printf("\033[31m jolin log   : \033[0m" "%s %s %4d error:%s\n",
				__FILE__, __func__, __LINE__, strerror(errno));
		return -1;
	}

	return 0;
}

void show( struct udev_device  *udev_device )
{
	const char *action
		= udev_device_get_action(udev_device);
	const char *devpath
		= udev_device_get_devpath(udev_device);
	const char *subsystem
		= udev_device_get_subsystem(udev_device);
	const char *devtype
		= udev_device_get_devtype(udev_device);
	const char *syspath
		= udev_device_get_syspath(udev_device);

	const char *sysname
		= udev_device_get_sysname(udev_device);

	const char *sysnum
		= udev_device_get_sysnum(udev_device);

	const char *devnode
		= udev_device_get_devnode(udev_device);

	int isinitialized
		= udev_device_get_is_initialized(udev_device);

	const char *driverrr
		= udev_device_get_driver(udev_device);

	printf("\nstart\n");
	printf("%s %d action %s \n", __func__, __LINE__, action);

	printf("%s %d devpath %s\n", __func__, __LINE__, devpath);

	printf("%s %d subsystem %s\n", __func__, __LINE__, subsystem);

	printf("%s %d devtype %s\n", __func__, __LINE__, devtype);

	printf("%s %d syspath %s\n", __func__, __LINE__, syspath);

	printf("%s %d sysname %s\n", __func__, __LINE__, sysname);

	printf("%s %d sysnum %s\n", __func__, __LINE__, sysnum);

	printf("%s %d devnode %s\n", __func__, __LINE__, devnode);

	printf("%s %d isinitialized %d\n", __func__, __LINE__, isinitialized);

	printf("%s %d driverrr %s\n", __func__, __LINE__, driverrr);

	printf("show parentstart \n");
	
	printf("show parentend \n");
	printf("end\n");

}

int udevmanager_handle_udev_event()
{

	udev_device = udev_monitor_receive_device(udev_monitor);
	if (udev_device == NULL)
	{
		printf("\033[31m jolin log   : \033[0m" "%s %s %4d error:%s\n",
				__FILE__, __func__, __LINE__, strerror(errno));
		return -1;
	}
	show(udev_device );

	struct udev_device *udev_device_parent = udev_device;

	if (strncmp(udev_device_get_devtype(udev_device),
				"usb_device", strlen("usb_device")))
	{
		udev_device_parent =
			udev_device_get_parent_with_subsystem_devtype
			(udev_device, "usb", "usb_device");
	}
//	printf("udev_device_get_devtype of parent %s\n", udev_device_get_devtype(udev_device_parent));
//	show(udev_device_parent);

	return 0;
}

int main()
{
	udev_handle_init();
	int ret;
	for(; ;)
	{
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(udev_monitor_fd, &fds);

		ret = select(FD_SETSIZE, &fds, NULL, NULL, NULL);

		if ((ret > 0 ) &&
				(FD_ISSET(udev_monitor_fd, &fds)))
		{
			udevmanager_handle_udev_event();
		}
	}
	return 0;
}

