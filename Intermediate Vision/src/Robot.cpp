#include "WPILib.h"
#include <iostream>
/**
 * Uses IMAQdx to manually acquire a new image each frame, and annotate the image by drawing
 * a circle on it, and show it on the FRC Dashboard.
 */
class IntermediateVisionRobot : public SampleRobot
{
	//A structure to hold measurements of a particle
	struct ParticleReport {
		double PercentAreaToImageArea;
		double Area;
		double BoundingRectLeft;
		double BoundingRectTop;
		double BoundingRectRight;
		double BoundingRectBottom;
	};

	//Structure to represent the scores for the various tests used for target identification
	struct Scores {
		double Area;
		double Aspect;
	};


	IMAQdxSession session;
	Image *frame;
	Image *binaryFrame;
	IMAQdxError imaqError;

	//Constants
	Range RING_HUE_RANGE = {0, 360};	//Default hue range for ring light
	Range RING_SAT_RANGE = {0, 50};	//Default saturation range for ring light
	Range RING_VAL_RANGE = {200, 255};	//Default value range for ring light
	double AREA_MINIMUM = 0.5; //Default Area minimum for particle as a percentage of total image area
	double LONG_RATIO = 2.22; //Tote long side = 26.9 / Tote height = 12.1 = 2.22
	double SHORT_RATIO = 1.4; //Tote short side = 16.9 / Tote height = 12.1 = 1.4
	double SCORE_MIN = 75.0;  //Minimum score to be considered a tote
	double VIEW_ANGLE = 49.4; //View angle fo camera, set to Axis m1011 by default, 64 for m1013, 51.7 for 206, 52 for HD3000 square, 60 for HD3000 640x480
	ParticleFilterCriteria2 criteria[1];
	ParticleFilterOptions2 filterOptions = {0,0,1,1};
	Scores scores;


public:
	void RobotInit() override {
	    // create an image
		frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
		//the camera name (ex "cam0") can be found through the roborio web interface
		binaryFrame = imaqCreateImage(IMAQ_IMAGE_U8, 0);
		imaqError = IMAQdxOpenCamera("cam2", IMAQdxCameraControlModeController, &session);
		if(imaqError != IMAQdxErrorSuccess) {
			std::cout << "IMAQdxOpenCamera error: " << std::to_string((long)imaqError) << "\n";
		}
		imaqError = IMAQdxConfigureGrab(session);
		if(imaqError != IMAQdxErrorSuccess) {
			std::cout << "IMAQdxConfigureGrab error: " << std::to_string((long)imaqError) << "\n";
		}
	}

	void OperatorControl() override {
	    // acquire images
		IMAQdxStartAcquisition(session);

        // grab an image, draw the circle, and provide it for the camera server which will
        // in turn send it to the dashboard.
		while(IsOperatorControl() && IsEnabled()) {
			IMAQdxGrab(session, frame, true, NULL);
			if(imaqError != IMAQdxErrorSuccess) {
				std::cout << "IMAQdxGrab error: " << std::to_string((long)imaqError) << "\n";
			} else {
				imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);

				//Threshold the image looking for ring light color
				imaqColorThreshold(binaryFrame, frame, 255, IMAQ_HSV, &RING_HUE_RANGE, &RING_SAT_RANGE, &RING_VAL_RANGE);

				CameraServer::GetInstance()->SetImage(frame);
			}
			Wait(0.005);				// wait for a motor update time
		}
        // stop image acquisition
		IMAQdxStopAcquisition(session);
	}
};

START_ROBOT_CLASS(IntermediateVisionRobot)

