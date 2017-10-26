function fastsumbuffer = computeBuffer(datapoints, degree)
% computes the quickly computable part of the prefix sums,
% the onetime buffer
% @datapoints = .f0Table loaded to a vector
% @degree = maximum degree of fitting polynom, 2 means linear and constant,
% 1 means constant fitting polynoms only

format long g

if isempty(datapoints)
    error('Error: Input Data is empty. Stop.')
else
    if (degree <= 0)
        error('Error: Maximum degree of fitting polynom must be greater than 0.')
    else        
        n = length(datapoints);
        fastsumbuffer = zeros(3, n, 2*degree+1);
        fastsumbuffer(1:1:3,1,1:1:(2*degree+1)) = 0.0;
        %x_exp = 1:1:(2*degree +1);
        %y_exp = 1:1:3
        for x_exp = 1:1:(2*degree+1)
            for y_exp = 1:1:3
        
                %y_exp = 1:1:3;
                %fastsumbuffer(y_exp,1,x_exp) = 0.0;

                for x = 1:1:n
                    fastsumbuffer(y_exp,x+1,x_exp) = fastsumbuffer(y_exp,x,x_exp) + (datapoints(x,1) .^ (x_exp-1)) .* (datapoints(x,2) .^ (y_exp-1));
                end
            end
        end
    end
end




