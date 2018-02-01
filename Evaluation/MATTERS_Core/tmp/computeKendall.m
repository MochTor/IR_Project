function [tau] = computeKendall (rankA, rankB)

    outputDir = '/Users/ferro/Documents/progetti/software/matters/output/';
    perlScript = '/Users/ferro/Documents/progetti/software/matters/trunk/third-party/soboroff/tools/tau-Soboroff.pl';

    rankA = sortrows(rankA, rankA.Properties.VariableNames, {'descend'});
    rankB = sortrows(rankB, rankB.Properties.VariableNames, {'descend'});
    
    writetable(rankA, [outputDir 'rankA.txt'], 'FileType', 'Text', ...
        'Delimiter', 'tab', 'WriteVariableNames', false, ...
        'WriteRowNames', true);
    
    
    writetable(rankB, [outputDir 'rankB.txt'], 'FileType', 'Text', ...
        'Delimiter', 'tab', 'WriteVariableNames', false, ...
        'WriteRowNames', true);
    
   a = perl(perlScript, strcat(outputDir, 'rankA.txt'), strcat(outputDir, 'rankB.txt'))
   
   tau = str2double(a);

end

