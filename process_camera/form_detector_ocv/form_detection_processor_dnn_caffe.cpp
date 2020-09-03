//
// Created by biba_bo on 2020-09-01.
//

#include "form_detection_processor_dnn_caffe.h"

FormDetectionProcessorDNN::FormDetectionProcessorDNN() {
    mutexProc.lock();
    dnn_model = cv::dnn::readNetFromCaffe(PROTOTXT_PATH, CAFFE_MODEL_PATH);
    if (dnn_model.empty())
        std::cerr << "Can't load network by using this files \n";
    else
        processRecognition();
}

FormDetectionProcessorDNN::~FormDetectionProcessorDNN() {

}

void FormDetectionProcessorDNN::add_frame(cv::Mat frame) {
    mutexRes.lock();
    if (queueFrames.size() > MAX_MATS_LIST_SIZE) {
        queueFrames.pop_front();
        queueFrames.push_back(frame);
    } else
        queueFrames.push_back(frame);
    mutexProc.unlock();
    mutexRes.unlock();
}

void FormDetectionProcessorDNN::processRecognition() {
    recognitionProcessThread = std::thread([this]() {
        while (true) {
            mutexProc.lock();
            mutexRes.lock();
            cv::Mat currentFrame = queueFrames.front();
            queueFrames.pop_front();
            mutexRes.unlock();

            std::vector<cv::Rect> *tmpFormsCoords = new std::vector<cv::Rect>();

            cv::Mat resizedFrame;
            cv::resize(currentFrame, resizedFrame, cv::Size(300, 300));
            cv::cvtColor(resizedFrame, resizedFrame, cv::COLOR_BGRA2BGR);
            cv::Mat inputBlob = cv::dnn::blobFromImage(resizedFrame, 1, cv::Size(300, 300), cv::Scalar(104, 177, 123), false);
            dnn_model.setInput(inputBlob, "data");
            //cv::Mat detection = dnn_model.forward("detection_out");
            cv::Mat detection = dnn_model.forward("conv8");
            cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
            float confidenceThreshold = 0.2;
            for (int i = 0; i < detectionMat.rows; i++) {
                float confidence = detectionMat.at<float>(i, 2);
                if (confidence > confidenceThreshold) {
                    int idx = static_cast<int>(detectionMat.at<float>(i, 1));
                    int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * currentFrame.cols);
                    int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * currentFrame.rows);
                    int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * currentFrame.cols);
                    int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * currentFrame.rows);

                    cv::Rect object((int)xLeftBottom, (int)yLeftBottom,
                                (int)(xRightTop - xLeftBottom),
                                (int)(yRightTop - yLeftBottom));
                    std::cout << "x: " << object.x << ", y: " << object.y << ", weight: " << object.width << ", height: " << object.height << "\n";
                    tmpFormsCoords->push_back(object);
                }
            }

            formsCoords = tmpFormsCoords;
            if (!queueFrames.empty())
                mutexProc.unlock();
            if (queueFrames.size() == -100)   // stupid lines of code for CLion
                return;                       //
        }
    });
}

std::vector<cv::Rect> *FormDetectionProcessorDNN::getLastDetectedFaces() {
    return formsCoords;
}