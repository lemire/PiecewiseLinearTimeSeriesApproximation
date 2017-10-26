#include "multidegree.h"
#include <vector>
#include <list>
#include <cmath>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

using namespace std;

typedef double flt;
typedef vector<vector<flt> > dparray;
typedef vector<flt> point;

int main(int params, char ** args) {

bool verbose = true;

//string filename = "/homes/dwesthue/dominik/develop/lemire/pitch_mit_nullen.test";
string filename = "/homes/dwesthue/dominik/develop/lemire/pitch_5.test";
cout << filename << endl;

//why 7? / = 2*3 +1, but degree = 2 means flat + linear, so 2*2+1 = 5. (3 = quadratic...?)
//int k = 7;
//k = Modelcomplexity
int k = 7;
int degreemin = 0;
int degree = 2;
	dparray datapoints;
	ifstream filein (filename.c_str());
	datapoints = getpoints(filein); //daniel's way

	cout << "Number of Datapoints read: " << datapoints.size() << endl;
	filein.close();
	
//	matrix3f fastsumbuffer = computeBuffer(datapoints,degree);
//	
//	float fast_sum = fastsum(fastsumbuffer,2,5,2,2);
//
//	cout << "Fastsum: " << fast_sum << endl;
//	vectorf bf = bestfit(fastsumbuffer,2,5,degree);
//
//	cout << "BestFit: " << endl; 
//	printv(bf,0);
//	
//	cout << "SquaredFitError: " << squaredfiterror(fastsumbuffer,0,4,1) << endl;
//   
//  
//	ResultOfDynProg rdp = dynamicprogramming(datapoints, k,degreemin, degree,verbose);
//	cout << "rdp" << endl;
//	rdp.print();
//  
//	pair<list<interval>,flt> answer = getSegmentsFromDynProg(rdp);
//  
//  
//		for(list<interval>::iterator it = answer.first.begin(); it != answer.first.end(); it++){
//			(*it).print(0);
//		}
//		cout << "error: " << answer.second << endl;
//		
//		
//		//----------GREEDY
//		
//		fltintervallist tags(0);
//		tags.push_back(pair<flt,interval>( 
//		                       squaredfiterror(fastsumbuffer,0,datapoints.size(),degreemin),
//		                       interval(0,datapoints.size(),degreemin)
//				  ) );		
//		
//		int budgetleft = k -2;
//
//		int extrabudget = 0;
//		flt greedy_savings = greedy(fastsumbuffer,tags,degreemin,degree,budgetleft,verbose,extrabudget);
//		cout << "greedy_savings: " << greedy_savings << endl;
//		
//		bool changetags = false; 
//		fltintervallist::iterator iterator = tags.begin();
//		
//		flt splitlinear1_savings = __splitlinear(fastsumbuffer, tags, iterator,verbose,changetags) ;
//		cout << "splitlinear1_savings: " << splitlinear1_savings << endl;
//		
//		flt splitlinear2_savings = splitlinear(fastsumbuffer, tags, verbose);
//		cout << "splitlinear2_savings: " << splitlinear2_savings << endl;
//		
		
		
		
		
		
		
		
		
		
		pair<list<interval>, flt> STDSH1 = solveTopDownSegmentationHeuristic1(datapoints,k,1);
		cout << "STDSH1 error: " << STDSH1.second << endl;
		for (list<interval>::iterator it = STDSH1.first.begin(); it != STDSH1.first.end(); it++){
			(*it).print(1);
		}
		
//
		//		fltintervallist tags(0);
		//		tags.push_back(pair<flt,interval>( 
		//		                          squaredfiterror(fastsumbuffer,0,datapoints.size(),degreemin),
		//		                          interval(0,datapoints.size(),degreemin)
		//		  ) );
//		
//		fltintervallist::iterator p = max_element(tags.begin(),tags.end());
//			flt cost = p->first;
//			interval inter = p->second;
//			if(verbose) {
//				cout << " selected interval "<< endl;
//				inter.print(true);
//				cout << " whose cost is " << cost << endl;
//			}
//			flt segmentedcost = cost;
//			int degree1 = 0;
//			int i = inter.start;
//			int bestextra = 0;
//			flt currentcost;
//			
//			int extrabudget = 0;
//			int degree_minvalue = degreemin;
//			int degree_upperbound = degree;
//			matrix3f buffer = fastsumbuffer;
//		
//		for (int extra = 0; extra <= extrabudget; ++ extra) {
//				// totalbudget is the sum of the degrees of freedom
//				int totalbudget = inter.degree+degree_minvalue+extra;
//				//cout << "totalbudget: " << totalbudget << endl;
//				if( budgetleft < degree_minvalue+extra+1) continue;
//				for(int d = degree_minvalue; d < degree_upperbound;++d) {
//					int otherdegree = totalbudget-d; 
//					cout << "otherdegree: " << otherdegree << endl;
//					if( ( otherdegree >= degree_upperbound) || (otherdegree < degree_minvalue) ) continue;
//					for(int  j =inter.start; j <inter.end;++j) {
//						currentcost = squaredfiterror(buffer,inter.start,j,d)+squaredfiterror(buffer,j,inter.end,otherdegree);
//						cout << "currentcost: " << currentcost << endl;
//						if(currentcost < segmentedcost ) {
//							segmentedcost = currentcost;
//							degree1 = d;
//							i = j;
//							bestextra = extra;
//							cout << "bestextra: " << bestextra << endl;
//						}
//					}
//				}
//			}
//		int degree2 = inter.degree-degree1 + degree_minvalue + bestextra;
//		cout << "degree2: " << degree2 << endl;
//			if(inter.degree < degree_upperbound-1) {
//				// here we should try to apply our extra budget, but it is not very useful in practice (never go quadratic)
//				flt increaseddegreecost = squaredfiterror(buffer,inter.start,inter.end,inter.degree+1);
//				cout << "increased degree cost: " << increaseddegreecost << endl;
//				if(increaseddegreecost < segmentedcost) {
//					if(verbose) {
//						cout << "I choose to increase the degree, new cost = "<< increaseddegreecost << endl;
//					}
//					p->second.degree += 1;
//					p->first = increaseddegreecost;
//					budgetleft -= 1;
//					cout << "budgetleft: " << budgetleft << endl;
//					cout << "savings: " << cost - increaseddegreecost << endl;
//					//return cost - increaseddegreecost;
//				}
//			}
//			if((! cost > segmentedcost)||(i==inter.start)) {
//					if(verbose) {
//						cout << "I'm bailing out early, best I can do is "<< segmentedcost<< endl;
//					}
//					//break;//early
//					//return 0.0;
//				}
//			budgetleft -= 1 + degree_minvalue + bestextra;
//			cout << "budgetleft: " << budgetleft << endl;
//			if(verbose) {
//				cout << "I choose to split the segment at " << i << " with degree1 = "<< degree1 << " and degree2 = " << degree2 << " new cost will be "<< segmentedcost << endl;
//			}
//			p = tags.insert(++p,pair<flt,interval>( 
//						squaredfiterror(buffer,i,inter.end,degree2),
//						interval(i,inter.end,degree2)
//				));
//			for (fltintervallist::iterator it = tags.begin(); it != tags.end(); it++){
//				cout << "p->first (error): " << it->first << endl;
//				cout << "p->second (interval): ";
//				it->second.print(1);
//			}
//			--p;
//			p->second.end = i;
//			p->second.degree = degree1;
//			p->first = squaredfiterror(buffer,inter.start,i,degree1);
//			for (fltintervallist::iterator it = tags.begin(); it != tags.end(); it++){
//				cout << "p->first (error): " << it->first << endl;
//				cout << "p->second (interval): ";
//				it->second.print(1);
//			}
//			cout << "savings: " << cost - segmentedcost << endl;
//			return cost - segmentedcost;
		//pair<list<interval>, flt> result = solveTopDownSegmentationFlat(datapoints,7,1);
		
		
		
}
