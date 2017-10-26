## Documentation regarding the multidegree piecewise segmentation code
December 3rd 2007
Daniel Lemire



To use the C++ command line utility:

1) type ``make multidegree``
2) ``./multidegree --help``

Example
=======

This will process the x.txt file, show the data points read, and
segment using a budget of 10, with flat and linear segments:

`` ./multidegree --showdatapoints -k 10 -d 2 -s -i x.txt``

If this is the output:

        # error 0.0133986
        # (l2 ** 2 )
        0 9 0
        9 17 0
        17 21 1
        21 26 0
        26 29 1
        29 35 0
        35 51 0
        51 61 0


Then this means that we have a flat segment from data point 0 up to
data point 9, then another flat segment from data point 9 to data point
17, a linear segment from data point 17 to data point 21, and so on.


Unsupported: To plot your results, use the "segmentandplot.py" script as an example:
python segmentandplot.py x.txt 10
This assumes you have Python, Gnuplot, and Gnuplot-py.

Note that
data files passed to multidegree must end with an empty line. Thanks to
Dominik Westhues for pointing out this limitation.
