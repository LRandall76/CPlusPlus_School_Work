function nNAlternate(train_input, train_output, test_input, test_output, AlgorithmUsed, epochs)

neurons         = 40;                        % Neurons
goal            = 1e-10;                     % Accuracy
fail            = 5000;                      % Validation Checks

 net_n = newff(train_input, train_output(1,:), neurons, {'tansig' 'purelin'}, AlgorithmUsed, 'learngdm');
 net_n.trainParam.epochs   = epochs;                                            
 net_n.trainParam.goal     = goal;   
 net_n.trainParam.max_fail = fail;                                           
[net_n]                    = train(net_n, train_input(1:6,:), train_output(1,:));                                   % Training


Forecasted  = (sim(net_n,test_input));  
Actual      = test_output(1,:);

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

