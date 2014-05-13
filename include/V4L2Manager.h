//
//  V4L2Manager.h
//  DeviceListing
//
//  Created by Nikolas Psaroudakis on 5/12/14.
//
//

#ifndef __DeviceListing__V4L2Manager__
#define __DeviceListing__V4L2Manager__

#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <vector>

using namespace std;
class V4L2Manager {
public:
    std::vector< std::string > getVideoDevices();
    int openDevice( const string& dev );
    bool checkSucceed( int er );
    int  getWidth( const string& dev );
    int  getHeight( const string& dev );
    unsigned char * getPixels(const string& dev);
    int             print_caps(int fd);
    int             init_mmap(int fd);
    static int xioctl(int fd, int request, void *arg)
    {
        int r;
        do r = ioctl (fd, request, arg);
        while (-1 == r && EINTR == errno);
        return r;
    }
private:
    uint8_t *   mPixelBuffer;

};
#endif /* defined(__DeviceListing__V4L2Manager__) */


//int main(){
//    int fd;
//    struct video_capability video_cap;
//    struct video_window     video_win;
//    struct video_picture   video_pic;
//
//    if((fd = open("/dev/video0", O_RDONLY)) == -1){
//        perror("cam_info: Can't open device");
//        return 1;
//    }
//
//    if(ioctl(fd, VIDIOCGWIN, &video_win) == -1)
//        perror("cam_info: Can't get window information");
//    else
//        printf("Current size:\t%d x %d\n", video_win.width, video_win.height);
//
//    if(ioctl(fd, VIDIOCGPICT, &video_pic) == -1)
//        perror("cam_info: Can't get picture information");
//    else
//        printf("Current depth:\t%d\n", video_pic.depth);
//
//    close(fd);
//    return 0;
//}
