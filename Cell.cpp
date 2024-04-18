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

// Cell::Cell(Cell&& rhv) 
//     :val(rhv.val)
// {
//     rhv.val = "";
// }
Cell::Cell(Cell&& rhv) 
    :val(std::move(rhv.val))
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
    :val(val)
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

// const Cell& Cell::operator=(Cell&& rhv){
//     if (this != &rhv) {
//         val = rhv.val;
//         rhv.val = "";
//     }
//     return *this;
// }
const Cell& Cell::operator=(Cell&& rhv){
    if (this != &rhv) {
        val = std::move(rhv.val);
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
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("Conversion to int failed: " + val);
    }
}

Cell::operator double(){
    try {
        return std::stod(val);
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("Conversion to double failed: " + val);
    }
}

Cell::operator char(){
    return val[0];
}

Cell::operator bool(){
    return !val.empty();
}

Cell::operator std::string(){
    return val;
}

Cell::operator std::vector<int>(){
    std::vector<int> result;
    std::string temp;
    temp.reserve(val.size());
    for (char ch : val) {
        if (ch == ',' || ch == ' ') {
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
    const std::string& val = ob.getVal();
    bool isString = false;
    bool isCharArray = false;
    bool isArray = false;

    for (char ch : val) {
        if (std::isspace(ch)) {
            isArray = true;
        } else if (ch == '"' && val.back() == '"') {
            isString = true;
        } else if (ch == '\'' && val.size() == 3 && val[1] != '\'') {
            isCharArray = true;
        } else {
            isArray = true;
        }
    }

    if (isString) {
        out << '"' << val << '"';
    } else if (isCharArray) {
        out << '\'' << val[1] << '\'';
    } else if (isArray) {
        out << "{" << val << "}";
    } else {
        out << val;
    }

    return out;
}