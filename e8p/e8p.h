/*----------------------------------------------------------------
Copyright (c) 2017 Author: Jagadeesh Vasudevamurthy
file: e8p.h
-----------------------------------------------------------------*/

/*----------------------------------------------------------------
This file has e8p class declaration 
-----------------------------------------------------------------*/

/*----------------------------------------------------------------
All includes here
-----------------------------------------------------------------*/
#ifndef e8p_H
#define e8p_H

#include "../util/util.h"
#include <unordered_map>
#include <queue>
#include <string>
#include <algorithm>

/*----------------------------------------------------------------
Node definition
-----------------------------------------------------------------*/
class node {
private:
    bool _display;
    static const int N = 3;
    void _get_vacant_grid(const int m[N][N], int& r, int& c) const;
public:
    node(bool verbose);
    ~node();
    void slide(int m[N][N], char direction) const;

};

/*----------------------------------------------------------------
Declaration of board
-----------------------------------------------------------------*/
class board{

private:
    bool _display;
    static const int N = 3;
    int _puzzle[N][N];
    unordered_map<int, char> _configHash;
    queue<vector<vector<int> >> _configList;
    int _moves;
    vector<char> _solution;
    node _n;
    bool _done;

    bool _compare_puzzle(int s[N][N], int f[N][N]);
    int _hash(int s[N][N]);
    void _add_config(int hash, int s[N][N], char direction);
    void _process_solution(int f[N][N]);

public:
    board(bool verbose, const int s[N][N]);
    ~board();
    int get_num_moves() const;
    string get_solution() const;
    void solve_puzzle(int s[N][N], int f[N][N]);
    void push(vector<vector<int> > array);
    bool getDone();
    bool isListEmpty();
    void popList();
    vector<vector<int> > getFront();
    void print_board(const int c[N][N]) const;
};


/*----------------------------------------------------------------
Declaration of e8p class
-----------------------------------------------------------------*/
class e8p{

public:
    static const int N = 3 ;
    e8p(const int s[N][N], const int f[N][N]) ;
    ~e8p();
    e8p(const e8p& from) = delete;
    e8p& operator=(const e8p& from) = delete;
    int get_num_moves() const;
    string get_solution() const ;

private:
    int _puzzle[N][N];
    int _solution[N][N];
    board _b;
    bool _display;

    void _solve_puzzle(int s[N][N], int f[N][N]);
};

#endif
//EOF

