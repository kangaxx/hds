//create by gxx
#pragma once
#include <limits.h>
#define INT_GOOD_NUM_MAX  20
#define INT_FAIL_NUM_MAX  40
#define INT_SUSPEND_NUM_MAX 60
#define INT_OFFLINE_NUM INT_MAX
#define FLOAT_PI 3.1415926

#define INT_PORT_BROADCASTER_START 5000
#define INT_PORT_STARTER_BROADCASTER 6000
#define INT_PORT_BROADCASTER_WORKER_1 6100
#define INT_PORT_BROADCASTER_WORKER_2 6101
#define INT_PORT_WORKER_BROADCASTER 6100
#define INT_PORT_BROADCASTER_WORKER_SHUTDOWN 6200
#define INT_PORT_START_BROADCASTER_SHUTDOWN 6201
#define INT_PORT_BROADCASTER_WORKER_DOREDUCE 2010
#define INT_PORT_UI_STARTER_JOBSIG 6300

#define INT_CONNECT_RETRY_TIME 1
#define INT_CONFIRM_RETRY_TIME 3
#define INT_REDUCE_TYPE_LEN 1
#define INT_WORKER_COUNT_FOREVER 2
#define INT_TOTAL_SHOT_NUM 2
#define INT_NETMSG_LENGTH 2048
#define INT_WORKDONE_MSG_LENGTH 256
#define INT_NETMSG_SIGNAL_LENGTH 256

#define INT_UI_TO_CONCURR_CMD_TYPE_JOB_START 1
#define INT_UI_TO_CONCURR_CMD_TYPE_JOB_PAUSE 2
#define INT_UI_TO_CONCURR_CMD_TYPE_JOB_STOP 3

#define CHARS_JOB_STATUS_WAIT "wait"
#define CHARS_JOB_STATUS_SENDED "sended"
#define CHARS_JOB_NAME_REDUCE "reduce"
#define INT_JOB_TYPE_MAPSTACK 1
#define INT_JOB_TYPE_REDUCE 2

#define CHARS_NETMSG_NORUN "NO-RUN"
#define INT_START_CALCULATOR_FALSE  -1
#define INT_START_CALCULATOR_EXISTS  1
#define INT_START_CALCULATOR_SUCCESS 0
#define INT_SLEEP_FOR_READ 2
#define INT_SLEEP_FOR_WORK 5
#define INT_THREAD_PARA_LENGTH 6
#define INT_JOBINFO_KEYTYPE_RANDOM 2
#define INT_JOBINFO_KEYTYPE_REGULAR_2D 1
#define INT_JOBINFO_KEYTYPE_REGULAR_1D 0

#define INT_MAX_CALNODE_COUNT 1000
#define INT_NETWORK_RETRYTIME 5
#define CHARS_STRING_SPLIT_FLAG "|"
#define CHARS_REDUCEFILE_XML_TAG "ReduceFileName"
#define CHARS_SHOTFILE_XML_TAG "ShotFileName"
#define CHARS_VELFILE_XML_TAG "VelFileName"
#define CHARS_PROJECT_NAME_XML_TAG "ProjectName"
#define CHARS_REDUCE_LEFT_XML_TAG "MigApeLeft"
#define CHARS_REDUCE_RIGHT_XML_TAG "MigApeRight"

#define CHARS_INPUT_FILE_1_XML_TAG "InputFileName1"
#define CHARS_INPUT_FILE_2_XML_TAG "InputFileName2"
#define CHARS_INPUT_FILE_3_XML_TAG "InputFileName3"
#define CHARS_INPUT_FILE_4_XML_TAG "InputFileName4"

#define CHARS_DEST_FILE_1_XML_TAG "OutputFileName1"
#define CHARS_DEST_FILE_2_XML_TAG "OutputFileName2"
#define CHARS_DEST_FILE_3_XML_TAG "OutputFileName3"
#define CHARS_DEST_FILE_4_XML_TAG "OutputFileName4"

#define CHARS_REDUCE_TYPE "reduce_type"
#define CHARS_REDUCE_CMD_NAME CHARS_JOB_NAME_REDUCE
#define CHARS_TEMP_FILE_4_XML_TAG "IsTemp"

#define CHARS_REDUCE_STATUS_FILE_NAME "reduce_status_file.tmp"
#define CHARS_WORKER_STATUS_FILE_NAME "worker_status_file.tmp"
#define CHARS_BIN_LOG_DEMO_FILE_NAME "./bin_log_demo.log"

#define CHARS_PROCARGV_CLOSE_SHM "-c" //use this for nodemanager -c , to close shm pipe
#define CHARS_PROCARGV_NOREBO "-nore" //use this for nodemanager ,if do shm reboot

#define CHARS_NETMSG_CMD_USER_FLAG " -u"
#define CHARS_NETMSG_CMD_CALCUL_FLAG " -p"
#define CHARS_CALL_PROC_TYPE_ROOT '0'
#define CHARS_CALL_PROC_TYPE_UESR '1'
#define Int_Len6 999999

#define INT_LIBSSH2_CUSTOMER_PARA_PORT 22
#define INT_LIBSSH2_CUSTOMER_PARA_INIT 0
#define INT_LIBSSH2_MSG_SIZE 4000
#define CHARS_LIBSSH2_CMD_NODEMANAGER_START "source ~/.bashrc;source ~/.bash_profile;nodemanager 1>>node_log 2>>node_err &"

#define INT_LIBSSH2_CMD_SMPORT_BEGIN 96
#define DEFAULT_INT_SOCKETWAIT 10
#define CHARS_CMD_FILE_PATH "$QM_ROOT/bin"
#define CHARS_CMD_CLL_START "calculator"

#define CHARS_FILENAME_JOBSERVER_SERVICE "Service_js.ini"
#define CHARS_FILENAME_DATABASE_CLIENTLIST "../config/clientlist.xml"
#define CHARS_FILENAME_NODEMANAGER_SERVICE "/../config/Service_nm.ini"
#define CHARS_FILENAME_DATABASE_ARITHLIST "/../config/arithlist.xml"

#define INT_MAX_FILELIST_NUM 4

#define INT_WORKPATH_LENGTH 1024
#define MAX_REDUCE_PROC_COUNT 1
#define INT_CALCULATOR_JOBID_NAN -1

#define DEFAULT_INT_IDENT 1
#define DEFAULT_INT_MEMBERLEN 6
#define DEFAULT_INT_TOTALLEN (DEFAULT_INT_MEMBERLEN*4+3)
#define DEFAULT_INT_SLEEPTIME 1
#define DEFAULT_INT_SOCK_TIMEOUT_SEC 0
#define DEFAULT_INT_SOCK_TIMEOUT_MSEC 500
#define DEFAULT_LINUX_COMMAND_LENGTH 256

#define INT_SHMLOG_SYS_BLOCK_INITIAL_COUNT 40
#define INT_SHMLOG_RQT_BLOCK_INITIAL_COUNT (INT_SHMLOG_SYS_BLOCK_INITIAL_COUNT / 4)
#define INT_SHMLOG_SYS_BLOCK_INC_COUNT 20
#define INT_SHMLOG_RQT_BLOCK_INC_COUNT (INT_SHMLOG_SYS_BLOCK_INC_COUNT / 4)
#define INT_SHMLOG_RQTLOG_MEMBER_ID_RANGE 10
#define INT_SHMLOG_RQT_BLOCK_SHM_KEY 98
#define INT_SHMLOG_SYS_BLOCK_SHM_KEY 99
#define MIN_SHMLOG_SHAREMEMORY_SIZE 128
#define FLOAT_SHMLOG_RQT_FREE_ARGU 0.5
#define FLOAT_SHMLOG_SYS_FREE_ARGU 0.5
#define INT_SHMLOG_FREE_BLOCK_LOOP_NUM_FOR_INSERT 3
#define DEFAULT_SHMLOG_RQT_BLOCKNUM_EACHPID 1
#define DEFAULT_SHMLOG_SYS_BLOCKNUM_EACHPID 4


#define INT_SQL_LOGFILE_TYPE_TABLEABLE_LOG 0
#define INT_SQL_LOGFILE_TYPE_JOB_SCHDULE 1
#define INT_SQL_LOGFILE_FILENAME_LENGTH 512
#define CHAR_SQL_FILEMAGIC_FIELDDEF_ELEMENT_SPLITER ","
#define CHAR_SQL_FILEMAGIC_FIELDDEF_FIELD_SPLITER ";"
#define INT_SQL_FILEMAGIC_FIELDDEFINE_Size 512
#define INT_SQL_FILEMAGIC_FIELDDEFINE_ELEMENT_NUM 4
#define CHAR_SQL_IDXFILE_FILENAME ".sidx"
#define MAX_INT_SQL_LOGFILE_TABLENAME_LENGTH 16
#define MAX_INT_SQL_LOGFILE_FIELDNAME_LENGTH 16
#define MAX_INT_BINLOG_FCN_LOCK_TRY_COUNT 4
//commonfunction_c
#define MAX_INIFILE_LINE_LENGTH 64
#define CHAR_INIFILE_PARAMETER_SPLIT_FLAG '='
#define DEFAULT_SHAREMEME_SIZE 65536
#define ERROR_SYSSHM_NORMAL -1
#define DEFAULT_INTCHAR_SIZE 9
#define DEFAULT_DATETIME_LEN 20
#define DEFAULT_MM_DD_HHMMSS_LEN 2
#define DEFAULT_DATETIME_FMT "%Y-%m-%d %I:%M:%S"
#define DEFAULT_LINUX_PATH_LENGTH 1024
#define CHARS_QMODEL_SYSENV_ROOT "QM_ROOT"
#define CHARS_QMODEL_LINUX_PATH_PARENT_FLAG "/.."
#define CHARS_QMODEL_CONFIG_PATH_NAME "/config/"
#define CHARS_QMODEL_LOG_PATH_NAME "/log/"
#define CHARS_QMODEL_ICON_PATH_NAME "/icon/"
#define INT_DAYNUM_ONEYEAR 366
#define INT_TYPE_LICENSE_NOLIMIT 0
#define INT_TYPE_LICENSE_BYDATE 1
#define DEFAULT_LONGCHAR_SIZE 64
#define DEFAULT_INTCHAR_BASE 10

//commonfunction_c::Logger
#define LOG_LEVEL_STD_OUT 1
#define LOG_LEVEL_STD_WRITE_FILE 2
#define LOG_LEVEL_STD_DATABASE 4
//GeoImageSo
#define LIMIT_SCALE_DATA_IN_MAP 1.0f //设定放大极限
#define MIN_SCALE_GRID_DISTANCE 50 //标尺刻度最小间距，太密了根本看不清
#define INT_SCALE_TYPE_VERT 1
#define INT_SCALE_TYPE_HERI 2
#define INT_SCALE_GRID_NUM_IDENTITY 5
#define INT_SCALE_GRID_SIZE_NORMAL 4  //普通刻度线的尺寸
#define INT_SCALE_PRIKEY_LINE_SIZE 16  //道集刻度线的尺寸
#define INT_SCALE_GRIDLINE_WIDTH_NORMAL 3     //普通刻度线的宽度，1的确有点窄
#define INT_DATAIMAGE_GRIDLINE_WIDTH_NORMAL 1 //数据图像上的线段宽度
#define INT_SCALE_TEXT_FONT_SIZE_NORMAL 2     //普通刻度字线条的宽度
#define INT_SCALE_TEXT_OVER_LINE 6 //字在刻度线上方（或下方的距离）
#define INT_SCALE_TEXT_CHAR_WIDTH 4 //一般情况下每个字符的像素宽度
#define INT_SCALE_TEXT_CHAR_HEIGHT 14 //一般情况下每个字符的像素高度，其实定的略高
#define INT_SCALE_PRIKEY_LINE_BEG_POS_OFFSET_X 5 //道集编号打印偏移X量
#define INT_SCALE_PRIKEY_LINE_BEG_POS_OFFSET_Y -26 //道集编号打印偏移Y量
#define INT_SCALE_PRIKEY_LABEL_BEG_POS_OFFSET_X 60 //道集编号打印首字名称标签偏移X量
#define INT_SCALE_PRIKEY_LABEL_BEG_POS_OFFSET_Y 30 //道集编号打印首字名称标签偏移Y量
#define INT_SCALE_VTEXT_HEIGHT ((14)+(INT_SCALE_TEXT_OVER_LINE)) //纵向刻度线上字的高度
#define MAX_COMPARE_DATA_NUM 1  //做多允许几个数据同时显示
#define INT_CUT_DIRECTION_LEFT 1 //绘制在cut位置左侧
#define INT_CUT_DIRECTION_RIGHT 2 //绘制在cut位置右侧
#define INT_CUT_DIRECTION_NONE 0 //无cut
#define INT_GEO_IMAGE_POINT_DISTANCE 16 //判定坐标点的响应范围（距离的平方）
#define INT_GEO_IMAGEBTN_RECT_WIDTH 32
#define INT_GEO_IMAGEBTN_RECT_HEIGHT 32 //图片控件的框体 宽 和 高
#define INT_GEO_IMAGEBTN_PICS_NUM 8 //自定义按钮图片数量
#define DEFAULT_GEO_IMAGEBTN_SHOW_ANIMA_DURATION_BASE 200  //动画显示基本时间
#define DEFAULT_GEO_IMAGEBTN_SHOW_ANIMA_DURATION_ADJUST 50  //动画调整间隔
#define INT_GEO_IMAGEBTN_RADUIS 40 //按钮布置半径,注意必须用float值

#define INT_TITLE_RECT_HEIGHT 30//图像抬头区域高度

#define INT_SCALERLAYOUT_MARGIN_RIGHT 80
#define INT_SCALERLAYOUT_MARGIN_LEFT  80
#define INT_SCALERLAYOUT_MARGIN_TOP 50
#define INT_SCALERLAYOUT_MARGIN_BOTTOM 20
#define INT_HEADER_HEIGHT 120 //道头数据图像高度
#define INT_HEADER_DATA_RECT_MARGIN 10 //道头与数据图像区域间的空隙距离

#define MIN_DATARECT_SIZE 40
#define INT_COLORRECT_HEIGHT 60
#define INT_COLORMAPWIDGET_CHANGE_NUM 29
#define INT_COLORBLOCK_HEIGHT 20
#define DEFAULT_COLORMAP_MAXVALUE 30

#define INT_GEO_IMAGEBTN_FUNC_NEWLEVEL 0
#define INT_GEO_IMAGEBTN_FUNC_CLEARPTS 1
#define INT_GEO_IMAGEBTN_FUNC_SELECTALL_PICKLEVEL 2

#define INT_COLORGROUP_COLORNUM 5

#define INT_GEO_IMAGEHEADER_FIELD_TYPE_NONE 0
#define INT_GEO_IMAGEHEADER_FIELD_TYPE_SHORT 1
#define INT_GEO_IMAGEHEADER_FIELD_TYPE_INT 2
#define INT_GEO_IMAGEHEADER_FIELD_TYPE_FLOAT 3

#define INT_GEO_IMAGEHEADER_RECT_TOPBOTTOM_MARGIN 4
#define INT_GEO_SCALELINE_SCALEGRID_DISTANCE_CONST 0.2f //固定格式刻度尺（道头区域刻度尺）间隔
#define INT_GEO_SCALELINE_TEXT_WIDTH 80

#define INT_GEO_COLORRECT_MARGIN_TOP 20
#define INT_GEO_COLORRECT_MARGIN_LEFT 20
#define INT_GEO_EXAMPLE_TEXT_SIZE 2 //示意色线条长度
#define INT_GEO_MOUSE_DRAG_MOVE_NUM 5 //超过5个像素的鼠标移动，就被认为是拖动了，反之则只是点击时的抖动
#define INT_GEO_MOUSE_NEARLY_ONE_LEVEL 10 //接近10个像素就可以认为是点在面上了，
#define INT_GEO_MOUSE_LEFT_CLICK 1
#define INT_GEO_MOUSE_MID_CLICK 2
#define INT_GEO_MOUSE_RIGHT_CLICK 3
#define INT_GEO_KEY_CTRL 4
#define INT_GEO_KEY_SHIFT 5

//HALCON edge_burrs
#define INT_BURRS_DIRECTION_HORIZ 0
#define INT_BURRS_DIRECTION_VERT 1
#define INT_WRITE_IMAGE_WIDTH 1296
#define INT_WRITE_IMAGE_HEIGHT 972
#define FLOAT_GRAB_IMAGE_WIDTH 2592.0
#define FLOAT_GRAB_IMAGE_HEIGHT 1944.0
#define FLOAT_FORM_IMAGE_WIDTH 300.0
#define FLOAT_FORM_IMAGE_HEIGHT 400.0


#define INT_FIXED2WAYLIST_DEFAULT_SIZE 1024
#define INT_FIXED2WAYLIST_CURRENT_IDX_ERROR -1

#define INT_HALCON_IMAGE_SIZE 4000*2000 //800万像素
#define INT_HALCON_BURR_RESULT_SIZE INT_HALCON_IMAGE_SIZE + 2048
#define FLOAT_BURR_LEVEL_LOW 25.0 //小于25um是小毛刺
#define FLOAT_BURR_LEVEL_MID 50.0 //25 ~ 50um 是中型毛刺
#define FLOAT_BURR_LEVEL_HIGH 75.0 //50 ~ 75um 是大型毛刺
#define FLOAT_BURR_LEVEL_UPH 100.0 //75 ~ 100um 是超大型毛刺，超过100um是巨型毛刺
#define TYPE_INFLECTION_MAX 1 //最大值波峰
#define TYPE_INFLECTION_MIN 2 //最小值波峰

//卷绕项目
#define GRAB_STATUS_NONE 0
#define GRAB_STATUS_FAILED 8
#define GRAB_STATUS_SUCCESSED 1
#define EAR_LOCATION_WAIT -2
#define EAR_LOCATION_GRAB_FAILED -1
#define EAR_LOCATION_CORRECT 0
#define EAR_LOCATION_ERROR 1
#define CONCAT_IMAGE_NONE 99
#define CONCAT_IMAGE_SUCCESS 0
#define CONCAT_IMAGE_FAIL 2
#define ROLLING_RESULT_NG false
#define ROLLING_RESULT_OK true
#define REDIS_WRITE_KEY "to_zj"
#define REDIS_READ_KEY "to_gxx"
#define REDIS_COMMAND_NUMBER "CMD_NUM"
#define REDIS_WORK_STATUS_KEY "WorkStatus"
#define REDIS_WORK_STATUS_START "Start"
#define REDIS_WORK_STATUS_STOP "-1"
#define REDIS_IS_WORK
#define REDIS_LIST_CALIBRATION_KEY "Calibration"
#define JSON_CALIBRATION_TOP_KEY "CalibrationTop"
#define JSON_CAMERA_PARAM_WIDTH "Camera_width"
#define JSON_CAMERA_PARAM_HEIGHT "Camera_height"
#define JSON_CAMERA_PARAM_PACKAGE_SIZE "Package_size"
#define JSON_CAMERA_PARAM_PACKAGE_DELAY "Package_delay"
#define JSON_CAMERA_PARAM_CENTER "Center"
#define JSON_CAMERA_PARAM_EXPOSURE_TIME "Exposure_time"
#define JSON_CALIBRATION_BOTTOM_KEY "CalibrationBottom"
#define WINDING_CALIBRATION_POINTS_FILENAME "calibration_points.log"
#define WINDING_CALIBRATION_INFO_FILENAME "calibration_info.log"
#define WINDING_CALIBRATION_PATH "d:\\calibration"
#define WINDING_PICTURE_CUT_PATH "d:\\picture_cut"
#define DEFAULT_LOG_PATH_WIN "d:"
#define MAX_LENGTH_TXT_LINE 1024
#define CALIBRATION_LINE_BAR_COUNT 88 * 2
#define MAX_BATTERY_WIDTH 4000
#define IMAGE_GRABED_PATH "d:\\grabs"
#define IMAGE_GRABED_FOLDER "grabs"
#define ROI_LEFT_START_LINE_NUM 8
#define ROI_LEFT_END_LINE_NUM 20
#define ROI_RIGHT_START_LINE_NUM -30
#define ROI_RIGHT_END_LINE_NUM -8
#define LEFT_EAR_TO_EDGE_MIN 4
#define LEFT_EAR_TO_EDGE_MAX 11
#define RIGHT_EAR_TO_EDGE_MIN 4
#define RIGHT_EAR_TO_EDGE_MAX 12
#define ELECTRIC_RELAY_COM_NUM 3
#define STANDARD_CAMERA_MODE 0 //标准拍摄模式
#define STANDARD_CAMERA_MODE_CHAR "0" //标准拍摄模式字符串
#define MSA_NO_TRIGGER_CAMERA_MODE 1 //msa 连续拍摄模式
#define EAR_EDGE_AREA_WIDTH 15 //极耳边缘检测宽度
#define LOAD_CALI_CONFIG_STRING_LENGTH 30
#define CALIB_MODE_STATIC_VALUE 0 //使用固定标定系数
#define CALIB_MODE_FROM_FILE 1 //读取标定系数文件（数据来源halcon标定助手）
#define XINYU_IMAGE_MEASURE_RESULT_SUCCESS 0//
#define XINYU_IMAGE_MEASURE_RESULT_FAIL 1//
#define XINYU_VIRTUAL_VALUES "{\"adjust_values\":[\"62.5\",\"113.2\",\"31.4\",\"13.9\",\"12.8\",\"6.1\",\"3.1\",\"3.0\",\"2.8\",\"2.9\"]}"
#define THREAD_SAFE_LOGGER_PATH_CHAR_LENGTH 200
#if defined(__linux__) || defined(__linux)
#  define __IS_LINUX__
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define __IS_WINDOW__
#  define __IS_WIN__
#endif

enum CameraPosition {
	CP_LEFT = 1,
	CP_LEFT_TOP = 2,
	CP_LEFT_MID = 3,
	CP_LEFT_BOTTOM = 4,
	CP_MID = 5,
	CP_MID_TOP = 6,
	CP_MID_MID= 7,
	CP_MID_BOTTOM = 8,
	CP_RIGHT = 9,
	CP_RIGHT_TOP = 10,
	CP_RIGHT_MID = 11,
	CP_RIGHT_BOTTOM = 12
};

enum LOADER_MODE {
	LM_CONSOLE_MODE = 0,
	LM_WINFORM_MODE = 1,
	LM_SERVICE_MODE = 2
};

#define MSEC_LENGTH 100000
static int g_log_msec_type = 0;
static int g_msec = GetTickCount() % MSEC_LENGTH;
inline void update_current_msec(int type) {
	g_log_msec_type = type;
}
inline int abs_used_time(int used_time) {
	if (used_time < 0) return MSEC_LENGTH + used_time;
	else return used_time;
}
#define MAX_VIRTUAL_CAMERA_COUNT 8
//	stdSize.W = fmt_value[0];
//                                      W         L      H       H1      W1      W2 
const double cathode_std_size[12] = { 62.534, 113.221, 31.436, 12.807, 13.884, 6.092, 3, 3, 3, 3, 3, 3 };
const double anode_std_size[12] = { 64.648, 115.3, 25.125, 6.192, 13.852, 7.242, 3, 3, 3, 3, 3, 3 };
#define WINDING_NG_RESULT_KEY "IsNg"
#define WINDING_NG_RESULT_ISNG "0"

#define CMD_NUM_FOR_LIB_LOADER_PLC_DEBUG 99
#define CMD_NUM_FOR_LIB_LOADER_HEART_BEAT 98
#define CMD_NUM_FOR_LIB_LOADER_CAMERA_NO_DELEGATE 1 //相机无回调模式（软硬触发皆可）
#define CMD_NUM_FOR_LIB_LOADER_MANUAL_PROCESSING 6//手动触发
#define CMD_NUM_FOR_LIB_LOADER_HSL_DEBUG 9
#define MAX_CAMERA_COUNT 10 //一次成像操作中允许的最多相机熟悉