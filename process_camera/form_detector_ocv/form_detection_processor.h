//
// Created by biba_bo on 2020-08-27.
//

#ifndef REAR_SIGHT_CLONE_PROJECT_FORM_DETECTION_PROCESSOR_H
#define REAR_SIGHT_CLONE_PROJECT_FORM_DETECTION_PROCESSOR_H


//#define MODEL_PATH                          "src/dnn/yolov3_tiny/yolov3-tiny.cfg"
//#define WIDTH_PATH                          "src/dnn/yolov3_tiny/yolov3-tiny.weights"
//#define COCO_NAMES_PATH                     "src/dnn/yolov3_tiny/coco.names"

//#define MODEL_PATH                          "src/dnn/yolov3_tiny_cross_hands/cross-hands-tiny.cfg"
//#define WIDTH_PATH                          "src/dnn/yolov3_tiny_cross_hands/cross-hands-tiny.weights"

//#define MODEL_PATH                          "src/dnn/yolov3_tiny_cross_hands_prn/cross-hands-tiny-prn.cfg"
//#define WIDTH_PATH                          "src/dnn/yolov3_tiny_cross_hands_prn/cross-hands-tiny-prn.weights"

//#define PROTOTXT_PATH                       "src/dnn/dnn_caffe/deploy.prototxt.txt"
//#define CAFFE_MODEL_PATH                    "src/dnn/dnn_caffe/res10_300x300_ssd_iter_140000.caffemodel"

//#define PROTOTXT_PATH                       "src/dnn/yolo_caffe/yolo_darknet_deploy.prototxt"
//#define CAFFE_MODEL_PATH                    "src/dnn/yolo_caffe/yolo_darknet.caffemodel"

#define SVM_MODEL_PATH                      "src/svm/Hand_Detector_v8_c8.svm"

#define CHECK_PER_FRAMES                    5
#define MAX_MATS_LIST_SIZE                  3
#define SCALING_IMG                         1

#include <list>
#include <thread>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/trace.hpp>

class FormDetectionProcessor {
private:
    std::list<cv::Mat> queueFrames;
    std::vector<cv::Rect> *formsCoords = nullptr;
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
