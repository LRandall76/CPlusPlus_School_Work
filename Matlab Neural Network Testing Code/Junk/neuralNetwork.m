function [] = neuralNetwork(train_input, train_output, test_input, test_output, AlgorithmUsed, epochs)
disp("Running Neural Network");

% Neural Network Factors
neurons         = 40;                        % Neurons
goal            = 1e-10;                     % Accuracy
fail            = 5000;                      % Validation Checks

tic;

train_inputA1         = train_input(1:6,:);
test_inputA1          = test_input (1:6,:);

train_inputD1         = train_input(7:10, :);
train_inputD1(5,:)    = train_input(11,:);
train_inputD1(6,:)    = train_input(14,:);
test_inputD1          = test_input(7:10,:);
test_inputD1(5,:)     = test_input(11,:);
test_inputD1(6,:)     = test_input(14,:);

train_inputD2         = train_input(7:10, :);
train_inputD2(5,:)    = train_input(12,:);
train_inputD2(6,:)    = train_input(15,:);
test_inputD2          = test_input(7:10,:);
test_inputD2(5,:)     = test_input(12,:);
test_inputD2(6,:)     = test_input(15,:);


train_inputD3         = train_input(7:10, :);
train_inputD3(5,:)    = train_input(13,:);
train_inputD3(6,:)    = train_input(16,:);
test_inputD3          = test_input(7:10,:);
test_inputD3(5,:)     = test_input(13,:);
test_inputD3(6,:)     = test_input(16,:);

% Approx
 net_nA1 = newff(train_inputA1, train_output(1,:), neurons, {'tansig' 'purelin'}, AlgorithmUsed, 'learngdm');
 net_nA1.trainParam.epochs   = epochs;                                            
 net_nA1.trainParam.goal     = goal;   
 net_nA1.trainParam.max_fail = fail;                                           
[net_nA1]                    = train(net_nA1,train_inputA1, train_output(1,:));                                   % Training




% Detail 
 net_nD1 = newff(train_inputD1, train_output(2,:), neurons,{'tansig' 'purelin'}, AlgorithmUsed, 'learngdm');
 net_nD1.trainParam.epochs   = epochs;                                             
 net_nD1.trainParam.goal     = goal;   
 net_nD1.trainParam.max_fail = fail;                                           
[net_nD1]                    = train(net_nD1,train_inputD1, train_output(2,:));                                   % Training


 net_nD2 = newff(train_inputD2, train_output(3,:), neurons,{'tansig' 'purelin'}, AlgorithmUsed, 'learngdm');
 net_nD2.trainParam.epochs   = epochs;                                             
 net_nD2.trainParam.goal     = goal;   
 net_nD2.trainParam.max_fail = fail;                                           
[net_nD2]                    = train(net_nD2, train_inputD2, train_output(3,:));                                   % Training

 net_nD3 = newff(train_inputD3, train_output(4,:), neurons,{'tansig' 'purelin'}, AlgorithmUsed, 'learngdm');
 net_nD3.trainParam.epochs   = epochs;                                             
 net_nD3.trainParam.goal     = goal;   
 net_nD3.trainParam.max_fail = fail;                                           
[net_nD3]                    = train(net_nD3, train_inputD3, train_output(4,:));                                   % Training

toc

% Simulation
A1 = (sim(net_nA1,test_inputA1));  
D1 = (sim(net_nD1,test_inputD1));                                                 
D2 = (sim(net_nD2,test_inputD2));
D3 = (sim(net_nD3,test_inputD3));
% Reconstruction
Forecasted  = A1 + D1 + D2 + D3;
Actual      = test_output(1,:) + test_output(2,:) + test_output(3,:) + test_output(4,:);

% Plot
subplot(3,1,1)
plot(Forecasted)
hold on

plot(Actual)
hold off

subplot(3,1,2)
plot(Forecasted)

subplot(3,1,3)
plot(Actual)

% MAPE
n   = length(Forecasted); 
sum = 0;

for i = 1:n 
 sum    = sum + abs(( Actual(i) - Forecasted(i) ) / Actual(i));
end
 MAPE   = sum * 100 / n;
 
disp('*******************');
disp(MAPE);
disp('*******************');
end

