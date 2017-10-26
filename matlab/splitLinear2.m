function finalsavings_tags = splitLinear2(fastsumbuffer, tags, verbose)
% this function searches for the optimal parameters for splitLinear1
% so the optimal pair_and_pos, and returns the costs that are saved
% by finding intervals that were formerly fitted linear and can
% be fitted constantly with reducing the costs.

% @fastsumbuffer = OnetimeBuffer computed from the datapoints by
%                  computeBuffer.m
% @tags = cell array: {{error,interval={s,n,d},{e,{s,n,d}},...}
% @verbose: bool for verbose output

% TODO: see below

%global tags;

if (isempty(tags))
    error('CellArray of (error,interval)s "Tags" is empty!')
end
if (isempty(fastsumbuffer))
    error('FastSumBuffer is empty!')
end

pair_and_pos = {tags{1},1};
bestsavings = 0.0;
besttags = {};

best_pair_and_pos = {{0.0,0.0,0.0},0.0};


for index = 1:1:length(tags)
    if (tags{index}{2}{3} == 1)
        %try to find the highest cost saved by splitting linear->degree=1
        %intervals into constant intervals. By now, don't change the
        %pair_cell 'tags'-->changetags=0
        savings_tags = splitLinear1(fastsumbuffer, tags, best_pair_and_pos, verbose, 0);
        savings = savings_tags{1};
        tags = savings_tags{2};
        if (savings >= bestsavings) %TODO: better > then >= ?
            bestsavings = savings;
            
            % save the pair and its position in the pair_cell array
            best_pair_and_pos = {tags{index},index};
        end
        
    end
end

% finally, when the pair with the highest cost (that can be saved by
% splitting) is found, do the split an add the two new pairs into the list

finalsavings_tags = splitLinear1( fastsumbuffer, tags , best_pair_and_pos, verbose, 1);
