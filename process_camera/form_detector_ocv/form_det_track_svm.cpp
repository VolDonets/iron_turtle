//
// Created by biba_bo on 2020-09-11.
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
    recognitionProcessThread = std::thread([this]() {
        cv::Ptr<cv::Tracker> tracker = cv::TrackerKCF::create();
        bool isNotDetectedImage = true;

        typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<4>> image_scanner_type;
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

        while (true) {
            mutexProc.lock();
            mutexRes.lock();
            currentFrame = queueFrames.front();
            queueFrames.pop_front();
            mutexRes.unlock();

            tmpFormsCoords = new std::vector<cv::Rect>();
            cv::resize(currentFrame, resizedFrame, cv::Size(newWidth, newHeight));
            cv::cvtColor(resizedFrame, resizedFrame, cv::COLOR_BGRA2GRAY);

            if (isNotDetectedImage) {
                cv::flip(resizedFrame, resizedFrame, 1);
                dlib::assign_image(dlibFormattedFrame, dlib::cv_image<unsigned char>(resizedFrame));
                std::vector<dlib::rectangle> dlibDetRectLst = detector(dlibFormattedFrame);
                for (int inx = 0; inx < dlibDetRectLst.size(); inx++) {
                    //with mirror effect
                    if (dlibDetRectLst[inx].left() * scaleCoefficient < middle) {
                        int new_x = middle + (middle - (dlibDetRectLst[inx].left() * scaleCoefficient +
                                                        ((dlibDetRectLst[inx].right() - dlibDetRectLst[inx].left()) *
                                                         scaleCoefficient)));
                        tmpFormsCoords->push_back(cv::Rect(new_x,
                                                           dlibDetRectLst[inx].top() * scaleCoefficient,
                                                           (dlibDetRectLst[inx].right() - dlibDetRectLst[inx].left()) *
                                                           scaleCoefficient,
                                                           (dlibDetRectLst[inx].bottom() - dlibDetRectLst[inx].top()) *
                                                           scaleCoefficient));
                    } else {
                        int new_x = middle + (middle - (dlibDetRectLst[inx].left() * scaleCoefficient)) -
                                    ((dlibDetRectLst[inx].right() - dlibDetRectLst[inx].left()) * scaleCoefficient);
                        tmpFormsCoords->push_back(cv::Rect(new_x,
                                                           dlibDetRectLst[inx].top() * scaleCoefficient,
                                                           (dlibDetRectLst[inx].right() - dlibDetRectLst[inx].left()) *
                                                           scaleCoefficient,
                                                           (dlibDetRectLst[inx].bottom() - dlibDetRectLst[inx].top()) *
                                                           scaleCoefficient));
                    }

                    //without mirror effect
                    /*tmpFormsCoords->push_back(cv::Rect(dlibDetRectLst[inx].left() * scaleCoefficient,
                                                       dlibDetRectLst[inx].top() * scaleCoefficient,
                                                       (dlibDetRectLst[inx].right() - dlibDetRectLst[inx].left()) *
                                                       scaleCoefficient,
                                                       (dlibDetRectLst[inx].bottom() - dlibDetRectLst[inx].top()) *
                                                       scaleCoefficient));
                                                       */
                    //just for testing
                    /*std::cout << "Rect: x=" << tmpFormsCoords->operator[](inx).x
                              << ", y=" << tmpFormsCoords->operator[](inx).y
                              << ", width=" << tmpFormsCoords->operator[](inx).width
                              << ", height=" << tmpFormsCoords->operator[](inx).height << "\n";
                              */
                }
                if (tmpFormsCoords->size() > 0) {
                    isNotDetectedImage = false;
                    cv::cvtColor(currentFrame, currentFrame, cv::COLOR_BGRA2BGR);
                    tracker->init(currentFrame, cv::Rect2d(tmpFormsCoords->operator[](0).x, tmpFormsCoords->operator[](0).y,
                                                           tmpFormsCoords->operator[](0).width, tmpFormsCoords->operator[](0).height));
                }
            } else {
                cv::Rect2d trackedRect;
                cv::cvtColor(currentFrame, currentFrame, cv::COLOR_BGRA2BGR);
                isNotDetectedImage = !tracker->update(currentFrame, trackedRect);
                if (!isNotDetectedImage) {
                    tmpFormsCoords->push_back(cv::Rect(trackedRect.x, trackedRect.y,
                                                       trackedRect.width, trackedRect.height));
                    std::cout << "tracked coords: " << trackedRect.x << " " << trackedRect.y << " " << trackedRect.width << " " << trackedRect.height << "\n";
                } else
                    std::cout << "cannot to track\n";
            }

            formsCoords = tmpFormsCoords;
            tmpFormsCoords = nullptr;
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