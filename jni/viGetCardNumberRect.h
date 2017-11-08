//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�õ����֤��������
#include "viPreGetCardNumRect.h"



//�Զ��� list �����������(����blob���ĵ�X����)
bool BlobInfoNodeXSortRule(const BlobInfoNode &leftData, const BlobInfoNode &rightData);

//�ڶ��ν���blob����
//���˶�ֵͼ��srcBinary�е�blob, �����µĶ�ֵͼ�񷵻ص�srcBinary
//������Ҫ���blob���뵽BiList��
void GetFurtherBlobsAndNewBinaryImg(IplImage *srcBinary, BlobInfoList &BiList);

//�õ����˺��blob��ƽ�����
double GetFilteredBlobAverageWidth(BlobInfoList &BiList);

//�õ����֤����ǰ���һ�����ֵ����ĵ��X����
//aveWidth��blob��ƽ�����
int GetLastCharacterEdgeX(BlobInfoList &BiList, double aveWidth);

//�˳����֤����ǰ���ĺ���blob
void RemoveFrontCharBlob(IplImage *srcBinary, BlobInfoList &BiList, int edgeX);

//�õ����֤����ĵ�һ�������һ���ַ�������Ϣ
BlobInfoNode GetAccuteNumRect(BlobInfoList &BiList);

//�õ����֤����ĵ�һ�������һ���ַ�������Ϣ
BlobInfoNode GetCardNumberRect(IplImage *accurate_binary_img);