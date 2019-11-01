function [DATE, LOAD] = dataRead(filename)
%% This functions extracs the data and seperates it into two different arrays: LOAD and DATE
disp("Reading in Load");
tic;

% Read in File
DATA = readtable(filename);

% Variables used in loops
n       = 1;                                  
[x,~]   = size(DATA);    

% Preallocations
DATE = NaT(66397,1);            % The value of 66397 just happens to be the size of DATA after it is shrunk           
LOAD = zeros(66397, 1);

% Seperation of Table Variables
COMPANIES   = table2array(DATA(:,3));     
DATETIME    = table2array(DATA(:, 1));  
VALUES      = table2array(DATA(:, 2));

% Separation of DATA table
for i = 1:x
   if   strcmp(char(COMPANIES(i)), "PGE" ) && i < x       % if the third category of the table is PGE
       DATE(n) = DATETIME(i);
       LOAD(n) = VALUES(i);
       i = i + 1; %#ok<*FXSET>
     while char(COMPANIES(i)) ~= "PGE" && i < x
       LOAD(n) = LOAD(n) + VALUES(i);
        i = i + 1; 
     end
      n = n + 1;
   end
end

% the last entry for the data is basically garbage because its half a day :(
LOAD(66397) = [];
DATE(66397) = [];       %  format is 'dd-mmm-yyyy HH:MM:SS'; for DATE

toc
end