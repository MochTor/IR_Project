addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/measure'); %Library for measure functions

%%%%%%%%%%% Importing runs %%%%%%%%%%
fprintf('>>>>>>>>>> Loading basic runs data <<<<<<<<<<\n');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/basicRunSetData.mat');

%%%%%%%%%%% Calculating AP %%%%%%%%%%
fprintf('>>>>>>>>>> Measuring Average Precision <<<<<<<<<<<\n');
%measuredRunSet = averagePrecision(pool, basicRunSet(:,1));
%[basicRunSet, basicRunReport] = importRunsFromDirectoryTRECFormat('Path', runsDirectoryFilePath, 'Identifier', 'basicRuns', 'Delimiter', 'space');


for i = 1 : 10
   currentRun = basicRunSet(:,i);
    
    
end

%[assessedRunSet, poolStats, runSetStats, inputParams] = assess(pool, basicRunSet(:,1));