#pragma once
// CopyRight 2021 Tizer inc
// License (BSD/GPL)
// Author : Gu xinxin
// halcon 算法相关类，编译此功能时需要引用halcon相关头文件及动态链接库

#include <iostream>

#include "HalconCpp.h"
#include "HDevThread.h"

using HalconCpp::HTuple;

namespace halconfunction_c {
	class HalconPosition {
	public:
		HalconPosition() {
			column_ = -1.0;
			row_ = -1.0;
		}
		HalconPosition(HTuple column, HTuple row) : column_(column), row_(row) {}

		HalconPosition(const float* x, const float* y){
			column_ = *x;
			row_ = *y;
		}

		void set_x(const float* x) { column_ = *x;}
		void set_y(const float* y) { row_ = *y; }
		void set_column(const HTuple* column) { column_ = *column; }
		void set_row(const HTuple* row) { row_ = *row; }
		float get_x() { return column_; }
		float get_y() { return row_; }
		HTuple get_column() { return column_; }
		HTuple get_row() { return row_; }
	private:
		HTuple column_, row_;
	};
}