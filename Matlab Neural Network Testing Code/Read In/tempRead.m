function [TEMP] = tempRead(filename)
disp("Reading in Temp");
tic;

% Read in File
DATA = readtable(filename);

% Kill Soil Temp
DATA(:,7) = [];
DATA(:,6) = [];

% Kill Date
DATA(:,1) = [];

% Convert DATA to double
DATA = table2array(DATA);


% OKAY WE HAVE A PROBLEM

for i = 1: length(DATA)


    if isnan(DATA(i,1))
    
        DATA(i,1) = DATA(i- 1, 1);
    end
    if isnan(DATA(i,2))
        DATA(i,2) = DATA(i- 1, 2);
    end
    if isnan(DATA(i,3))
        DATA(i,3) = DATA(i- 1, 3);
    end
    if isnan(DATA(i,4))
        DATA(i,4) = DATA(i- 1, 4);
    end
end
    
% Expand each data point 24 times
TEMP = zeros(length(DATA) * 24, 4);
for i = 1: length(DATA)
    for j = 1: 24
       
        TEMP(((i - 1) * 24) + j,:) = DATA(i,:);
        
    end
end



toc
end

