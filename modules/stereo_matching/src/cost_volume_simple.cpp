#include <cost_volume.hpp>

namespace stereo
{

class CostVolumeSimple: public CostVolume
{
public:
    ~CostVolumeSimple(){}
    CostVolumeSimple(int disp_max, int rows, int cols);
    void process(const cv::Mat& left, const cv::Mat& right);
    const cv::Mat& get() const;

protected:
    cv::Mat cost_volume_;
    int disp_max_;
    int rows_, cols_;
};

CostVolumeSimple::CostVolumeSimple(int disp_max, int rows, int cols):
    disp_max_(disp_max),
    rows_(rows),
    cols_(cols)
{
    cost_volume_.create(rows_, cols_ * disp_max_, CV_32FC1);
}

void CostVolumeSimple::process(const cv::Mat& left, const cv::Mat& right)
{
    CV_Assert(left.channels() == 1 && right.channels() == 1);

    float COST_MAX = 10000.0f;

    for(int i = 0; i < rows_; ++i)
    {
        for(int j = 0; j < cols_; ++j)
        {
            for(int k = 0; k < disp_max_; ++k)
            {
                if (j - k >= 0)
                {
                    float distance = std::abs(left.at<uchar>(i, j) - right.at<uchar>(i, j - k));
                    cost_volume_.at<float>(i, j * disp_max_ + k) = float(distance) / 255.0;
                }
                else
                {
                    cost_volume_.at<float>(i, j * disp_max_ + k) = COST_MAX;
                }
            }
        }
    }
}

const cv::Mat& CostVolumeSimple::get() const
{
    return cost_volume_;
}

cv::Ptr<CostVolume> createCostVolumeSimple(int disp_max, int rows, int cols)
{
    return cv::Ptr<CostVolume>(new CostVolumeSimple(disp_max, rows, cols));
}


}
