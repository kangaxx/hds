#ifndef SERIALIZATION_C11_H
#define SERIALIZATION_C11_H

#ifndef INT_SERIALIZABLE_OBJECT_SIZE
#define INT_SERIALIZABLE_OBJECT_SIZE 512
#define INT_SERIALIZABLE_BURRINFO_OBJECT_SIZE INT_SERIALIZABLE_OBJECT_SIZE
#define TYPE_BURRS_IMAGE_TAICHI_NORMAL 10
#define TYPE_BURRS_IMAGE_TAICHI_BURR 11
#define TYPE_BURRS_IMAGE_ERROR_NO_IMAGE 99
#define TYPE_BURRS_IMAGE_EDGE_NORMAL 20
#define TYPE_BURRS_IMAGE_EDGE_BURR 21

#define INT_FILE_NAME_SIZE 40
#define INT_FILE_TIME_SIZE 16
#endif //INT_SERIALIZABLE_OBJECT_SIZE

#define INT_PAINTTER_SERIAL_POINT_SIZE 10
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;
///////////////////////////////////////////////////////////////////////////////////
//    C++ serializable object and Serialize tool:                                //
//      for send message by socket or zmq easy and quickly                       //
//  20210414 最近时间比较紧，类型检查等功能先放放 gxx                            //
//     数据类应该是SerializationOjbect的子类而不是直接使用serializationObject    //
///////////////////////////////////////////////////////////////////////////////////
namespace serialization_c11 {

	class SerializationOjbect {
	public:
		string getVersion() { return string(version); }
		void setVersion(char* v) { strcpy_s(version, v); }
		int getMemberNum() { return memberNum; }
		void setMemberNum(int num) { memberNum = num; }
	private:
		char version[10];
		int memberNum;
	};



	class SerializationFactory{
	public:
		static void Serialize(const SerializationOjbect* o, char** result) {  
			memcpy(*result, (char*)o, INT_SERIALIZABLE_OBJECT_SIZE);
			int i = 1;
		}
		static SerializationOjbect* Deserialize(char* d) { return (SerializationOjbect*)d; }
		void initial(SerializationOjbect* o) { memcpy((char*)data[0], (char*)o, INT_SERIALIZABLE_OBJECT_SIZE); initialed = true; }
		char* getChars() { return (char*)data; }
	private:
		bool initialed = false;
		char data[INT_SERIALIZABLE_OBJECT_SIZE];
	};

	class BurrsPainter :public SerializationOjbect {
	public:
		float getPointerX(int index) { return pointX[index]; }
		void setPointerX(int index, float value) { pointX[index] = value; }

		float getPointerY(int index) { return pointY[index]; }
		void setPointerY(int index, float value) { pointY[index] = value; }

		float getDistance(int index) { return distance[index]; }
		void setDistance(int index, float value) { distance[index] = value; }

		int getMaxBurrX() { return maxBurrX; }
		void setMaxBurrX(int value) { maxBurrX = value; }

		int getMaxBurrY() { return maxBurrY; }
		void setMaxBurrY(int value) { maxBurrY = value; }

		string getFileName() { return string(fileName); }
		void setFileName(const char* v) { strncpy_s(fileName, v, INT_FILE_NAME_SIZE); }

		string getFileTime() { return string(fileTime); }
		void setFileTime(const char* v) { strncpy_s(fileTime, v, INT_FILE_TIME_SIZE); }

		int getBurrsNum() { return burrsNum; }
		void setBurrsNum(int num) { burrsNum = num; }

		int getGrabImageWidth() { return grabImageWidth; }
		void setGrabImageWidth(int w) { grabImageWidth = w; }

		int getGrabImageHeight() { return grabImageHeight; }
		void setGrabImageHeight(int h) { grabImageHeight = h; }

		int getSaveImageWidth() { return saveImageWidth; }
		void setSaveImageWidth(int w) { saveImageWidth = w; }

		int getSaveImageHeight() { return saveImageHeight; }
		void setSaveImageHeight(int h) { saveImageHeight = h; }

		int getType() { return type; }
		void setType(int t) { type = t; }
		
		//添加毛刺并排序,如果队列以满，则只接受最大的若干各毛刺，新添加的毛刺可能无法正确添加进去（或者可能挤掉原先队列里最小的毛刺)
		void insertBurrSorted(float x, float y, float value) {
			//逐个比较列表内的毛刺，如果新增毛刺大于比较毛刺，则新增毛刺替代原有毛刺，原有毛刺替代其后续毛刺
			for (int i = 0; i < INT_PAINTTER_SERIAL_POINT_SIZE; ++i) {
				if (value > distance[i]) {
					float tmp = value;
					value = distance[i];
					distance[i] = tmp;
					tmp = x;
					x = pointX[i];
					pointX[i] = tmp;
					tmp = y;
					y = pointY[i];
					pointY[i] = tmp;
				}
			}
			if (getBurrsNum() < INT_PAINTTER_SERIAL_POINT_SIZE)
				setBurrsNum(getBurrsNum() + 1);
		}
	private:
		float pointX[INT_PAINTTER_SERIAL_POINT_SIZE];
		float pointY[INT_PAINTTER_SERIAL_POINT_SIZE];
		float distance[INT_PAINTTER_SERIAL_POINT_SIZE];

		int maxBurrX;
		int maxBurrY;
		char fileName[INT_FILE_NAME_SIZE];
		char fileTime[INT_FILE_TIME_SIZE];
		int burrsNum = 0;
		//原图尺寸
		int grabImageWidth; 
		int grabImageHeight;
		//保存尺寸
		int saveImageWidth;
		int saveImageHeight;
		int type; //背光图：正常 10， 毛刺 11 ， 纵向图: 正常 20，毛刺 21
	};
}

#endif //SERIALIZATION_C11_H