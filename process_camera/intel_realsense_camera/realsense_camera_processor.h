//
// Created by biba_bo on 2020-10-26.
//

#ifndef IRON_TURTLE_REALSENSE_CAMERA_PROCESSOR_H
#define IRON_TURTLE_REALSENSE_CAMERA_PROCESSOR_H

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <librealsense2/rs.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <list>

constexpr int DEPTH_FRAME_WIDTH = 1280;
constexpr int DEPTH_FRAME_HEIGHT = 720;

constexpr int X_STEP_LENGTH = 5;

constexpr float MIN_POSSIBLE_DISTANCE = 0.6f;
constexpr float MAX_POSSIBLE_DISTANCE = 2.0f;
constexpr float MAX_DIFF_LR_DIST = 0.7f;

constexpr int MAX_FRAMES_QUEUE_LENGTH = 3;

class RealsenseCameraProcessor {
private:
    std::thread _realsenseCameraThread;
    std::mutex _mutexProc;
    std::mutex _mutexRes;
    std::atomic<bool> _isProcessedCalculation;

    int _imgWidth;
    int _imgHeight;

    std::list<std::pair<cv::Point, cv::Point>> _listPointPairs;
    std::list<cv::Rect> _listObjectRectangles;
    std::list<cv::Mat> _listFrames;


    bool set_points(cv::Point &leftPoint, cv::Point &rightPoint, const cv::Rect &objectRect, const rs2::depth_frame &depth);
    void process_realsense_camera_stream();
    void add_colored_frame(cv::Mat colorFrame);

public:
    RealsenseCameraProcessor(int imgWidth, int imgHeight);
    ~RealsenseCameraProcessor();

    void add_detected_rectangle(const cv::Rect &objectRectangle);
    void get_reference_points(cv::Point &leftPointCoord, cv::Point &rightPointCoord);
    int start_processing();
    int stop_processing();
    cv::Mat get_last_frame();
};


#endif //IRON_TURTLE_REALSENSE_CAMERA_PROCESSOR_H
