#pragma once
#include<fstream>
#include<string>
#include<iostream>
#include<stdexcept>
#include<utility>
#include<format>
#include<vector>
//Contains shared functions


std::pair<std::string, std::string> parseParam(std::string line, char delim);
void trim(std::string& line);