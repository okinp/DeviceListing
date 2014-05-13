//
//  V4L2Manager.cpp
//  DeviceListing
//
//  Created by Nikolas Psaroudakis on 5/12/14.
//
//

#include "V4L2Manager.h"
#include <boost/filesystem.hpp>
std::vector< std::string > V4L2Manager::getVideoDevices()
{
    boost::filesystem::directory_iterator iterator(std::string("/dev"));
	std::vector< std::string > devices;
	for(; iterator != boost::filesystem::directory_iterator(); ++iterator)
    {
        std::string fileName = iterator->path().filename().string();
        if (fileName.find("video") != std::string::npos) {
            devices.push_back("/dev/"+fileName);
        }
    }
    return devices;
}

bool V4L2Manager::checkSucceed( int er )
{
    if ( er == -1 )
        return false;
    return true;
}

int V4L2Manager::openDevice( const string& dev )
{
    return open(dev.c_str(), O_RDONLY);
}

int  V4L2Manager::getWidth( const string& dev )
{
    int fd = openDevice(dev);
    int minWidth = 0;
    // if ( checkSucceed(fd) )
    // {
    //     struct video_capability video_cap;
    //     if ( checkSucceed(ioctl( fd, VIDIOCGCAP, &video_cap )))
    //     {
    //         minWidth = video_cap.minwidth;
    //     }
    // }
    close(fd);
    return minWidth;
}

int  V4L2Manager::getHeight( const string& dev )
{
    int fd = openDevice(dev);
    int minHeight = 0;
    // if ( checkSucceed(fd) )
    // {
    //     struct video_capability video_cap;
    //     if ( checkSucceed(ioctl( fd, VIDIOCGCAP, &video_cap )))
    //     {
    //         minHeight = video_cap.minheight;
    //     }
    // }
    close(fd);
    return minHeight;
}
unsigned char * V4L2Manager::getPixels(const string& dev)
{
    return nullptr;
}

int V4L2Manager::print_caps(int fd)
{
    struct v4l2_capability caps = {};
    if (-1 == V4L2Manager::xioctl(fd, VIDIOC_QUERYCAP, &caps))
    {
        perror("Querying Capabilities");
        return 1;
    }
    printf( "Driver Caps:\n"
            " Driver: \"%s\"\n"
            " Card: \"%s\"\n"
            " Bus: \"%s\"\n"
            " Version: %d.%d\n"
            " Capabilities: %08x\n",
            caps.driver,
            caps.card,
            caps.bus_info,
            (caps.version>>16)&&0xff,
            (caps.version>>24)&&0xff,
            caps.capabilities );
            char fourcc[5] = {0};
            struct v4l2_format fmt = {0};
            fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            fmt.fmt.pix.width = 640;
            fmt.fmt.pix.height = 480;
            //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24;
            //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_GREY;
            fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
            fmt.fmt.pix.field = V4L2_FIELD_NONE;
            if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
            {
                perror("Setting Pixel Format");
                return 1;
            }
            strncpy(fourcc, (char *)&fmt.fmt.pix.pixelformat, 4);
            printf( "Selected Camera Mode:\n"
                    " Width: %d\n"
                    " Height: %d\n"
                    " PixFmt: %s\n"
                    " Field: %d\n",
                    fmt.fmt.pix.width,
                    fmt.fmt.pix.height,
                    fourcc,
                    fmt.fmt.pix.field);
            return 0;
}
int init_mmap(int fd)
{
    struct v4l2_requestbuffers req = {0};
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == V4L2Manager::xioctl(fd, VIDIOC_REQBUFS, &req))

    {
        perror("Requesting Buffer");
        return 1;
    }

    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if(-1 == V4L2Manager::xioctl(fd, VIDIOC_QUERYBUF, &buf))
    {
        perror("Querying Buffer");
        return 1;
    }

    //buffer = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    //printf("Length: %d\nAddress: %p\n", buf.length, buffer);
    //printf("Image Length: %d\n", buf.bytesused);

    return 0;
}

//int capture_image(int fd)
//{
//    struct v4l2_buffer buf = {0};
//    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//    buf.memory = V4L2_MEMORY_MMAP;
//    buf.index = 0;
//    if(-1 == xioctl(fd, VIDIOC_QBUF, &buf))
//    {
//        perror("Query Buffer");
//        return 1;
//    }
//
//    if(-1 == xioctl(fd, VIDIOC_STREAMON, &buf.type))
//    {
//        perror("Start Capture");
//        return 1;
//    }
//
//    fd_set fds;
//    FD_ZERO(&fds);
//    FD_SET(fd, &fds);
//    struct timeval tv = {0};
//    tv.tv_sec = 2;
//    int r = select(fd+1, &fds, NULL, NULL, &tv);
//    if(-1 == r)
//    {
//        perror("Waiting for Frame");
//        return 1;
//    }
//
//    if(-1 == xioctl(fd, VIDIOC_DQBUF, &buf))
//    {
//        perror("Retrieving Frame");
//        return 1;
//    }
//    printf ("saving image\n");
//
//    IplImage* frame;
//    CvMat cvmat = cvMat(480, 640, CV_8UC3, (void*)buffer);
//    frame = cvDecodeImage(&cvmat, 1);
//    cvNamedWindow("window",CV_WINDOW_AUTOSIZE);
//    cvShowImage("window", frame);
//    cvWaitKey(0);
//    cvSaveImage("image.jpg", frame, 0);
//
//    return 0;
//}
