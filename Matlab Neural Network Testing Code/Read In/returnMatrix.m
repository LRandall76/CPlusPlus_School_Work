function [MATRIX, lastday] = returnMatrix(DATE,LOAD, TEMP)
disp("Returning Matrix");




% Convert Date into Doubles
MATRIX = datevec(DATE);     
lastday = DATE(length(DATE));
% Eliminate the garbage rows
MATRIX(:,6) = [];           
MATRIX(:,5) = [];

dayVector = weekday(DATE);
MATRIX(:,5) = dayVector;

% Add zeros to make life easier
newTEMP = zeros(length(MATRIX),size(TEMP,2));
N = 1;
for i = length(MATRIX) - length(TEMP): length(MATRIX) - 1

    newTEMP(i + 1,:) = TEMP(N,:);
 N = N + 1;
end




MATRIX = horzcat(MATRIX, newTEMP);


    

% GIVE NETWORK PREVIOUS HOUR'S LOAD
N = size(MATRIX,2)+1;
for i = 1:66396                     % THIS IS THE LOAD THE HOUR BEFORE
   
    if i ~= 1
        MATRIX(i,N)  =  LOAD(i - 1);  
    else
        MATRIX(i,N) = 21000;              % COMPLETE GARBAGE VALUE I MADE UP
    end
    
end
% THIS IS THE PREVIOUS LOAD A WEEK PRIOR
N = size(MATRIX,2)+1;
for i = 1:66396                     
   
    
    if i > 168 && i < 66396
        MATRIX(i,N) =  LOAD(i - 168); 
    else
        MATRIX(i,N) = 21000;              % COMPLETE GARBAGE VALUE I MADE UP
    end
    
end

% Get Day of Week


% Remove Year
MATRIX(:,1) = [];


end
