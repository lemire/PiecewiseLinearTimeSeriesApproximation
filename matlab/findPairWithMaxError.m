function [max_pair,position] = findPairWithMaxError(pair_cell)
% this function returns the (error,interval) pair with max error from a
% cell array of (error,interval) pairs.
%
% data structure of pair_cell:
% interval = cell(1,3) = { 0, 9, 0}
% pair_error_interval = cell(1,2) = { error, interval }
% pair_cell = {pair_error_interval}


if (isempty(pair_cell))
    error('findPairWithMaxError: empty pair_cell. Stop.')
end

tmp = 0.0;
index = 1;

for i = 1:length(pair_cell)
    if (pair_cell{i}{1} > tmp)
        tmp = pair_cell{i}{1};
        index = i;
    end
end

max_pair = pair_cell{index};
position = index;