/**************
 * This solves the dynamic programming problem of minimizing
 * the residual squared error given a segmentation with
 * polynomial segments of varying degrees under a budget
 * constraint where a segment of degree d costs d+1.
 *
 * The data is read off standard input as pairs of comma
 * separated floating point values such as...
 * x1,y1
 * x2,y2
 * ...
 * (one per line)
 *
 * Update: the result can be taylored to your taste, you can now get
 * the segmentation, coming out as one segment per line with start end degree,
 * where start and end are the point index and degree is the degree (0=flat)
 * of the segment.
 *
 * What follows is my old documentation:
 *
 * The program outputs the 3 matrices which are the solution
 * of the dynamic programming problem (RSS, SegmentationPoints, Degrees).
 * Making sense of these 3 matrices is not meant to be trivial, but
 * if you understand dynamic programming, you have that the first matrix
 * is the optimal "cost" matrix, the second one tells you where to move
 * to (starting from the end column) and the degree matrix tells you
 * how many rows to move up. The previous sentence is not meant to be
 * easily parseable. I'm too busy to document this seriously, sorry.
 *
 * The output should look like this:
 * [
 * 0,0,0,6.75,10.8
 * 0,0,0,4.5,6
 * 0,0,0,0,0
 * 0,0,0,0,0
 * 0,0,0,0,0
 * 0,0,0,0,0
 * 0,0,0,0,0
 * ]
 * [
 * 0,0,0,0,0
 * 0,0,0,2,2
 * 0,0,0,2,2
 * 0,0,0,3,3
 * 0,0,0,3,3
 * 0,0,0,3,3
 * 0,0,0,3,3
 * ]
 * [
 * 0,0,0,0,0
 * 0,0,0,0,0
 * 0,0,0,1,1
 * 0,0,0,0,0
 * 0,0,0,0,0
 * 0,0,0,0,0
 * 0,0,0,0,0
 * ]
 *
 * I hope this program is useful to you! But if your computer goes out in
 * flame or your wife leaves you, I don't want to hear about it: this
 * software comes as is.
 *
 * You can assume GPL version 2.0 licensing, though other more liberal
 * licenses are possible, just ask. I will not charge money for this code.
 *
 * January 2006
 * Daniel Lemire
 * http://www.daniel-lemire.com/
 */

#include <vector>
#include <list>
#include <cmath>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
using namespace std;

/**
 * Here are some optimization flags.
 */
// to make things faster, you can try
// replacing double with float, but I have a 64 bits processor
// so double is probably nearly as fast as float here
typedef double flt;
// typedef float flt;
// turning of the asserts can make things run faster
const bool asserts = false;

/**
 * Next, for convenience, here are some typedefs
 */

// everything is a vector right now, probably inefficient
typedef vector<vector<vector<flt> > > matrix3f;
typedef vector<vector<flt> > matrix2f;
typedef vector<vector<int> > matrix2i;
typedef vector<flt> vectorf;
typedef vector<int> vectori;
// these two are a bit ugly
typedef vector<vector<flt> > dparray;
typedef vector<flt> point;

// static flt INFINITY = 9999999999999999999.0;

/**
 * Follows some printing function (to print on standard output)
 */
template <class printable> void printlist(list<printable> l, bool comment) {
  for (typename list<printable>::iterator k = l.begin(); k != l.end(); k++)
    k->print(comment);
}

template <class matrixtype>
void printm(matrixtype &array, bool comment = false) {
  stringstream strs;
  if (comment)
    strs << "#[\n";
  else
    strs << "[\n";
  for (int k = 0; k < (int)array.size(); ++k) {
    if (comment)
      strs << "#";
    for (int j = 0; j < (int)array[k].size() - 1; ++j)
      strs << array[k][j] << ",";
    strs << array[k][array[k].size() - 1] << "\n";
  }
  if (comment)
    strs << "#]";
  else
    strs << "]";
  cout << strs.str() << endl;
}

template <class vectortype>
void printv(vectortype &array, bool comment = false) {
  stringstream strs;
  if (comment)
    strs << "#";
  strs << "[";
  for (int k = 0; k < (int)array.size(); ++k)
    strs << array[k] << ",";
  strs << "]";
  cout << strs.str() << endl;
}

/**
 * this is a convenience function.
 */
point p(flt x, flt y) {
  point mypoint(2);
  mypoint[0] = x;
  mypoint[1] = y;
  return mypoint;
}

/**
 * follows the fast sum functions
 */
matrix3f computeBuffer(dparray &datapoints, int degree) {
  matrix3f fastsumbuffer(2 * degree + 1,
                         matrix2f(3, vectorf(datapoints.size() + 1, 0.0f)));
  for (int xexponent = 0; xexponent < 2 * degree + 1; ++xexponent) {
    for (int yexponent = 0; yexponent < 3; ++yexponent) {
      fastsumbuffer[xexponent][yexponent][0] = 0.0f; // to be sure
      for (int x = 0; x < (int)datapoints.size(); ++x) {
        fastsumbuffer[xexponent][yexponent][x + 1] =
            fastsumbuffer[xexponent][yexponent][x] +
            (flt)pow(datapoints[x][0], xexponent) *
                (flt)pow(datapoints[x][1], yexponent);
      }
    }
  }
  return fastsumbuffer;
}

flt fastsum(matrix3f &fastsumbuffer, int start, int end, int xexponent,
            int yexponent) {
  if (asserts) {
    assert(xexponent < (int)fastsumbuffer.size());
    assert(yexponent < (int)fastsumbuffer[xexponent].size());
    assert(end >= start);
    assert(end < (int)fastsumbuffer[xexponent][yexponent].size());
  }
  return fastsumbuffer[xexponent][yexponent][end] -
         fastsumbuffer[xexponent][yexponent][start];
}

flt slowsum(dparray &datapoints, int start, int end, int xexponent,
            int yexponent) {
  flt sum = 0.0;
  for (int x = start; x < end; ++x) {
    sum += (flt)pow(datapoints[x][0], xexponent) *
           (flt)pow(datapoints[x][1], yexponent);
  }
  return sum;
}
vectorf solve(matrix2f &a, vectorf &v) {
  assert(a.size() < 4);
  vectorf x(a.size(), 0.0);
  if (a.size() == 1) {
    assert(abs(a[0][0]) > 0.0);
    x[0] = 1 / a[0][0] * v[0];
  } else if (a.size() == 2) {
    flt det = a[0][0] * a[1][1] - a[0][1] * a[1][0];
    // cerr << "determinant2 = " << det <<endl;
    assert(abs(det) > 0.0);
    x[0] = -(a[0][1] * v[1] - a[1][1] * v[0]) / det;
    x[1] = (a[0][0] * v[1] - a[1][0] * v[0]) / det;
  } else if (a.size() == 3) {
    flt det = ((a[0][0] * a[1][1] - a[0][1] * a[1][0]) * a[2][2] +
               (a[0][2] * a[1][0] - a[0][0] * a[1][2]) * a[2][1] +
               (a[0][1] * a[1][2] - a[0][2] * a[1][1]) * a[2][0]);
    // cerr << "determinant3 = " << det <<endl;
    assert(abs(det) > 0.0);
    x[0] = ((a[0][1] * a[1][2] - a[0][2] * a[1][1]) * v[2] +
            (a[0][2] * a[2][1] - a[0][1] * a[2][2]) * v[1] +
            (a[1][1] * a[2][2] - a[1][2] * a[2][1]) * v[0]) /
           det;
    x[1] = -((a[0][0] * a[1][2] - a[0][2] * a[1][0]) * v[2] +
             (a[0][2] * a[2][0] - a[0][0] * a[2][2]) * v[1] +
             (a[1][0] * a[2][2] - a[1][2] * a[2][0]) * v[0]) /
           det;
    x[2] = ((a[0][0] * a[1][1] - a[0][1] * a[1][0]) * v[2] +
            (a[0][1] * a[2][0] - a[0][0] * a[2][1]) * v[1] +
            (a[1][0] * a[2][1] - a[1][1] * a[2][0]) * v[0]) /
           det;
  }
  return x;
}

vectorf multiply(matrix2f &m, vectorf &v) {
  vectorf result(m.size(), (flt)0.0);
  for (int k = 0; k < (int)m.size(); ++k) {
    for (int l = 0; l < (int)m[k].size(); ++l)
      result[k] += m[k][l] * v[l];
  }
  return result;
}
vectorf slowbestfit(dparray &datapoints, int start, int end, int degree) {
  if (degree >= end - start) {
    degree = end - start - 1;
  }
  // cerr << "degree = "<<degree << " end = " << end << " start = "<< start <<
  // endl;
  flt normfactor = slowsum(datapoints, start, end, 2 * degree, 0);
  matrix2f matrix = matrix2f(degree + 1, vectorf(degree + 1, 0));
  for (int k = 0; k < degree + 1; ++k)
    for (int j = 0; j < degree + 1; ++j)
      matrix[j][k] = slowsum(datapoints, start, end, k + j, 0) / normfactor;
  vectorf vector(degree + 1, 0);
  for (int k = 0; k < degree + 1; ++k)
    vector[k] = slowsum(datapoints, start, end, k, 1) / normfactor;
  return solve(matrix, vector);
}

vectorf bestfit(matrix3f &fastsumbuffer, int start, int end, int degree) {
  if (degree >= end - start) {
    degree = end - start - 1;
  }
  // cerr << "degree = "<<degree << " end = " << end << " start = "<< start <<
  // endl;
  flt normfactor = fastsum(fastsumbuffer, start, end, 2 * degree, 0);
  matrix2f matrix = matrix2f(degree + 1, vectorf(degree + 1, 0));
  for (int k = 0; k < degree + 1; ++k)
    for (int j = 0; j < degree + 1; ++j)
      matrix[j][k] = fastsum(fastsumbuffer, start, end, k + j, 0) / normfactor;
  vectorf vector(degree + 1, 0);
  for (int k = 0; k < degree + 1; ++k)
    vector[k] = fastsum(fastsumbuffer, start, end, k, 1) / normfactor;
  return solve(matrix, vector);
}

flt positive(flt e) {
  if (e < 0.0)
    return 0.0f;
  return e;
}

flt slowsquaredfiterror(dparray &datapoints, int start, int end, int degree) {
  flt error = 0.0f;
  if (start == end)
    return error;
  // next we compute something \sum (a x^2 + b x + c- y) ^2
  // my algo is probably less than optimal...
  vectorf coefs = slowbestfit(datapoints, start, end, degree);
  for (int d = 0; d < (int)coefs.size(); ++d) {
    for (int d2 = 0; d2 < (int)coefs.size(); ++d2)
      error +=
          coefs[d] * coefs[d2] * slowsum(datapoints, start, end, d + d2, 0);
    error += coefs[d] * (-1.0) * slowsum(datapoints, start, end, d, 1);
  }
  for (int d2 = 0; d2 < (int)coefs.size(); ++d2)
    error += (-1.0f) * coefs[d2] * slowsum(datapoints, start, end, d2, 1);
  error += (-1.0f) * (-1.0f) * slowsum(datapoints, start, end, 0, 2);
  return positive(error);
}
flt squaredfiterror(matrix3f &fastsumbuffer, int start, int end, int degree) {
  flt error = 0.0f;
  if (start == end)
    return error;
  // next we compute something \sum (a x^2 + b x + c- y) ^2
  // my algo is probably less than optimal...
  vectorf coefs = bestfit(fastsumbuffer, start, end, degree);
  for (int d = 0; d < (int)coefs.size(); ++d) {
    for (int d2 = 0; d2 < (int)coefs.size(); ++d2)
      error +=
          coefs[d] * coefs[d2] * fastsum(fastsumbuffer, start, end, d + d2, 0);
    error += coefs[d] * (-1.0) * fastsum(fastsumbuffer, start, end, d, 1);
  }
  for (int d2 = 0; d2 < (int)coefs.size(); ++d2)
    error += (-1.0f) * coefs[d2] * fastsum(fastsumbuffer, start, end, d2, 1);
  error += (-1.0f) * (-1.0f) * fastsum(fastsumbuffer, start, end, 0, 2);
  return positive(error);
}

/**
 * then some unit testing functions
 */

// this is for unit testing purposes
void testfastsums(bool verbose = true) {
  dparray datapoints(0);
  srand(0);
  for (int x = 0; x < 10; ++x)
    datapoints.push_back(p((flt)x, (flt)rand() / RAND_MAX));
  if (verbose)
    printm<dparray>(datapoints);
  int degree = 4;
  matrix3f fastsumbuffer = computeBuffer(datapoints, degree);
  for (int xexponent = 0; xexponent < degree + 1; ++xexponent) {
    assert(abs(slowsum(datapoints, 3, 7, xexponent, 0) -
               fastsum(fastsumbuffer, 3, 7, xexponent, 0)) < 0.001);
    assert(abs(slowsum(datapoints, 3, 7, xexponent, 1) -
               fastsum(fastsumbuffer, 3, 7, xexponent, 1)) < 0.001);
    assert(abs(slowsum(datapoints, 0, 10, xexponent, 0) -
               fastsum(fastsumbuffer, 0, 10, xexponent, 0)) < 0.001);
    assert(abs(slowsum(datapoints, 0, 10, xexponent, 1) -
               fastsum(fastsumbuffer, 0, 10, xexponent, 1)) < 0.001);
  }
}
// this is for unit testing purposes
void testbestfit(bool verbose = true) {
  dparray datapoints(0);
  for (int x = 0; x < 100; ++x) {
    datapoints.push_back(p((flt)x, 0.001 * (flt)pow(x, 2.0f) - (flt)x + 3.0f));
  }
  if (verbose)
    printm<dparray>(datapoints, true);
  matrix3f buf = computeBuffer(datapoints, 3);
  vectorf coefs = bestfit(buf, 10, 90, 2);
  if (verbose)
    printv<vectorf>(coefs, true);
  assert(abs(coefs[0] - 3) < 0.0001);
  assert(abs(coefs[1] + 1) < 0.0001);
  assert(abs(coefs[2] - 0.001) < 0.0000001);
}
void testdp();
void unit(bool verbose = true) {
  testfastsums(verbose);
  testbestfit(verbose);
  testdp();
}

class ResultOfDynProg {
public:
  ResultOfDynProg(matrix2f &rss, matrix2i &sp, matrix2i &d)
      : RSS(rss), SegmentationPoints(sp), Degrees(d) {}

  void print() {
    printm<matrix2f>(RSS);
    printm<matrix2i>(SegmentationPoints);
    printm<matrix2i>(Degrees);
  }

  matrix2f RSS;
  matrix2i SegmentationPoints;
  matrix2i Degrees;
};

// int min(int x, int y) {
//        return x>y ? y: x;
//}
/**
 * Finally, we make it to the dynamic programming part.
 */
/*ResultOfDynProg dynamicprogramming(dparray& datapoints,int k, int degree, bool
verbose = false) {
  if(verbose) cout << "#computing buffer..." <<endl;
  matrix3f fastsumbuffer=computeBuffer(datapoints, degree);
  if(verbose) cout << "#computing buffer...done" <<endl;
  int n = datapoints.size();
  //printm<dparray>(datapoints,true);
  matrix2f RSS (k, vectorf(n+1,0.0f));
  matrix2i SegmentationPoints(k,vectori(n+1,0));
  matrix2i Degrees(k,vectori(n+1,0));
  for(int row = 0; row < k; ++row) {
    if(verbose) cout << "# row = " << row << endl;
    for(int x = 1; x <= n; ++x) {
      RSS[row][x] = INFINITY;
      for (int deg = 0; deg < min(row+1,degree);++deg)  {
        int endofz = x+1;
        if(asserts) assert( row-1-deg >= -1);
        if(row-1-deg == -1) endofz = 1;
        int rowindex = max(row-1-deg,0);
        for(int  z =0; z < endofz; ++z) {
          //cout << " z+1 = "<< (z+1) << " x+1 = "<< (x+1) << endl;
          flt value =  RSS[rowindex][z]
                  +squaredfiterror(fastsumbuffer,z,x,deg);//squaredfiterror(fastsumbuffer,z,x+1,deg);
          //cout << " ok  " << endl;
          if(RSS[row][x] > value) {
            RSS[row][x] = value;
            SegmentationPoints[row][x] = z;
            Degrees[row][x]= deg;
          }
        }
      }
    }
  }
  return ResultOfDynProg(RSS,SegmentationPoints,Degrees);
}*/
/**
 * Finally, we make it to the dynamic programming part.
 */
ResultOfDynProg dynamicprogramming(dparray &datapoints, int k, int degreemin,
                                   int degreemax, bool verbose = false) {
  int degree = degreemax;
  if (degreemin > 0)
    cout << "#[WARNING] degreemin can give wrong results for some ks" << endl;
  if (verbose)
    cout << "#computing buffer..." << endl;
  matrix3f fastsumbuffer = computeBuffer(datapoints, degree);
  if (verbose)
    cout << "#computing buffer...done" << endl;
  int n = datapoints.size();
  // printm<dparray>(datapoints,true);
  matrix2f RSS(k, vectorf(n + 1, 0.0f));
  matrix2i SegmentationPoints(k, vectori(n + 1, 0));
  matrix2i Degrees(k, vectori(n + 1, 0));
  for (int row = 0; row < k; ++row) {
    if (verbose)
      cout << "# row = " << row << endl;
    for (int x = 1; x <= n; ++x) {
      RSS[row][x] = INFINITY;
      for (int deg = degreemin; deg < min(row + 1, degreemax); ++deg) {
        int endofz = x + 1;
        if (asserts)
          assert(row - 1 - deg >= -1);
        if (row - 1 - deg == -1)
          endofz = 1;
        int rowindex = max(row - 1 - deg, 0);
        for (int z = 0; z < endofz; ++z) {
          // cout << " z+1 = "<< (z+1) << " x+1 = "<< (x+1) << endl;
          flt value =
              RSS[rowindex][z] +
              squaredfiterror(fastsumbuffer, z, x,
                              deg); // squaredfiterror(fastsumbuffer,z,x+1,deg);
          // cout << " ok  " << endl;
          if (RSS[row][x] > value) {
            RSS[row][x] = value;
            SegmentationPoints[row][x] = z;
            Degrees[row][x] = deg;
          }
        }
      }
    }
  }
  return ResultOfDynProg(RSS, SegmentationPoints, Degrees);
}
class interval {
public:
  interval() : start(0), end(0), degree(0) {}
  interval(int s, int e, int d) : start(s), end(e), degree(d) {}
  void print(bool comment) {
    if (comment)
      cout << "#";
    cout << start << " " << end << " " << degree << endl;
  }
  int start, end, degree;
  bool operator<(const interval &other) const { return start < other.start; }
  // bool operator<= (const interval& other) const {return start <=other.start;}
};
typedef list<pair<flt, interval> > fltintervallist;

pair<list<interval>, flt> getSegmentsFromDynProg(ResultOfDynProg &rdp) {
  int startrow = 0;
  int n = (int)rdp.RSS[0].size();
  for (int row = 0; row < (int)rdp.RSS.size(); ++row)
    if (rdp.RSS[row][n - 1] + (n - 1) * 1E-7 < rdp.RSS[startrow][n - 1])
      startrow = row;
  int x = n - 1;
  int row = startrow;
  list<interval> solution(0);
  int nextpoint, degree;
  while ((row >= 0) and (x > 0)) {
    nextpoint = rdp.SegmentationPoints[row][x];
    degree = rdp.Degrees[row][x];
    row -= degree + 1;
    solution.push_back(interval(nextpoint, x, degree));
    x = nextpoint;
  }
  solution.reverse();
  return pair<list<interval>, flt>(solution, rdp.RSS[startrow][n - 1]);
}

flt segmentationerror(list<interval> &l, matrix3f &fastsumbuffer) {
  flt sum = 0.0;
  for (list<interval>::iterator k = l.begin(); k != l.end(); k++)
    sum += squaredfiterror(fastsumbuffer, k->start, k->end, k->degree);
  return sum;
}

template <int N> bool degreelarger(pair<flt, interval> &x) {
  return x.second.degree > N;
}
template <int N> bool degreeis(pair<flt, interval> &x) {
  return x.second.degree == N;
}

template <typename fi, typename p>
fi max_filtered_element(fi first, fi last, p pred) {
  if (first == last)
    return first;
  fi result = first;
  while (++first != last)
    if (pred(*first))
      if (*result < *first)
        result = first;
  return result;
}

//= max_filtered_element(tags.begin(),tags.end(),degreeis<1>);

flt __splitlinear(matrix3f &buffer, fltintervallist &tags,
                  fltintervallist::iterator p, bool verbose, bool changetags) {
  flt cost = p->first;
  flt segmentedcost = cost;
  interval inter = p->second;
  if (verbose)
    cout << "# selected segment ranging from " << inter.start << " to "
         << inter.end << " with cost " << cost << endl;
  int i = inter.start;
  flt currentcost;
  for (int j = inter.start; j < inter.end; ++j) {
    currentcost = squaredfiterror(buffer, inter.start, j, 0) +
                  squaredfiterror(buffer, j, inter.end, 0);
    if (currentcost < segmentedcost) {
      segmentedcost = currentcost;
      i = j;
    }
  }
  if (cost <= segmentedcost) {
    if (verbose)
      cout << "# could not find a way to improve!" << endl;
    return 0.0; // bailing out early
  } else if (verbose) {
    cout << "# found a way to save " << (double)(cost - segmentedcost) << endl;
  }
  if (changetags) {
    if (verbose)
      cout << "# actually changing the data " << endl;
    p = tags.insert(
        ++p, pair<flt, interval>(squaredfiterror(buffer, i, inter.end, 0),
                                 interval(i, inter.end, 0)));
    --p;
    p->second.end = i;
    p->second.degree = 0;
    p->first = squaredfiterror(buffer, inter.start, i, 0);
  }
  return cost - segmentedcost;
}

flt splitlinear(matrix3f &buffer, fltintervallist &tags, bool verbose) {
  flt bestsavings = 0.0;
  fltintervallist bestags;
  fltintervallist::iterator bestptr = tags.end();
  for (fltintervallist::iterator str =
           find_if(tags.begin(), tags.end(), degreeis<1>);
       str != tags.end(); str = find_if(++str, tags.end(), degreeis<1>)) {
    flt savings = __splitlinear(buffer, tags, str, verbose, false);
    if (savings >= bestsavings) {
      bestptr = str;
      bestsavings = savings;
    }
  }
  return __splitlinear(buffer, tags, bestptr, verbose, true);
}

flt greedy(matrix3f &buffer, fltintervallist &tags, int degree_minvalue,
           int degree_upperbound, int &budgetleft, bool verbose,
           int extrabudget = 0) {
  // first, we identify the most costly segment
  fltintervallist::iterator p = max_element(tags.begin(), tags.end());
  flt cost = p->first;
  interval inter = p->second;
  if (verbose) {
    cout << " selected interval " << endl;
    inter.print(true);
    cout << " whose cost is " << cost << endl;
  }
  flt segmentedcost = cost;
  int degree1 = 0;
  int i = inter.start;
  int bestextra = 0;
  flt currentcost;
  for (int extra = 0; extra <= extrabudget; ++extra) {
    // totalbudget is the sum of the degrees of freedom
    int totalbudget = inter.degree + degree_minvalue + extra;
    if (budgetleft < degree_minvalue + extra + 1)
      continue;
    for (int d = degree_minvalue; d < degree_upperbound; ++d) {
      int otherdegree = totalbudget - d;
      if ((otherdegree >= degree_upperbound) || (otherdegree < degree_minvalue))
        continue;
      for (int j = inter.start; j < inter.end; ++j) {
        currentcost = squaredfiterror(buffer, inter.start, j, d) +
                      squaredfiterror(buffer, j, inter.end, otherdegree);
        if (currentcost < segmentedcost) {
          segmentedcost = currentcost;
          degree1 = d;
          i = j;
          bestextra = extra;
        }
      }
    }
  }
  int degree2 = inter.degree - degree1 + degree_minvalue + bestextra;
  if (inter.degree < degree_upperbound - 1) {
    // here we should try to apply our extra budget, but it is not very useful
    // in practice (never go quadratic)
    flt increaseddegreecost =
        squaredfiterror(buffer, inter.start, inter.end, inter.degree + 1);
    if (increaseddegreecost < segmentedcost) {
      if (verbose) {
        cout << "I choose to increase the degree, new cost = "
             << increaseddegreecost << endl;
      }
      p->second.degree += 1;
      p->first = increaseddegreecost;
      budgetleft -= 1;
      return cost - increaseddegreecost;
    }
  }
  if ((!(cost > segmentedcost)) || (i == inter.start)) {
    if (verbose) {
      cout << "I'm bailing out early, best I can do is " << segmentedcost
           << endl;
    }
    // break;//early
    return 0.0;
  }
  budgetleft -= 1 + degree_minvalue + bestextra;
  if (verbose) {
    cout << "I choose to split the segment at " << i
         << " with degree1 = " << degree1 << " and degree2 = " << degree2
         << " new cost will be " << segmentedcost << endl;
  }
  p = tags.insert(
      ++p, pair<flt, interval>(squaredfiterror(buffer, i, inter.end, degree2),
                               interval(i, inter.end, degree2)));
  --p;
  p->second.end = i;
  p->second.degree = degree1;
  p->first = squaredfiterror(buffer, inter.start, i, degree1);
  return cost - segmentedcost;
}

pair<list<interval>, flt> solveTopDownSegmentation(dparray &data, int k,
                                                   int degree_minvalue,
                                                   int degree_upperbound,
                                                   bool verbose = 0) {
  matrix3f buffer(computeBuffer(data, degree_upperbound));
  // typedef list<pair<flt,interval> > fltintervallist;
  fltintervallist tags(0);
  tags.push_back(pair<flt, interval>(
      squaredfiterror(buffer, 0, data.size(), degree_minvalue),
      interval(0, data.size(), degree_minvalue)));
  int budgetleft = k - 1 - degree_minvalue;
  while (budgetleft > 0) {
    flt savings = greedy(buffer, tags, degree_minvalue, degree_upperbound,
                         budgetleft, 0, verbose);
    if (verbose)
      cout << "# budget left = " << budgetleft << endl;
    if (savings <= 0.0)
      break;
  }
  list<interval> answer(0);
  flt error = 0.0;
  for (fltintervallist::iterator p = tags.begin(); p != tags.end(); ++p) {
    error += p->first;
    answer.push_back(p->second);
  }
  answer.sort();
  return pair<list<interval>, flt>(answer, error);
}

pair<list<interval>, flt>
solveTopDownSegmentationHeuristic1(dparray &data, int k, bool verbose = 0) {
  matrix3f buffer(computeBuffer(data, 2));
  // typedef list<pair<flt,interval> > fltintervallist;
  fltintervallist tags(0);
  tags.push_back(pair<flt, interval>(squaredfiterror(buffer, 0, data.size(), 1),
                                     interval(0, data.size(), 1)));
  int budgetleft = k - 1 - 1;
  // first we do linear top down
  while (budgetleft > 0) {
    flt savings = greedy(buffer, tags, 1, 2, budgetleft, 0, verbose);
    if (verbose)
      cout << "# budget left = " << budgetleft << endl;
    if (savings <= 0.0)
      break;
  }
  // then we try to go further
  if (verbose)
    cout << "# now trying to improve on top down linear " << endl;
  while (1) {
    flt savings = splitlinear(buffer, tags, verbose);
    // if(verbose) cout << "# budget left = " << budgetleft << endl;
    if (savings <= 0.0)
      break;
  }
  list<interval> answer(0);
  flt error = 0.0;
  for (fltintervallist::iterator p = tags.begin(); p != tags.end(); ++p) {
    error += p->first;
    answer.push_back(p->second);
  }
  answer.sort();
  return pair<list<interval>, flt>(answer, error);
}
pair<list<interval>, flt>
solveTopDownSegmentationHeuristic2(dparray &data, int k, bool verbose = 0) {
  matrix3f buffer(computeBuffer(data, 2));
  // typedef list<pair<flt,interval> > fltintervallist;
  fltintervallist tags(0);
  tags.push_back(pair<flt, interval>(squaredfiterror(buffer, 0, data.size(), 0),
                                     interval(0, data.size(), 0)));
  int budgetleft = k - 1;
  // first we do top down
  while (budgetleft > 0) {
    flt savings = greedy(buffer, tags, 0, 2, budgetleft, 0, verbose);
    if (verbose)
      cout << "# budget left = " << budgetleft << endl;
    if (savings <= 0.0)
      break;
  }
  // then we try to go further
  if (verbose)
    cout << "# now trying to improve on top down " << endl;
  while (1) {
    flt savings = splitlinear(buffer, tags, verbose);
    // if(verbose) cout << "# budget left = " << budgetleft << endl;
    if (savings <= 0.0)
      break;
  }
  list<interval> answer(0);
  flt error = 0.0;
  for (fltintervallist::iterator p = tags.begin(); p != tags.end(); ++p) {
    error += p->first;
    answer.push_back(p->second);
  }
  answer.sort();
  return pair<list<interval>, flt>(answer, error);
}

/***
* Delete me!!!!!!!
*/
pair<list<interval>, flt> solveTopDownSegmentationFlat(dparray &data, int k,
                                                       bool verbose = 0) {
  int degree_upperbound = 1;
  matrix3f buffer(computeBuffer(data, degree_upperbound));
  fltintervallist tags(0);
  tags.push_back(pair<flt, interval>(squaredfiterror(buffer, 0, data.size(), 0),
                                     interval(0, data.size(), 0)));
  int budgetleft = k - 1;
  while (budgetleft > 0) {
    // first, we identify the most costly segment
    fltintervallist::iterator p = max_element(tags.begin(), tags.end());
    flt cost = p->first;
    interval inter = p->second;
    if (verbose) {
      cout << "#selected interval " << endl;
      inter.print(true);
      cout << "# whose cost is " << cost << endl;
    }
    flt segmentedcost = cost;
    int i = inter.start;
    flt currentcost;
    for (int j = inter.start; j < inter.end; ++j) {
      currentcost = squaredfiterror(buffer, inter.start, j, 0) +
                    squaredfiterror(buffer, j, inter.end, 0);
      if (currentcost < segmentedcost) {
        segmentedcost = currentcost;
        i = j;
      }
    }
    if ((!(cost > segmentedcost)) || (i == inter.start)) {
      if (verbose) {
        cout << "#I'm bailing out early, best I can do is " << segmentedcost
             << endl;
      }
      break; // early
    }
    budgetleft -= 1;
    if (verbose) {
      cout << "#I choose to split the segment at " << i << " new cost will be "
           << segmentedcost << endl;
    }
    p = tags.insert(
        ++p, pair<flt, interval>(squaredfiterror(buffer, i, inter.end, 0),
                                 interval(i, inter.end, 0)));
    --p;
    p->second.end = i;
    p->second.degree = 0;
    p->first = squaredfiterror(buffer, inter.start, i, 0);
  }
  list<interval> answer(0);
  flt error = 0.0;
  for (fltintervallist::iterator p = tags.begin(); p != tags.end(); ++p) {
    error += p->first;
    answer.push_back(p->second);
  }
  answer.sort();
  return pair<list<interval>, flt>(answer, error);
}

/***
* Delete me!!!!!!!
*/
pair<list<interval>, flt> solveTopDownSegmentationLinear(dparray &data, int k,
                                                         bool verbose = 0) {
  int degree_upperbound = 2;
  matrix3f buffer(computeBuffer(data, degree_upperbound));
  typedef list<pair<flt, interval> > fltintervallist;
  fltintervallist tags(0);
  tags.push_back(pair<flt, interval>(squaredfiterror(buffer, 0, data.size(), 1),
                                     interval(0, data.size(), 1)));
  int budgetleft = k - 2;
  while (budgetleft > 1) {
    // first, we identify the most costly segment
    fltintervallist::iterator p = max_element(tags.begin(), tags.end());
    flt cost = p->first;
    interval inter = p->second;
    if (verbose) {
      cout << "# selected interval " << endl;
      inter.print(true);
      cout << "# whose cost is " << cost << endl;
    }
    flt segmentedcost = cost;
    int i = inter.start;
    flt currentcost;
    for (int j = inter.start; j < inter.end; ++j) {
      currentcost = squaredfiterror(buffer, inter.start, j, 1) +
                    squaredfiterror(buffer, j, inter.end, 1);
      if (currentcost < segmentedcost) {
        segmentedcost = currentcost;
        i = j;
      }
    }
    if ((!(cost > segmentedcost)) || (i == inter.start)) {
      if (verbose) {
        cout << "#I'm bailing out early, best I can do is " << segmentedcost
             << endl;
      }
      break; // early
    }
    budgetleft -= 2;
    if (verbose) {
      cout << "#I choose to split the segment at " << i << " new cost will be "
           << segmentedcost << endl;
    }
    p = tags.insert(
        ++p, pair<flt, interval>(squaredfiterror(buffer, i, inter.end, 1),
                                 interval(i, inter.end, 1)));
    --p;
    p->second.end = i;
    p->second.degree = 1;
    p->first = squaredfiterror(buffer, inter.start, i, 1);
  }
  list<interval> answer(0);
  flt error = 0.0;
  for (fltintervallist::iterator p = tags.begin(); p != tags.end(); ++p) {
    error += p->first;
    answer.push_back(p->second);
  }
  answer.sort();
  return pair<list<interval>, flt>(answer, error);
}

dparray getpoints(istream &in, bool blank = false) {
  dparray datapoints(0);
  float x, y;
  char c;
  while (in) {
    if (blank)
      in >> x >> y;
    else
      in >> x >> c >> y;
    // cout << x << " " << y << endl;
    if (in.good())
      datapoints.push_back(p(x, y));
  }
  // float curmin = datapoints[0][0] ;
  // float curmax = datapoints[datapoints.size()-1][0];
  /*if(curmax - curmin > 0.00001)
          for(uint k = 0; k< datapoints.size() ; ++k) {
                  datapoints[k][0] = (datapoints[k][0] - curmin) / (curmax -
     curmin) ;
      }*/
  if ((datapoints[datapoints.size() - 1][0] > 1000.0) ||
      (datapoints[0][0] < -1000.0)) {
    cerr << "==============================" << endl;
    cerr << "Warning: better normalize the x values to be between 0 and 1 for "
            "numerical stability purposes."
         << endl;
    cerr << "THIS CODE MAY GIVE YOU CRAP" << endl;
    cerr << "==============================" << endl;
    cerr << " first x value = " << datapoints[0][0] << endl;
    cerr << " last x value = " << datapoints[datapoints.size() - 1][0] << endl;
  }
  return datapoints;
}

void testdp() {
  // we just make sure it doesn't crash!
  dparray datapoints(0);
  datapoints.push_back(p(1.0, 1.0));
  datapoints.push_back(p(2.0, 2.0));
  datapoints.push_back(p(3.0, 1.0));
  printm<dparray>(datapoints);
  dynamicprogramming(datapoints, 7, 0, 3);
}
