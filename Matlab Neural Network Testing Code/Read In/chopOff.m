function [MATRIX,LOAD] = chopOff(MATRIX,LOAD)
disp("Chopping Off Data");

% % Early Data Set
% MATRIX([26961:end],:) = [];
% LOAD([26961: end]) = [];

% Later Data Set
MATRIX(1:27129,:) = [];
LOAD(1:27129) = [];
MATRIX(39267,:) = [];
LOAD(39267,:) = [];


end

