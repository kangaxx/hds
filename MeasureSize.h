#pragma once

#include "HalconCpp.h"
using namespace HalconCpp;

// ��������ߴ�ṹ��mm
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

// �������0�ߴ�����λ��
typedef struct __F0SIZE_PIXEL
{
	// ���0�ĸ���λ�ã����أ�
	double		_c0Row0, _c0C0;	// W����
	double		_c0Row1, _c0C1;	// W����
	double		_c0Row2, _c0C2;	// W1����
	double		_c0Row3, _c0C3;	// W1����
	double		_c0Col0, _c0R0;	// �������
	double		_c0Col1, _c0R1;	// �������������ұ�
	double		_c0Col2, _c0R2;	// ��Ƭ�粿
	double		_c0Col3, _c0R3;	// L����
} F0SIZE_PIXEL;

// �������2�ߴ�����λ��
typedef struct __F2SIZE_PIXEL
{
	// ���2�ĸ���λ�ã����أ�
	double		_c2Row0, _c2C0;	// L���򣬼�Ƭ���
	double		_c2Row1, _c2C1;	// L���򣬼�Ƭ�ұ�
	double		_c2Col0, _c2R0;	// W����
	double		_c2Col1, _c2R1;	// W����
} F2SIZE_PIXEL;

// ������λ
typedef struct __MPOINT
{
	double		_row;
	double		_col;

	// ����ԭ��˳ʱ����ת�Ƕ�a����λ�����ȣ�
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
	// ���ò���״̬��ͼ���������
	void Reset();

	// ���ñ�׼�ߴ磬���������һƬ�����ؽ����ߴ�������
	// ����0���ɹ���������ʧ��
	int SetStdSize(const FSIZE& size);

	void SetStdPos(const F0SIZE_PIXEL& c0Pos, const F2SIZE_PIXEL& c2Pos);

	// �������0,2�ĳߴ����ر���
	void SetRate(const double& c0Rate, const double& c2Rate)
	{
		_c0RateX = _c0RateY = c0Rate;
		_c2RateX = _c2RateY = c2Rate;
	}

	// �������0�ĸ���������λ��
	// ����0���ɹ���������ʧ��
	int CalcCamera0(HImage& image, F0SIZE_PIXEL& c0Pos);

	// �������2�ĸ���������λ��
	// ����0���ɹ���������ʧ��
	int CalcCamera2(HImage& image, F2SIZE_PIXEL& c2Pos);

	// �������0��2�ĸ���������λ�ã����ڱ�׼ͼ�񣬼��㵱ǰͼ�ĸ���ߴ�
	// ����0���ɹ���������ʧ��
	int CalcSize(const F0SIZE_PIXEL& c0Pos, const F2SIZE_PIXEL& c2Pos, FSIZE& size);

	// ����Ĭ�ϵĲ�����λ
	void SetMeasurePos();

	// �����0Ŀ��ͼ��ƥ�������ģ�壬Ŀ��ͼimage�Ѿ���С0.2��
	// a�������ģ�����ת�ǣ���λ�����ȣ�����ʱ��Ϊ��
	// ����0���ɹ���������ʧ��
	int CalcMeasurePosC0(HImage& image, MPOINT mp[8], double& a);

	// �����2Ŀ��ͼ��ƥ�������ģ�壬Ŀ��ͼimage�Ѿ���С0.2��
	// a�������ģ�����ת�ǣ���λ�����ȣ�����ʱ��Ϊ��
	// ����0���ɹ���������ʧ��
	int CalcMeasurePosC2(HImage& image, MPOINT mp[4], double& a);

private:
	int				_c0Count;	// ���0ͼ�����

	int				_c2Count;	// ���2ͼ�����

	// ��׼�ߴ�
	FSIZE			_stdSize;

	// ���0�ĸ���λ�ã����أ�����Ӧ��׼�ߴ�����ļ�Ƭͼ��
	F0SIZE_PIXEL	_c0StdPos;
	F0SIZE_PIXEL	_c0LastPos;	// ��ǰ���

	// ���2�ĸ���λ�ã����أ�����Ӧ��׼�ߴ�����ļ�Ƭͼ��
	F2SIZE_PIXEL	_c2StdPos;
	F2SIZE_PIXEL	_c2LastPos;	// ��ǰ���

	// ���0,2�ĳߴ����ر���
	double			_c0RateX, _c0RateY;
	double			_c2RateX, _c2RateY;
	// ���0����Ƭ�����ֵ���Ҷ�
	double			_c0BodyGray;

	// ���0�����������ֵ���Ҷ�
	double			_c0EarGray;

	// Ѱ����ֵ
	double			_c0EdgeThreshold;
	double          _c2EdgeThreshold;
	// ���2����Ƭ�����ֵ���Ҷ�
	double			_c2BodyGray;

	// ���2�����Ȳ���ͼ
	HImage			_addImage;	
	double          _gray_ramp_beta; //����ͼ����
	int             _gray_ramp_column; //����ͼ����(x)
	double             _c0_image_add;
	double             _c2_image_add;
	// ���0��nccģ��
	HNCCModel		_c0Model;

	MPOINT			_c0MPoints[8]; // F0SIZE_PIXEL�е�6��������λ+����б�߲�����������ĵ�

	// ���2��nccģ��
	HNCCModel		_c2Model;

	MPOINT			_c2MPoints[4]; // F0SIZE_PIXEL�е�4��������λ

	// nccģ����С����
	double			_scale;		   // Ĭ��0.2
};
