//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////得到所有预定义的文字区域
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
enum BOX_ORG															//身份证中预定义的9个区域的编号
{
	NAME_ORG, SEX_ORG, MINZU_ORG, YEAR_ORG, MONTH_ORG, DAY_ORG, ADDRESS_ORG, PICTURE_ORG, NUMBER_ORG
};

extern CvRect gOrgRectBoxArray[NUM_BOX_ORG];			//每个区域的大致位置，绝对坐标
extern CvRect gAccurateRectBoxArray[NUM_BOX_ORG];	//每个区域中的文字区域的位置，相对坐标
extern CvRect gAbsoluteBoxRectArray[NUM_BOX_ORG];	//其实gAbsoluteBoxRectArray中存储的不是真正的CvRect结构，而是该结构的左上角和右下角的坐标值
extern int gBlobFilterSize[NUM_BOX_ORG];

void GetLabelAeras(IplImage *corrected_RGB_img, int real_width, int real_height, bool whetherDraw);

//得到所有预定义的文字区域，并将其存入数组gOrgRectBoxArray中
IplImage* GetPreDefinedAboutBoxes(IplImage* src, CvHaarClassifierCascade *cascade);

//寻找二值图像srcBinary中可能涉及到文字的blob
//将估计出的精确区域返回到rtAccurate中
void GetTextContainedBlobs(IplImage *srcBinary, CvRect &rtAccurate);

//根据每一个小区域gOrgRectBoxArray[]的绝对坐标和
//该区域内文字所在精确区域gAccurateRectBoxArray[]的相对坐标
//得到文字所在精确区域的绝对坐标gAbsoluteBoxRectArray
void GetAbsoluteBoxRect();

//得到文字所在的精确区域, 绝对坐标
//存储在gAbsoluteBoxRectArray[]中
void GetAbsoluteAccurateBoxRects(IplImage *img);
