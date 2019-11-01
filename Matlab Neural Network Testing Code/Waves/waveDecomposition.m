function [LOAD, MATRIX] = waveDecomposition(LOAD, MATRIX, wname, level)
disp("Decomposing Wave");               
                                

% Decomp of LOAD
[C,L] = wavedec(LOAD, level, wname);
clear LOAD;
for i = 1: (level + 1)
    if i == 1
        LOAD(:,i) = wrcoef('a', C, L, wname, level);
    else
        LOAD(:,i) = wrcoef('d', C, L, wname, i - 1);
    end
end

%%
% Seperating Matrices to Approx and Detail
MATRIXA = MATRIX;
MATRIXD = MATRIX;

% Decomp of the Hour Before Load
[C,L] = wavedec(MATRIX(:, 9), level, wname);

for i = 1: (level + 1)
    if i == 1
        AH = wrcoef('a', C, L, wname, level);
        AH = normalizeData(AH);
        MATRIXA(:, 8 + i) = AH;
    else
        DH = wrcoef('d', C, L, wname, i - 1);
        DH = normalizeData(DH);
        MATRIXD(:, 5 + i * 2) = DH;
    end
end


% Decomp of the Week Before Load
[C,L] = wavedec(MATRIX(:,10), level, wname);

for i = 1: (level + 1)
    if i == 1
        AW = wrcoef('a', C, L, wname, level);
        AW = normalizeData(AW);
        MATRIXA(:, 9 + i) = AW;
    else
        DW = wrcoef('d', C, L, wname, i - 1);
        DW = normalizeData(DW);
        MATRIXD(:, 6 + i * 2) = DW;
    end
end
clear MATRIX

% Shuffle Data
%  [MATRIXA, LOAD(:,1)]  = shuffleData(MATRIXA, LOAD(:,1));
%  [MATRIXD, LOAD(:,2)]  = shuffleData(MATRIXD, LOAD(:,2));


MATRIXD(:,1:8) = [];
MATRIX = horzcat(MATRIXA, MATRIXD);


end

