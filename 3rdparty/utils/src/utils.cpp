#include "utils/utils.hpp"
#include <iostream>

namespace utils
{

cv::Vec3b jet(float d)
{
    if (d < 0)
    {
        d = -0.05;
    }
    if (d > 1)
    {
        d =  1.05;
    }

    d = d / 1.15 + 0.1;
    cv::Vec3b bgr;
    bgr[2] = std::max(0, std::min(255, (int)(round(255 * (1.5 - 4 * fabs(d - .75))))));
    bgr[1] = std::max(0, std::min(255, (int)(round(255 * (1.5 - 4 * fabs(d - .5))))));
    bgr[0] = std::max(0, std::min(255, (int)(round(255 * (1.5 - 4 * fabs(d - .25))))));
    return bgr;
}

cv::Mat applyColorMap(const cv::Mat& disp)
{
    CV_Assert(disp.depth() == CV_32F);
    CV_Assert(disp.channels() == 1);
    cv::Mat color_map(disp.size(), CV_8UC3);
    for (int i = 0; i < disp.size().width; i++)
    {
        for (int j = 0; j < disp.size().height; j++)
        {
            color_map.at<cv::Vec3b>(j, i) = jet(disp.at<float>(j, i));
        }
    }
    return color_map;
}

void convertDisparityToCloud(const cv::Mat &img, const cv::Mat &disp,
                             const cv::Matx33f K, float baseline,
                             std::vector<cv::Vec3f> &object_points,
                             std::vector< cv::Point3_<uchar> > &colors,
                             const cv::Vec3f& volume)
{
    CV_Assert(img.size() == disp.size());
    CV_Assert(img.type() == CV_8UC3);
    CV_Assert(disp.type() == CV_32FC1);

    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            float d = disp.ptr<float>(y)[x];
            float depth = d <= 0 ? std::numeric_limits<float>::max()
                : K(0, 0) * baseline / d;

            cv::Vec3f p3d = K.inv() * cv::Vec3f(x * depth, y * depth, depth);
            if ( fabs(p3d[0]) < volume[0] &&
                 fabs(p3d[1]) < volume[1] &&
                 fabs(p3d[2]) < volume[2])
            {
                object_points.push_back(p3d);
                colors.push_back(img.ptr< cv::Point3_<uchar> >(y)[x]);
            }
        }
    }
}

}
