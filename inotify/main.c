#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
int fd;

int inotify_handle_init()
{
	char path[] = "/tmp/.automount/";

	fd = inotify_init();
	if(fd < 0)
	{
		printf("%s %d\n", __func__, __LINE__);
	}

	int wd = inotify_add_watch (fd, path, IN_DELETE | IN_CREATE | IN_CLOSE_WRITE);

	return 0;
}

int udevmanager_handle_inotify_event()
{
	char buf[1024] = {0};

	struct inotify_event *event;

	if (read(fd, buf, sizeof(buf)) < 0)
    {
        printf("\033[31m jolin log   : \033[0m" "%s %s %4d %s\n",
                __FILE__, __func__, __LINE__, strerror(errno));
        return -1;
    }

	 event = (struct inotify_event *)buf;

	 printf("%s %dmask %d\n", __func__, __LINE__, event->mask);

	 printf("%s %dname %s len %d\n", __func__, __LINE__, event->name, event->len);

	return 0;
}

int main()
{
	inotify_handle_init();
	int ret;
	for(; ;)
	{
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		ret = select(FD_SETSIZE, &fds, NULL, NULL, NULL);

		if ((ret > 0 ) &&
			(FD_ISSET(fd, &fds)))
		{
			udevmanager_handle_inotify_event();
		}
	}
	return 0;
}

