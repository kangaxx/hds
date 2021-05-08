#ifndef SERIALIZATION_C11_H
#define SERIALIZATION_C11_H

#ifndef INT_SERIALIZABLE_OBJECT_SIZE
#define INT_SERIALIZABLE_OBJECT_SIZE 512
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
		void setFileName(const char* v) { strcpy_s(fileName, v); }

		string getFileTime() { return string(fileTime); }
		void setFileTime(const char* v) { strcpy_s(fileTime, v); }

		int getBurrsNum() { return burrsNum; }
		void setBurrsNum(int num) { burrsNum = num; }

		int getGrabImageWidth() { return grabImageWidth; }
		void setGrabImageWidth(int w) { grabImageWidth = w; }

		int getGrabImageHeight() { return grabImageHeight; }
		void setGrabImageHeight(int w) { grabImageHeight = w; }

		int getSaveImageWidth() { return saveImageWidth; }
		void setSaveImageWidth(int w) { saveImageWidth = w; }

		int getSaveImageHeight() { return saveImageHeight; }
		void setSaveImageHeight(int w) { saveImageHeight = w; }
	private:
		float pointX[INT_PAINTTER_SERIAL_POINT_SIZE];
		float pointY[INT_PAINTTER_SERIAL_POINT_SIZE];
		float distance[INT_PAINTTER_SERIAL_POINT_SIZE];

		int maxBurrX;
		int maxBurrY;
		char fileName[40];
		char fileTime[16];
		int burrsNum;
		//原图尺寸
		int grabImageWidth; 
		int grabImageHeight;
		//保存尺寸
		int saveImageWidth;
		int saveImageHeight;

	};
}

#endif //SERIALIZATION_C11_H