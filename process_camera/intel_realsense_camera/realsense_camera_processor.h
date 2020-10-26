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

class RealsenseCameraProcessor {
private:
    std::thread _realsense_camera_thread;
    cv::Rect _humanRectangle;


    void process_realsense_camera_stream();

public:
    RealsenseCameraProcessor();
    ~RealsenseCameraProcessor();

    void set_new_position(const cv::Rect &humanRectangle);
    bool get_reference_points(cv::Point &leftPointCoord, cv::Point &rightPointCoord);
};


#endif //IRON_TURTLE_REALSENSE_CAMERA_PROCESSOR_H
