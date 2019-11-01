function [MATRIX, LOAD] = shuffleData(MATRIX, LOAD)
disp("Shuffling")

N = length(MATRIX(:,1));
tic;
for i = 1: N

 index                  = randperm(N - i + 1, 1);
 NMATRIX(i,:)           = MATRIX(index,:); 
 MATRIX(index, :)       = [];
 NLOAD(i,:)             = LOAD(index,:);
 LOAD(index,:)          = [];
 
end
toc

MATRIX  = NMATRIX;
LOAD    = NLOAD;

end

