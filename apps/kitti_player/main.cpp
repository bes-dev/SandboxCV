#include <iostream>
#include <dataset.hpp>
#include <utils/viewer.hpp>
#include <opencv2/core/core.hpp>

void help()
{
    std::cout<<"Using: ./kitti_player <path to one of the kitti stereo sequence>"<<std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        help();
        return 0;
    }
    cv::Ptr<dataset::Dataset> cap = dataset::createDatasetKITTI();
    cap->init(std::string(argv[1]));

    std::vector<utils::viz::ImageNode> image_nodes;
    utils::viz::Viewer viewer("kitti");

    cv::Mat left, right;
    image_nodes.push_back(utils::viz::ImageNode(left, "left"));
    image_nodes.push_back(utils::viz::ImageNode(right, "right"));
    while(cap->grab(left, right))
    {
        if (viewer.show(image_nodes) == 27)
        {
            break;
        }
    }

    return 0;
}
