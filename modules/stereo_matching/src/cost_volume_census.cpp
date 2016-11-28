#include <cost_volume.hpp>
#include <bitset>

#define CENSUS_WND 15
#define CENSUS_BIT 224
#define COST_MAX 10000

namespace stereo
{

class CostVolumeCensus: public CostVolume
{
public:
    ~CostVolumeCensus(){}
    CostVolumeCensus(int disp_max, int rows, int cols);
    void process(const cv::Mat& left, const cv::Mat& right);
    const cv::Mat& get() const;

protected:
    cv::Mat cost_volume_;
    int disp_max_;
    int rows_, cols_;
};

CostVolumeCensus::CostVolumeCensus(int disp_max, int rows, int cols):
    disp_max_(disp_max),
    rows_(rows),
    cols_(cols)
{
    cost_volume_.create(rows_, cols_ * disp_max_, CV_32FC1);
}

void CostVolumeCensus::process(const cv::Mat& left, const cv::Mat& right)
{
    CV_Assert(left.channels() == 1 && right.channels() == 1);
    int H_WD = CENSUS_WND / 2;
    std::bitset<CENSUS_BIT>* left_code = new std::bitset<CENSUS_BIT>[ left.rows * left.cols ];
    std::bitset<CENSUS_BIT>* right_code = new std::bitset<CENSUS_BIT>[ left.rows * left.cols ];
    for ( int y = 0; y < rows_; y ++ )
		for ( int x = 0; x < cols_; x ++ ) {
			int bitCnt = 0;
			for ( int wy = - H_WD; wy <= H_WD; wy ++ ) {
				int qy = ( y + wy + left.rows ) % left.rows;
				for ( int wx = - H_WD; wx <= H_WD; wx ++ ) {
					if ( wy != 0 || wx != 0 ) {
						int qx = ( x + wx + left.cols ) % left.cols;
						left_code[left.cols * y + x][ bitCnt ] = ( left.at<uchar>(y, x) > left.at<uchar>(qy, qx) );
						right_code[left.cols * y + x][ bitCnt ] = ( right.at<uchar>(y, x) > right.at<uchar>(qy, qx) );
						bitCnt ++;
					}
				}
			}
		}
    for( int y = 0; y < rows_; y ++ )
		for( int x = 0; x < cols_; x ++ )
			for( int d = 0; d < disp_max_; d ++ )
                cost_volume_.at<float>(y, x * disp_max_ + d) = x - d >= 0 ? ( left_code[cols_ * y + x] ^ right_code[cols_ * y + x - d] ).count() / (float)CENSUS_BIT : COST_MAX;
	delete [] left_code;
	delete [] right_code;
}

const cv::Mat& CostVolumeCensus::get() const
{
    return cost_volume_;
}

cv::Ptr<CostVolume> createCostVolumeCensus(int disp_max, int rows, int cols)
{
    return cv::Ptr<CostVolume>(new CostVolumeCensus(disp_max, rows, cols));
}

}
