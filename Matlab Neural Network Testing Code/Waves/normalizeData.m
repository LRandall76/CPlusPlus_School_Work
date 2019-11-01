function [wave] = normalizeData(wave)

 mwave = min(wave);
 Mwave = max(wave);
 
 
    for i = 1:length(wave)
    wave(i) = (wave(i) - mwave) / (Mwave - mwave);
    end
    
end

