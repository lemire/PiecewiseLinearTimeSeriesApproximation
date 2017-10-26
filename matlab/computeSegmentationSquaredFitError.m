function segmentationSquaredFitError = computeSegmentationSquaredFitError(data, fastsumbuffer, tags, verbose)
% @data : the .f0table data
% @buffer: onetime Buffer computed by computeBuffer.m given the
%          .f0table Pitch/Time Ascii Data
% @tags: must have the format: [(x0,x1,degree),(x1,x2,degree),...], it's a
% list of intervals, a description of the segmentation: "from x0 to x1 you fit
% best by a polynom of degree 'degree'"
% @verbose: false is default

format long g

segmentationSquaredFitError = 0.0;

for interval = 1:1:length(tags)
    if(verbose)
      display('[Segmentation Squared Fit Error]:')
      tags(interval)
      display('error = ')
      sqfe = computeSquaredFitError(data,fastsumbuffer,tags(interval,1)+1,tags(interval,2)+1+1,tags(interval,3));
      display(sqfe)
    end
     segmentationSquaredFitError = segmentationSquaredFitError + computeSquaredFitError(data, fastsumbuffer,tags(interval,1)+1,tags(interval,2)+1+1,tags(interval,3));
end