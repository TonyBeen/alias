/*************************************************************************
    > File Name: convert.h
    > Author: hsz
    > Brief: 字符串编码转换
    > Created Time: Thu 28 Jul 2022 02:54:12 PM CST
 ************************************************************************/

#ifndef __EULAR_CODE_CONVERT_H__
#define __EULAR_CODE_CONVERT_H__

#include <stdint.h>
#include <string>
#include <iconv.h>

namespace eular {

bool GBK2UTF8(std::string &out, const std::string &in);
bool UTF82GBK(std::string &out, const std::string &in);

} // namespace eular

#endif
