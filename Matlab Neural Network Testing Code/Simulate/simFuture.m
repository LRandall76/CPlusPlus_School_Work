function [] = simFuture(NET, level, MATRIX, lastday, LOAD)
disp("Simulating Future");

% One year is 8760 data points
% Last day in known data is 8/14/2019 at 12:00pm
% We Need to add
%
%   8760 + (17 x 24) + (30 x 24) + (31 x 24) + (30 x 24) + (31 x 24) =

%   = 12096   


%% Retrieve Last Weeks Data
for i = 1: 168
A(i,2) = MATRIX(length(MATRIX)- 168 + i, 5);
end

A(1,1) = MATRIX(length(MATRIX)- 1, 5);


for k = 1: level
     D(1, k * 2 - 1) = MATRIX(length(MATRIX) - 1, 9 + k * 2);
     
    for i = 1: level
    D(i,k * 2)   = MATRIX(length(MATRIX) - 168, 10 + k * 2); 
    end
    
end


%%
next = dateshift(lastday,'start','day','next');
h = 12;
tic;
for i = 1: 12096
    
    if i ~= 1 && h == 0
   next = dateshift(next, 'start','day','next');

    end
       
   
   
   input(:,1) = month(next);
   input(:,2) = day(next);
   input(:,3) = h;
   input(:,4) = day(next, 'dayofweek');
    
%        if i < 169
       input(:,5)    = A(i,1);
       input(:,6)    = A(i,2);
       A(i + 1, 1)   = (sim(NET{1}, input'));
       A(i + 168, 2) = A(i + 1, 1);
     %  [NET{1}]      = train(NET{1}, input', A(i+1,1));
       
       for k = 1:level
       input(:,5)    = D(i, k * 2 - 1);
       input(:,6)    = D(i, k * 2);
       D(i + 1, k*2 - 1)   = (sim(NET{k}, input'));
       D(i + 168, k*2) = D(i + 1, k * 2 - 1);
      % [NET{2}]      = train(NET{2}, input', D(i+1,1));
       end

       
    
  h = h + 1;
  
  if( h >= 24)
  h = 0;
  end
end
toc

%%
% 
% A(12098:12265,:) = [];
% D(12098:12265,:) = [];
% A(1:168,:) = [];
% D(1:168,:) = [];

% Get Predicted
for i = 1: level + 1
    if i == 1
PREDICTED = A(:,1);
    else
PREDICTED = PREDICTED + D(:, i * 2 - 3);
    end
end

% Get Actual
totalLOAD = 0;
for i = 1: level + 1
totalLOAD = LOAD(:,i) + totalLOAD;
end

BOTH = [totalLOAD; PREDICTED];

   subplot(3,1,1)   
   plot(totalLOAD);
   
   subplot(3,1,2)
   plot(PREDICTED);
   
   subplot(3,1,3)
   plot(BOTH);
   
 R = 77;



%         if i < 169
%        input(:,5)    = A(i,1);
%        input(:,6)    = A(i,2);
%        A(i + 1, 1)   = (sim(NET{1}, input'));
%        A(i + 169, 2) = A(i + 1, 1);
%      %  [NET{1}]      = train(NET{1}, input', A(i+1,1));
%        
%        for k = 1:level
%        input(:,5)    = D(i, k * 2 - 1);
%        input(:,6)    = D(i, k * 2);
%        D(i + 1, k*2 - 1)   = (sim(NET{k}, input'));
%        D(i + 169, k*2) = D(i + 1, k * 2 - 1);
%       % [NET{2}]      = train(NET{2}, input', D(i+1,1));
% 


