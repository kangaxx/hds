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
using namespace boost::property_tree;

namespace commonfunction_c {
	class JsonHelper {
	public:
		JsonHelper(string json) {
			initial(json);
		}

		JsonHelper() {
			// do nothing
		}

		void initial(string json) {
			stringstream  jsonstream(json);
			json_parser::read_json<ptree>(jsonstream, _pt);
			_isInitialed = true;
		}

		void initialByFile(string file) {
			read_json(file, _pt);
			_isInitialed = true;
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

		std::vector<std::string>* findAll(std::string tree, std::string key, ptree const& pt, std::vector<std::string>* out = NULL, int type = 0) {
			std::vector<std::string>* result = NULL;
			if (out != NULL)
				result = out;
			else
				result = new std::vector<std::string>();
			for (auto& child : pt) {
				if (child.first._Equal(tree)) {
					findAll(key, child.second, result);
					if (type == 0)
						continue;
					else
						return result;
				}
				findAll(tree, key, child.second, result);
			}

			return result;
		}
	};
}
#endif //JSONHELPER_H
