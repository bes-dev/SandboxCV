#include <stereo_matcher.hpp>

using namespace std;

namespace stereo
{

class StereoMatcherSGM: public StereoMatcher
{
public:
    ~StereoMatcherSGM(){}
    StereoMatcherSGM(int disp_max, int rows, int cols, float P1, float P2, int max_diff);
    void process(const cv::Mat& cost_volume);
    const cv::Mat& get() const;

protected:
    int rows_, cols_, disp_max_;
    float P1_, P2_;
    int max_diff_;

    cv::Mat Sm_;
    cv::Mat disp_;

    void aggregateScanLine(const cv::Mat& Cm, cv::Mat& Sm,
                           int step, int idx, int pidx);
};

StereoMatcherSGM::StereoMatcherSGM(int disp_max, int rows, int cols, float P1, float P2, int max_diff):
    disp_max_(disp_max),
    rows_(rows),
    cols_(cols),
    P1_(P1),
    P2_(P2),
    max_diff_(max_diff)
{
    disp_.create(rows_, cols_, CV_32FC1);
    Sm_.create(rows_, cols_ * disp_max_, CV_32FC1);
}

void StereoMatcherSGM::aggregateScanLine(const cv::Mat& Cm, cv::Mat& Sm,
                                         int step, int idx, int pidx)
{
    float COST_MAX = 10000.0f;

    std::vector<float> Lr[2];
    std::vector<float> Lr_min[2];

    for (int i = 0; i < 2; i++)
    {
        Lr[i].resize((cols_ + 2) * disp_max_, 0);
        Lr_min[i].resize(cols_ + 2, 0);
        Lr_min[i][0] = Lr_min[i][Lr_min[0].size() - 1] = COST_MAX;
    }

    int x_min = step > 0 ? 0 : cols_ - 1;
    int x_max = step > 0 ? cols_ - 1 : 0;

    int y_min = step > 0 ? 0 : rows_ - 1;
    int y_max = step > 0 ? rows_ - 1 : 0;
    for (int y = y_min; y != y_max; y+=step)
    {
        for (int x = x_min; x != x_max; x+=step)
        {
            float* lr = &Lr[0][(x + 1) * disp_max_];
            float* plr = &Lr[pidx][(x + idx + 1) * disp_max_];

            float plr_min = Lr_min[pidx][x + 1 + idx];
            float lr_min = std::numeric_limits<float>::max();
            for (int d = 0; d < disp_max_; d++)
            {
                float cost = Cm.at<float>(y, x * disp_max_ + d) + min(min(plr[d],
                                                                          plr[max(d - 1, 0)] + P1_),
                                                                      min(plr[min(d + 1, disp_max_ - 1)] + P1_,
                                                                          plr_min + P2_)) - plr_min;

                Sm.at<float>(y, x * disp_max_ + d) += cost;
                lr_min = std::min(lr_min, cost);
                lr[d] = cost;
            }
            Lr_min[0][x + 1] = lr_min;
        }
        std::swap(Lr[0], Lr[1]);
        std::swap(Lr_min[0], Lr_min[1]);
    }
}

void StereoMatcherSGM::process(const cv::Mat& cost_volume)
{
    Sm_.setTo(0);

    for (int step = -1; step < 2; step += 2)
    {
        for (int idx = -1; idx < 2; idx++)
            aggregateScanLine(cost_volume, Sm_, step, idx, 1);
        aggregateScanLine(cost_volume, Sm_, step, -1 * step, 0);
    }

    for(int i = 0; i < rows_; ++i)
    {
        for(int j = 0; j < cols_; ++j)
        {
            int best_disp = -1;
            float best_cost = std::numeric_limits<float>::max();
            int d;
            for(d = 0; d < disp_max_; ++d)
            {
                if(Sm_.at<float>(i, j * disp_max_ + d) < best_cost)
                {
                    best_disp = d;
                    best_cost = Sm_.at<float>(i, j * disp_max_ + d);
                }
            }

            for( d = 0; d < disp_max_; d++ )
            {
                if(d != best_disp && best_cost / Sm_.at<float>(i, j * disp_max_ + d) > 0.97)
                {
                    break;
                }
            }

            if( d != disp_max_ )
            {
                disp_.at<float>(i, j) = -1;
                continue;
            }

            int best_match = j - best_disp;
            int D = j - best_disp - disp_max_ > 0 ? disp_max_ : j - best_disp;
            for (d = 0; d < D; d++)
            {
                if (Sm_.at<float>(i, (best_match - d) * disp_max_ + d) < best_cost && std::abs(d - best_disp) > max_diff_)
                {
                    break;
                }
            }
            if (d != D)
            {
                disp_.at<float>(i, j) = -1;
            }
            else
            {
                disp_.at<float>(i, j) = best_disp;
            }
        }
    }
}

const cv::Mat& StereoMatcherSGM::get() const
{
    return disp_;
}

cv::Ptr<StereoMatcher> createStereoMatcherSGM(int disp_max, int rows, int cols, float P1, float P2, int max_diff)
{
    return cv::Ptr<StereoMatcher>(new StereoMatcherSGM(disp_max, rows, cols, P1, P2, max_diff));
}

}
