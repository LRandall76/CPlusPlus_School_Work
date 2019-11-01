%Neural Network Code Using A Feed Forward Network
%10/11/2019
 
%% Loop Variables
ALGOR  = ["trainlm","trainbfg","trainrp","trainscg","traincgb","traincgf","traincgp","trainoss","traingdx"];
WAVES  = ["haar", "db2", "db3","db4","db5","db6","db7","db8","db9","db10","coif1","coif2","coif3","coif4","coif5","sym2","sym3","sym4","sym5","sym6","sym7","sym8"];

VALUES = ["start", "start", 0,0];
TEST   = 5;
epoch  = 1000;               % Epochs


% READ IN TEMPERATURE
filename            = "air temp max.xlsx";
TEMP = tempRead(filename);

% READ IN LOAD
filename            = "20120101-20190814 CAISO Actual Load.xlsx";
[DATE, LOAD1]       = dataRead(filename);       % Read in File
[MATRIX1, lastday]  = returnMatrix(DATE, LOAD1, TEMP);       % Get MATRIX from DATE and LOAD
clear DATE;      
[MATRIX1, LOAD1]    = chopOff(MATRIX1, LOAD1);   % Chop Off the initial few years



 for j = 1: length(ALGOR)            % ALGORITHMS
    for k = 1: length(WAVES)            % WAVES
        for i = 1: 1                        % LEVELS
            for L = 1: TEST                      % MULTIPLE TIMES TO TEST
%% Variables In Use
    AlgorithmUsed   = ALGOR(j);          % Algorithm
    wname           = WAVES(k);          % Wavelet Transform
    level           = (i);               % Level of Wavelet Decomposition
   

%% Program Start
% [MATRIX, LOAD]  = shuffleData(MATRIX, LOAD); % randomize data

[LOAD, MATRIX] = waveDecomposition(LOAD1, MATRIX1, wname, level); % Decompose Wave
        


 [train_input, train_output, test_input, test_output] = randomizeData(MATRIX, LOAD, TEMP); % Randomization
% [train_input, train_output, test_input, test_output] = DONTrandomizeData(MATRIX, LOAD); 
      

[NET, MAPE] = waveNetwork(train_input, train_output, test_input, test_output, AlgorithmUsed, level, epoch);% Train and Test
        

%  simFuture(NET, level, MATRIX, lastday, LOAD); %Simulate Future
        disp("END OF PROGRAM");
        disp(AlgorithmUsed);
        disp(wname);
        disp(level);
        

%% Testing STUFF        
 

% Populate Matrix
VALUES(datasize,1) = (WAVES(k));
VALUES(datasize,2) = (ALGOR(j));
VALUES(datasize,3) = (i);
VALUES(datasize,4) = (MAPE);
datasize = size(VALUES,1)+1;
% datasize = Index for VALUES matrix
datasize = size(VALUES,1)+1;
% Write to File
writematrix(VALUES, "MAPES.xlsx")

            end
        end
    end
%%

      % Run Normal Network
      for L = 1: TEST
       AlgorithmUsed   = ALGOR(j); 
       MAPE2 = normalNetwork(MATRIX1, LOAD1, AlgorithmUsed, epoch);
       NVALUES(L,1) = "None";
       NVALUES(L,2) = AlgorithmUsed;
       NVALUES(L,3) = 0;
       NVALUES(L,4) = MAPE2;
       
      end
    
      VALUES = vertcat(VALUES,NVALUES);
      datasize = size(VALUES,1)+1;
      writematrix(VALUES, "MAPES.xlsx")
 end
 
writematrix(VALUES, "MAPESDONE.xlsx")



       
