/*----------------------------------------------------------------
Copyright (c) 2017 Author: Jagadeesh Vasudevamurthy
file: e8ptest.cpp

On linux:
g++ e8p.cpp e8ptest.cpp
valgrind a.out
==166143== All heap blocks were freed -- no leaks are possible
-----------------------------------------------------------------*/

/*----------------------------------------------------------------
This file test e8p object 
-----------------------------------------------------------------*/

/*----------------------------------------------------------------
All includes here
-----------------------------------------------------------------*/
#include "e8p.h"


/*----------------------------------------------------------------
Nothing can be changed in this file
-----------------------------------------------------------------*/


/*----------------------------------------------------------------
static to this file
-----------------------------------------------------------------*/
static const int N = 3 ;

/*----------------------------------------------------------------
Declaration of e8ptest class
-----------------------------------------------------------------*/
class e8ptest{
public:
  e8ptest(int s[N][N], const int f[N][N]) ;
  ~e8ptest(){}
  void print_board(const int c[N][N]) const;
  e8ptest(const e8ptest& from) = delete;
  e8ptest& operator=(const e8ptest& from) = delete;
  bool is_equal(int a[N][N], const int b[N][N]) const;
  void assert_moves(int s[N][N], const int f[N][N], const string& ans) const;
  void get_vacent_grid(const int m[N][N], int& r, int& c) const ;
  void slide(int m[N][N], char dir) const;
private: 
};


/*----------------------------------------------------------------
print board
-----------------------------------------------------------------*/
void e8ptest::print_board(const int c[N][N]) const {
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

/*----------------------------------------------------------------
Where is the vacent grid
-----------------------------------------------------------------*/
void e8ptest::get_vacent_grid(const int m[N][N], int& r, int& c) const {
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


/*----------------------------------------------------------------
slide
-----------------------------------------------------------------*/
void e8ptest::slide(int m[N][N], char dir) const{
  int vr = 0 ;
  int vc = 0;
  get_vacent_grid(m, vr, vc) ;
  if (dir == 'U') {
    int vrn = vr - 1;
    if (vrn < 0) {
      assert(0);
    }
    m[vr][vc] = m[vrn][vc] ;
    m[vrn][vc] = 0 ;
    return;
  }
  if (dir == 'D') {
    int vrn = vr + 1;
    if (vrn >= N) {
      assert(0); ;
    }
    m[vr][vc] = m[vrn][vc] ;
    m[vrn][vc] = 0 ;
    return;
  }
  if (dir == 'L') {
    int vcn = vc - 1;
    if (vcn < 0) {
      assert(0) ;
    }
    m[vr][vc] = m[vr][vcn] ;
    m[vr][vcn] = 0 ;
    return ;
  }
  if (dir == 'R') {
    int vcn = vc + 1;
    if (vcn >= N) {
      assert(0) ;
    }
    m[vr][vc] = m[vr][vcn] ;
    m[vr][vcn] = 0 ;
    return;
  }
  assert(0) ;
}

/*----------------------------------------------------------------
is board a == board b 
-----------------------------------------------------------------*/
bool e8ptest::is_equal(int a[N][N], const int b[N][N]) const {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      int av = a[i][j];
      int bv = b[i][j] ;
      if (av != bv) {
        return false ;
      }
    }
  }
  return true ;
}


/*----------------------------------------------------------------
make sure the moves indeed goes the answer
-----------------------------------------------------------------*/
void e8ptest::assert_moves(int s[N][N], const int f[N][N], const string& a) const {
  int size = a.size() ;
  for (int i = 0; i < size; ++i) {
    char c = a[i] ;
    slide(s, c);
  }
  //Now you should be in final state
  bool t = is_equal(s,f) ;
  assert(t) ;
}

/*----------------------------------------------------------------
constructor e8ptest class
-----------------------------------------------------------------*/
e8ptest::e8ptest(int s[N][N], const int f[N][N]) {
  clock_t start = clock();
  e8p a(s,f);
  clock_t end = clock();
  cout << "------------------start state ------------\n";
  print_board(s) ;
  int num_moves = a.get_num_moves() ;
  cout << "Number of moves = " << num_moves << endl ;
  string ans = a.get_solution();
  if (num_moves == 0 || ans.size()) {
    cout << "Solution        = " << ans << endl ;
    assert_moves(s,f,ans) ;
    cout << "If I follow your directions, I indeed reach the state below\n" ;
    print_board(s) ;
  }else {
    cout << "A solution to this problem cannot be found\n" ;
  }
  double d = double(end - start) / CLOCKS_PER_SEC;
  cout << "Run time        " << d << " secs" << endl;
}

/*----------------------------------------------------------------
test one board
-----------------------------------------------------------------*/
static void test1(int s[N][N], const int f[N][N]) {
  e8ptest a(s,f);  
}

/*----------------------------------------------------------------
main
-----------------------------------------------------------------*/
int main() {
  int f[N][N] = { {1,2,3},
                  {4,5,6},
                  {7,8,0} 
                } ;
  {
    int s[N][N] = { {1,2,3},
                    {4,5,6},
                    {7,8,0} 
                  } ;
    test1(s,f) ;
  }

  {
    int s[N][N] = { {1,2,3},
                    {4,5,6},
                    {7,0,8} 
                  } ;
    test1(s,f) ;
  }
  
  {
    int s[N][N] = { {1,8,4},
                    {3,7,0},
                    {5,6,2} 
                  } ;
  

    test1(s,f) ;
  }
  {
    const int m = 100 ;
    
    for (int k = 0; k < m ; ++k) {
      int a[] = {0,1,2,3,4,5,6,7,8} ;
      int size = sizeof(a)/sizeof(int) ;
      Random r ;
      for (int t = 0; t < m ; ++t) {
        int v1 = r.get_random_number(0,size) ;
        assert(v1 >= 0 && v1 < size) ;
        int v2 = r.get_random_number(0,size) ;
        assert(v2 >= 0 && v2 < size) ;
        int l = a[v1] ;
        a[v1] = a[v2] ;
        a[v2] = l ;
      }
      int e = 0 ;
      int s[N][N] ;
      for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
          s[i][j] = a[e++] ;
        }
      }
      test1(s,f) ;
    }

  }
  cout << "If you have solved this problem on your own congratulation\n" ;
  cout << "If you have enjoyed this problem, send me a gift card\n" ;
  return 0 ;
}

//EOF
