//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�õ�����Ԥ�������������
#include "viGetCardNumberRect.h"


#define NAME_SX		92/1.0/498
#define NAME_BX		181/1.0/498
#define NAME_SY		33/1.0/308
#define NAME_BY		76/1.0/308

#define SEX_SX			92/1.0/498
#define SEX_BX			133/1.0/498
#define SEX_SY			76/1.0/308
#define SEX_BY			112/1.0/308

#define MINZU_SX		197/1.0/498
#define MINZU_BX		255/1.0/498
#define MINZU_SY		76/1.0/308
#define MINZU_BY		112/1.0/308

#define YEAR_SX		93/1.0/498
#define YEAR_BX		153/1.0/498
#define YEAR_SY		114/1.0/308
#define YEAR_BY		145/1.0/308

#define MONTH_SX	172/1.0/498
#define MONTH_BX	202/1.0/498
#define MONTH_SY	114/1.0/308
#define MONTH_BY	145/1.0/308

#define DAY_SX			222/1.0/498
#define DAY_BX			250/1.0/498
#define DAY_SY			114/1.0/308
#define DAY_BY			145/1.0/308

#define ADDRESS_SX		90/1.0/498
#define ADDRESS_BX	320/1.0/498
#define ADDRESS_SY		152/1.0/308
#define ADDRESS_BY	240/1.0/308

#define PICTURE_SX		321/1.0/498
#define PICTURE_BX		482/1.0/498
#define PICTURE_SY		34/1.0/308
#define PICTURE_BY		237/1.0/308

#define NUMBER_SX		170/1.0/498
#define NUMBER_BX		460/1.0/498
#define NUMBER_SY		252/1.0/308
#define NUMBER_BY		291/1.0/308

#define NUM_BOX_ORG	9
enum BOX_ORG															//���֤��Ԥ�����9������ı��
{
	NAME_ORG, SEX_ORG, MINZU_ORG, YEAR_ORG, MONTH_ORG, DAY_ORG, ADDRESS_ORG, PICTURE_ORG, NUMBER_ORG
};

extern CvRect gOrgRectBoxArray[NUM_BOX_ORG];			//ÿ������Ĵ���λ�ã���������
extern CvRect gAccurateRectBoxArray[NUM_BOX_ORG];	//ÿ�������е����������λ�ã��������
extern CvRect gAbsoluteBoxRectArray[NUM_BOX_ORG];	//��ʵgAbsoluteBoxRectArray�д洢�Ĳ���������CvRect�ṹ�����Ǹýṹ�����ϽǺ����½ǵ�����ֵ
extern int gBlobFilterSize[NUM_BOX_ORG];

void GetLabelAeras(IplImage *corrected_RGB_img, int real_width, int real_height, bool whetherDraw);

//�õ�����Ԥ������������򣬲������������gOrgRectBoxArray��
IplImage* GetPreDefinedAboutBoxes(IplImage* src, CvHaarClassifierCascade *cascade);

//Ѱ�Ҷ�ֵͼ��srcBinary�п����漰�����ֵ�blob
//�����Ƴ��ľ�ȷ���򷵻ص�rtAccurate��
void GetTextContainedBlobs(IplImage *srcBinary, CvRect &rtAccurate);

//����ÿһ��С����gOrgRectBoxArray[]�ľ��������
//���������������ھ�ȷ����gAccurateRectBoxArray[]���������
//�õ��������ھ�ȷ����ľ�������gAbsoluteBoxRectArray
void GetAbsoluteBoxRect();

//�õ��������ڵľ�ȷ����, ��������
//�洢��gAbsoluteBoxRectArray[]��
void GetAbsoluteAccurateBoxRects(IplImage *img);
