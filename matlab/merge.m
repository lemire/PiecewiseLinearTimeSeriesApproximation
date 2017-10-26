function newcell = merge(leftcell,rightcell)
% merge part of mergesort


newcell = {};
while (~ isempty(leftcell) && ~ isempty(rightcell))
    if (leftcell{1}{1} <= rightcell{1}{1})
        newcell{end+1} = leftcell{1};
        leftcell = leftcell(2:end);
    else
        newcell{end+1} = rightcell{1};
        rightcell = rightcell(2:end);
    end
end

while (~ isempty(leftcell))
    newcell{end+1} = leftcell{1};
    leftcell = leftcell(2:end);
end

while (~ isempty(rightcell))
    newcell{end+1} = rightcell{1};
    rightcell = rightcell(2:end);
end