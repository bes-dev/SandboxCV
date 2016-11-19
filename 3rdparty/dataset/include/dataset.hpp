#ifndef __DATASET_HPP__
#define __DATASET_HPP__

#include <string>
#include <opencv2/core/core.hpp>

namespace dataset
{

class Dataset
{
public:
    virtual ~Dataset(){}
    virtual bool init(const std::string& path) = 0;
    virtual bool grab(cv::Mat& left, cv::Mat& right) = 0;
    virtual const cv::Mat& getK() = 0;
    virtual float getBaseline() = 0;
    virtual float getFPS() = 0;
};

Dataset* createDatasetKITTI();
Dataset* createDatasetBahnhof();

}

#endif // __DATASET_HPP__
