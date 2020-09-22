//
// Created by biba_bo on 2020-08-27.
//

#ifndef REAR_SIGHT_CLONE_PROJECT_FORM_DETECTION_PROCESSOR_H
#define REAR_SIGHT_CLONE_PROJECT_FORM_DETECTION_PROCESSOR_H

#ifdef HAARCASCADES
    #ifdef FRONTAL_FACE
        #define HAARCASCADE_PATH            "src/haarcascade/haarcascade_frontalface_alt_tree.xml"
    #endif //FRONTAL_FACE
    #ifdef ANOTHER_HAND
        #define HAARCASCADE_PATH            "src/haarcascade/hands_haarcascades/another_hand.xml"
    #endif //ANOTHER_HAND
    #ifdef FIST
        #define HAARCASCADE_PATH            "src/haarcascade/hands_haarcascades/fist.xml"
    #endif //FIST
    #ifdef RPALM
        #define HAARCASCADE_PATH            "src/haarcascade/hands_haarcascades/rpalm.xml"
    #endif //RPALM
#endif //HAARCASCADES

#ifdef YOLO3_TINY
    #ifdef YOLO3_TINY_COCO
        #define MODEL_PATH                 "src/dnn/yolov3_tiny/yolov3-tiny.cfg"
        #define WIDTH_PATH                 "src/dnn/yolov3_tiny/yolov3-tiny.weights"
        #define COCO_NAMES_PATH            "src/dnn/yolov3_tiny/coco.names"
    #endif //YOLO3_TINY_COCO
    #ifdef CROSS_HANDS
        #define MODEL_PATH                 "src/dnn/yolov3_tiny_cross_hands/cross-hands-tiny.cfg"
        #define WIDTH_PATH                 "src/dnn/yolov3_tiny_cross_hands/cross-hands-tiny.weights"
        #define COCO_NAMES_PATH            "src/dnn/yolov3_tiny/coco.names"
    #endif //CROSS_HANDS
    #ifdef CROSS_HANDS_PRN
        #define MODEL_PATH                 "src/dnn/yolov3_tiny_cross_hands_prn/cross-hands-tiny-prn.cfg"
        #define WIDTH_PATH                 "src/dnn/yolov3_tiny_cross_hands_prn/cross-hands-tiny-prn.weights"
        #define COCO_NAMES_PATH            "src/dnn/yolov3_tiny/coco.names"
    #endif //CROSS_HANDS_PRN
#endif //YOLO3_TINY

#ifdef DNN_CAFFE
    #ifdef BY_BALD_GUY
        #define PROTOTXT_PATH               "src/dnn/dnn_caffe/deploy.prototxt.txt"
        #define CAFFE_MODEL_PATH            "src/dnn/dnn_caffe/res10_300x300_ssd_iter_140000.caffemodel"
    #endif //BY_BALD_GUY
    #ifdef FROM_YOLO
        #define PROTOTXT_PATH               "src/dnn/yolo_caffe/yolo_darknet_deploy.prototxt"
        #define CAFFE_MODEL_PATH            "src/dnn/yolo_caffe/yolo_darknet.caffemodel"
    #endif //FROM_YOLO
#endif //DNN_CAFFE

#ifdef SVM_HOG
    #ifdef V8_C8
        #define SVM_MODEL_PATH              "src/svm/Hand_Detector_v8_c8.svm"
    #endif //v6_C20
    #ifdef V6_C20
        #define SVM_MODEL_PATH              "src/svm/Hand_Detector_v6_c20.svm"
    #endif //V6_C20
    #ifdef V10_C10
        #define SVM_MODEL_PATH               "src/svm/Hand_Detector_v10_c10.svm"
    #endif //V10_C10
#endif //SVM_HOG

#define CHECK_PER_FRAMES                    2
#define MAX_MATS_LIST_SIZE                  3
#define SCALING_IMG                         1

#include <list>
#include <thread>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/trace.hpp>
#include <opencv2/tracking.hpp>

#include <dlib/image_processing.h>
#include <dlib/image_keypoint.h>
#include <dlib/svm_threaded.h>
#include <dlib/opencv.h>

/**class FormDetectionProcessor - is a class which realize functionality for an image detection and an image tracking,
 * (depending from the realization). The constructor initialize needed mutex, and start detection thread*/
class FormDetectionProcessor {
private:
    /**this list contains a Mat's queue which waiting for processing (it can be an image detection or an image tracking (depending from the realization)*/
    std::list<cv::Mat> queueFrames;
    /**this array contains a list of the last detected object rectangles*/
    std::vector<cv::Rect> *formsCoords = nullptr;
    /**this thread is a image processing thread, here processes an object detection and an object tracking*/
    std::thread recognitionProcessThread;

    /**this mutex synchronize a block code where we process a resources (here is - std::list<cv::Mat> queueFrames)*/
    std::mutex mutexRes;
    /**this mutex used for blocking processing, it blocks code when we trying to process image, but actually we don't have a Mat object for this
     * (it means std::list<cv::Mat> queueFrames is empty)
     * Firstly it blocks when we are creating a FromDetectionProcessor class (calls in constructor)*/
    std::mutex mutexProc;

public:
    /**this is a default constructor here in other thread blocks mutesProc, and also starts a new thread for the independent object processing*/
    FormDetectionProcessor();
    /**this is a default destructor, this one doing nothing, cause in this class used smart pointers (std:: and cv:: memory allocation and de-allocation)*/
    ~FormDetectionProcessor();

    /**this function just add a new Mat object to the queueFrames for the next processing*/
    void add_frame(cv::Mat frame);
    /**this function starts a new thread for image processing*/
    void processRecognition();
    /**this function returns an empty std::vector<cv::Rect> or nullptr if it was nothing to detect, or an filled std::vector<... if it was detect some objects*/
    std::vector<cv::Rect> *getLastDetectedFaces();
};


#endif //REAR_SIGHT_CLONE_PROJECT_FORM_DETECTION_PROCESSOR_H
