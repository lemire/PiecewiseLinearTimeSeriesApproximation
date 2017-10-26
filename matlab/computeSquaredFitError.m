function squaredFitError = computeSquaredFitError(fastsumbuffer, start_index, end_index, degree)
% @fastsumbuffer : onetime Buffer computed by computeBuffer.m given the
%                  .f0table Pitch/Time Ascii Data
% @data : the .f0table data
% @start_index : time index of the begin of the data interval
% @end_index : time index of the end of the data interval
% @degree : maximum degree of the fitting polynom
% This function computes the sum of the square of the Fit Error for the polynom
% found by findBestFit.m

% tested versus the c++ and python implementations, same results on data
% squaredfiterror_py(fsb,st,en,deg) = computeSquareFitError_mat(fsb,st + 1,end + 1,deg)

error = 0.0;

% compute sum( a*x^2 + b*x + c - y)^2, i.e. the distance / 2-norm of the
% difference of the fitting polyom and y

coefs = findBestFit(fastsumbuffer,start_index,end_index,degree);

coef_length = length(coefs);
for d1 = 1:1:coef_length
    for d2 = 1:1:coef_length
      error = error + coefs(d1) * coefs(d2) * fastsum(fastsumbuffer,start_index,end_index,d1+d2-1,1); % 0 --> 1
    end 
    error = error + coefs(d1) * (-1.0) * fastsum(fastsumbuffer,start_index,end_index,d1,2); % 1 --> 2
end
for d2 = 1:1:coef_length
    error = error +  (-1.0) * coefs(d2) * fastsum(fastsumbuffer,start_index,end_index,d2,2); % 1 --> 2
end
  error = error + fastsum(fastsumbuffer,start_index,end_index,1,3); % 0 --> 1, 2 --> 3
  if (error >= 1e-10)  %in python: error manchmal negativ, da unsinnig besser error = 0.0
      squaredFitError = error;
  else
    squaredFitError = 0.0;
  end