//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ͨ���������ֶ�λ���֤��������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ͨ����ͨ����ȷ�����֤����������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ͨ����������ˮƽ����ļн���תУ��ͼ��
#include "highgui.h"
#include "cv.h"
#include "BlobResult.h"
#include "blob.h"

#include "viDefineStruct.h"

using namespace cv;


//�������ɫͼ�� img �м������
//���������򷵻ص�CvRect�ͱ�����
//�����������ļ�Ϊcascade
CvRect  GetFacialRect( IplImage *img, CvHaarClassifierCascade *cascade);

//��������RGB��ɫͼ��srcRGB��OTSU��ֵͼ��
//�������ӳ�����ͼ��dstBinary��
void getBinaryImageOTSU(IplImage* srcRGB, IplImage* dstBinary);

//��������Ҷ�ͼ��srcGray������Ӧ����ֵͼ��
//�������ӳ�����ͼ��dstBinMat��
void GetBinaryImageAdaptive(IplImage* srcGray, Mat &dstBinMat);

//�������������������ͼ��
IplImage* ResizeInputImage(IplImage *src);

//��һ�ν���blob����, �ֹ���
//Ѱ�Ҷ�ֵͼ��srcBinary�е�blobs
//������һ��������blobs�����ĵ㱣�浽Cplist��
void GetOriginalCenterPointList(IplImage *srcBinary, CenterPointList &CpList);

//�Զ��� list �����������(����blob���ĵ�X����)
bool BlobCenterXSortRule(const CenterPointNode &leftData, const CenterPointNode &rightData);

//ȷ�������ַ���������
//ɸѡblob, ɾ��ƫ���������ߵ�blob
//����CpListΪ������blob�е��list
//����blobs�е�y����ľ�ֵ
int RemovePartCenterLinePoints(CenterPointList &Cplist);

//����ʹ�õ�ͼ����ת��������ȫ����������ļ�
//��ת���ͼ����Զ��������µĴ�С
void bound(int x, int y, float ca, float sa, int *xmin, int *xmax, int *ymin, int *ymax);
IplImage* cvMyRotateImage(IplImage *src, double angle);