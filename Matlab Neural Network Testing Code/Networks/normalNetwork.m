function [MAPE] = normalNetwork(MATRIX, LOAD, AlgorithmUsed, epoch)
% No Waves
disp("Running Normal Neural Network")

% Get Training Set and Testing Set
[train_input, train_output, test_input, test_output] = randomizeData(MATRIX, LOAD);

% Neural Network Factors
neurons         = 40;                        % Neurons
goal            = 1e-10;                     % Accuracy
fail            = 5000;                      % Validation Checks
% NET             = cell(level, 1);

 net_n = newff(train_input, train_output, neurons, {'tansig' 'purelin'}, AlgorithmUsed, 'learngdm');
 net_n.trainParam.epochs   = epoch;                                            
 net_n.trainParam.goal     = goal;   
 net_n.trainParam.max_fail = fail;                                           
[net_n]                    = train(net_n, train_input, train_output);  

Pred = (sim(net_n,test_input));

%% Reconstruction *******
% Plot
subplot(3,1,1)
plot(Pred)
hold on

plot(test_output)
hold off

subplot(3,1,2)
plot(Pred)

subplot(3,1,3)
plot(test_output)

% Aquire MAPE
n   = length(Pred); 
sum = 0;
for i = 1:n 
 sum    = sum + abs(( test_output(i) - Pred(i) ) / test_output(i));
end
 MAPE   = sum * 100 / n;
 
 % Display MAPE
disp('*******************');
disp('MAPE:');
disp(MAPE);
disp('*******************');
end

