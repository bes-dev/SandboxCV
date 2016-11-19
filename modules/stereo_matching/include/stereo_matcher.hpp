#ifndef __STEREO_MATCHER_HPP__
#define __STEREO_MATCHER_HPP__

#include <opencv2/core/core.hpp>

namespace stereo
{

class StereoMatcher
{
public:
    virtual ~StereoMatcher(){}
    virtual void process(const cv::Mat& cost_volume) = 0;
    virtual const cv::Mat& get() const = 0;
};

cv::Ptr<StereoMatcher> createStereoMatcherSimple(int disp_max, int rows, int cols);
cv::Ptr<StereoMatcher> createStereoMatcherSGM(int disp_max, int rows, int cols, float P1, float P2, int max_diff);

}

#endif // __STEREO_MATCHER_HPP__
