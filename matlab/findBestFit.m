function bestfit = findBestFit(fastsumbuffer, start_index, end_index, degree)
% @fastsumbuffer : onetime Buffer computed by computeBuffer.m given the
%                  .f0table Pitch/Time Ascii Data
% @data : the .f0table data
% @start_index : time index of the begin of the data interval
% @end_index : time index of the end of the data interval
% @degree : maximum degree of the fitting polynom

% tested versus the c++ and python implementations, same results on data
% bestfit_py(fsb,st,en,deg) = findBestFit_mat(fsb,st + 1,end + 1,deg)

format long g
if (degree >= (end_index - start_index)) 
    %display('Degree is higher than needed. Cutting Degree.')
    %display(degree)
    %display(end_index)
    degree = end_index - start_index -1; 
    %display('Setze degree auf')
    %display(degree)
end

%normfactor = fastsum(fastsumbuffer,start_index,end_index,2*(degree+1)+1,1)

    matrix = zeros(degree + 1, degree +1); 
    for k = 1:(degree + 1)
        for j = 1:(degree + 1)
            matrix(j,k) = fastsum(fastsumbuffer,start_index,end_index,k+j-1,1); % / normfactor
        end
    end
    %matrix
    vector = zeros(1,degree + 1); %soll ja nen vektor sein
    for k = 1:(degree + 1)
        vector(k) = fastsum(fastsumbuffer,start_index,end_index,k,2); % / normfactor
    end
    %vector
    
    % x = bestfit, A = matrix, B = vector: 
    % A*x = B is solved directly, iff A is not singular, i.e. A is (n x n)
    % and rank(A) is n (singular = A has linear dependent rows or cols)
    % If A is singular, bestfit is the least-square solution, found by
    % minimizing the eucledean 2-norm |A*x-b|.
    % pinv means pseudo-inverse (moore-penrose).
    
    bestfit = pinv(matrix) * vector';