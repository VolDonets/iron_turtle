//
// Created by biba_bo on 2020-09-01.
//

#ifndef IRON_TURTLE_FORM_DETECTION_PROCESSOR_DNN_CAFFE_H
#define IRON_TURTLE_FORM_DETECTION_PROCESSOR_DNN_CAFFE_H

//#define PROTOTXT_PATH                       "src/dnn/dnn_caffe/deploy.prototxt.txt"
//#define CAFFE_MODEL_PATH                    "src/dnn/dnn_caffe/res10_300x300_ssd_iter_140000.caffemodel"

#define PROTOTXT_PATH                       "src/dnn/yolo_caffe/yolo_darknet_deploy.prototxt"
#define CAFFE_MODEL_PATH                    "src/dnn/yolo_caffe/yolo_darknet.caffemodel"

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


#endif //IRON_TURTLE_FORM_DETECTION_PROCESSOR_DNN_CAFFE_H
