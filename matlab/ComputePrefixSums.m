function PS = ComputePrefixSums(ts,p,q,d)
% This function computes the minimum Fit Error by computing 
% polynomial fitting of a regression polynom of degree d over data points
% in the interval [x_p,x_q).
%
% See Part 4 and 5 from the paper
% 'A better alternative to piecewise linear time series segmentation'
% by Daniel Lemire
%
% input variables
% ts = timeseries, pitch data from f0table in a 'timeseries' matlab datatype
% p = start (time) index of the interval
% q = stop (time) index of the interval
% d = maximum degree of fitting function, 0 = constant, 1 = linear fitting
%
% return variables
% PS = Matrix of Prefix Sum Values

if isempty(ts)
    error('Error: Input Time Series is empty. Stop.')
else
    if (q < p) || (q <= 0) || (p <= 0) || (d < 0)
        error('ErrorTests:convertTest',...
            'Error: One of the following errors occurred:\nStart time > End Time.\nStart time <= 0.\nEnd time <= 0.\nFitting Polynom Degree is <= 0.\n')
    else
        %i = p:1:q; % i steps one by one through the interval from p to q
        y_mean = [];
        regr_coeff = [];
        for i=p:1:q
            y_mean(i) = mean(ts.Data(p:1:i,2)); %mean Pitch Value in the interval from p to i
            
            %Sum from p to q of (Pitch(i) - Mean_Pitch(i))^2
            %this is the constant polynom fit error
            PS(i,1) = sum( [(ts.Data(p:1:i,2) - y_mean(i)) .^ 2 ] );
            
            %linear case: sum form p to q of (Pitch(i) - (a*(Time(i) + b))^2
            %this is linear regression: first the regression coeffecients
            % if p == i, then there is only one data point available, so no
            % regression line can be found.
            if i ~= p
                tmp = polyfit(ts.Data(p:1:i,1), ts.Data(p:1:i,2), 1);
                regr_coeff(i,:) = tmp;
                
                %then the linear polynom
                %sum i=p to q of (Pitch(i) - (a* Time(i) +b)).^2
                %Pitch(p:i) = (ts.Data(p:1:i,2); Time(p:i) = ts.Data(p:1:i,1)
                PS(i,2) = sum([(ts.Data(p:1:i,2) - (regr_coeff(i,1) * ts.Data(p:1:i,1) + regr_coeff(i,2))).^2]);
            end
            
            
           %regr_coeff(i,1)
            
        end
        
        %for testing:
        %x = [1:1:10; 10*rand(1,10)];
        %xt = xt = timeseries(x');
        %ComputePrefixSums(xt,1,10,1);
        %output should be similar...
        %y_mean
        %regr_coeff
        %display 'Länge von PräfixSummenVektors PS:', length(PS)
        %display 'Länge von RegressionsKoeffizientenVektors regr_coeff:',length(regr_coeff)
        %PS
    end
end

