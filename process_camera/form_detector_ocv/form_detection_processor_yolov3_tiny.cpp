//
// Created by biba_bo on 2020-09-08.
//

#include "form_detection_processor_yolov3_tiny.h"


FormDetectionProcessorDNN::FormDetectionProcessorDNN() {
    mutexProc.lock();
    dnn_model = cv::dnn::readNetFromDarknet(MODEL_PATH, WIDTH_PATH);
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
    dnn_model.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    dnn_model.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    recognitionProcessThread = std::thread([this]() {
        cv::Mat currentFrame;
        std::vector<cv::Rect> *tmpFormsCoords;
        cv::Mat resizedFrame;
        cv::Mat inputBlob;

        double confThreshold = 0.5;
        double nmsThreshold = 0.4;
        double scale = 1.0 / 255.0;
        cv::Scalar mean = cv::Scalar(0, 0, 0);
        bool swapRB = true;
        int inputWith = 256;//416;
        int inputHeight = 256;//416;
        cv::Size newFrameSize(inputWith, inputHeight);

        while (true) {
            mutexProc.lock();
            mutexRes.lock();
            currentFrame = queueFrames.front();
            queueFrames.pop_front();
            mutexRes.unlock();

            tmpFormsCoords = new std::vector<cv::Rect>();

            cv::resize(currentFrame, resizedFrame, newFrameSize);
            cv::cvtColor(resizedFrame, resizedFrame, cv::COLOR_BGRA2BGR);
            cv::dnn::blobFromImage(resizedFrame, inputBlob, scale, newFrameSize, mean, swapRB, false, CV_32F);

            dnn_model.setInput(inputBlob);
            std::vector<std::string> outNames = dnn_model.getUnconnectedOutLayersNames();
            std::vector<cv::Mat> outs;
            dnn_model.forward(outs, outNames);

            static std::vector<int> outLayers = dnn_model.getUnconnectedOutLayers();
            static std::string outLayerType = dnn_model.getLayer(outLayers[0])->type;

            if (outLayerType == "Region") {
                for (size_t i = 0; i < outs.size(); ++i) {
                    // Network produces output blob with a shape NxC where N is a number of
                    // detected objects and C is a number of classes + 4 where the first 4
                    // numbers are [center_x, center_y, width, height]
                    float *data = (float *) outs[i].data;
                    for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
                        cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
                        cv::Point classIdPoint;
                        double confidence;
                        minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
                        if (confidence > confThreshold) {
                            int centerX = (int) (data[0] * currentFrame.cols);
                            int centerY = (int) (data[1] * currentFrame.rows);
                            int width = (int) (data[2] * currentFrame.cols);
                            int height = (int) (data[3] * currentFrame.rows);
                            int left = centerX - width / 2;
                            int top = centerY - height / 2;

                            tmpFormsCoords->push_back(cv::Rect(left, top, width, height));
                        }
                    }
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