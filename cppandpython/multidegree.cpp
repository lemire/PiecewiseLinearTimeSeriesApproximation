#include "multidegree.h"

int main(int params, char **args) {
  bool unitflag = false;
  bool verbose = false;
  bool stdinflag = true;
  bool outputpoints = true;
  bool topdown = false;
  bool topdownflat = false;
  bool topdownlinear = false;
  bool showdatapoints = false;
  bool alternate = false;
  bool fromfile = false;
  bool heuristic1 = false;
  bool heuristic2 = false;
  bool blank = false;
  string filename;
  int k = 7;
  int degreemin = 0;
  int degree = 3;
  for (int i = 1; i < params; ++i) {
    if (strcmp(args[i], "--nostdin") == 0) {
      stdinflag = false;
      continue;
    }
    if (strcmp(args[i], "-u") == 0) {
      unitflag = true;
      continue;
    }
    if (strcmp(args[i], "-v") == 0) {
      verbose = true;
      continue;
    }
    if (strcmp(args[i], "--verbose") == 0) {
      verbose = true;
      continue;
    }
    if (strcmp(args[i], "--dynprog") == 0) {
      outputpoints = false;
      continue;
    }
    if (strcmp(args[i], "--topdown") == 0) {
      topdown = true;
      continue;
    }
    if (strcmp(args[i], "--alternate") == 0) {
      alternate = true;
      continue;
    }
    if (strcmp(args[i], "--topdownflat") == 0) {
      topdownflat = true;
      topdown = true;
      degree = 1;
      continue;
    }
    if (strcmp(args[i], "--heuristic1") == 0) {
      topdown = true;
      heuristic1 = true;
      degree = 2;
      continue;
    }
    if (strcmp(args[i], "--heuristic2") == 0) {
      topdown = true;
      heuristic2 = true;
      continue;
    }
    if (strcmp(args[i], "--topdownlinear") == 0) {
      topdownlinear = true;
      degree = 2;
      topdown = true;
      continue;
    }
    if (strcmp(args[i], "--showdatapoints") == 0) {
      showdatapoints = true;
      continue;
    }
    if (strcmp(args[i], "-d") == 0) {
      if (params - i > 1)
        degree = atoi(args[++i]);
      else {
        cerr << "-d expects an integer" << endl;
        return -1;
      }
      continue;
    }
    if (strcmp(args[i], "-dmin") == 0) {
      if (params - i > 1)
        degreemin = atoi(args[++i]);
      else {
        cerr << "-dmin expects an integer" << endl;
        return -1;
      }
      continue;
    }
    if (strcmp(args[i], "-k") == 0) {
      if (params - i > 1)
        k = atoi(args[++i]);
      else {
        cerr << "-k expects an integer" << endl;
        return -2;
      }
      continue;
    }

    if (strcmp(args[i], "-s") == 0) {
      blank = true;
      continue;
    }
    if (strcmp(args[i], "-i") == 0) {
      fromfile = true;
      if (params - i > 1)
        filename = args[++i];
      else {
        cerr << "-i expects a filename" << endl;
        return -2;
      }
      continue;
    }
    if (strcmp(args[i], "--help") == 0) {
      cout << "Dynamic Programming Multidegree Segmentation Solver" << endl;
      cout << "(c) 2006 by Daniel Lemire " << endl << endl;
      cout << "I will read the data points (x,y) from standard" << endl;
      cout << "input as comma separated points," << endl;
      cout << "one pair per line. " << endl << endl;
      cout << "-u for unit testing" << endl;
      cout << "-v for verbose " << endl;
      cout << "-s accept blank-separated data points " << endl;
      cout << "-k 2 to give a budget of 2 " << endl;
      cout << "-d 2 to allow degrees 0 and 1 " << endl;
      cout << "-dmin 1 would exclude flat segments " << endl;
      cout << "--topdown runs the top-down approximate algo " << endl;
      cout << "--topdownflat runs the top-down flat approximate algo " << endl;
      cout << "--topdownlinear runs the top-down linear approximate algo "
           << endl;
      cout << "--nostdin disables reading data from stdin" << endl;
      cout << "-i gives a file from which to read the data" << endl;
      cout << "      from stdin " << endl;
      cout << "--dynprog disables the data points output and replace by dyn. "
              "prog. output"
           << endl;
      cout << "--showdatapoints prints out the data points I read (for "
              "debugging)"
           << endl;
      return 0;
    }
    cerr << " I don't recognize flag " << args[i] << ". Please try --help!"
         << endl;
  }
  cout << "# result is formatted as start data point, end data point, and "
          "degree of the poly, each line is a segment"
       << endl;
  if (unitflag)
    unit(verbose);
  if (stdinflag || fromfile) {
    dparray datapoints;
    if (fromfile) {
      ifstream filein(filename.c_str());
      if (filein) {
        datapoints = getpoints(filein, blank);
        filein.close();
      } else {
        cerr << " Could not open " << filename << endl;
      }
    } else
      datapoints = getpoints(cin, blank);
    if (verbose)
      cout << "# read " << datapoints.size() << " data points" << endl;
    if (showdatapoints)
      printm<dparray>(datapoints);
    if (!topdown && !outputpoints) {
      ResultOfDynProg rdp =
          dynamicprogramming(datapoints, k, degreemin, degree, verbose);
      rdp.print();
      return 0;
    }
    pair<list<interval>, flt> answer;
    if (topdown) {
      if (heuristic1)
        answer = solveTopDownSegmentationHeuristic1(datapoints, k, verbose);
      else if (heuristic2)
        answer = solveTopDownSegmentationHeuristic2(datapoints, k, verbose);
      else if (topdownflat)
        if (alternate)
          answer = solveTopDownSegmentationFlat(datapoints, k, verbose);
        else
          answer = solveTopDownSegmentation(datapoints, k, 0, 1, verbose);
      else if (topdownlinear)
        if (alternate)
          answer = solveTopDownSegmentationLinear(datapoints, k, verbose);
        else
          answer = solveTopDownSegmentation(datapoints, k, 1, 2, verbose);
      else
        answer = solveTopDownSegmentation(datapoints, k, 0, degree, verbose);
    } else {
      ResultOfDynProg rdp =
          dynamicprogramming(datapoints, k, degreemin, degree, verbose);
      answer = getSegmentsFromDynProg(rdp);
    }
    list<interval> tags = answer.first;
    flt error = answer.second;
    if (true) {
      // we do a little check
      int observedk = 0;
      for (list<interval>::iterator i = tags.begin(); i != tags.end(); ++i)
        observedk += 1 + i->degree;
      if (observedk != k)
        cout << "# observedk = " << observedk << ", k = " << k << endl;
      // assert (observedk == k);
    }
    cout << "# error " << error << endl;
    cout << "# (l2 ** 2 )" << endl;
    printlist<interval>(tags, false);
  }
  return 0;
}
