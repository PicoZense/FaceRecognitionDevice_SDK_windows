#ifndef PsCamera_API_H
#define PsCamera_API_H

/**
* @file PsCamera_API.h
* @brief Pico Zense API header file.
* Copyright (c) 2018-2019 Pico Interactive, Inc.
*/

/*! \mainpage Pico Zense API Documentation
*
* \section intro_sec Introduction
*
* Welcome to the PsCamera API documentation. This documentation enables you to quickly get started in your development efforts to programmatically interact with the Pico Zense TOF RGBD Camera.
*/

#include "PsCamera_define.h"

//enum
/**
* @brief Specifies the image pixel format.
*/
typedef enum {
	PixelFormatDepthMM16,        //!< Depth image pixel format, 16 bits per pixel in mm.
	PixelFormatGray16,           //!< IR image pixel format, 16 bits per pixel.
	PixelFormatGray8,            //!< Gray image pixel format, 8 bits per pixel.

	//Color
	PixelFormatRGB888,           //!< Color image pixel format, 24 bits per pixel RGB format.
	PixelFormatBGR888            //!< Color image pixel format, 24 bits per pixel BGR format.
}PixelFormat;

/**
* @brief Specifies the type of image frame.
*/
typedef enum {
	DepthFrame = 0,              //!< Depth frame with 16 bits per pixel in millimeters.
	IRFrame = 1,                 //!< IR frame with 16 bits per pixel.
	RGBFrame = 2,                //!< RGB frame with 24 bits per pixel in RGB/BGR format.
}FrameType;

/**
* @brief Specifies the work mode of camera.
*/
typedef enum {
	ScanFace = 1,              //!< Depth_640*480@15fps + IR_640*480@15fps + RGB_640*480@30fps.
	ScanCode = 2,              //!< RGB_640*480@30fps.
	Standby = 3,               //!< No image.
}WorkMode;

#pragma pack (push, 1)
//struct
typedef struct
{
        int depth;
        int ir;
        int rgb;
}FrameReady;

/**
* @brief Depth/IR/RGB image frame data.
*/
typedef struct
{
	int frameType;
	int pixelFormat;
	int fps;
	int width;
	int height;
	int bytesPerPixel;
	int dataLength;
	int frameIndex;
	long timeStamp;
	uint8_t* frameData;
}Frame;

/**
* @brief Camera intrinsic parameters and distortion coefficients.
*/
typedef struct
{
	double	fx;  //!< Focal length x (pixel)
	double	fy;  //!< Focal length y (pixel)
	double	cx;  //!< Principal point x (pixel)
	double	cy;  //!< Principal point y (pixel)
	double	k1;  //!< Radial distortion coefficient, 1st-order
	double	k2;  //!< Radial distortion coefficient, 2nd-order
	double	p1;  //!< Tangential distortion coefficient
	double	p2;  //!< Tangential distortion coefficient
}CameraParameter;

/**
* @brief Specifies the cameras location and orientation extrinsic parameters.
*/
typedef struct
{
	double rotation[9];     //!< Orientation stored as an array of 9 double representing a 3x3 rotation matrix.
	double translation[3];  //!< Location stored as an array of 3 double representing a 3-D translation vector.
	double e[9];
	double f[9];
}CameraExtrinsicParameter;

#pragma pack (pop)

/*
*  Init PicoZense SDK, should be called first before calling any other api
*  @Parameters: void
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int init();

/*
*  destroy PicoZense SDK, release all resources the SDK created, it is forbidden to call any other api after destroy is called
*  @Parameters: void
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int destroy();

/*
*  Start to capture the image frame
*  @Parameters: void
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int start();

/*
*  Stop to capture the image frame
*  @Parameters: void
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int stop();

/*
*  Read frame from the corresponding device by this api, it should be called first before you want to get one frame data through getFrame
*  @Parameters: 
*	ready[Out]: image types available through getFrame
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int readNext(FrameReady *ready);

/*
*  Get one frame data by this api, it should be called after readNext
*  @Parameters: 
*	frameType[In]: frame type, refer to FrameType
*	pFrame[Out]: pointer to the variable that used to store returned Frame, 
*					which the frameData of Frame point to the buffer stored image frame, 
*					you need to create a Frame variable and pass its pointer to this api 
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int getFrame(FrameType frameType, Frame* pFrame);
/*
*  Set the working mode of the camera
*  @Parameters:
*	workMode[In]: work mode to be set, refer to WorkMode
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int setWorkMode(WorkMode workMode);

/*
*  Set gmmgain of IR frame
*  @Parameters:
*	gmmGain[In]: the gmmgain value, its range is 0 to 4095
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int setGmmGain(int gmmGain);

/*
*  Get gmmgain of IR frame
*  @Parameters: void
*  @Return: nonnegative:current gmmgain of IR frame, Others: Failed
*/
PSCAMERA_C_API_EXPORT int getGmmGain();

/*
*  Set resolution of RGB frame
*  @Parameters:
*	resolutionIndex[In]: 0:1920*1080; 1:1280*720; 2:640*480; 3:640*360
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int setRgbResolution(int resolutionIndex);

/*
*  Get detpth camera intrinsic and distortion coefficient parameters
*  @Parameters:
*	pDepthParameter[Out]: pointer to the CameraParameter structure variable that used to store returned depth camera parameters
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int getDepthCameraParameter(CameraParameter* pDepthParameter);

/*
*  Get rgb camera intrinsic and distortion coefficient parameters
*  @Parameters:
*	pRgbParameter[Out]: pointer to the CameraParameter structure variable that used to store returned rgb camera parameters
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int getRgbCameraParameter(CameraParameter* pRgbParameter);

/*
*  Get camera rotation and transfer coefficient parameters
*  @Parameters:
*	pExtrinsicParameter[out]: pointer to the CameraExtrinsicParameter structure variable that used to store returned extrinsic parameters
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int getCameraExtrinsicParameter(CameraExtrinsicParameter* pExtrinsicParameter);

/*
*  Get the serial number of the device
*  @Parameters:
*	sn[out]: Pointer to a buffer containing the serial number
*	length[in]: length of buf pointed by sn
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int getSn(char* sn, int length);

/*
*  Get firmware version of the device
*  @Parameters:
*	fw[out]: Pointer to a buffer containing the firmware version
*	length[in]: length of buf pointed by fw
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int getFWVersion(char* fw, int length);

/*
*  Get hardware version of the device
*  @Parameters:
*	hw[out]: Pointer to a buffer containing the hardware version
*	length[in]: length of buf pointed by hw
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int getHWVersion(char* hw, int length);

/*
*  Get name of the device
*  @Parameters:
*	hw[out]: Pointer to a buffer containing the hardware version
*	length[in]: length of buf pointed by hw
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int getDeviceName(char* name, int length);

/*
*  set switch on/off of ir and depth frame 
*  @Parameters:
*	bEnabled[in]: true:switch on; false: switch off
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int setTofFrameEnabled(bool bEnabled);

/*
*  set switch on/off of rgb frame 
*  @Parameters:
*	bEnabled[in]: true:switch on; false: switch off
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int setRgbFrameEnabled(bool bEnabled);

/*
*  Set image mirror operation type 
*  @Parameters:
*	mirrorValue[in]: 1 left-right mirror; 2 up-down mirror;3 both mirror (rotation 180)
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int setImageMirror(int mirrorValue);
/*
*  set switch on/off of Mapped
*  @Parameters:
*	bEnabled[in]: true:switch on; false: switch off
*  @Return: 0: Succeed, Others: Failed
*/
PSCAMERA_C_API_EXPORT int setMappedEnabled(bool bEnabled);

#endif /* PsCamera_API_H */
