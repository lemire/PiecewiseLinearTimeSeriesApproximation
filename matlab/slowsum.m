function sum = slowsum(datapoints, start_index, end_index, x_exp, y_exp)
% this computes a range sum the standard O(n) way
% variables same as in fastsum.m

% Caution: slowsum operates with DATAPOINTS, not with 
% FASTSUMBUFFER = computeBuffer(DATAPOINTS,DEGREE)

% @fastsumbuffer = onetimebuffer precomputed by computeBuffer.m
% @start = starting index of the range sum
% @end = end index of the range sum
% @x_exp = the exponent of x (Time Value) in the range sum
% @y_exp = the exponent of y (Pitch Value) in the range sum

if (isempty(datapoints))
    error('Error. FastSumBuffer, the precomputed one time buffer, is empty.')
else
    if (end_index < start_index)
                display('Start_index:')
                display(start_index)
                display('End_index:')
                display(end_index)
                error('Start_index is smaller than End_Index.')
    else
                % x_max represents the number of Time/Pitch Pair Values
                % read from the data, equivalently it represents the
                % maximum value of x as defined in computeBuffer.m
                x_max = length(datapoints)+1;
                if (end_index > x_max)
                    display('End_index:')
                    display(end_index)
                    display('Maximum value of the Pitch/Time Pairs read in OneTimeBuffer:')
                    display(x_max)
                    error('Error. (End_)Index out of bounds')
                else
                    sum = 0.0;
                    for x = start_index:1:(end_index-1);
                        sum = sum + (datapoints(x,1).^(x_exp-1) * (datapoints(x,2).^(y_exp-1)));
                    end
                end
    end
end
% fastsum and slowsum have the same output:
% test = load('.../VP001_...Becher.wav.f0table')
% onetime = computeBuffer(test, 2) 
% fastsum(onetime,1,6068,2,3) =
% slowsum(test,1,6068,2,3)