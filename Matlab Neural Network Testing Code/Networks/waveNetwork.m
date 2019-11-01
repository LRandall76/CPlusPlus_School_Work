function [NET, MAPE] = waveNetwork(train_input, train_output, test_input, test_output, AlgorithmUsed, level, epoch)
disp("Running Wavelet Neural Network");

% Neural Network Factors
neurons         = 40;                        % Neurons
goal            = 1e-10;                     % Accuracy
fail            = 5000;                      % Validation Checks
% NET             = cell(level, 1);

tic;
disp("--------NETWORKS TRAINED--------");
for i = 1: level + 1
 % Approx  
 if i == 1
   train_inputNN = train_input(1:10,:);
   test_inputNN  = test_input (1:10,:);
   
% Detail
elseif i == 2
    train_inputNN(1:8,:)  = train_input(1:8, :);
    train_inputNN(9,:)    = train_input(7 + i* 2,:);
    train_inputNN(10,:)    = train_input(8 + i * 2,:);
    
    test_inputNN(1:8,:)   = test_input(1:8,:);
    test_inputNN(9,:)     = test_input(7 + i * 2, :);
    test_inputNN(10,:)     = test_input(8 + i * 2, :);
 elseif i > 2 
    train_inputNN(9,:)    = train_input(7 + i * 2,:);
    train_inputNN(10,:)    = train_input(8 + i* 2,:); 
    
    test_inputNN(9,:)     = test_input(7 + i * 2, :);
    test_inputNN(10,:)     = test_input(8 + i * 2, :);
 end
 
 % Training
 net_n = newff(train_inputNN, train_output(i,:), neurons, {'tansig' 'purelin'}, AlgorithmUsed, 'learngdm');
 net_n.trainParam.epochs   = epoch;                                            
 net_n.trainParam.goal     = goal;   
 net_n.trainParam.max_fail = fail;                                           
[net_n]                    = train(net_n, train_inputNN, train_output(i,:));  

% Networks Trained
disp(i); 

%Save Network in Cell Vector
% NET{i} = net_n;
NET = 0;

% Simulation
Pred(i,:) = (sim(net_n,test_inputNN));

 end
toc

%% Reconstruction *******
% Pred
Forecasted = 0;
for i = 1: level + 1
Forecasted = Pred(i,:) + Forecasted;
end

% Actual
Actual = 0;
for i = 1: level + 1
Actual = test_output(i,:) + Actual;
end


% Plot
subplot(3,1,1)
title("FORECASTED ON ACTUAL");
plot(Forecasted)
hold on

plot(Actual)
hold off
title("FORECASTED");
subplot(3,1,2)
plot(Forecasted)
title("ACTUAL");
subplot(3,1,3)
plot(Actual)

% Aquire MAPE
n   = length(Forecasted); 
sum = 0;
for i = 1:n 
 sum    = sum + abs(( Actual(i) - Forecasted(i) ) / Actual(i));
end
 MAPE   = sum * 100 / n;
 
 % Display MAPE
disp('*******************');
disp('MAPE:');
disp(MAPE);
disp('*******************');
end

