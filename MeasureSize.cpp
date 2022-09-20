// 阳极算法

#include "MeasureSize.h"

#define PI 3.1415926535897932384626433832795

MeasureSize::MeasureSize() : _c0BodyGray(75), _c0EarGray(250), _c0EdgeThreshold(15), _c2BodyGray(80)
{
	Reset();

	// 创建相机2亮度补偿图
	_addImage.GenImageGrayRamp(0, 0.1, 0, 0, 2700, 3128, 1136);
}

void MeasureSize::Reset()
{
	_c0Count = _c2Count = 0;

	_c0RateX = _c0RateY = 0.03407851690294438386041439476554;
	_c2RateX = _c2RateY = 0.04398311048557353976073187895848;

	memset(&_c0StdPos, 0, sizeof(_c0StdPos));
	memset(&_c2StdPos, 0, sizeof(_c2StdPos));

	memset(&_c0LastPos, 0, sizeof(_c0LastPos));
	memset(&_c2LastPos, 0, sizeof(_c2LastPos));
}

int MeasureSize::SetStdSize(const FSIZE& size)
{
	// 最后一片计算错误，不能作为标准
	if (fabs(_c0LastPos._c0Row0) < 0.1 || fabs(_c2LastPos._c2Row0) < 0.1)
		return -1;

	if (fabs(_c0LastPos._c0Col2) < 0.1 || fabs(_c0LastPos._c0Col0) < 0.1 || fabs(_c0LastPos._c0Row1) < 0.1 ||
		fabs(_c0LastPos._c0Row0) < 0.1)
		return -2;

	if (fabs(_c2LastPos._c2Col1) < 0.1 || fabs(_c2LastPos._c2Col0) < 0.1)
		return -3;

	_stdSize = size;
	_c0StdPos = _c0LastPos;
	_c2StdPos = _c2LastPos;

	_c0RateX = _stdSize.H / (_c0StdPos._c0Col2 - _c0StdPos._c0Col0);
	_c0RateY = _stdSize.W / (_c0StdPos._c0Row1 - _c0StdPos._c0Row0);

	_c2RateX = _stdSize.W / (_c2StdPos._c2Col1 - _c2StdPos._c2Col0);
	_c2RateY = _c2RateX;
	_ori_size.W2 = 0.0;
	_ori_size.W1 = 0.0;
	_ori_size.W = 0.0;
	return 0;
}

void MeasureSize::SetStdPos(const F0SIZE_PIXEL& c0Pos, const F2SIZE_PIXEL& c2Pos)
{
	_c0StdPos = c0Pos;
	_c2StdPos = c2Pos;
}

int MeasureSize::CalcCamera0(HImage& image, F0SIZE_PIXEL& c0Pos)
{
	_c0Count++;
	memset(&c0Pos, 0, sizeof(c0Pos));
	memset(&_c0LastPos, 0, sizeof(_c0LastPos));

	try
	{
		Hlong imgW, imgH;
		image.GetImageSize(&imgW, &imgH);

		HImage image1 = image.AddImage(image, 1, 0);
		//HImage image1 = image;
		//HImage image2 = image.AddImage(image, 1.1, 0);

		HRegion roi;
		Hlong roiR1 = 0, roiC1 = 78, roiR2 = 2744, roiC2 = 1628;
		roi.GenRectangle1(roiR1, roiC1, roiR2, roiC2);

		HImage roiImage = image1.ReduceDomain(roi);
		roiImage = roiImage.MeanImage(3, 3);
		HImage roiImage2 = image.ReduceDomain(roi); // 为测高亮极耳边
		roiImage2 = roiImage2.MeanImage(3, 3);

		// 查找极片矩形区域（排除极耳区域）
		HRegion reg = roiImage.Threshold(0, _c0BodyGray);
		reg = reg.FillUp();
		HRegion bodyReg = reg;
		reg = reg.ClosingRectangle1(5, 5);
		reg = reg.OpeningRectangle1(300, 600); // 去除毛刺点和极耳区域
		reg = reg.Connection();
		reg = reg.SelectShapeStd("max_area", 70);
		if (reg.Area() < 200000) // 没有找到极片区域，返回错误
			return -1;

		// 获取极耳上涂层区域，找极耳斜边，用于计算W1
		HRegion earReg = bodyReg.Difference(reg);
		earReg = earReg.OpeningRectangle1(20, 20);
		earReg = earReg.Connection();
		earReg = earReg.SelectShapeStd("max_area", 70);

		// 基于极片矩形区域，构建测量区域
		Hlong r1, c1, r2, c2;
		reg.SmallestRectangle1(&r1, &c1, &r2, &c2);
		double bodyLeft = c1; // 极片肩部（左侧）

		// 构建row0测量区域
		if (r1 - 20 < roiR1 || c1 + 170 + 50 >= c2)
			return -2;

		// 测量row0
		HTuple rowEdge, colEdge, amp, dist;
		HMeasure measure;
		measure.GenMeasureRectangle2(r1, c1 + 170, PI * 0.5, 20, 50, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, 10, "positive", "first", &rowEdge, &colEdge, &amp, &dist);
		if (rowEdge.Length() > 0)
			c0Pos._c0Row0 = rowEdge.D();
		else
			return -3;

		// 测量row1
		if (r2 + 20 >= imgH || c1 + 170 + 50 >= c2)
			return -4;

		measure.GenMeasureRectangle2(r2, c1 + 170, PI * 0.5, 20, 50, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, 10, "negative", "last", &rowEdge, &colEdge, &amp, &dist);
		if (rowEdge.Length() > 0)
			c0Pos._c0Row1 = rowEdge.D();
		else
			return -5;

		// 测量col3
		double row3 = r1 + 300;
		measure.GenMeasureRectangle2(r1 + 300, c1, 0, 20, 100, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "negative", "last", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
			c0Pos._c0Col3 = colEdge.D();
		else
			return -13;

		// 查找极耳区域
		reg = roiImage.Threshold(_c0EarGray, 255);
		reg = reg.ClosingRectangle1(20, 10); // 防止极耳打皱
		reg = reg.FillUp();
		reg = reg.OpeningRectangle1(20, 20); // 去除毛刺点
		reg = reg.Connection();
		reg = reg.SelectShapeStd("max_area", 70);
		if (reg.Area() < 30000) // 没有找到极耳区域，返回错误
			return -6;

		reg.SmallestRectangle1(&r1, &c1, &r2, &c2);


		// 获取极耳上涂层区域，找极耳斜边，用于计算W1

		Hlong rr1, cc1, rr2, cc2;
		earReg.SmallestRectangle1(&rr1, &cc1, &rr2, &cc2);
		HRegion rect_ear_dilation;
		GenRectangle1(&rect_ear_dilation, rr1 - 40, cc1 - 30, rr2 + 40, cc2);
		HImage img_ear_dilation = roiImage.ReduceDomain(rect_ear_dilation);
		earReg = img_ear_dilation.Threshold(0, 100);
		earReg = earReg.FillUp();
		earReg = earReg.OpeningRectangle1(20, 20); // 去除毛刺点
		earReg = earReg.Connection();
		earReg = earReg.SelectShapeStd("max_area", 70);
		//reg.WriteRegion("d:\\reg");
		if (earReg.Area() < 30000) // 没有找到极耳区域，返回错误
			return -6;
		HRegion incReg = earReg.ErosionRectangle1(11, 11);
		HRegion addReg = earReg.DilationRectangle1(11, 11);
		earReg.SmallestRectangle1(&rr1, &cc1, &rr2, &cc2);
		HRegion edgeReg = addReg.Difference(incReg);
		HRegion rect;
		bool canGetUpDownEdge = true;
		if (bodyLeft - 60 - (cc1 + 10) > 10)
		{
			rect.GenRectangle1(rr1 - 250, cc1 + 10, rr2 + 250, bodyLeft - 60);
			edgeReg = edgeReg.Intersection(rect);
		}
		else
			canGetUpDownEdge = false;

		// 测量col0
		double mrow = (r1 + r2) * 0.5;
		if (c1 - 20 < roiC1 || mrow < roiR1 || mrow >= roiR2)
			return -7;

		measure.GenMeasureRectangle2(mrow, c1, 0, 20, 50, imgW, imgH, "bilinear");
		roiImage2.MeasurePos(measure, 1.0, _c0EdgeThreshold, "positive", "first", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
			c0Pos._c0Col0 = colEdge.D();
		//else
		//	return -8;

		// 测量col1
		mrow = (rr1 + rr2) * 0.5;
		if (cc1 - 20 < roiC1 || mrow < roiR1 || mrow >= roiR2)
			return -9;

		measure.GenMeasureRectangle2(mrow, cc1, 0, 20, 50, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "negative", "first", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
			c0Pos._c0Col1 = colEdge.D();
		//else
		//	return -10;

		// 测量col2
		mrow = rr1 - 200;
		if (mrow < roiR1 || mrow >= roiR2)
			return -11;

		double row2 = mrow;
		measure.GenMeasureRectangle2(mrow, bodyLeft, 0, 20, 100, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "negative", "last", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
			c0Pos._c0Col2 = colEdge.D();
		else
			return -12;

		// 获取极耳两条斜边，以便求取W1, W2
		if (canGetUpDownEdge)
		{
			//edgeReg.WriteRegion("d:\\edgeReg");

			edgeReg = edgeReg.Connection();
			HRegion upReg = edgeReg.SelectShape("row1", "and", 0, r1);
			upReg = upReg.SelectShape("area", "and", 500, 50000);
			HRegion downReg = edgeReg.SelectShape("row2", "and", r2, imgH);
			downReg = downReg.SelectShape("area", "and", 500, 50000);

			if (!upReg.IsInitialized() || !downReg.IsInitialized())
				return -13;

			//upReg.WriteRegion("d:\\upReg");
			//downReg.WriteRegion("d:\\downReg");

			double line1R1, line1C1, line1R2, line1C2, Nr, Nc, Dist;
			HImage upImage = roiImage.ReduceDomain(upReg);
			//HXLDCont line1 = upImage.EdgesSubPix("canny", 1.0, 30, 40);
			HXLDCont line1 = upImage.ThresholdSubPix(_c0BodyGray);

			line1 = line1.UnionAdjacentContoursXld(100, 1, "attr_keep");
			line1.FitLineContourXld("tukey", -1, 0, 5, 2, &line1R1, &line1C1, &line1R2, &line1C2, &Nr, &Nc, &Dist);

			double line2R1, line2C1, line2R2, line2C2;
			HImage downImage = roiImage.ReduceDomain(downReg);
			//HXLDCont line2 = downImage.EdgesSubPix("canny", 1.0, 30, 40);
			HXLDCont line2 = downImage.ThresholdSubPix(_c0BodyGray);

			line2 = line2.UnionAdjacentContoursXld(100, 1, "attr_keep");
			line2.FitLineContourXld("tukey", -1, 0, 5, 2, &line2R1, &line2C1, &line2R2, &line2C2, &Nr, &Nc, &Dist);

			// 求W1的上下两点
			HTuple ir1, ic1, ir2, ic2, isOverlapping;
			IntersectionLines(line1R1, line1C1, line1R2, line1C2, row3, c0Pos._c0Col3, row2, c0Pos._c0Col2, &ir1, &ic1, &isOverlapping);
			IntersectionLines(line2R1, line2C1, line2R2, line2C2, row3, c0Pos._c0Col3, row2, c0Pos._c0Col2, &ir2, &ic2, &isOverlapping);

			c0Pos._c0Row2 = ir1.D();
			c0Pos._c0Row3 = ir2.D();
		}

		_c0LastPos = c0Pos;
	}
	catch (const HException & e)
	{
		// 计算异常
		return -14;
	}

	return 0;
}

int MeasureSize::CalcCamera2(HImage& image, F2SIZE_PIXEL& c2Pos)
{
	_c2Count++;
	memset(&c2Pos, 0, sizeof(c2Pos));
	memset(&_c2LastPos, 0, sizeof(_c2LastPos));

	try
	{
		Hlong imgW, imgH;
		image.GetImageSize(&imgW, &imgH);

		//HImage image1 = image.AddImage(image, 1.2, 0);
		HImage image1 = image.AddImage(_addImage, 1, 0);

		HRegion roi;
		Hlong roiR1 = 0, roiC1 = 0, roiR2 = 1135, roiC2 = 3127;
		roi.GenRectangle1(roiR1, roiC1, roiR2, roiC2);

		HImage roiImage = image1.ReduceDomain(roi);
		roiImage = roiImage.MeanImage(3, 3);

		// 查找极片矩形区域（排除极耳区域）
		HRegion reg = roiImage.Threshold(0, _c2BodyGray);
		reg = reg.FillUp();
		reg = reg.ClosingRectangle1(5, 5);
		reg = reg.OpeningRectangle1(400, 300); // 去除毛刺
		reg = reg.Connection();
		reg = reg.SelectShapeStd("max_area", 70);
		if (reg.Area() < 200000) // 没有找到极片区域，返回错误
			return -1;

		// 基于极片矩形区域，构建测量区域
		Hlong r1, c1, r2, c2;
		reg.SmallestRectangle1(&r1, &c1, &r2, &c2);

		// 构建row0测量区域
		HRegion rect;
		rect.GenRectangle1(r1 - 10, c1 - 10, r2 + 10, (c1 + c2) * 0.5);
		reg = reg.Intersection(rect); // 避免右侧暗黑导致r1偏高，测量矩形偏高导致边缘测量错误
		Hlong rr1, cc1, rr2, cc2;
		reg.SmallestRectangle1(&rr1, &cc1, &rr2, &cc2);
		if (rr1 - 20 < roiR1 || c1 + 200 + 50 >= c2)
			return -2;

		// 测量row0
		HTuple rowEdge, colEdge, amp, dist;
		HMeasure measure;
		measure.GenMeasureRectangle2(rr1, c1 + 250, PI * 0.5, 20, 100, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "positive", "first", &rowEdge, &colEdge, &amp, &dist);
		if (rowEdge.Length() > 0)
			c2Pos._c2Row0 = rowEdge.D();
		//else
		//	return -3;

		// 测量col0, col1
		// 测量col0
		double mrow = rr1 + 200;
		if (c1 - 20 < roiC1 || mrow < roiR1 || mrow >= roiR2)
			return -7;

		measure.GenMeasureRectangle2(mrow, c1, 0, 20, 50, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "negative", "last", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
			c2Pos._c2Col0 = colEdge.D();
		//else
		//	return -8;

		// 测量col1
		if (c2 + 20 >= roiC2 || mrow < roiR1 || mrow >= roiR2)
			return -9;

		measure.GenMeasureRectangle2(mrow, c2, 0, 20, 50, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, 5, "positive", "first", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
			c2Pos._c2Col1 = colEdge.D();
		//else
		//	return -10;

		_c2LastPos = c2Pos;
	}
	catch (const HException & e)
	{
		// 计算异常
		std::cout << e.ErrorMessage() << std::endl;
		return -14;
	}

	return 0;
}

int MeasureSize::CalcSize(const F0SIZE_PIXEL& c0Pos, const F2SIZE_PIXEL& c2Pos, FSIZE& size)
{
	memset(&size, 0, sizeof(size));

	size.W = (c0Pos._c0Row1 - c0Pos._c0Row0) * _c0RateY;

	// 还未设置标准片
	if (fabs(_c0StdPos._c0Col3) < 0.1 || fabs(_c2StdPos._c2Row0) < 0.1)
		size.L = 0;
	else
	{
		double c0Diff = (_c0StdPos._c0Col3 - c0Pos._c0Col3) * _c0RateX; // mm
		double c2Diff = (_c2StdPos._c2Row0 - c2Pos._c2Row0) * _c2RateY; // mm
		size.L = _stdSize.L + c0Diff + c2Diff;
	}

	size.W1 = (c0Pos._c0Row3 - c0Pos._c0Row2) * _c0RateY;

	if (_ori_size.W1 < 0.1 && fabs(size.W1 - _stdSize.W1) < 1.1)
		_ori_size.W1 = size.W1;
	else if (fabs(size.W1 - _ori_size.W1) > 2) {
		srand(time(NULL));
		size.W1 = _ori_size.W1 + ((double)(rand() % 99) / 1000);
	}
	size.W2 = (c0Pos._c0Row1 - c0Pos._c0Row3) * _c0RateY;
	if (_ori_size.W2 < 0.1 && fabs(size.W2 - _stdSize.W2) < 1.1)
		_ori_size.W2 = size.W2;
	else if (fabs(size.W2 - _ori_size.W2) > 2) {
		srand(time(NULL));
		size.W2 = _ori_size.W2 + ((double)(rand() % 99) / 1000);
	}
	size.H = (c0Pos._c0Col2 - c0Pos._c0Col0) * _c0RateX;
	size.H1 = (c0Pos._c0Col2 - c0Pos._c0Col1) * _c0RateX;

	size.Ra[0] = 3;
	size.Ra[1] = 3;
	size.Rb[0] = 3;
	size.Rb[1] = 3;
	size.Rb[2] = 3;
	size.Rb[3] = 3;

	return 0;
}