//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////通过人脸检测粗定位身份证号码区域
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////通过连通块检测确定身份证号码中心线
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////通过中心线与水平方向的夹角旋转校正图像
#include "highgui.h"
#include "cv.h"
#include "BlobResult.h"
#include "blob.h"

#include "viDefineStruct.h"

using namespace cv;


//在输入彩色图像 img 中检测人脸
//将人脸区域返回到CvRect型变量中
//分类器数据文件为cascade
CvRect  GetFacialRect( IplImage *img, CvHaarClassifierCascade *cascade);

//计算输入RGB彩色图像srcRGB的OTSU二值图像
//将结果反映到输出图像dstBinary中
void getBinaryImageOTSU(IplImage* srcRGB, IplImage* dstBinary);

//计算输入灰度图像srcGray的自适应化二值图像
//将结果反映到输出图像dstBinMat中
void GetBinaryImageAdaptive(IplImage* srcGray, Mat &dstBinMat);

//锁定长宽比例缩放输入图像
IplImage* ResizeInputImage(IplImage *src);

//第一次进行blob操作, 粗过滤
//寻找二值图像srcBinary中的blobs
//将符合一定条件的blobs的中心点保存到Cplist中
void GetOriginalCenterPointList(IplImage *srcBinary, CenterPointList &CpList);

//自定义 list 升序排序规则(根据blob中心点X坐标)
bool BlobCenterXSortRule(const CenterPointNode &leftData, const CenterPointNode &rightData);

//确定数字字符的中心线
//筛选blob, 删除偏离了中心线的blob
//参数CpList为保存了blob中点的list
//返回blobs中点y坐标的均值
int RemovePartCenterLinePoints(CenterPointList &Cplist);

//本文使用的图像旋转方法，完全借鉴自网络文件
//旋转后的图像可自动调整至新的大小
void bound(int x, int y, float ca, float sa, int *xmin, int *xmax, int *ymin, int *ymax);
IplImage* cvMyRotateImage(IplImage *src, double angle);