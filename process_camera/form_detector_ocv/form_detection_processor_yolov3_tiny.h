//
// Created by biba_bo on 2020-09-08.
//

#ifndef IRON_TURTLE_FORM_DETECTION_PROCESSOR_YOLOV3_TINY_H
#define IRON_TURTLE_FORM_DETECTION_PROCESSOR_YOLOV3_TINY_H

//#define MODEL_PATH                          "src/dnn/yolov3_tiny/yolov3-tiny.cfg"
//#define WIDTH_PATH                          "src/dnn/yolov3_tiny/yolov3-tiny.weights"
//#define COCO_NAMES_PATH                     "src/dnn/yolov3_tiny/coco.names"

//#define MODEL_PATH                          "src/dnn/yolov3_tiny_cross_hands/cross-hands-tiny.cfg"
//#define WIDTH_PATH                          "src/dnn/yolov3_tiny_cross_hands/cross-hands-tiny.weights"

#define MODEL_PATH                          "src/dnn/yolov3_tiny_cross_hands_prn/cross-hands-tiny-prn.cfg"
#define WIDTH_PATH                          "src/dnn/yolov3_tiny_cross_hands_prn/cross-hands-tiny-prn.weights"

#define CHECK_PER_FRAMES                    5
#define MAX_MATS_LIST_SIZE                  3
#define SCALING_IMG                         1

#include <list>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/trace.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <thread>

class FormDetectionProcessorDNN {
private:
    std::list<cv::Mat> queueFrames;
    std::vector<cv::Rect> *formsCoords = nullptr;
    cv::dnn::Net dnn_model;

    std::thread recognitionProcessThread;
    std::mutex mutexRes;
    std::mutex mutexProc;

public:
    FormDetectionProcessorDNN();
    ~FormDetectionProcessorDNN();

    void add_frame(cv::Mat frame);
    void processRecognition();
    std::vector<cv::Rect> *getLastDetectedFaces();
};


#endif //IRON_TURTLE_FORM_DETECTION_PROCESSOR_YOLOV3_TINY_H
