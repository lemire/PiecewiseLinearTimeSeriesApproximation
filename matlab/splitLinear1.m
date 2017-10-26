function savings_tags = splitLinear1(fastsumbuffer, tags, pair_and_pos, verbose, changetags)
% this function tries to split linear fitted intervals into two constant fitted
% intervals without increasing the fit error

% @fastsumbuffer = OnetimeBuffer computed from the datapoints by
%                  computeBuffer.m
% @tags = cell array: {{error,interval={s,n,d},{e,{s,n,d}},...}
% @pair_and_pos: a cell in the role of an iterator p: contains position and
%                a pair {error,interval}
% @verbose: bool for verbose output
% changetags: bool

% there are if (position==0) conditions, because position=0 means
% that splitLinear2 could not find an interval to split

%global tags;
savings_tags = {};

pair = pair_and_pos{1};
position = pair_and_pos{2};
if (position == 0.0)
    cost = 0.0;
else
    cost = tags{position}{1};
end
segmentedcost = cost;
if (position == 0.0)
    interval = {0,0,0};
else
    interval = tags{position}{2};
end
if(verbose)
	display('# Selected segment ranging from [start,end,cost] :')
    [interval{1}, interval{2},cost]
end

i = interval{1};
currentcost = 0.0;
    
for j =interval{1}:1:interval{2}-1 % -1 correct???
    currentcost = computeSquaredFitError(fastsumbuffer,interval{1}+1,j+1,0) + computeSquaredFitError(fastsumbuffer,j+1,interval{2}+1,0);

    %display('[j, currentcost]')
    %[j, currentcost]
   
		if ( currentcost < segmentedcost ) 
			segmentedcost = currentcost;
			i = j;
        end
end
%display('cost,segmentedcost');
%[cost, segmentedcost];
if( cost <= segmentedcost ) 
		if (verbose)
			display('# Could not find a way to improve!')
        end
		savings = 0.0;
        savings_tags{1} = savings;
        savings_tags{2} = tags;
        %return   
elseif (verbose)
		display('# Found a way to save ') 
        cost - segmentedcost
end

if( changetags && position > 0) 
	if(verbose) 
        display('# actually changing the data ')
    end
    error = computeSquaredFitError(fastsumbuffer,i+1,interval{2}+1,0);
    
    new_pair = {error,{i,interval{2},0}};
    tags = insertPairIntoPairCell(new_pair,position+1,tags);


    %for (a = 1:1:length(tags))
    %    display('[PairNr, error, start, end, degree]')
    %    [a,tags{a}{1},tags{a}{2}{1},tags{a}{2}{2},tags{a}{2}{3}]
    %end
    
    new_error = computeSquaredFitError(fastsumbuffer,interval{1}+1,i+1,0);
    
    tags{position}{1} = new_error;
    tags{position}{2}{2} = i;
    tags{position}{2}{3} = 0;

    
    %for (a = 1:1:length(tags))
    %    display('[PairNr, error, start, end, degree]')
    %    [a,tags{a}{1},tags{a}{2}{1},tags{a}{2}{2},tags{a}{2}{3}]
    %end
    
    %tags
end
	savings = cost - segmentedcost; 
    savings_tags{1} = savings;
    savings_tags{2} = tags;