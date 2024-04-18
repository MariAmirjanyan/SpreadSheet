#include "SpreadSheet.h"

SpreadSheet::SpreadSheet() 
    :SpreadSheet(2, 2)
{}

SpreadSheet::SpreadSheet(const SpreadSheet& rhv) 
    :rowcnt(rhv.rowcnt)
    ,colcnt(rhv.colcnt)
    ,board(new Cell*[rhv.rowcnt])
{
    for (size_t i = 0; i < rowcnt; ++i) {
        board[i] = new Cell[rhv.colcnt];
        for (size_t j = 0; j < colcnt; ++j) {
            board[i][j] = rhv.board[i][j]; 
        }
    }
}

SpreadSheet::SpreadSheet(SpreadSheet&& rhv) noexcept
    :board(rhv.board)
    ,rowcnt(rhv.rowcnt)
    ,colcnt(rhv.colcnt)
{
    rhv.board = nullptr;
    rhv.rowcnt = 0;
    rhv.colcnt = 0;
}

SpreadSheet::SpreadSheet(size_t size) 
    :SpreadSheet(size, size)
{}

SpreadSheet::SpreadSheet(size_t row, size_t col){
    if(row < 1 || col < 1){
        throw std::invalid_argument("");
    }
    rowcnt = row;
    colcnt = col;
    board = new Cell*[rowcnt];
    for (size_t i = 0; i < rowcnt; i++){
        board[i] = new Cell[colcnt];
    }
}    

SpreadSheet::~SpreadSheet(){
    clear();
}

const SpreadSheet& SpreadSheet::operator=(const SpreadSheet& rhv){
    if(this != &rhv){
        clear();
        rowcnt = rhv.rowcnt;
        colcnt = rhv.colcnt;
        board = new Cell*[rowcnt];
        for (size_t i = 0; i < rowcnt; ++i) {
            board[i] = new Cell[colcnt];
            for (size_t j = 0; j < colcnt; ++j) {
                board[i][j] = rhv.board[i][j];
            }
        }
    }
    return *this;
}

const SpreadSheet& SpreadSheet::operator=(SpreadSheet&& rhv){
    if(this != &rhv){
        clear();
        board = rhv.board;
        rowcnt = rhv.rowcnt;
        colcnt = rhv.colcnt;
        rhv.board = nullptr;
        rhv.rowcnt = 0;
        rhv.colcnt = 0;
    }
    return *this;
}

SpreadSheet::Column SpreadSheet::operator[](size_t pos){
    return Column(board[pos]);
}

const SpreadSheet::Column SpreadSheet::operator[](size_t pos) const{
    return Column(board[pos]);
}

void SpreadSheet::clear() noexcept{
    if(board != nullptr){
        for(size_t i = 0; i < rowcnt; ++i){
            delete[] board[i];
        }
        delete[] board;
        rowcnt = 0;
        colcnt = 0;
    }
}

size_t SpreadSheet::row() const{
    return rowcnt;
}

size_t SpreadSheet::col() const{
    return colcnt;
}

void SpreadSheet::mirrorH(){
    for (size_t i = 0; i < rowcnt / 2; ++i) {
        for (size_t j = 0; j < colcnt; ++j) {
            std::swap(board[i][j], board[rowcnt - i - 1][j]);
        }
    }
}

void SpreadSheet::mirrorV(){
     for (size_t i = 0; i < rowcnt; ++i) {
        for (size_t j = 0; j < colcnt / 2; ++j) {
            std::swap(board[i][j], board[i][colcnt - j - 1]);
        }
    }
}

void SpreadSheet::mirrorD(){
    for (size_t i = 0; i < rowcnt; ++i) {
        for (size_t j = i + 1; j < colcnt; ++j) {
            std::swap(board[i][j], board[j][i]);
        }
    }
}

void SpreadSheet::mirrorSD(){
    for (size_t i = 0; i < rowcnt; ++i) {
        for (size_t j = 0; j < colcnt - i - 1; ++j) {
            std::swap(board[i][j], board[rowcnt - j - 1][colcnt - i - 1]);
        }
    }
}

void SpreadSheet::rotate(int cnt){
    cnt %= 4; 
    if (cnt == 0){
        return;
    }
    size_t new_row = colcnt;
    size_t new_col = rowcnt;
    SpreadSheet rotated(new_row, new_col);
    for (size_t i = 0; i < rowcnt; ++i) {
        for (size_t j = 0; j < colcnt; ++j) {
            size_t new_i, new_j;
            if (cnt == 1) {
                new_i = j;
                new_j = rowcnt - i - 1;
            } else if (cnt == 2) {
                new_i = rowcnt - i - 1;
                new_j = colcnt - j - 1;
            } else if (cnt == 3) {
                new_i = colcnt - j - 1;
                new_j = i;
            }
            rotated[new_i][new_j] = board[i][j];
        }
    }
    *this = std::move(rotated); 
}

void SpreadSheet::removeRow(size_t row){
    if (row >= rowcnt) {
        throw std::invalid_argument("");
    }
    delete[] board[row];
    for (size_t i = row + 1; i < rowcnt; ++i) {
        board[i - 1] = board[i];
    }
    --rowcnt;
}

void SpreadSheet::removeRows(std::initializer_list<size_t> rows){
    for (size_t row : rows) {
        removeRow(row);
    }
}

void SpreadSheet::removeCol(size_t col){
    if (col >= colcnt) {
        throw std::invalid_argument("");
    }
    for (size_t i = 0; i < rowcnt; ++i) {
        for (size_t j = col + 1; j < colcnt; ++j) {
            board[i][j - 1] = board[i][j];
        }
    }
    --colcnt;
}

void SpreadSheet::removeCols(std::initializer_list<size_t> cols){
    for (size_t col : cols) {
        removeCol(col);
    }
}

void SpreadSheet::resizeRow(size_t r){
    if(r == rowcnt){
        return;
    }
    if(r < 1){
        throw std::invalid_argument("");
    }
    resize(r, colcnt);
}

void SpreadSheet::resizeCol(size_t c){
    if(c == colcnt){
        return;
    }
    if(c < 1){
        throw std::invalid_argument("");
    }
    resize(rowcnt, c);
}

void SpreadSheet::resize(size_t r, size_t c){
    Cell** new_arr = new Cell*[r];
    for (size_t i = 0; i < r; ++i) {
        new_arr[i] = new Cell[c]; 
        for (size_t j = 0; j < c; ++j) {
            if (i < rowcnt && j < colcnt) {
                new_arr[i][j] = board[i][j];
            }
            else{
                new_arr[i][j] = Cell();
            }
        }
    }
    clear();
    rowcnt = r;
    colcnt = c;
    board = new_arr;
}

SpreadSheet SpreadSheet::slice(std::initializer_list<size_t> rows, std::initializer_list<size_t> cols){
    size_t new_rows = rows.size();
    size_t new_cols = cols.size();
    SpreadSheet result(new_rows, new_cols);
    size_t row_index = 0;
    for (auto row_ind : rows) {
        size_t col_index = 0;
        for (auto col_ind : cols) {
            result.board[row_index][col_index] = board[row_ind][col_ind];
            ++col_index;
        }
        ++row_index;
    }
    return result;
}

SpreadSheet::Column::Column(Cell* col)
    :col(col) 
{}

Cell& SpreadSheet::Column::operator[](size_t pos){
    return col[pos];
}

const Cell& SpreadSheet::Column::operator[](size_t pos) const{
    return col[pos];
}

bool operator==(const SpreadSheet& lhv, const SpreadSheet& rhv){
    if (lhv.row() != rhv.row() || lhv.col() != rhv.col()){
        return false;
    }
    for (size_t i = 0; i < lhv.row(); ++i) {
        for (size_t j = 0; j < lhv.col(); ++j) {
            if (lhv[i][j] != rhv[i][j]){ 
                return false;
            }
        }
    }
    return true;
}

bool operator!=(const SpreadSheet& lhv, const SpreadSheet& rhv){
    return !(lhv == rhv);
}
std::string SpreadSheet::getVal(size_t row, size_t col) const {
    if (row >= rowcnt || col >= colcnt) {
        throw std::out_of_range("Invalid cell index");
    }
    return board[row][col].getVal();
}

std::ostream& operator<<(std::ostream& os, const SpreadSheet& sheet) {
    constexpr int cellWidth = 10; 

    for (size_t i = 0; i < sheet.row(); ++i) {
        for (size_t j = 0; j < sheet.col(); ++j) {
            os << std::setw(cellWidth) << sheet.getVal(i, j) << ' ';
        }
        os << '\n';
    }

    return os;
}