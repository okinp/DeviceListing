//
//  V4L2Manager.cpp
//  DeviceListing
//
//  Created by Nikolas Psaroudakis on 5/12/14.
//
//

#include "V4L2Manager.h"

std::vector<std::string> V4L2Manager::getVideoDevices()
{
    boost::filesystem::directory_iterator iterator(std::string("/dev"));
	std::vector< std::string > devices;
	for(; iterator != boost::filesystem::directory_iterator(); ++iterator)
    {
        std::string fileName = iterator->path().filename().string();
        if (fileName.find("tty") != std::string::npos) {
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
    return open(dev, O_RDONLY);
}
int  V4L2Manager::getMinWidth( const string& dev )
{
    int fd = openDevice(dev);
    int minWidth = 0;
    if ( checkSucceed(fd) )
    {
        struct video_capability video_cap;
        if ( checkSucceed(ioctl( fd, VIDIOCGCAP, &video_cap )))
        {
            minWidth = video_cap.minwidth;
        }
    }
    close(fd);
    return minWidth;
}
int  V4L2Manager::getMaxWidth( const string& dev )
{
    int fd = openDevice(dev);
    int maxWidth = 0;
    if ( checkSucceed(fd) )
    {
        struct video_capability video_cap;
        if ( checkSucceed(ioctl( fd, VIDIOCGCAP, &video_cap )))
        {
            maxWidth = video_cap.maxwidth;
        }
    }
    close(fd);
    return maxWidth;
}
int  V4L2Manager::getMinHeight( const string& dev )
{
    int fd = openDevice(dev);
    int minHeight = 0;
    if ( checkSucceed(fd) )
    {
        struct video_capability video_cap;
        if ( checkSucceed(ioctl( fd, VIDIOCGCAP, &video_cap )))
        {
            minHeight = video_cap.minheight;
        }
    }
    close(fd);
    return minHeight;
}
int  V4L2Manager::getMaxHeight( const string& dev )
{
    int fd = openDevice(dev);
    int maxHeight = 0;
    if ( checkSucceed(fd) )
    {
        struct video_capability video_cap;
        if ( checkSucceed(ioctl( fd, VIDIOCGCAP, &video_cap )))
        {
            maxHeight = video_cap.maxheight;
        }
    }
    close(fd);
    return maxHeight;
}
unsigned char * V4L2Manager::getPixels(const string& dev)
{
    
}