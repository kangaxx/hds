#include "MeasureSize.h"

#define PI 3.1415926535897932384626433832795

MeasureSize::MeasureSize() : _c0BodyGray(80), _c0EarGray(250), _c0EdgeThreshold(20), _c2BodyGray(75), _scale(0.2)
{
	Reset();

	//HImage image;
	//image.ReadImage("D:\\TestImage\\XinYu\\6\\Image_20220904090313873.bmp");
	//HRegion rect;
	//rect.GenRectangle1(532.343, 892.415, 2559.74, 2248.45);
	//HImage image1 =  image.ReduceDomain(rect);
	//_c0Model = image1.CreateNccModel(9, -0.18, 0.18, 0.01, "use_polarity");
	//
	//image.ReadImage("D:\\TestImage\\XinYu\\6\\Image_20220904090443739.bmp");

	//HTuple Row, Column, Angle, Score;
	//image.FindNccModel(_c0Model, -0.18, 0.18, 0.7, 1, 0.5, "true", 9, &Row, &Column, &Angle, &Score);
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

	if (fabs(_c0LastPos._c0Col2 - _c0LastPos._c0Col0) < 0.1 || fabs(_c0LastPos._c0Row1 - _c0LastPos._c0Row0) < 0.1)
		return -2;

	if (fabs(_c2LastPos._c2Col1 - _c2LastPos._c2Col0) < 0.1)
		return -3;

	_stdSize = size;
	_c0StdPos = _c0LastPos;
	_c2StdPos = _c2LastPos;

	_c0RateX = _stdSize.H / (_c0StdPos._c0Col2 - _c0StdPos._c0Col0);
	_c0RateY = _stdSize.W / (_c0StdPos._c0Row1 - _c0StdPos._c0Row0);

	_c2RateX = _stdSize.W / (_c2StdPos._c2Col1 - _c2StdPos._c2Col0);
	_c2RateY = _c2RateX;

	return 0;
}

void MeasureSize::SetStdPos(const F0SIZE_PIXEL& c0Pos, const F2SIZE_PIXEL& c2Pos)
{
	_c0StdPos = c0Pos;
	_c2StdPos = c2Pos;
}

void MeasureSize::SetMeasurePos()
{
	// 设置相机0的模板和测量点
	HImage image;
	image.ReadImage("D:\\TestImage\\XinYu\\5\\0 (1).jpg");

	// 创建用于匹配的模板
	double r1 = 833.357, c1 = 229.436, r2 = 2774.56, c2 = 1457.45;
	HRegion rect;
	rect.GenRectangle1(r1, c1, r2, c2);
	HImage image1 =  image.ReduceDomain(rect);
	image1 = image1.ZoomImageFactor(_scale, _scale, "bilinear");
	_c0Model = image1.CreateNccModel(9, -0.18, 0.36, 0.01, "use_polarity"); // 允许±10度的旋转

	// 设置各测量点，相对于模板中心
	double cr = (r1 + r2) * 0.5;
	double cc = (c1 + c2) * 0.5;

	// row0测量点
	_c0MPoints[0]._row = 891 - cr;
	_c0MPoints[0]._col = 1360 - cc;

	// row1测量点
	_c0MPoints[1]._row = 2721 - cr;
	_c0MPoints[1]._col = 1360 - cc;

	// col0测量点
	_c0MPoints[2]._row = 2338 - cr;
	_c0MPoints[2]._col = 280 - cc;

	// col1测量点
	_c0MPoints[3]._row = 2338 - cr;
	_c0MPoints[3]._col = 868 - cc;

	// col2测量点
	_c0MPoints[4]._row = 1967 - cr;
	_c0MPoints[4]._col = 1192 - cc;

	// col3测量点
	_c0MPoints[5]._row = 1191 - cr;
	_c0MPoints[5]._col = 1190 - cc;

	// 极耳斜边测量区域的中心点
	_c0MPoints[6]._row = 2146 - cr;
	_c0MPoints[6]._col = 1020 - cc;
	_c0MPoints[7]._row = 2530 - cr;
	_c0MPoints[7]._col = 1020 - cc;

	// 设置相机2的模板和测量点
	image.ReadImage("D:\\TestImage\\XinYu\\5\\2 (6).jpg");

	// 创建用于匹配的模板
	r1 = 433.852; c1 = 1019.41; r2 = 821.347; c2 = 2614.07;
	rect.GenRectangle1(r1, c1, r2, c2);
	image1 = image.ReduceDomain(rect);
	image1 = image1.ZoomImageFactor(_scale, _scale, "bilinear");
	_c2Model = image1.CreateNccModel(9, -0.18, 0.36, 0.01, "use_polarity"); // 允许±10度的旋转

	// 设置各测量点，相对于模板中心
	cr = (r1 + r2) * 0.5;
	cc = (c1 + c2) * 0.5;

	// row0测量点
	_c2MPoints[0]._row = 525 - cr;
	_c2MPoints[0]._col = 1336 - cc;

	// row1测量点
	_c2MPoints[1]._row = 525 - cr;
	_c2MPoints[1]._col = 1336 - cc + 600;

	// col0测量点
	_c2MPoints[2]._row = 683 - cr;
	_c2MPoints[2]._col = 1104 - cc;

	// col1测量点
	_c2MPoints[3]._row = 679 - cr;
	_c2MPoints[3]._col = 2525 - cc;
}

int MeasureSize::CalcMeasurePosC0(HImage& image, MPOINT mp[8], double& a)
{
	a = 0;
	HTuple row, col, angle, score;
	
	image.FindNccModel(_c0Model, -0.18, 0.36, 0.7, 1, 0.5, "true", 9, &row, &col, &angle, &score);
	if (row.Length() < 1 || col.Length() < 1 || angle.Length() < 1)
		return -21;

	// 计算新测量点位
	a = angle.D();
	double s = score.D();
	for (int i = 0; i < 8; i++)
	{
		mp[i] = _c0MPoints[i];
		mp[i].Rotate(-angle.D());

		mp[i]._row += row.D() / _scale;
		mp[i]._col += col.D() / _scale;
	}

	return 0;
}

int MeasureSize::CalcMeasurePosC2(HImage& image, MPOINT mp[4], double& a)
{
	a = 0;
	HTuple row, col, angle, score;

	image.FindNccModel(_c2Model, -0.18, 0.36, 0.7, 1, 0.5, "true", 9, &row, &col, &angle, &score);
	if (row.Length() < 1 || col.Length() < 1 || angle.Length() < 1)
		return -22;

	// 计算新测量点位
	a = angle.D();
	double s = score.D();
	for (int i = 0; i < 4; i++)
	{
		mp[i] = _c2MPoints[i];
		mp[i].Rotate(-angle.D());

		mp[i]._row += row.D() / _scale;
		mp[i]._col += col.D() / _scale;
	}

	return 0;
}

// 找到最长的轮廓
HXLDCont FindMaxLengthContour(const HXLDCont& cont)
{
	int index = 0;
	double maxLength = 0;
	for (int i = 1; i <= cont.CountObj(); i++)
	{
		double len = cont[i].LengthXld().D();
		if (len > maxLength)
		{
			maxLength = len;
			index = i;
		}
	}

	return cont[index];
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

		HImage image1 = image.AddImage(image, 1.5, 0);
		//HImage image2 = image.AddImage(image, 1.1, 0);

		// 定位待测量目标的位置、角度
		MPOINT mp[8];
		double angle;
		HImage zimage = image.ZoomImageFactor(_scale, _scale, "bilinear");
		int ret = CalcMeasurePosC0(zimage, mp, angle);
		if (ret != 0)
			return ret;

		HRegion roi;
		//Hlong roiR1 = 250, roiC1 = 814, roiR2 = 2785, roiC2 = 2293;
		Hlong roiR1 = mp[0]._row - 50, roiC1 = mp[2]._col, roiR2 = mp[1]._row + 50, roiC2 = imgW - 1;
		if (roiR1 < 0)
			roiR1 = 0;
		if (roiR2 > imgH - 1)
			roiR2 = imgH - 1;
		roi.GenRectangle1(roiR1, roiC1, roiR2, roiC2);

		HImage roiImage = image1.ReduceDomain(roi);
		roiImage = roiImage.MeanImage(3, 3);
		HImage roiImage2 = image.ReduceDomain(roi); // 为测高亮极耳边
		roiImage2 = roiImage2.MeanImage(3, 3);

		// 测量row0
		HTuple rowEdge, colEdge, amp, dist;
		HMeasure measure;
		measure.GenMeasureRectangle2(mp[0]._row, mp[0]._col, PI * 0.5 + angle, 20, 50, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "positive", "first", &rowEdge, &colEdge, &amp, &dist);
		if (rowEdge.Length() > 0)
		{
			c0Pos._c0Row0 = rowEdge.D();
			c0Pos._c0C0 = colEdge.D();
		}
		else
			return -3;

		// row1
		measure.GenMeasureRectangle2(mp[1]._row, mp[1]._col, PI * 0.5 + angle, 20, 50, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "negative", "last", &rowEdge, &colEdge, &amp, &dist);
		if (rowEdge.Length() > 0)
		{
			c0Pos._c0Row1 = rowEdge.D();
			c0Pos._c0C1 = colEdge.D();
		}
		else
			return -5;

		// 测量col3
		double row3 = 0;
		measure.GenMeasureRectangle2(mp[5]._row, mp[5]._col, angle, 20, 100, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "negative", "last", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
		{
			c0Pos._c0Col3 = colEdge.D();
			c0Pos._c0R3 = rowEdge.D();
			row3 = rowEdge.D();
		}
		else
			return -13;

		// 测量col0
		measure.GenMeasureRectangle2(mp[2]._row, mp[2]._col, angle, 20, 50, imgW, imgH, "bilinear");
		roiImage2.MeasurePos(measure, 1.0, _c0EdgeThreshold, "positive", "first", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
		{
			c0Pos._c0Col0 = colEdge.D();
			c0Pos._c0R0 = rowEdge.D();
		}
		else
			return -8;

		// 测量col1
		measure.GenMeasureRectangle2(mp[3]._row, mp[3]._col, angle, 40, 30, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "negative", "last", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
		{
			c0Pos._c0Col1 = colEdge.D();
			c0Pos._c0R1 = rowEdge.D();
		}
		else
			return -10;

		// 测量col2
		double row2 = 0;
		measure.GenMeasureRectangle2(mp[4]._row, mp[4]._col, angle, 20, 100, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "negative", "last", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
		{
			c0Pos._c0Col2 = colEdge.D();
			c0Pos._c0R2 = rowEdge.D();
			row2 = rowEdge.D();
		}
		else
			return -12;

		// 获取极耳两条斜边，以便求取W1, W2
		{
			HRegion upReg, downReg;
			upReg.GenRectangle1(mp[6]._row - 40, mp[3]._col + 10, mp[6]._row + 40, mp[4]._col - 50);
			downReg.GenRectangle1(mp[7]._row - 40, mp[3]._col + 10, mp[7]._row + 40, mp[4]._col - 50);

			//downReg.WriteRegion("d:\\downReg");

			double line1R1, line1C1, line1R2, line1C2, Nr, Nc, Dist;
			HImage upImage = roiImage.ReduceDomain(upReg);
			//HXLDCont line1 = upImage.EdgesSubPix("canny", 1.0, 30, 40);
			HXLDCont line1 = upImage.ThresholdSubPix(_c0BodyGray - _c0EdgeThreshold);

			line1 = line1.UnionAdjacentContoursXld(30, 1, "attr_keep");
			line1 = FindMaxLengthContour(line1);
			line1.FitLineContourXld("tukey", -1, 0, 5, 2, &line1R1, &line1C1, &line1R2, &line1C2, &Nr, &Nc, &Dist);

			double line2R1, line2C1, line2R2, line2C2;
			HImage downImage = roiImage.ReduceDomain(downReg);
			//HXLDCont line2 = downImage.EdgesSubPix("canny", 1.0, 30, 40);
			HXLDCont line2 = downImage.ThresholdSubPix(_c0BodyGray - _c0EdgeThreshold);

			line2 = line2.UnionAdjacentContoursXld(30, 1, "attr_keep");
			line2 = FindMaxLengthContour(line2);
			line2.FitLineContourXld("tukey", -1, 0, 5, 2, &line2R1, &line2C1, &line2R2, &line2C2, &Nr, &Nc, &Dist);

			//line1.WriteObject("d:\\line1");
			//line2.WriteObject("d:\\line2");

			// 求W1的上下两点
			HTuple ir1, ic1, ir2, ic2, isOverlapping;
			IntersectionLines(line1R1, line1C1, line1R2, line1C2, row3, c0Pos._c0Col3, row2, c0Pos._c0Col2, &ir1, &ic1, &isOverlapping);
			IntersectionLines(line2R1, line2C1, line2R2, line2C2, row3, c0Pos._c0Col3, row2, c0Pos._c0Col2, &ir2, &ic2, &isOverlapping);

			c0Pos._c0Row2 = ir1.D();
			c0Pos._c0C2 = ic1.D();
			c0Pos._c0Row3 = ir2.D();
			c0Pos._c0C3 = ic2.D();
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
		HImage image1 = image.AddImage(image, 1.5, 0);

		// 定位待测量目标的位置、角度
		MPOINT mp[4];
		double angle;
		HImage zimage = image.ZoomImageFactor(_scale, _scale, "bilinear");
		int ret = CalcMeasurePosC2(zimage, mp, angle);
		if (ret != 0)
			return ret;

		HRegion roi;
		//Hlong roiR1 = 908, roiC1 = 644, roiR2 = 1820, roiC2 = 3995;
		Hlong roiR1 = min(mp[0]._row, mp[1]._row) - 50, roiC1 = mp[2]._col - 50, roiR2 = imgH - 10, roiC2 = mp[3]._col + 50;
		if (roiR1 < 0)
			roiR1 = 0;
		if (roiC1 < 0)
			roiC1 = 0;
		if (roiC2 > imgW - 1)
			roiC2 = imgW - 1;
		roi.GenRectangle1(roiR1, roiC1, roiR2, roiC2);

		HImage roiImage = image1.ReduceDomain(roi);
		roiImage = roiImage.MeanImage(3, 3);

		// 测量row0
		HTuple rowEdge, colEdge, amp, dist;
		HMeasure measure;
		measure.GenMeasureRectangle2(mp[0]._row, mp[0]._col, PI * 0.5 + angle, 20, 100, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, 20, "positive", "first", &rowEdge, &colEdge, &amp, &dist);
		if (rowEdge.Length() > 0)
		{
			c2Pos._c2Row0 = rowEdge.D();
			c2Pos._c2C0 = colEdge.D();
		}
		else
			return -3;

		// 测量col0, col1
		// 测量col0
		measure.GenMeasureRectangle2(mp[2]._row, mp[2]._col, 0, 20, 50, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "negative", "last", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
		{
			c2Pos._c2Col0 = colEdge.D();
			c2Pos._c2R0 = rowEdge.D();
		}
		else
			return -8;

		// 测量col1
		measure.GenMeasureRectangle2(mp[3]._row, mp[3]._col, 0, 20, 50, imgW, imgH, "bilinear");
		roiImage.MeasurePos(measure, 1.0, _c0EdgeThreshold, "positive", "first", &rowEdge, &colEdge, &amp, &dist);
		if (colEdge.Length() > 0)
		{
			c2Pos._c2Col1 = colEdge.D();
			c2Pos._c2R1 = rowEdge.D();
		}
		else
			return -10;

		_c2LastPos = c2Pos;
	}
	catch (const HException & e)
	{
		// 计算异常
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
	size.W2 = (c0Pos._c0Row1 - c0Pos._c0Row3) * _c0RateY;

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