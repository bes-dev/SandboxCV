#include "dataset.hpp"
#include <dirent.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <fstream>
#include <iostream>

namespace dataset
{

class DatasetBahnhof: public Dataset
{
public:
    DatasetBahnhof() {}
    ~DatasetBahnhof(){}
    bool init(const std::string& path);
    bool grab(cv::Mat& left, cv::Mat& right);
    const cv::Mat& getK();
    float getBaseline();
    float getFPS();

protected:
    std::string path_;
    size_t idx_;
    std::vector<std::string> left_, right_;
    float baseline_;
    float fps_;

    cv::Mat K1, D1, K2, D2;
    cv::Mat R, T;

    cv::Mat R1, R2, P1, P2, Q;
    cv::Mat rmap[2][2];
};

bool DatasetBahnhof::init(const std::string& path)
{
    path_ = path;

    std::ifstream is;
    is.open(path_ + "cam1.cal");
    K1 = cv::Mat(3, 3, CV_64FC1, cv::Scalar::all(0));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            is >> K1.at<double>(i, j);
        }
    }
    D1 = cv::Mat(1, 4, CV_64FC1, cv::Scalar::all(0));
    for (int i = 0; i < 4; i++)
    {
        is >> D1.at<double>(0, i);
    }
    is.close();
    is.open(path_ + "cam2.cal");
    K2 = cv::Mat(3, 3, CV_64FC1, cv::Scalar::all(0));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            is >> K2.at<double>(i, j);
        }
    }
    D2 = cv::Mat(1, 4, CV_64FC1, cv::Scalar::all(0));
    for (int i = 0; i < 4; i++)
    {
        is >> D2.at<double>(0, i);
    }

    R = cv::Mat(3, 3, CV_64FC1, cv::Scalar::all(0));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            is >> R.at<double>(i, j);
        }
    }
    T = cv::Mat(3, 1, CV_64FC1, cv::Scalar::all(0));
    for (int i = 0; i < 3; i++)
    {
        is >> T.at<double>(i, 0);
    }

    is.close();
    is.open(path_ + "left/imgs.txt");
    while(!is.eof())
    {
        std::string s;
        is >> s;
        left_.push_back(path_ + "left/" + s);
    }
    is.close();
    is.open(path_ + "right/imgs.txt");
    while(!is.eof())
    {
        std::string s;
        is >> s;
        right_.push_back(path_ + "right/" + s);
    }
    is.close();
    idx_ = 0;

    baseline_ = 0.4;
    fps_ = 15.0f;

    cv::Mat img = cv::imread(left_[0]);
    cv::stereoRectify(K1, D1, K2, D2, img.size(), R, T, R1, R2, P1, P2, Q, cv::CALIB_ZERO_DISPARITY, 0, img.size());
    cv::initUndistortRectifyMap(K1, D1, R1, P1, img.size(), CV_16SC2, rmap[0][0], rmap[0][1]);
    cv::initUndistortRectifyMap(K2, D2, R2, P2, img.size(), CV_16SC2, rmap[1][0], rmap[1][1]);

    return true;
}

bool DatasetBahnhof::grab(cv::Mat& left, cv::Mat& right)
{
    bool status = true;
    left = cv::imread(left_[idx_]);
    right = cv::imread(right_[idx_]);

    cv::remap(left, left, rmap[0][0], rmap[0][1], cv::INTER_AREA);
    cv::remap(right, right, rmap[1][0], rmap[1][1], cv::INTER_AREA);

    // cv::imshow("left", left);
    // cv::imshow("right", right);
    // cv::waitKey();
    idx_++;

    if(idx_ == left_.size())
    {
        idx_ = 0;
        status = false;
    }
    return status;
}

const cv::Mat& DatasetBahnhof::getK()
{
    return K1;
}

float DatasetBahnhof::getBaseline()
{
    return baseline_;
}

float DatasetBahnhof::getFPS()
{
    return fps_;
}

Dataset* createDatasetBahnhof()
{
    return new DatasetBahnhof();
}

}
