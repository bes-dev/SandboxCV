#include <stereo_matcher.hpp>
#include <iostream>

namespace stereo
{

class StereoMatcherSimple: public StereoMatcher
{
public:
    ~StereoMatcherSimple(){}
    StereoMatcherSimple(int disp_max, int rows, int cols);
    void process(const cv::Mat& cost_volume);
    const cv::Mat& get() const;

protected:
    cv::Mat disp_;
    int disp_max_;
    int rows_, cols_;
};

StereoMatcherSimple::StereoMatcherSimple(int disp_max, int rows, int cols):
    disp_max_(disp_max),
    rows_(rows),
    cols_(cols)
{
    disp_.create(rows_, cols_, CV_32FC1);
}

void StereoMatcherSimple::process(const cv::Mat& cost_volume)
{
    for(int i = 0; i < rows_; ++i)
    {
        for(int j = 0; j < cols_; ++j)
        {
            int best_disp = -1;
            float best_cost = std::numeric_limits<float>::max();
            int d;
            for(d = 0; d < disp_max_; ++d)
            {
                if(cost_volume.at<float>(i, j * disp_max_ + d) < best_cost)
                {
                    best_disp = d;
                    best_cost = cost_volume.at<float>(i, j * disp_max_ + d);
                }
            }
            disp_.at<float>(i, j) = best_disp;
        }
    }
}

const cv::Mat& StereoMatcherSimple::get() const
{
    return disp_;
}

cv::Ptr<StereoMatcher> createStereoMatcherSimple(int disp_max, int rows, int cols)
{
    return cv::Ptr<StereoMatcher>(new StereoMatcherSimple(disp_max, rows, cols));
}

}
