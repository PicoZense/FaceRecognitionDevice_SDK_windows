#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "PsCamera_api.h"
#include <thread>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	uint32_t slope = 1450;
	int ret = -1;
	ret = init();
	if (ret != 0)
	{
		cout << "init failed!" << endl;
		system("pause");
		return -1;
	}

	ret = start();
	if (ret != 0)
	{
		cout << "start failed!" << endl;
		system("pause");
		return -1;
	}

	ret = setWorkMode(ScanFace);
	if (ret != 0)
	{
		cout << "setWorkMode failed!" << endl;
		system("pause");
		return -1;
	}
	ret = setMappedEnabled(true);
	if (ret != 0)
	{
		cout << "setMappedEnabled failed!" << endl;
		system("pause");
		return -1;
	}
	bool tofEnabled = false;
	bool rgbEnabled = false;
	int mirror = 0;
	bool bMapped = false;
	cv::Mat imageMatDepth;
	cv::Mat imageMatIr;
	cv::Mat imageMatRGB;
	const string RgbImageWindow   = "RGB Image";
	const string DepthImageWindow = "Depth Image";
	const string IRImageWindow    = "IR Image";
	
	cout << "\n-------------------------------------------------------------------------" << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "Press following key to set corresponding feature:" << endl;
	cout << "M/m: Change data mode: input corresponding index in terminal:" << endl;
	cout << "                    1: Face mode" << endl;
	cout << "                    2: Code mode" << endl;
	cout << "                    3: Standby" << endl;
	cout << endl;
	cout << "******************************************************************** " << endl;
	cout << "The following key is effective that the Camera is in FaceMode or CodeMode" << endl;
	cout << "******************************************************************** " << endl;
	cout << endl;
	cout << "R/r: Change the RGB resolution: input corresponding index in terminal:" << endl;
	cout << "                    0: 1920*1080" << endl;
	cout << "                    1: 1280*720" << endl;
	cout << "                    2: 640*480" << endl;
	cout << "                    3: 640*360" << endl;
	cout << "G/g: Get the device info" << endl;
	cout << "A/a: Set GmmGain" << endl;
	cout << "S/s: Get GmmGain" << endl;
	cout << "Q/q: Change the status that the TofFrame can be Get or Not," << endl;
	cout << "     and if the RgbFrame can not be Get ,this change should not be done" << endl;
	cout << "W/w: Change the status that the RgbFrame can be Get or not" << endl;
	cout << "0/1/2/3: Change Mirror  no mirror/left-right/up-down/both mirror" << endl;
	cout << "E/e: Enable/Disable the Mapped feature " << endl;
	cout << "Esc: Program quit " << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "-------------------------------------------------------------------------\n" << endl;

	for (;;)
	{
		Frame rgbFrame = { 0 };
		Frame depthFrame = { 0 };
		Frame irFrame = { 0 };

		FrameReady ready = { 0 };
		ret = readNext(&ready);
		if (ret != 0)
		{
			cout << "readNext failed " << endl;
			continue;
		}

		if (1 == ready.depth)
		{
			ret = getFrame(DepthFrame, &depthFrame);
			if (ret != 0)
			{
				cout << "DepthFrame failed " << endl;
			}
			else
			{
				if (depthFrame.frameData != NULL)
				{
					imageMatDepth = cv::Mat(depthFrame.height, depthFrame.width, CV_16UC1, depthFrame.frameData);
					cv::Mat mappedDepthMat;
					imageMatDepth.convertTo(mappedDepthMat, CV_8U, 255.0 / slope);
					cv::applyColorMap(mappedDepthMat, mappedDepthMat, cv::COLORMAP_RAINBOW);

					cv::imshow(DepthImageWindow, mappedDepthMat);
				}
			}
		}

		if (1 == ready.ir)
		{
			ret = getFrame(IRFrame, &irFrame);
			if (ret != 0)
			{
				cout << "IRFrame failed " << endl;
			}
			else
			{
				if (irFrame.frameData != NULL)
				{
					imageMatIr = cv::Mat(irFrame.height, irFrame.width, CV_16UC1, irFrame.frameData);
					imageMatIr.convertTo(imageMatIr, CV_8U, 255.0 / 3840);
					cv::imshow(IRImageWindow, imageMatIr);
				}
			}
		}

		if (1 == ready.rgb)
		{
			ret = getFrame(RGBFrame, &rgbFrame);
			if (ret != 0)
			{
				cout << "RGBFrame failed " << endl;
			}
			else
			{
				if (rgbFrame.frameData != NULL)
				{
					imageMatRGB = cv::Mat(rgbFrame.height, rgbFrame.width, CV_8UC3, rgbFrame.frameData);
					cv::imshow(RgbImageWindow, imageMatRGB);
				}
			}
		}
 
		unsigned char key = waitKey(1);
		imageMatDepth.release();
		imageMatIr.release();
		imageMatRGB.release();

		if (key == 'M' || key == 'm')
		{
			cout << "Selection: 1:ScanFace; 2:ScanCode; 3:StandBy" << endl;
			int index = -1;
			cin >> index;
			//clear buffer and cin flag. if not, cin will not get input anymore;
			if (cin.fail())
			{
				std::cout << "Unexpected input\n";
				cin.clear();
				cin.ignore(1024, '\n');
				continue;
			}
			else
			{
				cin.clear();
				cin.ignore(1024, '\n');
			}

			ret = setWorkMode((WorkMode)index);
			if (ret != 0)
			{
				cout << "setWorkMode  failed" << endl;
			}

		}
		else if (key == 'R' || key == 'r')
		{
			cout << "please select RGB resolution to set: 0:1080P; 1:720P; 2:480P; 3:360P" << endl;
			int index = 0;
			cin >> index;
			if (cin.fail())
			{
				std::cout << "Unexpected input\n";
				cin.clear();
				cin.ignore(1024, '\n');
				continue;
			}
			else
			{
				cin.clear();
				cin.ignore(1024, '\n');
			}

			ret = setRgbResolution(index);
			if (ret != 0)
			{
				cout << "setRgbResolution failed" << endl;
			}
		}
		else if (key == 'G' || key == 'g')
		{
			cout << "getGmmGain : " << getGmmGain() << endl;

			char sn[63] = {0};
			getSn(sn, sizeof(sn));
			cout << "SN: " << sn << endl;

			char fw[63] = {0};
			getFWVersion(fw, sizeof(fw));
			cout << "FW: " << fw << endl;

			char hw[63] = {0};
			getHWVersion(hw, sizeof(hw));
			cout << "FW: " << hw << endl;

			char name[63] = {0};
			getDeviceName(name, sizeof(name));
			cout << "DeviceName: " << name << endl;

			CameraParameter cameraParameters = { 0 };
			ret = getDepthCameraParameter(&cameraParameters);

			cout << "Get PsGetCameraParameters status: " << ret << endl;
			cout << "Depth Camera Intinsic: " << endl;
			cout << "Fx: " << cameraParameters.fx << endl;
			cout << "Cx: " << cameraParameters.cx << endl;
			cout << "Fy: " << cameraParameters.fy << endl;
			cout << "Cy: " << cameraParameters.cy << endl;
			cout << "Depth Distortion Coefficient: " << endl;
			cout << "K1: " << cameraParameters.k1 << endl;
			cout << "K2: " << cameraParameters.k2 << endl;
			cout << "P1: " << cameraParameters.p1 << endl;
			cout << "P2: " << cameraParameters.p2 << endl;

			ret = getRgbCameraParameter(&cameraParameters);

			cout << "Get PsGetCameraParameters status: " << ret << endl;
			cout << "RGB Camera Intinsic: " << endl;
			cout << "Fx: " << cameraParameters.fx << endl;
			cout << "Cx: " << cameraParameters.cx << endl;
			cout << "Fy: " << cameraParameters.fy << endl;
			cout << "Cy: " << cameraParameters.cy << endl;
			cout << "RGB Distortion Coefficient: " << endl;
			cout << "K1: " << cameraParameters.k1 << endl;
			cout << "K2: " << cameraParameters.k2 << endl;
			cout << "P1: " << cameraParameters.p1 << endl;
			cout << "P2: " << cameraParameters.p2 << endl;

			CameraExtrinsicParameter CameraExtrinsicParameters = { 0 };
			ret = getCameraExtrinsicParameter(&CameraExtrinsicParameters);

			cout << "Get PsGetCameraExtrinsicParameters status: " << ret << endl;
			cout << "Camera rotation: " << endl;
			cout << CameraExtrinsicParameters.rotation[0] << " "
				<< CameraExtrinsicParameters.rotation[1] << " "
				<< CameraExtrinsicParameters.rotation[2] << " "
				<< CameraExtrinsicParameters.rotation[3] << " "
				<< CameraExtrinsicParameters.rotation[4] << " "
				<< CameraExtrinsicParameters.rotation[5] << " "
				<< CameraExtrinsicParameters.rotation[6] << " "
				<< CameraExtrinsicParameters.rotation[7] << " "
				<< CameraExtrinsicParameters.rotation[8] << " "
				<< endl;

			cout << "Camera transfer: " << endl;
			cout << CameraExtrinsicParameters.translation[0] << " "
				<< CameraExtrinsicParameters.translation[1] << " "
				<< CameraExtrinsicParameters.translation[2] << " "
				<< endl;

			cout << "Camera e: " << endl;
			cout << CameraExtrinsicParameters.e[0] << " "
				<< CameraExtrinsicParameters.e[1] << " "
				<< CameraExtrinsicParameters.e[2] << " "
				<< CameraExtrinsicParameters.e[3] << " "
				<< CameraExtrinsicParameters.e[4] << " "
				<< CameraExtrinsicParameters.e[5] << " "
				<< CameraExtrinsicParameters.e[6] << " "
				<< CameraExtrinsicParameters.e[7] << " "
				<< CameraExtrinsicParameters.e[8] << " "
				<< endl;

			cout << "Camera f: " << endl;
			cout << CameraExtrinsicParameters.f[0] << " "
				<< CameraExtrinsicParameters.f[1] << " "
				<< CameraExtrinsicParameters.f[2] << " "
				<< CameraExtrinsicParameters.f[3] << " "
				<< CameraExtrinsicParameters.f[4] << " "
				<< CameraExtrinsicParameters.f[5] << " "
				<< CameraExtrinsicParameters.f[6] << " "
				<< CameraExtrinsicParameters.f[7] << " "
				<< CameraExtrinsicParameters.f[8] << " "
				<< endl;

		}
		else if (key == 'A' || key == 'a')
		{
			static int gmm = 0;
			gmm += 100;
			if (gmm > 600)
				gmm = 100;
			ret = setGmmGain(gmm);
			cout << "setGmmGain :   " << gmm << " status: " << ret << endl;
		}
		else if (key == 'S' || key == 's')
		{
			cout << "getGmmGain :   " << getGmmGain() << endl;
		}
		else if (key == 'Q' || key == 'q')
		{
			ret = setTofFrameEnabled(tofEnabled);
			cout << "setTofFrameEnabled :   " << tofEnabled << " status: " << ret << endl;
			tofEnabled = !tofEnabled;
		}
		else if (key == 'W' || key == 'w')
		{
			ret = setRgbFrameEnabled(rgbEnabled);
			cout << "setRgbFrameEnabled :   " << rgbEnabled << " status: " << ret << endl;
			rgbEnabled = !rgbEnabled;
		}
		else if ((key == '0') || (key == '1') || (key == '2') || (key == '3'))
		{
			switch (key)
			{
			case '0':
				mirror = 0;
				break;
			case '1':
				mirror = 1;
				break;
			case '2':
				mirror = 2;
				break;
			case '3':
				mirror = 3;
				break;
			default:
				mirror = 0;
				continue;
			}

			ret = setImageMirror(mirror);
			cout << "setImageMirror :   " << mirror << " status: " << ret << endl;
		}
		else if (key == 'E' || key == 'e')
		{	 
			ret = setMappedEnabled(bMapped);
			cout << "setMappedEnabled :   " << bMapped << " status: " << ret << endl;
			bMapped = !bMapped;
		}
		else if (key == 27)	//ESC Pressed
		{
			break;
		}
	}

	ret = stop();
	cout << "stop status: " << ret << endl;

	ret = destroy();
	cout << "destroy status: " << ret << endl;

	cv::destroyAllWindows();

	return 0;
}
