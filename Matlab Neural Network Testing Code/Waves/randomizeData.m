function [train_input, train_output, test_input, test_output] = randomizeData(MATRIX, LOAD, TEMP)
disp("Randomizing Data");
    
% Ratio 70% n 30%
N = length(MATRIX(:,1));
M = fix(N*.7);              
ceil(M);

%%
train_input     = MATRIX;
train_output    = LOAD;

for i = 1: N - M

 index                  = randperm(N - i, 1);
 test_input(i,:)        = train_input(index,:); 
 train_input(index, :)  = [];
 test_output(i,:)       = train_output(index,:);
 train_output(index,:)  = [];
 
end

 
% Transpose
train_input     = train_input';
train_output    = train_output'; 
test_input      = test_input';
test_output     = test_output'; 


end

