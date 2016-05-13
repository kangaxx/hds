/*
 *Copyright@2013 上海青凤致远地球物理地质勘探科技有限公司
 *All Right Reserved
 *
 *简    介：绘图功能基本功能函数类。
 *作    者：GXX
 *完成日期：2016.3.28

 *功能类：GeoImageBaseFunction        基本功能类，本类函数全是静态函数具体说明详见函数说明
 *      baseGraphUnit               基本图像计量单位
 */

#ifndef GEOIMAGEBASEFUNCTION_H
#define GEOIMAGEBASEFUNCTION_H

#include <assert.h>
#include <vector>
#include <QList>
#include <QDebug>

#include "common.h"
#include "geoimage_2d.h"
#include "math.h"

using namespace std;
using namespace geoimage_2d;
//业务功能类，专门处理图形图像GUI用户体验相关的事务，像素，颜色，线条之类，不过不处理IO数据，暂时放在这里规模扩大后考虑转移到commonfunction_qt里
class GeoImageBaseFunction
{
public:
    //将GUI界面上的基本单元：像素与 IO数据中的基本单元综合起来，用以综合平衡数据的实质与展现，画面精确与用户习惯之间的冲突的要求。
    //根据已知条件计算当前基本单元情况,最后两个参数是道间距和采样点间距,及刻度线数量（scaleNum）
    static baseGraphUnit makeBaseUnit(const int &totalLength, const int &dataNum, const eScaleType &type,
                                      int &scaleNum, float dtrace=0.0f, float dns=0.0f);
    //同上，不过这个版本可以调节最小间隔，最小0个像素
    static baseGraphUnit makeBaseUnit(const int &totalLength, const int &dataNum, const eScaleType &type,
                                      int &scaleNum, int minDist, float dtrace=0.0f, float dns=0.0f);
    //动态垂直刻度尺可读端点坐标获取函数。根据基准点,坐标尺类型，间隔距离，算出每个刻度尺的两个端点（P1，P2关系不必确定，调用者如需要自己算算）；
    static void calculatorVScaleLinePoint(const QPointF &ptOri, const eLabelPosition &lp, int lineSize, float offsetY, int MapWidth,
                                          QPoint &p1, QPoint &p2);
    //水平刻度尺可读端点坐标获取函数。
    static void calculatorHScaleLinePoint(const QPointF &ptOri, const eLabelPosition &lp, float distance, float offset,
                                          int disNum, int lineSize, QPoint &p1, QPoint &p2);
    //动态垂直刻度尺上的刻度字位置换算函数
    static void calculatorVScaleTextPoint(const QPoint &p1, const QPoint &p2, const eLabelPosition &lp, QString text, QPoint &pText);
    //水平刻度尺上的刻度字位置换算函数
    static void calculatorHScaleTextPoint(const QPointF &ptOri, const eLabelPosition &lp, int distance , float offset,
                                          int disNum, int lineSize, QPoint &p, QString text);

    //固定垂直刻度尺刻度点获取函数，与之前的动态刻度尺的两端需要计算取得不同，固定刻度尺的起始端点是人为设定好的输入参数，本函数只是根据输入的两端坐标及每个刻度间隔占总高度的百分比，计算出每个刻度点的坐标（包含起始端点）
    //scaleBothEnds表示返回刻度数内是否包含了两个端点
    static vector<QPointF> calculatorConstVScalePoint(const QPointF &begin,const QPointF &end, float scalePrecent, bool scaleBothEnds = false);
    static vector<float> calculatorConstScaleValue(float min, float max, float scalePrecent, bool scaleBothEnds = false); //结合刻度坐标使用，这个是算出刻度线旁边的读数来，
    //没错，使用的时候外侧包一个函数将两者合并起来会更好哦
    //在两个float数间取出若干刻度数，要求数的小数位数尽量小，整数位数尽量整齐（非0位尽量少），间隔数的非0位只有1位，结果数的非0位是lg(resultNum*间隔数)
    static vector<float> getBestScaleNum(float min, float max, int resultNum = 5);  //resultNum是预设的返回数量由于存在进位问题所以未必一定是5个返回值，也可能是4个或6个

    //计算纵横某个方向的放大倍数
    static float getZoomValue(const int &oriSize,const int &newSize,const float &oriZoomValue){
        if (oriSize == 0)
            throw QString("ERROR:Original map size is 0!");
        return fabs(float(newSize)/float(oriSize)) * oriZoomValue;
    }

    //根据当前放大倍数，原始取样总数，坐标值（x或y），总长度，算出取样点下标偏移量。 f(off) = totalNum * zoom * (X坐标 or Y坐标/rectSize)
    //例如，放大倍数为1，总取样点数100个，在长度200的图像轴上的坐标50处取样，取到的数据下标偏移量就是24（从0开始）
    static int getCurrentIdxOffset(const float &zoom, const int &totalNum, const int &rectSize, const int &XorY);

    //计算垂直网格线坐标
    static void calculatorVGridLinePoint(const QPointF &ptOri, const float &distance, const float &offset, const int &height, int disNum, QPointF &p1, QPointF &p2);
    //计算水平网格线坐标
    static void calculatorHGridLinePoint(const QPointF &ptOri, const int &offsetY, const int &width,  QPointF &p1, QPointF &p2);
    //根据数据值，极值，坐标轴线的x值及振幅极值，计算出某个点在波形图上的振幅坐标，phase：界面设置，似乎是正负数据对换用的
    static float getPointByValue(const float &data, const float &dataLimit, const int &base, const float &offset, const int &phase);

    //波形图step计算，在预设值的基础上优化抽稀，确保每道数据宽度至少一个像素,如果原始设置就能保证的话，直接返回原始step
    static int getGoodStep(int width, int step, int horizNum);



    //将zoom坐标转换成常规坐标 ，in是输入坐标值，zoom是缩放比例，直接来自图像控件的缩放变量，dataIdx,当前最左侧的数据编号，
    //    dataNum，一屏显示多少数据， screen_len屏幕宽度
    static int zoomd2org(int in, float zoom, int dataIdx, int dataNum, int screen_len);
    static float zoomd2orgF(int in, float zoom, int dataIdx, int dataNum, int screen_len);
    //将原始坐标按照参数转为缩放时的坐标 ， 小于 0 或者大于screen len表示不在当前屏幕内
    static int org2zoomd(int in, float zoom, int dataIdx, int dataNum, int screen_len);
    static float org2zoomdF(int in, float zoom, int dataIdx, int dataNum, int screen_len);

    static QPointF org2zoomd(QPointF in, float zoom_x, float zoom_y, int horizIdx, int vertIdx,
                             int horizNum, int vertNum, int width, int height);

    static QPointF zoomd2org(QPointF in, float zoom_x, float zoom_y, int horizIdx, int vertIdx,
                             int horizNum, int vertNum, int width, int height);
    //测试用，生成指定个随机浮点数
    static int getRandomFloat(float *out, int num);

    //将测井的坐标参数转换成QPoint轴线坐标
    static float getLoggingValue(float in, int zoomedDataNum, int beginIdx, int length);
    static QPointF getLoggingPoint(float logX, float logY, int zoomedHorizNum, int zoomedVertNum,
                                  int horizBeginIdx, int vertBeginIdx, int width, int height);
    static QPointF getLoggingWavePoint(QPointF p1, QPointF p2, float len); //根据偏移距离len，中轴线上的点p1, p2 计算垂直于波形振幅点的位置
    static float getWaveLen(float max, float min , float value, float dist); //根据最大最小值，当前值，振幅极限，计算当前振幅
    static float getDistanceToLevel(QPointF p, vector<QPointF> level);
    //判断一个点是否落在区域内
    static bool isPointInRect(QPoint pt, QRect rect);
    //判断一条线段是否和区域有交点
    static bool isLineInRect(QLine line, QRect rect);
    //坐标变幻函数，当界面整体放大，放大比率不变时，需要重新计算坐标点的新坐标
    static float getPointInNewMap(int oriMax, int oriMin, int newMax, int newMin, float oriValue);

    //在指定point为中心区域绘制图形
    //static bool drawCustomPic(QPointF middle, QPainter *painter, int type, QPen pen, int size);
private:
    //获取最高有效位
    static int getHeadNum(float t);

    /*--------------------------------------------------------------------------------------------------
     *
     * 子函数
     *
     * 功能：
     *     求解二维介质中点到折线的距离。
     *     已知点的坐标，和组成折线的多个点的坐标。返回距离。
     *
     * 运行环境：
     *
     * 作者：李辉
     * 单位：上海青凤致远地球物理地质勘探科技有限公司
     * 日期：2016年4月14日
     *
     * 作者单位保留所有权利
     *
     * 参数说明：
     *   nps:			输入。组成折线的点的个数。
     *   xp:			输入。已知的点的x坐标。
     *   zp:			输入。已知的点的z坐标。
     *   xs:			输入。大小为nps的数组，组成折线的点的x坐标。
     *   zs:			输入。大小为nps的数组，组成折线的点的z坐标。
     *
     *--------------------------------------------------------------------------------------------------*/
    static float distance_point_to_surface_2D(int nps, float xp, float zp, float *xs, float	*zs);

};
#endif // GEOIMAGEBASEFUNCTION_H
