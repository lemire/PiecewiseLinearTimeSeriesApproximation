function pair_cell_new = insertPairIntoPairCell(pair,index,pair_cell)
% this function inserts a given pair {error,interval = {start,end,degree}}
% into a given pair_cell at a given position



%if (isempty(pair_cell))
%    error('insertPairIntoPairCell() Error: empty pair_cell. Stop.')
%end

if (index > length(pair_cell)+1)
   % error('insertPairIntoPairCell() Error: index > length(pair_cell),')
end

tmp = {};

% copy pairs from pair_cell array from given index to end to a tmp cellarr
if (index == length(pair_cell)+1)
    pair_cell{index}{1} = pair{1};
    pair_cell{index}{2} = pair{2};
    
    %used for triples instead of pairs
    if (length(pair) == 3)
       pair_cell{index}{3} = pair{3}; 
    end
elseif(index < length(pair_cell)+1)
    for i = index:1:length(pair_cell)
        tmp{i-index+1}{1} = pair_cell{i}{1};
        tmp{i-index+1}{2} = pair_cell{i}{2};
        
        %used for triples instead of pairs
        if (length(pair_cell{i}) == 3)
            tmp{i-index+1}{3} = pair_cell{i}{3};
        end
    end
end

% insert given pair at given index
pair_cell{index}{1} = pair{1};
pair_cell{index}{2} = pair{2};

%used for triples instead of pairs
if (length(pair) == 3)
    pair_cell{index}{3} = pair{3};
end

% insert pairs from tmp to pair_cell array
for j = 1:1:length(tmp)
    pair_cell{index+j}{1} = tmp{j}{1};
    pair_cell{index+j}{2} = tmp{j}{2};
    %used for triples instead of pairs
    if (length(tmp{j}) == 3)
        pair_cell{index}{3} = tmp{j}{3};
    end
end

% return new cell_array with inserted pair
pair_cell_new = pair_cell;


