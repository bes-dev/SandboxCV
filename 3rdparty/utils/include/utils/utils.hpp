#ifndef __ADAS_UTILS_HPP__
#define __ADAS_UTILS_HPP__

#include <opencv2/core/core.hpp>

namespace utils
{

cv::Mat applyColorMap(const cv::Mat& disp);
void convertDisparityToCloud(const cv::Mat &img, const cv::Mat &disp,
                             const cv::Matx33f K, float baseline,
                             std::vector<cv::Vec3f> &object_points,
                             std::vector< cv::Point3_<uchar> > &colors,
                             const cv::Vec3f& volume = cv::Vec3f(5, 5, 15));

}

#endif // __ADAS_UTILS_HPP__
