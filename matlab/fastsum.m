function fastsumbuffer = fastsum(fastsumbuffer, start_index, end_index, x_exp, y_exp)
% computes any range sum in constant time
% @fastsumbuffer = onetimebuffer precomputed by computeBuffer.m
% @start = starting index of the range sum
% @end = end index of the range sum
% @x_exp = the exponent of x (Time Value) in the range sum
% @y_exp = the exponent of y (Pitch Value) in the range sum

% results are comparable to the python and c++ implementation
% if fastsum_py(fsb,st,en,xe,ye) = fastsum_matlab(fsb,st+1,en+1,xe+1,ye+1)
% (liegt an arraygrenzen).

if (isempty(fastsumbuffer))
    error('Error. FastSumBuffer, the precomputed one time buffer, is empty.')
else
    % the maximal value of x_exp, as definend in computeBuffer.m:
    % is 2*degree + 1 (one-time-buffer dimension for x_exp)
    x_exp_max = length(fastsumbuffer(:,1,:));
    if (x_exp > x_exp_max)
        display('The x_exp, the exponent of x (Time Value), is too large:')
        display(x_exp)
        display('Maximum Value allowed is 2*(maximum degree of polynom) + 1:')
        display(x_exp_max)
        error('Error.')
    else
        % the maximal value of y_exp, as definend in computeBuffer.m:
        % it is defined as 3
        y_exp_max = length(fastsumbuffer(:,1,1));
        if (y_exp > y_exp_max)
            display('The y_exp, the exponent of y (Pitch Value), is too large:')
            display(y_exp)
            display('Maximum Value allowed should be 3 and is:')
            display(y_exp_max)
            error('Error.')
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
                x_max = length(fastsumbuffer);
                if (end_index > x_max)
                    display('End_index:')
                    display(end_index)
                    display('Maximum value of the Pitch/Time Pairs read in OneTimeBuffer:')
                    display(x_max)
                    error('Error. (End_)Index out of bounds')
                else
                    fastsumbuffer = fastsumbuffer(y_exp,end_index,x_exp) - fastsumbuffer(y_exp,start_index,x_exp);
                end
            end
        end
    end
end


