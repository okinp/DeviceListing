#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "V4L2Manager.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class DeviceListingApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
    V4L2Manager manager;
};

void DeviceListingApp::setup()
{

}

void DeviceListingApp::mouseDown( MouseEvent event )
{

    for ( auto& dev: manager.getVideoDevices()  )
    {
        std::cout << dev << std::endl;

    }

    std::cout << manager.getMinWidth(manager.getVideoDevices().front() ) << std::endl;

}

void DeviceListingApp::update()
{
}

void DeviceListingApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
}

CINDER_APP_NATIVE( DeviceListingApp, RendererGl )
