#ifndef __COST_VOLUME_HPP__
#define __COST_VOLUME_HPP__

#include <opencv2/core/core.hpp>

namespace stereo
{

class CostVolume
{
public:
    virtual ~CostVolume(){}
    virtual void process(const cv::Mat& left, const cv::Mat& right) = 0;
    virtual const cv::Mat& get() const = 0;
};

cv::Ptr<CostVolume> createCostVolumeSimple(int disp_max, int rows, int cols);
cv::Ptr<CostVolume> createCostVolumeCensus(int disp_max, int rows, int cols);

}

#endif // __COST_VOLUME_HPP__
