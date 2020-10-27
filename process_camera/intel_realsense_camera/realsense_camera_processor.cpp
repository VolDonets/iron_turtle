//
// Created by biba_bo on 2020-10-26.
//

#include "realsense_camera_processor.h"

RealsenseCameraProcessor::RealsenseCameraProcessor(int imgWidth, int imgHeight) {
    _imgWidth = imgWidth;
    _imgHeight = imgHeight;
    _isProcessedCalculation = false;
    _mutexProc.unlock();
    _mutexRes.unlock();
}

RealsenseCameraProcessor::~RealsenseCameraProcessor() {

}

void RealsenseCameraProcessor::get_reference_points(cv::Point &leftPointCoord, cv::Point &rightPointCoord) {
    if (!_listPointPairs.empty()) {
        leftPointCoord.x = _listPointPairs.front().first.x;
        leftPointCoord.y = _listPointPairs.front().first.y;
        rightPointCoord.x = _listPointPairs.front().second.x;
        rightPointCoord.y = _listPointPairs.front().second.y;
        _listPointPairs.front();
    } else {
        leftPointCoord.x = 10;
        leftPointCoord.y = 10;
        rightPointCoord.x = 20;
        rightPointCoord.y = 10;

    }
}

void RealsenseCameraProcessor::add_detected_rectangle(const cv::Rect &objectRectangle) {
    _mutexRes.lock();
    _listObjectRectangles.push_back(objectRectangle);
    _mutexProc.unlock();
    _mutexRes.unlock();
}

int RealsenseCameraProcessor::start_processing() {
    if (!_isProcessedCalculation) {
        process_realsense_camera_stream();
        _mutexProc.lock();
    }
    return 0;
}

int RealsenseCameraProcessor::stop_processing() {
    if (_isProcessedCalculation) {
        _isProcessedCalculation = false;
        _mutexProc.unlock();
        _listPointPairs.clear();
        _listObjectRectangles.clear();
    }
    return 0;
}


bool RealsenseCameraProcessor::set_points(cv::Point &leftPoint, cv::Point &rightPoint, const cv::Rect &objectRect, const rs2::depth_frame &depth) {
    leftPoint.x = objectRect.x;
    leftPoint.y = objectRect.y + (objectRect.height / 2);
    rightPoint.x = objectRect.x + objectRect.width;
    rightPoint.y = objectRect.y + (objectRect.height / 2);

    leftPoint.x = (leftPoint.x * DEPTH_FRAME_WIDTH) / _imgWidth;
    rightPoint.x = (rightPoint.x * DEPTH_FRAME_WIDTH) / _imgWidth;

    int yDepth = (leftPoint.y * DEPTH_FRAME_HEIGHT) / _imgHeight;
    float distLeftPoint, distRightPoint;
    bool isNotFit = true;

    while (isNotFit) {
        distLeftPoint = depth.get_distance(leftPoint.x, yDepth);
        distRightPoint = depth.get_distance(rightPoint.x , yDepth);
        if (distLeftPoint >= MIN_POSSIBLE_DISTANCE && distLeftPoint <= MAX_POSSIBLE_DISTANCE) {
            if (distRightPoint >= MIN_POSSIBLE_DISTANCE && distRightPoint <= MAX_POSSIBLE_DISTANCE) {
                if (abs(distRightPoint - distLeftPoint) <= MAX_DIFF_LR_DIST)
                    isNotFit = false;
                else if (distRightPoint > distLeftPoint) {
                    rightPoint.x -= X_STEP_LENGTH;
                } else {
                    leftPoint.x += X_STEP_LENGTH;
                }
            } else {
                rightPoint.x -= X_STEP_LENGTH;
            }
        } else {
            leftPoint.x += X_STEP_LENGTH;
            if (!(distRightPoint >= MIN_POSSIBLE_DISTANCE && distRightPoint <= MAX_POSSIBLE_DISTANCE)) {
                rightPoint.x -= X_STEP_LENGTH;
            }
        }
        if (rightPoint.x - leftPoint.x <= 0)
            break;
    }

    leftPoint.x = (leftPoint.x * _imgWidth) / DEPTH_FRAME_WIDTH;
    rightPoint.x = (rightPoint.x * _imgWidth) / DEPTH_FRAME_WIDTH;
    return !isNotFit;
}

void RealsenseCameraProcessor::process_realsense_camera_stream() {
    _realsenseCameraThread = std::thread([this]() {
        rs2::pipeline rsPipe;
        rsPipe.start();

        while(_isProcessedCalculation) {
            _mutexProc.lock();
            _mutexRes.lock();
            cv::Rect objectRect = _listObjectRectangles.front();
            _listObjectRectangles.pop_front();
            _mutexRes.unlock();

            rs2::frameset data = rsPipe.wait_for_frames();
            rs2::depth_frame depthFrame = data.get_depth_frame();

            cv::Point leftPoint;
            cv::Point rightPoint;
            set_points(leftPoint, rightPoint, objectRect, depthFrame);
            _listPointPairs.push_back(std::pair(leftPoint, rightPoint));
        }

        rsPipe.stop();
    });
}
