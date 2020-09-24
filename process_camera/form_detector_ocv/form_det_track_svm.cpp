//
// Created by biba_bo on 2020-09-11.
//

#include "form_detection_processor.h"
#include <chrono>

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
        cv::Ptr<cv::Tracker> tracker;
        bool isNotDetectedImage = true;

        typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6>> image_scanner_type;
        dlib::object_detector<image_scanner_type> detector;
        dlib::deserialize(SVM_MODEL_PATH) >> detector;

        cv::Mat currentFrame, resizedFrame;
        dlib::array2d<unsigned char> dlibFormattedFrame;
        std::vector<cv::Rect> *tmpFormsCoords = nullptr;
        int newWidth = 320, newHeight = 240;
        double scaleCoefficient;
        int middle;

        mutexProc.lock();
        mutexRes.lock();
        scaleCoefficient = queueFrames.front().rows / newHeight;
        middle = queueFrames.front().cols / 2;
        mutexRes.unlock();
        mutexProc.unlock();

        std::chrono::steady_clock::time_point begin;
        std::chrono::steady_clock::time_point end;
        unsigned int duration;
        const unsigned int framerate = 1000000 / 15;
        while (true) {
            begin = std::chrono::steady_clock::now();
            mutexProc.lock();
            mutexRes.lock();
            currentFrame = queueFrames.front();
            queueFrames.pop_front();
            mutexRes.unlock();

            tmpFormsCoords = new std::vector<cv::Rect>();
            cv::resize(currentFrame, resizedFrame, cv::Size(newWidth, newHeight));
            cv::cvtColor(resizedFrame, resizedFrame, cv::COLOR_BGRA2GRAY);

            if (isNotDetectedImage) {
                dlib::assign_image(dlibFormattedFrame, dlib::cv_image<unsigned char>(resizedFrame));
                std::vector<dlib::rectangle> dlibDetRectLst = detector(dlibFormattedFrame);
                for (int inx = 0; inx < dlibDetRectLst.size(); inx++) {
                    tmpFormsCoords->push_back(cv::Rect(dlibDetRectLst[inx].left() * scaleCoefficient,
                                                       dlibDetRectLst[inx].top() * scaleCoefficient,
                                                       (dlibDetRectLst[inx].right() - dlibDetRectLst[inx].left()) *
                                                       scaleCoefficient,
                                                       (dlibDetRectLst[inx].bottom() - dlibDetRectLst[inx].top()) *
                                                       scaleCoefficient));
                }
                if (tmpFormsCoords->size() > 0) {
                    isNotDetectedImage = false;
                    cv::cvtColor(currentFrame, currentFrame, cv::COLOR_BGRA2BGR);
                    tracker = cv::TrackerKCF::create();
                    tracker->init(currentFrame,
                                  cv::Rect2d(tmpFormsCoords->operator[](0).x, tmpFormsCoords->operator[](0).y,
                                             tmpFormsCoords->operator[](0).width,
                                             tmpFormsCoords->operator[](0).height));
                }
            } else {
                cv::Rect2d trackedRect;
                cv::cvtColor(currentFrame, currentFrame, cv::COLOR_BGRA2BGR);
                isNotDetectedImage = !tracker->update(currentFrame, trackedRect);
                if (!isNotDetectedImage) {
                    tmpFormsCoords->push_back(cv::Rect(trackedRect.x, trackedRect.y,
                                                       trackedRect.width, trackedRect.height));
                }
            }

            formsCoords = tmpFormsCoords;
            tmpFormsCoords = nullptr;
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            if (duration < framerate) {
                std::this_thread::sleep_for(std::chrono::microseconds(framerate - duration));
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
            }
            if (!queueFrames.empty())
                mutexProc.unlock();
            if (queueFrames.size() == -100)   // stupid lines of code for CLion
                return;                       //
        }
    });
}

std::vector<cv::Rect> *FormDetectionProcessor::getLastDetectedFaces() {
    return formsCoords;
}