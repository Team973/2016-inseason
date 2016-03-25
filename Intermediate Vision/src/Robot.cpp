#include "WPILib.h"
#include <iostream>
#include <vector>
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
	Image *thresholdImage;
	Image *convexHullImage;
	Image *bigObjImage;
	int imaqError;
	const double TARGET_WIDTH_INCHES = 18;
	const double FOCAL_LENGTH = 713.75;
	const double X_FOV = 34.25 * 0.69;

	//Constants
	Range RING_HUE_RANGE = {80, 160};	//Default hue range for ring light
	Range RING_SAT_RANGE = {100, 255};	//Default saturation range for ring light
	Range RING_VAL_RANGE = {180, 255};	//Default value range for ring light
	double AREA_MINIMUM = 0.5; //Default Area minimum for particle as a percentage of total image area
	double LONG_RATIO = 2.22; //Tote long side = 26.9 / Tote height = 12.1 = 2.22
	double SHORT_RATIO = 1.4; //Tote short side = 16.9 / Tote height = 12.1 = 1.4
	double SCORE_MIN = 75.0;  //Minimum score to be considered a tote
	double VIEW_ANGLE = 49.4; //View angle fo camera, set to Axis m1011 by default, 64 for m1013, 51.7 for 206, 52 for HD3000 square, 60 for HD3000 640x480
	ParticleFilterCriteria2 criteria[1];
	ParticleFilterOptions2 filterOptions = {0,0,1,1};
	Scores scores;

	int SetCameraSettings(IMAQdxSession &sess, double exp, double brit, double contrast) {
		int ret = 0;

		ret |= IMAQdxSetAttribute(sess,
				"CameraAttributes::Exposure::Mode",
				IMAQdxValueTypeString, "Manual");
		ret |= IMAQdxSetAttribute(sess,
				"CameraAttributes::Exposure::Value",
				IMAQdxValueTypeF64, exp);
		ret |= IMAQdxSetAttribute(sess,
				"CameraAttributes::Brightness::Mode",
				IMAQdxValueTypeString, "Manual");
		ret |= IMAQdxSetAttribute(sess,
				"CameraAttributes::Brightness::Value",
				IMAQdxValueTypeF64, brit);

		ret |= IMAQdxSetAttribute(sess,
				"CameraAttributes::Contrast::Mode",
				IMAQdxValueTypeString, "Manual");
		ret |= IMAQdxSetAttribute(sess,
				"CameraAttributes::Contrast::Value",
				IMAQdxValueTypeF64, contrast);
		return ret;
	}

	static bool CompareParticleSizes(ParticleReport particle1, ParticleReport particle2)
	{
		//we want descending sort order
		return particle1.PercentAreaToImageArea > particle2.PercentAreaToImageArea;
	}

public:
	void RobotInit() override {
	    // create an image
		frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
		//the camera name (ex "cam0") can be found through the roborio web interface
		thresholdImage = imaqCreateImage(IMAQ_IMAGE_U8, 0);
		convexHullImage = imaqCreateImage(IMAQ_IMAGE_U8, 0);
		bigObjImage = imaqCreateImage(IMAQ_IMAGE_U8, 0);
		imaqError = IMAQdxOpenCamera("cam1", IMAQdxCameraControlModeController, &session);
		SetCameraSettings(session, 5.0, 30.0, 10.0);
		SetCameraSettings(session, 5.0, 30.0, 10.0);
		SetCameraSettings(session, 5.0, 30.0, 10.0);

		/*
		double min = -1.0, max = -1.0;
		IMAQdxGetAttributeMinimum(session,
						"CameraAttributes::Exposure::Value",
						IMAQdxValueTypeF64, &min);
		IMAQdxGetAttributeMaximum(session,
						"CameraAttributes::Exposure::Value",
						IMAQdxValueTypeF64, &max);


		char buff[100];
		snprintf(buff, sizeof(buff), "exp min %lf", min);
		SmartDashboard::PutString("DB/String 1", buff);

		snprintf(buff, sizeof(buff), "exp max %lf", max);
		SmartDashboard::PutString("DB/String 2", buff);

		min = -1; max = -1;
		IMAQdxGetAttributeMinimum(session,
						"CameraAttributes::Brightness::Value",
						IMAQdxValueTypeF64, &min);
		IMAQdxGetAttributeMaximum(session,
						"CameraAttributes::Brightness::Value",
						IMAQdxValueTypeF64, &max);

		snprintf(buff, sizeof(buff), "brit min %lf", min);
		SmartDashboard::PutString("DB/String 3", buff);

		snprintf(buff, sizeof(buff), "brit max %lf", max);
		SmartDashboard::PutString("DB/String 4", buff);

		min = -1; max = -1;
		IMAQdxGetAttributeMinimum(session,
						"CameraAttributes::Contrast::Value",
						IMAQdxValueTypeF64, &min);
		IMAQdxGetAttributeMaximum(session,
						"CameraAttributes::Contrast::Value",
						IMAQdxValueTypeF64, &max);

		snprintf(buff, sizeof(buff), "c min %lf", min);
		SmartDashboard::PutString("DB/String 5", buff);

		snprintf(buff, sizeof(buff), "c max %lf", max);
		SmartDashboard::PutString("DB/String 6", buff);
		*/

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
		char buff[100];
		IMAQdxStartAcquisition(session);

        // grab an image, draw the circle, and provide it for the camera server which will
        // in turn send it to the dashboard.
		while(IsOperatorControl() && IsEnabled()) {
			IMAQdxGrab(session, frame, true, NULL);
			//imaqError = imaqReadFile(frame, "//home//lvuser//SampleImages//image.jpg", NULL, NULL);
			printf("grabbed frame\n");

			imaqClearError();

			//imaqDrawShapeOnImage(frame, frame, { 10, 10, 100, 100 }, DrawMode::IMAQ_DRAW_VALUE, ShapeMode::IMAQ_SHAPE_OVAL, 0.0f);

			//Threshold the image looking for ring light color
			imaqError = imaqColorThreshold(thresholdImage, frame, 255, IMAQ_HSV, &RING_HUE_RANGE, &RING_SAT_RANGE, &RING_VAL_RANGE);

			int width = -1, height = -1;
			imaqGetImageSize(frame, &width, &height);
			snprintf(buff, sizeof(buff), "(%d, %d)", width, height);
			SmartDashboard::PutString("DB/String 9", buff);

			if (imaqError >= IMAQdxErrorSuccess) {
				int numParticles = 0;
				imaqCountParticles(thresholdImage, 1, &numParticles);

				snprintf(buff, sizeof(buff), "#parts %d", numParticles);
				SmartDashboard::PutString("DB/String 3", buff);

				//float areaMin = 0.05;
				//criteria[0] = {IMAQ_MT_AREA_BY_IMAGE_AREA, areaMin, 100, false, false};
				//imaqError = imaqParticleFilter4(bigObjImage, thresholdImage, criteria, 1, &filterOptions, NULL, NULL);
				imaqError = imaqSizeFilter(bigObjImage, thresholdImage, 1, 2, IMAQ_KEEP_LARGE, NULL);
			}
			else {
				printf("th er %d\n", imaqError);
				snprintf(buff, sizeof(buff), "th er %d", imaqError);
				SmartDashboard::PutString("DB/String 3", buff);
			}

			if (imaqError >= IMAQdxErrorSuccess) {
				int numParticles = 0;
				imaqCountParticles(thresholdImage, 1, &numParticles);

				std::vector<ParticleReport> particles;
				for(int particleIndex = 0; particleIndex < numParticles; particleIndex++)
				{
					ParticleReport par;
					imaqMeasureParticle(thresholdImage, particleIndex, 0, IMAQ_MT_AREA_BY_IMAGE_AREA, &(par.PercentAreaToImageArea));
					imaqMeasureParticle(thresholdImage, particleIndex, 0, IMAQ_MT_AREA, &(par.Area));
					imaqMeasureParticle(thresholdImage, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_TOP, &(par.BoundingRectTop));
					imaqMeasureParticle(thresholdImage, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_LEFT, &(par.BoundingRectLeft));
					imaqMeasureParticle(thresholdImage, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_BOTTOM, &(par.BoundingRectBottom));
					imaqMeasureParticle(thresholdImage, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_RIGHT, &(par.BoundingRectRight));
					if (par.Area < 100) {
						continue;
					}
					printf("Particle report: %lf %lf %lf %lf %lf %lf\n",
							par.PercentAreaToImageArea, par.Area, par.BoundingRectTop,
							par.BoundingRectLeft, par.BoundingRectBottom, par.BoundingRectRight);
					particles.push_back(par);
				}
				sort(particles.begin(), particles.end(), CompareParticleSizes);
				if (particles.size() != 0){
					double xoff = (particles[0].BoundingRectLeft + particles[0].BoundingRectRight) / 2.0;
					double TARGET_WIDTH_PIXELS = particles[0].BoundingRectRight - particles[0].BoundingRectLeft;
					snprintf(buff, sizeof(buff), "aa %.2lf x %lf", particles[0].PercentAreaToImageArea,
							(particles[0].BoundingRectLeft + particles[0].BoundingRectRight) / 2.0);
					SmartDashboard::PutString("DB/String 6", buff);
					snprintf(buff, sizeof(buff), "u %lf z %lf", TARGET_WIDTH_PIXELS,
							TARGET_WIDTH_INCHES*FOCAL_LENGTH/TARGET_WIDTH_PIXELS);
					SmartDashboard::PutString("DB/String 7", buff);
					snprintf(buff, sizeof(buff), "xang %lf", (xoff - 320)*X_FOV/320);
					SmartDashboard::PutString("DB/String 8", buff);
				}
				else {
					SmartDashboard::PutString("DB/String 6", "noparticles");
				}
			}
			else {
				printf("bigO er %d\n", imaqError);
				snprintf(buff, sizeof(buff), "bigO er %d", imaqError);
				SmartDashboard::PutString("DB/String 3", buff);
			}

			//imaqConvexHull(convexHullImage, thresholdImage, 0);//3rd arg is connectivity8

			CameraServer::GetInstance()->SetImage(frame);

			Wait(0.05);				// wait for a motor update time
		}
        // stop image acquisition
		IMAQdxStopAcquisition(session);
	}
};

START_ROBOT_CLASS(IntermediateVisionRobot)

