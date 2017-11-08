#include "viGetPreDefinedBoxes.h"


extern CvRect gEveryNumRect[18];			//身份证号码中每一个数字的准确区域
extern IplImage *iplTemplateImgArray[11];	//每一个数字的匹配识别模板图像
extern char strNumberRecogResult[19];		//身份证号码识别结果

//得到身份证号码区域二值图像中每一个数字的准确区域
//将结果按顺序保存在gEveryNumRect[18]中
void GetEveryNumRect(IplImage *srcBinary);

//识别身份证号码中的每一个数字
char RecognizeEveryNumber(IplImage *img);

void GetCardNumber(IplImage *idCardCutImg);

//总API, srcCaptured为输入原始图像, 返回处理后的图像
//并将识别的身份证号码存入数组strNumberRecogResult中
IplImage* RecognizeIDCardNumber(IplImage *srcCaptured);
