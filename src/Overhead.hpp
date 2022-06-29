#ifndef OVERHEAD_H
#define OVERHEAD_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <random> 
#include <chrono>
#include <deque>
#include <map>
#include <unordered_map>
#include <list>
#include <set>
#include <climits>
#include <iterator>
#include <regex>
#include <experimental/filesystem>
#include <cmath>
#include <cassert>

std::pair<size_t, size_t> convertToCellCoordinate(const std::string &str);
std::string convertToSheetCoordinate(size_t x);

/// Thrown when trying to evaluate math expression with string
class StringEval : public std::exception {
public:
    explicit StringEval(const std::string& message);
    const char* what() const noexcept override {
        return message_.c_str();
    }
private:
    std::string message_;
};



#endif