#ifndef JSONHELPER_H
#define JSONHELPER_H
//20210616 gxx Taize corp.
/*****************************************************************************************
json string Serialize/Desserialize , json node find/append/remove
base on <boost/property_tree/json_parser.hpp>
*****************************************************************************************/
#include <iostream>

#include "common.h"
#include "commonfunction_c.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace boost::property_tree;

namespace commonfunction_c {
	enum TRAVERSE_TYPE {
		TRAVERSE_TYPE_ALL = 1,
		TRAVERSE_TYPE_FIRST = 2,
		TRAVERSE_TYPE_LAST = 3
	};

	class JsonHelper {
	public:
		JsonHelper(string json) {
			initial(json);
		}

		JsonHelper() {
			// do nothing
		}

		void initial(string json) {
			try {
				stringstream  jsonstream(json);
				json_parser::read_json<ptree>(jsonstream, _pt);
				_isInitialed = true;
			}
			catch (...) {
				_isInitialed = false;
			}
		}

		void initialByFile(string file) {
			try {
				read_json(file, _pt);
				_isInitialed = true;
			}
			catch (...) {
				_isInitialed = false;
			}
		}

		void initial_new_root(string key, string value) {
			try {
				char* json;
				int json_length = key.length() + value.length() + 20;
				json = new char[json_length];
				sprintf_s(json, json_length, "{\"%s\":\"%s\"}", key.c_str(), value.c_str());
				stringstream  jsonstream(json);
				json_parser::read_json<ptree>(jsonstream, _pt);
				_isInitialed = true;
			}
			catch (...) {
				_isInitialed = false;
			}
		}

		string search(string key) {
			string result;
			if (!_isInitialed)
				return "";
			std::vector<std::string>* values = findAll(key, _pt);
			int size = values->size();

			if (size <= 0)
				result = "";
			else
				result = (*values)[0];
			delete values;
			return result;
		}

		int search_all(string key, std::vector<std::string>& result) {
			if (!_isInitialed)
				return 0;
			std::vector<std::string>* values = findAll(key, _pt);
			int size = values->size();
			if (size <= 0)
				return 0;
			for (int i = 0; i < size; ++i) 
				result.push_back((*values)[i]);
			return size;
		}

		string search(string tree, string key) {
			string result;
			if (!_isInitialed)
				return "";
			std::vector<std::string>* values = findAll(tree, key, _pt);
			int size = values->size();

			if (size <= 0)
				result = "";
			else
				result = (*values)[0];
			delete values;
			return result;
		}

		template<class T>
		int read_array(std::string key, std::vector<T>& out) {
			int result = 0;
			boost::property_tree::ptree position_array = _pt.get_child(key);
			boost::property_tree::ptree::iterator pos = position_array.begin();
			for (; pos != position_array.end(); ++pos)
			{
				T value = pos->second.get_value<T>();
				out.push_back(value);
				++result;
			}
			return result;
		}

		void append_child(string key, string value, const char* parent_key = NULL) {
			if (NULL == parent_key) {
				//上级菜单key是NULL表示child直接插在第一层上
				_pt.put(key.c_str(), value.c_str());
			}
		}

		void append_array(string key, vector<ptree> values, const char* parent_key = NULL) {
			if (NULL == parent_key) {
				ptree children;
				for (int i = 0; i < values.size(); ++i)
					children.push_back(std::make_pair("",values.at(i)));
				_pt.add_child(key, children);
			}
		}

		void append_array(string key, ptree values[], int count, const char* parent_key = NULL) {
			if (NULL == parent_key) {
				ptree children;
				for (int i = 0; i < count; ++i)
					children.push_back(std::make_pair("", values[i]));
				_pt.add_child(key, children);
			}
		}

		string get_json_string(const char* key = NULL) {
			std::stringstream ss;
			if (NULL == key) {
				//返回json的根数据
				write_json(ss, _pt);
				return ss.str();
			}
			else {
				return "";
			}

		}
	private:
		wstring _json_w_string;
		string _json_string;
		ptree _pt;
		bool _isInitialed = false;

		/* just for show how to use boost json gxx */
		void list_all_key(ptree const& pt) {
			//ptree child;
			for (auto& child : pt) {
				std::cout << "key : " << child.first << " value : " << child.second.data() << std::endl;
				list_all_key(child.second);
			}
		}
		/**/

		//get all json value by key
		std::vector<std::string>* findAll(std::string key, ptree const& pt, std::vector<std::string>* out = NULL) {
			std::vector<std::string>* result = NULL;
			if (out != NULL)
				result = out;
			else
				result = new std::vector<std::string>();
			for (auto& child : pt) {
				if (child.first._Equal(key)) {
					result->push_back(child.second.data());
					continue;
				}
				findAll(key, child.second, result);
			}
			return result;
		}

		std::string findFirst(std::string key, ptree const& pt) {
			std::vector<std::string> values;
			findAll(key, pt, &values);
			int size = values.size();
			if (size < 1) return "";
			else
				return values[0];
		}

		std::vector<std::string>* findAll(std::string tree, std::string key, ptree const& pt, std::vector<std::string>* out = NULL, TRAVERSE_TYPE type = TRAVERSE_TYPE_ALL) {
			std::vector<std::string>* result = NULL;
			if (out != NULL)
				result = out;
			else
				result = new std::vector<std::string>();
			for (auto& child : pt) {
				if (child.first._Equal(tree)) {
					findAll(key, child.second, result);
					if (type == TRAVERSE_TYPE_FIRST)
						return result;
					else
						continue;
				}
				findAll(tree, key, child.second, result);
			}
			if (type == TRAVERSE_TYPE_ALL)
				return result;
			else if (type == TRAVERSE_TYPE_LAST) {

			}
		}

		std::string findFirst(std::string tree, std::string key, ptree const& pt, std::vector<std::string>* out = NULL, TRAVERSE_TYPE type = TRAVERSE_TYPE_ALL) {
			std::vector<std::string> values;
			findAll(tree, key, pt, &values);
			int size = values.size();
			if (size < 1) return "";
			else
				return values[0];
		}
	};
}
#endif //JSONHELPER_H 
