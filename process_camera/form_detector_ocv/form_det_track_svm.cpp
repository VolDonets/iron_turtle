//
// Created by biba_bo on 2020-09-11.
//

#include "form_detection_processor.h"
#include <dlib/image_processing.h>
#include <dlib/image_keypoint.h>

#include <dlib/svm_threaded.h>

FormDetectionProcessor::FormDetectionProcessor() {
    mutexProc.lock();
    processRecognition();
}

FormDetectionProcessor::~FormDetectionProcessor() {

}

void FormDetectionProcessor::add_frame(cv::Mat frame) {
    mutexRes.lock();
    if (queueFrames.size() > MAX_MATS_LIST_SIZE) {
        queueFrames.pop_front();
        queueFrames.push_back(frame);
    } else
        queueFrames.push_back(frame);
    mutexProc.unlock();
    mutexRes.unlock();
}

void FormDetectionProcessor::processRecognition() {
    recognitionProcessThread = std::thread([this]() {
        typedef dlib::matrix < double, 1980, 1 > sample_type;
        typedef dlib::linear_kernel< sample_type > kernel_type;
        typedef dlib::scan_fhog_pyramid<dlib::rect_detection> image_scanner_type;
        dlib::object_detector<image_scanner_type> detector;

        dlib::deserialize(SVM_MODEL_PATH) >> detector;
    });
}

std::vector<cv::Rect> *FormDetectionProcessor::getLastDetectedFaces() {
    return formsCoords;
}