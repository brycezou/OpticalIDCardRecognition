//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////得到身份证号码区域
#include "viPreGetCardNumRect.h"



//自定义 list 升序排序规则(根据blob中心点X坐标)
bool BlobInfoNodeXSortRule(const BlobInfoNode &leftData, const BlobInfoNode &rightData);

//第二次进行blob操作
//过滤二值图像srcBinary中的blob, 并将新的二值图像返回到srcBinary
//将符合要求的blob加入到BiList中
void GetFurtherBlobsAndNewBinaryImg(IplImage *srcBinary, BlobInfoList &BiList);

//得到过滤后的blob的平均宽度
double GetFilteredBlobAverageWidth(BlobInfoList &BiList);

//得到身份证号码前最后一个汉字的中心点的X坐标
//aveWidth是blob的平均宽度
int GetLastCharacterEdgeX(BlobInfoList &BiList, double aveWidth);

//滤除身份证号码前方的汉字blob
void RemoveFrontCharBlob(IplImage *srcBinary, BlobInfoList &BiList, int edgeX);

//得到身份证号码的第一个和最后一个字符坐标信息
BlobInfoNode GetAccuteNumRect(BlobInfoList &BiList);

//得到身份证号码的第一个和最后一个字符坐标信息
BlobInfoNode GetCardNumberRect(IplImage *accurate_binary_img);