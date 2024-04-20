#include "Cell.h"

std::ostream& operator<<(std::ostream& out, const std::vector<int>& ob){
    out << "{";
    for (size_t i = 0; i < ob.size(); ++i) {
        out << ob[i];
        if (i < ob.size() - 1) {
            out << ", ";
        }
    }
    out << "}";
    return out;
}

Cell::Cell() 
    :val("")
{}

Cell::Cell(const Cell& rhv) 
    :val(rhv.val)
{}

Cell::Cell(Cell&& rhv) 
    :val(rhv.val)
{
    rhv.val = "";
}

Cell::Cell(int val) 
    :val(std::to_string(val))
{}

Cell::Cell(double val) 
    :val(std::to_string(val))
{}

Cell::Cell(char val) 
    :val(std::string(1, val))
{}

Cell::Cell(bool val) 
    :val(val ? "true" : "false") 
{}

Cell::Cell(std::string val) 
    :val(std::move(val))
{}


Cell::Cell(const std::vector<int>& val) {
    std::string result = "{";
    for (size_t i = 0; i < val.size(); ++i) {
        result += std::to_string(val[i]);
        if (i < val.size() - 1) {
            result += ", ";
        }
    }
    result += "}";
    this->val = result;
} 

const Cell& Cell::operator=(const Cell& rhv){
    if (this != &rhv) {
        val = rhv.val;
    }
    return *this;
}

const Cell& Cell::operator=(Cell&& rhv){
    if (this != &rhv) {
        val = rhv.val;
        rhv.val = "";
    }
    return *this;
}

const Cell& Cell::operator=(int rhv){
    val = std::to_string(rhv);
    return *this;
}

const Cell& Cell::operator=(double rhv){
    val = std::to_string(rhv);
    return *this;
}

const Cell& Cell::operator=(char rhv){
    val = rhv;
    return *this;
}

const Cell& Cell::operator=(bool rhv){
    val = rhv ? "true" : "false";
    return *this;
}

const Cell& Cell::operator=(std::string rhv){
    val = rhv;
    return *this;
}

const Cell& Cell::operator=(const std::vector<int>& rhv){
    std::string result = "{";
    for (size_t i = 0; i < rhv.size(); ++i) {
        result += std::to_string(rhv[i]);
        if (i < rhv.size() - 1) {
            result += ", ";
        }
    }
    result += "}";
    val = result;
    return *this;
}

Cell::operator int(){
    try {
        return std::stoi(val);
    } catch (...) {
        return 0;
    }
}

Cell::operator double(){
    try {
        return std::stod(val);
    } catch (...) {
        return 0.0;
    }
}

Cell::operator char(){
    if (!val.empty()) {
        return val[0];
    } else {
        return '\0';
    }
}

Cell::operator bool(){
    return !val.empty();
}

Cell::operator std::string(){
    return val;
}

Cell::operator std::vector<int>() {
    std::vector<int> result;
    std::string temp;
    temp.reserve(val.size());
    for (char ch : val) {
        if (ch == ',' || std::isspace(ch)) { 
            if (!temp.empty()) {
                result.push_back(std::stoi(temp));
                temp.clear();
            }
        } else {
            temp.push_back(ch);
        }
    }
    if (!temp.empty()) {
        result.push_back(std::stoi(temp));
    }
    return result;
}

bool operator==(const Cell& lhv, const Cell& rhv) {
    return lhv.getVal() == rhv.getVal();
}

bool operator!=(const Cell& lhv, const Cell& rhv) {
    return !(lhv == rhv);
}

std::string Cell::getVal() const {
    return val;
}

std::istream& operator>>(std::istream& in, Cell& ob) {
    std::string input;
    in >> input;
    ob = Cell(input);
    return in;
}

std::ostream& operator<<(std::ostream& out, const Cell& ob) {
    out << ob.getVal();
    return out;
}