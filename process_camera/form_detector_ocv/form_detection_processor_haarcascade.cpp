//
// Created by biba_bo on 2020-08-27.
//

#include "form_detection_processor.h"

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
    recognitionProcessThread = std::thread([this](){
        cv::CascadeClassifier cascadeClassifier;
        cascadeClassifier.load(HAARCASCADE_PATH);
        while (true) {
            mutexProc.lock();
            mutexRes.lock();
            cv::Mat currentFrame, greyFrame;
            currentFrame = queueFrames.front();
            queueFrames.pop_front();
            mutexRes.unlock();
            cv::cvtColor(currentFrame, greyFrame, cv::COLOR_BGRA2GRAY);
            cv::resize(greyFrame, currentFrame, cv::Size(), 1/SCALING_IMG, 1/SCALING_IMG, cv::INTER_LINEAR);
            cv::equalizeHist(currentFrame, currentFrame);
            formsCoords = new std::vector<cv::Rect>();
            cascadeClassifier.detectMultiScale(currentFrame, *formsCoords, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
            if (!queueFrames.empty())
                mutexProc.unlock();
            if (queueFrames.size() == -100)
                return ;
        }
    });
}

std::vector<cv::Rect> *FormDetectionProcessor::getLastDetectedFaces() {
    return formsCoords;
}