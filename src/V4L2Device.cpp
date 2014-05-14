//
//  V4L2Device.cpp
//  DeviceListing
//
//  Created by Nikolas Psaroudakis on 5/13/14.
//
//

#include "V4L2Device.h"

V4L2Device::V4L2Device()
:mName("/dev/video0")
{
    
}
V4L2Device::V4L2Device( const string& name )
:mName(name)
{
    
}
V4L2Device::~V4L2Device()
{
    close();
}

int V4L2Device::open()
{
    mFileDescriptor = open(mName.c_str(), O_RDONLY)
    return -1;
}

int V4L2Device::close()
{
    return close(mFileDescriptor);
}
int V4L2Device::getWidth()
{
    
}
int V4L2Device::getHeight()
{
    
}
uint8_t * V4L2Device::getPixels()
{
    if ( -1 == updateImage() )
    {
        return nullptr;
    }
    return mPixelBuffer;
}
int V4L2Device::print_caps()
{
    struct v4l2_capability caps = {};
    if (-1 == V4L2Manager::xioctl(mFileDescriptor, VIDIOC_QUERYCAP, &caps))
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
    if (-1 == xioctl(mFileDescriptor, VIDIOC_S_FMT, &fmt))
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
int V4L2Device::init_mmap()
{
    struct v4l2_requestbuffers req = {0};
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    
    if (-1 == V4L2Manager::xioctl(mFileDescriptor, VIDIOC_REQBUFS, &req))
        
    {
        perror("Requesting Buffer");
        return 1;
    }
    
    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if(-1 == V4L2Manager::xioctl(mFileDescriptor, VIDIOC_QUERYBUF, &buf))
    {
        perror("Querying Buffer");
        return 1;
    }
    
    mPixelBuffer = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, mFileDescriptor, buf.m.offset);
    //printf("Length: %d\nAddress: %p\n", buf.length, buffer);
    //printf("Image Length: %d\n", buf.bytesused);
    
    return 0;
}
int V4L2Device::updateImage()
{
    struct v4l2_buffer buf = {0};
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if(-1 == xioctl(mFileDescriptor, VIDIOC_QBUF, &buf))
    {
        perror("Query Buffer");
        return 1;
    }
    
    if(-1 == xioctl(mFileDescriptor, VIDIOC_STREAMON, &buf.type))
    {
        perror("Start Capture");
        return 1;
    }
    
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(mFileDescriptor, &fds);
    struct timeval tv = {0};
    tv.tv_sec = 2;
    int r = select(mFileDescriptor+1, &fds, NULL, NULL, &tv);
    if(-1 == r)
    {
        perror("Waiting for Frame");
        return 1;
    }
    
    if(-1 == xioctl(mFileDescriptor, VIDIOC_DQBUF, &buf))
    {
        perror("Retrieving Frame");
        return 1;
    }
    mFrame = 	Surface( mPixelBuffer, 640, 480, 640*3, ci::SurfaceChannelOrder::RGB);
    return 0;
}
Surface V4L2Device::getImage()
{
    if ( -1 == updateImage() )
    {
        return Surface();
    }
    return mFrame;
}