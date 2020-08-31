//
// Created by biba_bo on 2020-08-27.
//

#ifndef REAR_SIGHT_CLONE_PROJECT_FORM_DETECTION_PROCESSOR_H
#define REAR_SIGHT_CLONE_PROJECT_FORM_DETECTION_PROCESSOR_H

#define CHECK_PER_FRAMES                    5
#define MAX_MATS_LIST_SIZE                  3
#define SCALING_IMG                         1

#include <list>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <thread>

class FormDetectionProcessor {
private:
    std::list<cv::Mat> queueFrames;
    std::vector<cv::Rect> *formsCoords = nullptr;
    cv::CascadeClassifier cascadeClassifier;
    std::thread recognitionProcessThread;

    std::mutex mutexRes;
    std::mutex mutexProc;

public:
    FormDetectionProcessor();
    ~FormDetectionProcessor();

    void add_frame(cv::Mat frame);
    void processRecognition();
    std::vector<cv::Rect> *getLastDetectedFaces();
};


#endif //REAR_SIGHT_CLONE_PROJECT_FORM_DETECTION_PROCESSOR_H
