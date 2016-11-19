#ifndef __SIMPLE_VIEWER_HPP__
#define __SIMPLE_VIEWER_HPP__

#include <opencv2/core/core.hpp>
#include <string>

namespace utils
{
namespace viz
{

struct ImageNode
{
    ImageNode(const cv::Mat& img, const std::string& label, bool is_depth = false, cv::Matx33f K = cv::Matx33f::zeros(), float baseline = 1.f, int texture_id = -1):
        img_(img),
        label_(label),
        is_depth_(is_depth) {}

    const cv::Mat& img_;
    std::string label_;
    bool is_depth_;
};

class Viewer
{
public:
    Viewer(const std::string& window_name);
    char show(const std::vector<ImageNode>& nodes, int delay = 0);
    bool isShowMenu();
    void setShowMenu(bool mode);

protected:
    std::string window_name_;
    bool is_show_menu_;
};


}
}

#endif // __SIMPLE_VIEWER_HPP__
