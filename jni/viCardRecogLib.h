#include "viGetPreDefinedBoxes.h"


extern CvRect gEveryNumRect[18];			//���֤������ÿһ�����ֵ�׼ȷ����
extern IplImage *iplTemplateImgArray[11];	//ÿһ�����ֵ�ƥ��ʶ��ģ��ͼ��
extern char strNumberRecogResult[19];		//���֤����ʶ����

//�õ����֤���������ֵͼ����ÿһ�����ֵ�׼ȷ����
//�������˳�򱣴���gEveryNumRect[18]��
void GetEveryNumRect(IplImage *srcBinary);

//ʶ�����֤�����е�ÿһ������
char RecognizeEveryNumber(IplImage *img);

void GetCardNumber(IplImage *idCardCutImg);

//��API, srcCapturedΪ����ԭʼͼ��, ���ش�����ͼ��
//����ʶ������֤�����������strNumberRecogResult��
IplImage* RecognizeIDCardNumber(IplImage *srcCaptured);
