#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdint.h>
#include <stdexcept>

#include "libapogee/Alta.h"
#include "libapogee/CameraInfo.h"
#include "libapogee/FindDeviceEthernet.h"
#include "apgSampleCmn.h"

////////////////////////////
// MAIN
int main()
{
	try
	{
        std::cout << "Apogee Alta Simple Exposure Sample" << std::endl;
        
        // use this for ethernet cameras
        std::string ioInterface("ethernet");
        FindDeviceEthernet look4cam;
        std::string subnet = "192.168.1.255";
        std::string msg = look4cam.Find( subnet );
		std::string addr = apgSampleCmn::GetEthernetAddress( msg );

        // std::string ioInterface("ethernet");
        // std::string addr = "192.168.1.8";
        
		uint16_t id = apgSampleCmn::GetID( msg );
        uint16_t frmwrRev = apgSampleCmn::GetFrmwrRev( msg );
        
		//create the camera object and conneting to the camera
		// assuming alta camera for this example, use the Ascent
		// camera object for Ascent cameras.
        Alta cam;
        // cam.OpenConnection( ioInterface, addr, 27, 2 );
		cam.OpenConnection( ioInterface, addr, frmwrRev, id );
		cam.Init();
		
        // print basic camera info
        std::cout << "Connected to Alta camera " << std::endl;
		std::cout << addr << std::endl;
		apgSampleCmn::printInfo( cam.GetModel(),  cam.GetMaxImgRows(),
								cam.GetMaxImgCols() );
        
        // fetch the default image size
		const uint16_t rows = cam.GetRoiNumRows();
		const uint16_t cols = cam.GetRoiNumCols();
		
		//allocate vector for the image data
		std::vector<uint16_t> img( rows*cols );

		// start 0.1 sec light (shutter will open) expousre
		const double exposeTime = 0.01;
		std::cout << "Starting a " << exposeTime << " sec light exposure" << std::endl;
		cam.StartExposure( exposeTime, true );
		
		// Check camera status to make sure image data is ready
		Apg::Status status = Apg::Status_Flushing;
		while( Apg::Status_ImageReady !=  status )
		{
			status = cam.GetImagingStatus();
			//make sure there isn't an error
			//throw here if there is
			apgSampleCmn::checkStatus( status );
		}

		std::cout << "Retrieving image data from camera" << std::endl;

		cam.GetImage( img );

		std::cout << "Writing image to file" << std::endl;
		
		// Write the test image to an output file (overwrite if it already exists)
		apgSampleCmn::saveImg2File( (char*)&img[0], "simpleExposure.bin",
					 img.size()*sizeof(uint16_t) );
	
        std::cout << "Closing connection to camera " << std::endl;
        cam.CloseConnection();
	}
	catch( std::exception & err )
	{
		std::cout << "std exception what = " << err.what() << std::endl;
		return 1;
	}
	catch( ... )
	{
		std::cout << "Unknown exception caught." << std::endl;
		return 1;
	}

	return 0;
}