/*----------------------------------------------------------------
Copyright (c) 2017 Author: Jagadeesh Vasudevamurthy
file: e8p.cpp
-----------------------------------------------------------------*/

/*----------------------------------------------------------------
This file has e8p class definition
-----------------------------------------------------------------*/

/*----------------------------------------------------------------
All includes here
-----------------------------------------------------------------*/
#include "e8p.h"

/*----------------------------------------------------------------
e8p Class Definition
-----------------------------------------------------------------*/
e8p::e8p(const int s[N][N], const int f[N][N]) : _display(false), _b(false, s)
{
    if (_display) cout <<"Inside e8p constructor\n";
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            _puzzle[i][j] = s[i][j];
        }
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            _solution[i][j] = f[i][j];
        }
    }
   
    vector<vector<int> > array;
    for (int i = 0; i < N; i++)
    {
        vector<int> row;
        for (int j = 0; j < N; j++)
        {
            row.push_back(s[i][j]);
        }
        array.push_back(row);
    }
    _b.push(array);
    
    while (!_b.getDone())
    {
        if (!_b.isListEmpty()) 
        {
            vector<vector<int> > next = _b.getFront();
            int a[N][N];
            for (int i = 0; i < next.size(); i++)
            {
                for (int j = 0; j < next[i].size(); j++)
                {
                    a[i][j] = next[i][j];
                }
            }
            _solve_puzzle(a, _solution);
            _b.popList();
        }
        else
        {
            break;
        }
    }
}

e8p::~e8p()
{
    if (_display) {
        cout<<"Inside e8p destructor\n";
    }
}

int e8p::get_num_moves() const 
{
    return _b.get_num_moves();
}

string e8p::get_solution() const
{
    return _b.get_solution();
}

void e8p::_solve_puzzle(int s[N][N], int f[N][N])
{
    _b.solve_puzzle(s, f);
    return;
}

/*----------------------------------------------------------------
board Class Definition
-----------------------------------------------------------------*/
board::board(bool verbose, const int s[N][N]) : _display(verbose), _moves(0),
_n(_display), _done(false)
{
    if (_display)
    {
        cout<<"Inside Board Constructor\n";
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            _puzzle[i][j] = s[i][j];
        }
    }
}

board::~board()
{
    if (_display)
    {
        cout<<"Inside Board Destructor\n";
    }
}

void board::push(vector<vector<int> > array)
{
    _configList.push(array);
}

bool board::getDone()
{
    return _done;
}

void board::popList()
{
    _configList.pop();
}

bool board::isListEmpty()
{
    return _configList.empty();
}

vector<vector<int> > board::getFront()
{
    return _configList.front();
}

int board::get_num_moves() const
{
    return _moves;
}

string board::get_solution() const
{
    string s {_solution.begin(), _solution.end()};
    return s;
}

void board::solve_puzzle(int s[N][N], int f[N][N])
{
    if (_display)
    {
        cout<<"Inside solve puzzle\n";
    }
    
    bool equal = _compare_puzzle(s, f);
    if (equal) {
        _done = true;
        _process_solution(f);
        std::reverse(_solution.begin(), _solution.end());
        return;
    }
    
    _moves++;
    
    //Slide here 
    string dir = "U";
    dir += "D";
    dir += "L";
    dir += "R";

    int orig_puzzle[N][N];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            orig_puzzle[i][j] = s[i][j];
        }
    }
    
    for (int i = 0; i < dir.size(); i++)
    {
        _n.slide(s, dir[i]);
        
        if (!_compare_puzzle(s, orig_puzzle))
        {
            // Find the Hash of the puzzle
            int hash = _hash(s);
            
            // Add this configuration to the hash
            _add_config(hash, s, dir[i]);

            //Revert s back to original puzzle
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    s[i][j] = orig_puzzle[i][j];
                }
            }
        }

    }
}

/*----------------------------------------------------------------
print board
-----------------------------------------------------------------*/
void board::print_board(const int c[N][N]) const {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      int v = c[i][j] ;
      if (v) {
        cout << v << ' ' ;
      }else {
        cout << ' ' << ' ' ;
      }
    }
    cout << endl ; 
  }
}

void board::_process_solution(int f[N][N])
{
    if (_display)
    {
        cout<<"Inside process_solution\n";
    }
    while (!_compare_puzzle(_puzzle, f))
    {
        //Find char pointed to by this final solution
        int hash = _hash(f);

        // Lookup the hash of the solution in the map
        // and get the char associated with it
        unordered_map<int, char>::iterator lookup = _configHash.find(hash);
        if (lookup == _configHash.end()) assert(0);

        // Add it to the vector<char> to start storing the string
        // This string is in reverse order
        _solution.push_back(lookup->second);

        // Slide in the opposite direction since you are
        // walking back to the start state
        char dir;
        if (lookup->second == 'U') dir = 'D';
        if (lookup->second == 'D') dir = 'U';
        if (lookup->second == 'L') dir = 'R';
        if (lookup->second == 'R') dir = 'L';
        
        //slide in the reverse direction
        _n.slide(f, dir);
    }
}

void board::_add_config(int hash, int s[N][N], char direction)
{
    if (_display)
    {
        cout<<"Inside check set\n";
    }
    // If this configuration has already been reached,
    // Then do not add it to the Hash & List
    unordered_map<int, char>::iterator lookup = _configHash.find(hash);
    if (lookup == _configHash.end())
    {
        // This config has not been used
        // Add them to the Hash and Queue
        _configHash.insert( { hash, direction } );

        vector<vector<int> > array;
        for (int i = 0; i < N; i++)
        {
            vector<int> row;
            for (int j = 0; j < N; j++)
            {
                row.push_back(s[i][j]);
            }
            array.push_back(row);
        }
        _configList.push(array);
    }
    return;
}

int board::_hash(int s[N][N])
{
    if (_display)
    {
        cout<<"Inside hash\n";
    }
    int key=0;
    for (int i = 0; i < N; i++)
    {   
        for (int j = 0; j < N; j++)
        {
            if (s[i][j] != 0)
            {
                key += (3*i + 7*j) * s[i][j];
                key = ~key + (key << 15); // key = (key << 15) - key - 1;
                key = key ^ (key >> 12);
                key = key + (key << 2);
                key = key ^ (key >> 4);
                key = key * 2057; // key = (key + (key << 3)) + (key << 11)
                key = key ^ (key >> 16);
            }
        }
    }
    return key;
}

bool board::_compare_puzzle(int s[N][N], int f[N][N])
{
    if (_display)
    {
        cout<<"Inside compare puzzle\n";
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (s[i][j] != f[i][j]) return false;
        }
    }
    return true;
}

/*----------------------------------------------------------------
node Class Definition
-----------------------------------------------------------------*/
node::node(bool verbose) : _display(verbose)
{
    if (_display) {
        cout<<"Inside node constructor\n";
    }
}


node::~node() {
    if (_display) {
        cout<<"Inside node destructor\n";
    }
}

void node::_get_vacant_grid(const int m[N][N], int& r, int& c) const {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      int v = m[i][j] ;
      if (v == 0) {
        r = i ;
        c = j ;
        return ;
      }
    }
  }
  assert(0) ;
}

void node::slide(int m[N][N], char dir) const{
    if (_display) {
    cout<<"Inside Board::slide with dir "<<dir<<endl;
    }
    int vr = 0 ;
    int vc = 0;
    _get_vacant_grid(m, vr, vc) ;
    if (dir == 'U') {
        int vrn = vr - 1;
        if (vrn < 0) {
          return;
        }
        m[vr][vc] = m[vrn][vc] ;
        m[vrn][vc] = 0 ;
        return;
    }
    if (dir == 'D') {
        int vrn = vr + 1;
        if (vrn >= N) {
          return;
        }
        m[vr][vc] = m[vrn][vc] ;
        m[vrn][vc] = 0 ;
        return;
    }
    if (dir == 'L') {
        int vcn = vc - 1;
        if (vcn < 0) {
          return;
        }
        m[vr][vc] = m[vr][vcn] ;
        m[vr][vcn] = 0 ;
        return ;
    }
    if (dir == 'R') {
        int vcn = vc + 1;
        if (vcn >= N) {
          return;
        }
        m[vr][vc] = m[vr][vcn] ;
        m[vr][vcn] = 0 ;
        return;
    }
    return;
}

//EOF

