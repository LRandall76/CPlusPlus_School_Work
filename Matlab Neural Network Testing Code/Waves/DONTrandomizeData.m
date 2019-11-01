function [train_input, train_output, test_input, test_output] = DONTrandomizeData(MATRIX, LOAD)
disp("NOT Randomizing Data");
    
% Ratio 70% n 30%
N = length(MATRIX(:,1));
M = fix(N*.7);              
ceil(M);

% Without Randomization
train_input  = MATRIX(1:M,:);
train_output = LOAD(1:M,:);
test_input   = MATRIX(M:N,:);
test_output  = LOAD(M:N,:);


 
% Transpose
train_input     = train_input';
train_output    = train_output'; 
test_input      = test_input';
test_output     = test_output'; 


end

