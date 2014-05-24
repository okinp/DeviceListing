//
//  V4L2Device.h
//  DeviceListing
//
//  Created by Nikolas Psaroudakis on 5/13/14.
//
//

#ifndef __DeviceListing__V4L2Device__
#define __DeviceListing__V4L2Device__

#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

#include "cinder/Surface.h"
using namespace std;
class V4L2Device {
public:
    V4L2Device();
    V4L2Device( const string& name );
    ~V4L2Device();

    ci::Surface         getImage();

    int             openDevice();
    int             closeDevice();
    void            initDevice();
    void            captureStart();
    void            captureStop();
    void            loop();
    int             getWidth();
    int             getHeight();

    uint8_t *       getPixels();

    int             print_caps();
    int             init_mmap();
    int             updateImage();

    static int xioctl(int fd, int request, void *arg)
    {
        int r;
        do r = v4l2_ioctl (fd, request, arg);
        while (-1 == r && EINTR == errno);
        return r;
    }
private:
    int             frameRead();
    void            createSurface(const void* p);
    uint8_t *       mPixelBuffer;
    int             mFileDescriptor;
    string          mName;
    ci::Surface     mFrame;
    io_method       mIo = IO_METHOD_MMAP;
    struct buffer * mBuffers = nullptr;
    unsigned int    mNumBuffers = 0;
    // global settings
    unsigned int mWidth = 640;
    unsigned int mHeight = 480;
    unsigned int mHeight = 480;
    unsigned int mFps = 30;
    SurfaceT<uint8_t> mSurface;
};

#endif /* defined(__DeviceListing__V4L2Device__) */
