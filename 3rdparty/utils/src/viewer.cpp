#include <utils/viewer.hpp>
#include <utils/utils.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// #include <opencv2/viz/viz3d.hpp>

namespace utils
{
namespace viz
{

Viewer::Viewer(const std::string& window_name):
    window_name_(window_name),
    is_show_menu_(false)
{}

void drawMenu(cv::Mat& screen, const std::string& label)
{
    cv::Mat menu(screen.rows, 200, screen.type(), cv::Scalar::all(0));
    cv::Scalar white = cv::Scalar::all(255);
    const int font = cv::FONT_HERSHEY_COMPLEX_SMALL;

    cv::putText(menu, label, cv::Point(5, 20), font, 1.2, white, 1, CV_AA);
    cv::putText(menu, "'ESC' - exit", cv::Point(5, 40), font, 0.7, white, 1, CV_AA);
    cv::putText(menu, "'p' - previous view", cv::Point(5, 60), font, 0.7, white, 1, CV_AA);
    cv::putText(menu, "'n' - next view", cv::Point(5, 80), font, 0.7, white, 1, CV_AA);
    cv::putText(menu, "'SPACE' - next step", cv::Point(5, 100), font, 0.7, white, 1, CV_AA);
    cv::putText(menu, "'m' - hide menu", cv::Point(5, 120), font, 0.7, white, 1, CV_AA);
    cv::hconcat(menu, screen, screen);
}

char Viewer::show(const std::vector<ImageNode>& nodes, int delay)
{
    int id = 0;
    char key = 0;
    while(true)
    {
        cv::Mat screen = nodes[id].img_.clone();
        if (nodes[id].is_depth_)
        {
            double max = 0;
            cv::minMaxLoc(screen, NULL, &max);
            screen.convertTo(screen, CV_8UC1, 255.0/max);
        }
        if (screen.channels() != 3)
        {
            cv::cvtColor(screen, screen, CV_GRAY2BGR);
        }
        if (is_show_menu_)
        {
            drawMenu(screen, nodes[id].label_);
        }
        else
        {
            cv::Scalar white = cv::Scalar::all(0);
            const int font = cv::FONT_HERSHEY_COMPLEX_SMALL;
            cv::rectangle(screen, cv::Rect(3, 3, 170, 22), cv::Scalar::all(255), -1);
            cv::putText(screen, "'m' - show menu", cv::Point(5, 20), font, 0.7, white, 1, CV_AA);
        }
        cv::imshow(window_name_, screen);
        key = cv::waitKey(delay);
        if (key == 27 || key == 32)
        {
            break;
        }
        else if ( key == 'n')
        {
            id = (id+1) % nodes.size();
        }
        else if ( key == 'p')
        {
            id--;
            if (id < 0)
            {
                id = nodes.size() - 1;
            }
        }
        // else if ( key == 'v' && nodes[id].is_depth_)
        // {
        //     std::vector<cv::Vec3f> cloud;
        //     std::vector<cv::Point3_<uchar> > colors;

        //     std::string title = "depth";
        //     cv::viz::Viz3d viz(title);

        //     cv::Matx33f K;
        //     K(0, 0) = config_.focal[0]; K(0, 1) = 0; K(0, 2) = rgb.size().width / 2;
        //     K(1, 0) = 0; K(1, 1) = config_.focal[0]; K(1, 2) = rgb.size().height / 2;
        //     K(2, 0) = 0; K(2, 1) = 0; K(2, 2) = 1;

        //     cloud.clear();
        //     colors.clear();
        //     cv::Mat texture = nodes[nodes[id].texture_id_].img_.clone();
        //     if (rgb.type() != CV_8UC3)
        //     {
        //         cv::cvtColor(rgb, texture, CV_GRAY2BGR);
        //     }
        //     utils::convertDisparityToCloud(texture, nodes[id].img_, nodes[id].K, nodes[id].baseline, cloud, colors);

        //     cv::Affine3d pose = cv::Affine3d().rotate(cv::Vec3d(CV_PI + CV_PI / 16, 0, 0));
        //     viz.showWidget("cloud", cv::viz::WCloud(cloud, colors), pose);

        //     viz.spin();
        // }
        else if ( key == 'm')
        {
            is_show_menu_ = !is_show_menu_;
        }
        else if (delay)
        {
            break;
        }
    }

    return key;
}

bool Viewer::isShowMenu()
{
    return is_show_menu_;
}

void Viewer::setShowMenu(bool mode)
{
    is_show_menu_ = mode;
}

}
}
