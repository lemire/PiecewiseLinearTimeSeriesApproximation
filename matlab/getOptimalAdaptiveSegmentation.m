function tags  = getOptimalAdaptiveSegmentation(cost_matrix,degree_matrix,segmentation_matrix)
% @cost, degree,segmentation_matrix =  Matrices computed by
% getCostDegreeSegmentation.m, R = cost, D = degree, P = Segmentation
% 
% This algorithm computes from the (k x (n+1)) matrices R,D,P the optimal
% adaptive segmentation for the datapoints. For more Details see
% "a better alternative to piecewise linear timeseries segmentation"
% by Daniel Lemire.
%
% Return: the optimal solution

format long g

startrow = 1;
n = length(cost_matrix(1,:)); % 9 datapoints => n = 10

% modelkomplexity = 5 => m = 5
% getCostDegreeSegmentation(datapoints,k = 5,degree_min,degree_max, verbose)
m = length(cost_matrix(:,1)); 
for row = 1:1:m
    if(cost_matrix(row,n) + (n-1)*(1e-7) < cost_matrix(startrow,n)) 
        %display('true for:')
        %[row,n,cost_matrix(row,n) + (n-1)*(1e-7)]
        startrow = row;
    end
end
x = n;
row = startrow;
%row

% c++: solution = interval(startindex, endindex, degree), just three ints.
solution = zeros(3);

nextpoint = 0;
degree = 0;
counter = 1;
while ((row >= 1) && (x > 1))
    nextpoint = segmentation_matrix(row,x);
    degree = degree_matrix(row,x);
    row = row - (degree +1);
    %display('[nextpoint, x, degree, row]')
    %[nextpoint, x, degree, row]
    
    solution(counter,:) = [nextpoint, x, degree];
    counter = counter +1;
    x = nextpoint; %gerechtfertigt??? +1
end

%entspricht reverse(solution) in c++
solution = solution(end:-1:1,1:1:end);

error = cost_matrix(startrow,n);

tags = {solution, error};