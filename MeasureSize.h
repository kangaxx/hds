#pragma once

#include "HalconCpp.h"
using namespace HalconCpp;

// 面阵相机尺寸结构，mm
typedef struct __FSIZE
{
	double		W;
	double		L;
	double		W1;
	double		W2;
	double		H;
	double		H1;
	double		Ra[2];
	double		Rb[4];
} FSIZE;

// 面阵相机0尺寸像素位置
typedef struct __F0SIZE_PIXEL
{
	// 相机0的各边位置（像素）
	double		_c0Row0, _c0C0;	// W方向
	double		_c0Row1, _c0C1;	// W方向
	double		_c0Row2, _c0C2;	// W1方向
	double		_c0Row3, _c0C3;	// W1方向
	double		_c0Col0, _c0R0;	// 极耳左边
	double		_c0Col1, _c0R1;	// 极耳（金属）右边
	double		_c0Col2, _c0R2;	// 极片肩部
	double		_c0Col3, _c0R3;	// L方向
} F0SIZE_PIXEL;

// 面阵相机2尺寸像素位置
typedef struct __F2SIZE_PIXEL
{
	// 相机2的各边位置（像素）
	double		_c2Row0, _c2C0;	// L方向，极片左边
	double		_c2Row1, _c2C1;	// L方向，极片右边
	double		_c2Col0, _c2R0;	// W方向
	double		_c2Col1, _c2R1;	// W方向
} F2SIZE_PIXEL;

// 测量点位
typedef struct __MPOINT
{
	double		_row;
	double		_col;

	// 点绕原点顺时针旋转角度a（单位：弧度）
	void Rotate(const double& a)
	{
		double x1 = _col;
		double y1 = _row;
		_col = x1 * cos(a) - y1 * sin(a);
		_row = x1 * sin(a) + y1 * cos(a);
	}
} MPOINT;

class MeasureSize
{
public:
	MeasureSize();
	MeasureSize(double c0BodyGray, double c0EarGray, double c0EdgeThreshold, double c2BodyGray, double c2EdgeThreshold,
		 double gray_ramp_beta, int gray_ramp_column, double c0ImageAdd, double c2ImageAdd) : _c0BodyGray(c0BodyGray),
		_c0EarGray(c0EarGray), _c0EdgeThreshold(c0EdgeThreshold), _c2BodyGray(c2BodyGray), _c2EdgeThreshold(c2EdgeThreshold),
		_gray_ramp_beta(gray_ramp_beta), _gray_ramp_column(gray_ramp_column), _c0_image_add(c0ImageAdd), 
		_c2_image_add(c2ImageAdd), _scale(0.2) {
		Reset();
	}
	// 重置测量状态，图像计数清零
	void Reset();

	// 设置标准尺寸，将采用最后一片的像素结果与尺寸结果关联
	// 返回0：成功，其他：失败
	int SetStdSize(const FSIZE& size);

	void SetStdPos(const F0SIZE_PIXEL& c0Pos, const F2SIZE_PIXEL& c2Pos);

	// 设置相机0,2的尺寸像素比例
	void SetRate(const double& c0Rate, const double& c2Rate)
	{
		_c0RateX = _c0RateY = c0Rate;
		_c2RateX = _c2RateY = c2Rate;
	}

	// 计算相机0的各条边像素位置
	// 返回0：成功，其他：失败
	int CalcCamera0(HImage& image, F0SIZE_PIXEL& c0Pos);

	// 计算相机2的各条边像素位置
	// 返回0：成功，其他：失败
	int CalcCamera2(HImage& image, F2SIZE_PIXEL& c2Pos);

	// 根据相机0，2的各条边像素位置，基于标准图像，计算当前图的各项尺寸
	// 返回0：成功，其他：失败
	int CalcSize(const F0SIZE_PIXEL& c0Pos, const F2SIZE_PIXEL& c2Pos, FSIZE& size);

	// 设置默认的测量点位
	void SetMeasurePos();

	// 从相机0目标图像匹配待测量模板，目标图image已经缩小0.2倍
	// a：相对于模板的旋转角（单位：弧度），逆时针为正
	// 返回0：成功，其他：失败
	int CalcMeasurePosC0(HImage& image, MPOINT mp[8], double& a);

	// 从相机2目标图像匹配待测量模板，目标图image已经缩小0.2倍
	// a：相对于模板的旋转角（单位：弧度），逆时针为正
	// 返回0：成功，其他：失败
	int CalcMeasurePosC2(HImage& image, MPOINT mp[4], double& a);

private:
	int				_c0Count;	// 相机0图像计数

	int				_c2Count;	// 相机2图像计数

	// 标准尺寸
	FSIZE			_stdSize;

	// 相机0的各边位置（像素），对应标准尺寸测量的极片图像
	F0SIZE_PIXEL	_c0StdPos;
	F0SIZE_PIXEL	_c0LastPos;	// 当前结果

	// 相机2的各边位置（像素），对应标准尺寸测量的极片图像
	F2SIZE_PIXEL	_c2StdPos;
	F2SIZE_PIXEL	_c2LastPos;	// 当前结果

	// 相机0,2的尺寸像素比例
	double			_c0RateX, _c0RateY;
	double			_c2RateX, _c2RateY;
	// 相机0，极片区域二值化灰度
	double			_c0BodyGray;

	// 相机0，极耳区域二值化灰度
	double			_c0EarGray;

	// 寻边阈值
	double			_c0EdgeThreshold;
	double          _c2EdgeThreshold;
	// 相机2，极片区域二值化灰度
	double			_c2BodyGray;

	// 相机2，亮度补偿图
	HImage			_addImage;	
	double          _gray_ramp_beta; //补光图参数
	int             _gray_ramp_column; //补光图坐标(x)
	double             _c0_image_add;
	double             _c2_image_add;
	// 相机0的ncc模型
	HNCCModel		_c0Model;

	MPOINT			_c0MPoints[8]; // F0SIZE_PIXEL中的6个测量点位+极耳斜边测量区域的中心点

	// 相机2的ncc模型
	HNCCModel		_c2Model;

	MPOINT			_c2MPoints[4]; // F0SIZE_PIXEL中的4个测量点位

	// ncc模型缩小比例
	double			_scale;		   // 默认0.2
};
