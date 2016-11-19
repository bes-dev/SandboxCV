#include <cost_volume.hpp>
#include <bitset>

#define CENSUS_WND 15
#define CENSUS_BIT 224

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
    std::bitset<CENSUS_BIT>* left_code = new std::bitset<CENSUS_BIT>[ rows_ * cols_ ];
    std::bitset<CENSUS_BIT>* right_code = new std::bitset<CENSUS_BIT>[ rows_ * cols_ ];
    std::bitset<CENSUS_BIT>* pLCode = left_code;
    std::bitset<CENSUS_BIT>* pRCode = right_code;

    for ( int y = 0; y < rows_; y ++ )
    {
		uchar* pLData = ( uchar* ) ( left.ptr<uchar>( y ) );
		uchar* pRData = ( uchar* ) ( right.ptr<uchar>( y ) );
		for ( int x = 0; x < cols_; x ++ )
        {
			int bitCnt = 0;
			for ( int wy = - H_WD; wy <= H_WD; wy ++ )
            {
				int qy = ( y + wy + rows_ ) % rows_;
				uchar* qLData = ( uchar* ) ( left.ptr<uchar>( qy ) );
				uchar* qRData = ( uchar* ) ( right.ptr<uchar>( qy ) );
				for ( int wx = - H_WD; wx <= H_WD; wx ++ )
                {
					if ( wy != 0 || wx != 0 )
                    {
						int qx = ( x + wx + cols_ ) % cols_;
						( *pLCode )[ bitCnt ] = ( pLData[ x ] > qLData[ qx ] );
						( *pRCode )[ bitCnt ] = ( pRData[ x ] > qRData[ qx ] );
						bitCnt ++;
					}
				}
			}
			pLCode ++;
			pRCode ++;
		}
	}

    std::bitset<CENSUS_BIT> lB;
    std::bitset<CENSUS_BIT> rB;
	pLCode = left_code;

    cost_volume_.setTo(0);

    for( int y = 0; y < rows_; y ++ )
    {
		int index = y * cols_;
		for( int x = 0; x < cols_; x ++ )
        {
            float* cost = cost_volume_.ptr<float>(y);
			lB = *pLCode;
			for( int d = 0; d < disp_max_; d ++ )
            {
                cost[x * disp_max_ + d] = 1.0f;
				if( x - d >= 0 )
                {
					rB = right_code[ index + x - d ];
					cost[ x * disp_max_ + d ] = ( lB ^ rB ).count() / (float)CENSUS_BIT;
				}
			}
			pLCode ++;
		}
	}
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
