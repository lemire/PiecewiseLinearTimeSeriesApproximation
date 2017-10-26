function cellarray_out = mergesort(cellarray_in)

if (length(cellarray_in) <= 1)
    cellarray_out = cellarray_in;
    return
else
    splitindex = floor(length(cellarray_in)/2);
    cellarray_out = merge(mergesort(cellarray_in(1:splitindex)),mergesort(cellarray_in(splitindex+1:end)));
    return
end
