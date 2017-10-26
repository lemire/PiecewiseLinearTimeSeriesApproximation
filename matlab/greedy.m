function savings_budgetleft_tags = greedy(buffer, tags, degree_min, degree_max, budgetleft, verbose, extrabudget)
% a greedy heuristic used by solveTopDownSegmentation*** Scripts
% @buffer = 3D matrix for computeBuffer.m output (needed for a fastsum)
% @tags = data structure of pair_cell:
%         interval = cell(1,3) = { 0, 9, 0} (interval_mat = interval_c++, no +1)                 
%         pair_error_interval = cell(1,2) = { error, interval }
% @degree_min: minimal degree (double)
% @degree_max: maximum degree (double)
% budgetleft: double, must be GLOBAL
% verbose: 0, or 1 for more output
% extrabudget: double

% first, identify the segment with the highest costs

%global tags;



[pair,position] = findPairWithMaxError(tags);
savings = 0.0;
savings_budgetleft_tags = {savings,budgetleft,tags};

cost = pair{1};
interval = pair{2};
if (verbose)
    display('selected interval:')
    interval
    display('whose cost is:')
    cost
end

segmentedcost = cost;
degree1 = 0;
i = interval{1};
bestextra = 0;
currentcost = 0;

for extra = 0:1:extrabudget
    %totalbudget is the sum of the degrees of freedom
    totalbudget = interval{3} + degree_min + extra;
    %totalbudget

    if (budgetleft < degree_min + extra +1)
        continue;
    end
    for d = degree_min:1:degree_max-1
        otherdegree = totalbudget - d;
        %otherdegree
        if ((otherdegree >= degree_max) || (otherdegree < degree_min))
            continue;
        end
            for j = interval{1}:1:interval{2}-1
                currentcost = computeSquaredFitError(buffer,interval{1}+1,j+1,d) + computeSquaredFitError(buffer,j+1,interval{2}+1,otherdegree);
                %currentcost
                if(currentcost < segmentedcost )
                        segmentedcost = currentcost;
                        degree1 = d;
                        i = j;
                        bestextra = extra;
                        %bestextra
                end
            end
    end
end

degree2 = interval{3} - degree1 + degree_min + bestextra;
%degree2
if (interval{3} < degree_max -1)
    %here we should try to apply our extra budget, but it is not very useful in practice (never go quadratic)
    increaseddegreecost = computeSquaredFitError(buffer,interval{1}+1,interval{2}+1,interval{3}+1);
    %increaseddegreecost
    if(increaseddegreecost < segmentedcost)
        if (verbose)
            display('Greedy Algorithm chooses to increase the degree, new cost= ')
            increaseddegreecost
        end
        %tags{pos}{2}{3} is the degree of the interval
        tags{position}{2}{3} =  pair{2}{3} +1;
        tags{position}{1} = increaseddegreecost;
        budgetleft = budgetleft -1;
        %budgetleft
        savings = cost - increaseddegreecost; % RETURN
        savings_budgetleft_tags{1} = savings;
        savings_budgetleft_tags{2} = budgetleft;
        savings_budgetleft_tags{3} = tags;
        return
        %savings
    end
end
if (( cost <= segmentedcost) || (i == interval{1}))
    if (verbose)
        display('Algorithm is bailing out early, best result is: ')
        segmentedcost
    end
    savings = 0.0;
    savings_budgetleft_tags{1} = savings;
    savings_budgetleft_tags{2} = budgetleft;
    savings_budgetleft_tags{3} = tags;
    return
    % RETURN
end
budgetleft = budgetleft - (1 + degree_min + bestextra);
if (verbose)
    display('Algorithm chooses to split the segment at [index, degree1, degree2] ')
    [i,degree1,degree2]
    display('and newcost will be: ')
    segmentedcost
end

new_pair = {computeSquaredFitError(buffer,i+1,interval{2}+1,degree2),{i,interval{2},degree2}};
tags = insertPairIntoPairCell(new_pair,position+1,tags);
%[tags{1}{1},tags{1}{2}]
%[tags{2}{1},tags{2}{2}]
tags{position}{1} = computeSquaredFitError(buffer,interval{1}+1,i+1,degree1);
tags{position}{2}{2} = i;
tags{position}{2}{3} = degree1;
%[tags{1}{1},tags{1}{2}]
%[tags{2}{1},tags{2}{2}]
%tags

savings = cost - segmentedcost;

savings_budgetleft_tags{1} = savings;
savings_budgetleft_tags{2} = budgetleft;
savings_budgetleft_tags{3} = tags;

