#include "dataset.hpp"
#include <dirent.h>
#include <opencv2/highgui/highgui.hpp>

namespace dataset
{

class DatasetKITTI: public Dataset
{
public:
    DatasetKITTI() {}
    ~DatasetKITTI(){}
    bool init(const std::string& path);
    bool grab(cv::Mat& left, cv::Mat& right);
    const cv::Mat& getK();
    float getBaseline();
    float getFPS();

protected:
    std::string path_;
    std::vector<std::string> files_;
    int idx_;
    cv::Mat K_;
    float baseline_;
    float fps_;
};

bool DatasetKITTI::init(const std::string& path)
{
    path_ = path;

    std::string left_path = path_ + "image_02/";

    DIR *dir;
    struct dirent *ent;
    if ( (dir = opendir(left_path.c_str())) != NULL )
    {
        while ((ent = readdir (dir)) != NULL)
		{
            if(std::string(ent->d_name) != "." && std::string(ent->d_name) != "..")
			{
                files_.push_back(ent->d_name);
            }
        }
        closedir(dir);
    }
    else
    {
        return false;
    }

    std::sort(files_.begin(), files_.end());
    idx_ = 0;

    K_ = cv::Mat(3, 3, CV_64FC1, cv::Scalar::all(0));
    K_.at<double>(0, 0) = K_.at<double>(1, 1) = 645.24;
    K_.at<double>(0, 2) = 635.96;
    K_.at<double>(1, 2) = 194.13;
    K_.at<double>(2, 2) = 1;
    baseline_ = 0.5707;
    fps_ = 15.0f;

    return true;
}

bool DatasetKITTI::grab(cv::Mat& left, cv::Mat& right)
{
    bool status = true;
    if (! (idx_ < files_.size()))
    {
        idx_ = 0;
        status = false;
    }
    left = cv::imread(path_ + "image_02/" + files_[idx_]);
    right = cv::imread(path_ + "image_03/" + files_[idx_]);
    idx_++;
    return status;
}

const cv::Mat& DatasetKITTI::getK()
{
    return K_;
}

float DatasetKITTI::getBaseline()
{
    return baseline_;
}

float DatasetKITTI::getFPS()
{
    return fps_;
}

Dataset* createDatasetKITTI()
{
    return new DatasetKITTI();
}

}
