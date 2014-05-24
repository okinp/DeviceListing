#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"
#include "V4L2Device.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class DeviceListingApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
    V4L2Device mWebCam;
    int fd;
    bool first = true;
    SurfaceT<uint8_t> mSurface;
};

void DeviceListingApp::setup()
{
    fd = mWebCam.openDevice();
    mWebCam.init_mmap();
}

void DeviceListingApp::mouseDown( MouseEvent event )
{



    //std::cout << manager.getMinWidth(manager.getVideoDevices().front() ) << std::endl;

}

void DeviceListingApp::update()
{
    if ( fd!=-1 )
    {

        std::cout << "Update" <<std::endl;
        //std::cout << mWebCam.updateImage() << std::endl;
        mSurface = SurfaceT<uint8_t>(mWebCam.getPixels(), mWebCam.getWidth(), mWebCam.getHeight(), mWebCam.getWidth()*3, SurfaceChannelOrder::RGB );
    }
}

void DeviceListingApp::draw()
{
	// clear out the window with black
	if (true )
    {
        std::cout << "draw" <<std::endl;
        gl::clear( Color( 0, 0, 0 ) );
        //mWebCam.updateImage();
        gl::draw( gl::Texture(mSurface));
        first = false;
    }
}

CINDER_APP_NATIVE( DeviceListingApp, RendererGl )
