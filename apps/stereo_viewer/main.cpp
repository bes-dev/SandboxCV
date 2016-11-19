#include <iostream>
#include <dataset.hpp>
#include <utils/viewer.hpp>
#include <cost_volume.hpp>
#include <stereo_matcher.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void help()
{
    std::cout<<"Using: ./stereo_viewer <path to one of the kitti stereo sequence> disp_max method_id P1 P2 max_diff"<<std::endl;
    std::cout<<"disp_max (default = 128)"<<std::endl;
    std::cout<<"method_id: 0 - BM, 1 - SGM (default = 1)"<<std::endl;
    std::cout<<"P1: SGM parameter (default = 0.3)"<<std::endl;
    std::cout<<"P2: SGM parameter (default = 1.0)"<<std::endl;
    std::cout<<"max_diff: SGM parameter (default = 1)"<<std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        help();
        return 0;
    }
    std::string path = argv[1];
    int disp_max = argc > 2 ? atoi(argv[2]) : 128;
    int method_id = argc > 3 ? atoi(argv[3]) : 1;
    float P1 = argc > 4 ?  atof(argv[4]) : 0.3f;
    float P2 = argc > 5 ? atof(argv[5]) : 1.0f;
    int max_diff = argc > 6 ? atoi(argv[6]) : 1;

    cv::Ptr<dataset::Dataset> cap = dataset::createDatasetKITTI();
    cap->init(std::string(argv[1]));

    std::vector<utils::viz::ImageNode> image_nodes;
    utils::viz::Viewer viewer("kitti");

    cv::Mat left, right, disp;
    cap->grab(left, right);

    cv::Ptr<stereo::CostVolume> volume = stereo::createCostVolumeCensus(disp_max, left.rows, left.cols);

    cv::Ptr<stereo::StereoMatcher> matcher;
    if (method_id)
    {
        matcher = stereo::createStereoMatcherSGM(disp_max, left.rows, left.cols, P1, P2, max_diff);
    }
    else
    {
        matcher = stereo::createStereoMatcherSimple(disp_max, left.rows, left.cols);
    }

    image_nodes.push_back(utils::viz::ImageNode(left, "left"));
    image_nodes.push_back(utils::viz::ImageNode(right, "right"));
    image_nodes.push_back(utils::viz::ImageNode(disp, "disp", true));
    while(cap->grab(left, right))
    {
        cv::Mat left_g, right_g;
        cv::cvtColor(left, left_g, CV_BGR2GRAY);
        cv::cvtColor(right, right_g, CV_BGR2GRAY);

        volume->process(left_g, right_g);
        matcher->process(volume->get());
        disp = matcher->get().clone();

        if (viewer.show(image_nodes) == 27)
        {
            break;
        }
    }

    return 0;
}
