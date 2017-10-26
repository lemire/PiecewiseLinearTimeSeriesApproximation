function [R,P,D] = getCostDegreeSegmentation(datapoints,k,degree_min,degree_max, verbose)
% variables:
% ts = TimeSeries
% k = model complexity
% degreemin = minimum degree of fitting polynom, 
%             =1: constant; 
%             =2: linear + constant
% degreemax = maximum degree od fitting polynom
% verbose: print information about what is how computed
%
% return:
% R : Cost Matrix, contains the "optimal cost" for the segmentation
% D : Degree Matrix, tells you where to move to (starting from the end column)
% P : Segmentation Matrix, tells you how many rows to move up.
%
% This function represents 'Algorithm 1' from the paper
% 'A better alternative to piecewise linear time series segmentation'
% by Daniel Lemire

% asymptotical computation time for the whole dynamic algorithm is O(n^2),
% n = # of datapoints

format long g

if isempty(datapoints)
    error('Error: Empty list of datapoints. Stop.')
else
    n = length(datapoints); 
    if (k <= 0) || (degree_max < 0) 
        error('ErrorTests:convertTest',...
            '#Error: One of the following errors occurred:\n#Model Complexity is < 0?\n#Maximum degree of fitting polynom is < 0?\n')
    else
        if (degree_min > 1) 
            display('#[WARNING]: degree_min can give wrong results for some k.')
        end
        if (verbose) 
            display('#Computing Buffer')
        end
        fastsumbuffer = computeBuffer(datapoints,degree_max); 
        if (verbose)
            display('#Computing Buffer: completed.')
        end
        R = zeros(k,n+1); %double matrix
        D = zeros(k,n+1); %int matrix
        P = zeros(k,n+1); %int matrix
        
        %r = 0:1:k-1; % r scans the rows of these three matrices
        %q = 0:1:n    % q scans the columns of these three matrices
        
        for row = 1:1:k
            if (verbose)
                row
            end
            for x = 2:1:n+1 
                R(row,x) = Inf;
                for deg = degree_min:1:min(row, degree_max)-1 %cpp: row+1
                     end_of_z = x+1;
                     %end_of_z
%                     assert = (row-1-deg >= 1);
%                     if (verbose)
%                         display(assert) 
%                     end
                    if (row-1-deg == 0) %cpp: -1 
                        end_of_z = 2; %cpp: 1
                        %end_of_z
                    end
                     rowindex = max(row-1-deg, 1); %cpp:(...,0)
                     %rowindex
                     for z = 1:1:(end_of_z -1) %cpp: 0:1:end_of_z
                         value = R(rowindex,z)+ computeSquaredFitError(fastsumbuffer,z,x,deg); %hier mal die Value Matrix, R Matrix, CSfE MAtrix einzeln aufstelln und ausgeben lassen
                         if (R(row,x) > value) 
                             R(row,x) = value;
                             P(row,x) = z; %so P_c++(x,y) = P_matlab(x+1,y+1)+1
                             D(row,x) = deg;
                         end
                     end
                end
            end
        end
    end
end

