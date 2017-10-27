
degree = 2;
k = 7; %model complexity
degreemin = 0; 

%load the data
datapoints = load('pitch_5.test')

% compute the FastSum Buffer
fastsumbuffer = computeBuffer(datapoints,degree)

% compute the range sum
fastsum_new = fastsum(fastsumbuffer,3,6,3,3)

%compute the bestFit Matrix/Vector
bestfit = findBestFit(fastsumbuffer,3,6,degree)

%Algorithm 1: R = Cost, P = Segm., D = Degree
[R,P,D] = getCostDegreeSegmentation(datapoints,k,0,degree,0)

%Algorithm 2: 
tags_rdp = getOptimalAdaptiveSegmentation(R,D,P);
solution = tags_rdp{1}
error = tags_rdp{2}

global tags;
tags = {{computeSquaredFitError(fastsumbuffer,0+1,length(datapoints)+1,degreemin),{0,length(datapoints),degreemin}}};

% global --> int&
global budgetleft;
budgetleft = k - 2;

verbose = 1;
extrabudget = 0;

savings_budgetleft_tags = greedy(fastsumbuffer,tags,degreemin,degree,budgetleft,verbose,extrabudget);
greedy_savings = savings_budgetleft_tags{1}
budgetleft = savings_budgetleft_tags{2}
tags = savings_budgetleft_tags{3}


changetags = 0; %bool --> false
pair_and_pos = {tags{1},1}; %so equal to an iterator that points to tags.begin()

splitlinear1_savings_tags = splitLinear1(fastsumbuffer, tags, pair_and_pos, verbose, changetags);
splitlinear1_savings = splitlinear1_savings_tags{1}

splitlinear2_savings_tags = splitLinear2(fastsumbuffer, tags, verbose);
splitlinear2_savings = splitlinear2_savings_tags{1}
verbose = 1;
answer_error = solveTopDownSegmentationHeuristic1(datapoints, k, verbose);
display('answer_error{2}:')
answer_error{2}

for index = 1:1:length(answer_error{1})
   answer_error{1}{index}
end
