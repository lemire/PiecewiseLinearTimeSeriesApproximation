function answer_and_error = solveTopDownSegmentationHeuristic1(datapoints, k, verbose)
% this function firstly computes a linear fitting and then tries to break
% down linear fitted intervals into 2 constant fitted intervals.

% asymptotical computation time is O(k*n), k = budget and n = # of
% datapoints..

fastsumbuffer = computeBuffer(datapoints,2);
tags = {{computeSquaredFitError(fastsumbuffer,0+1,length(datapoints)+1,1),{0,length(datapoints),1}}};
%global budgetleft;
budgetleft = k-2;

%first compute the linear top down segmentation
while(budgetleft > 0)
    savings_budgetleft_tags = greedy(fastsumbuffer, tags, 1, 2, budgetleft, verbose, 0); % ...,0,verbose) originally, but thats wrong
    savings = savings_budgetleft_tags{1};
    budgetleft = savings_budgetleft_tags{2};
    tags = savings_budgetleft_tags{3};
    if (verbose) 
        budgetleft
    end
    if (savings <= 0.0) 
        break
    end
end    
% now try to improve linear fitting
if(verbose) 
    display('# Now trying to improve on top down linear segmentation.')
end
while(1) 
	savings_tags = splitLinear2(fastsumbuffer,tags,verbose);
    savings = savings_tags{1};
    tags = savings_tags{2};
	if(verbose)
        budgetleft
    end
    if(savings <= 0.0)
        break;
    end
end
    
answer = {};
error = 0.0;

for index = 1:1:length(tags)
    error = error + tags{index}{1};
    tags{index}{2}
    answer = insertPairIntoPairCell(tags{index}{2},index,answer);
end


% sort the answer for correct sequence of intervals via mergesort

answer = mergesort(answer);

answer_and_error = {answer,error};


