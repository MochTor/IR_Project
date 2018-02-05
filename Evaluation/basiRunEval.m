addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/measure'); %Library for measure functions
addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/io'); %Library for io functions
addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/util'); %Library for io functions


%%%%%%%%%%% Importing runs %%%%%%%%%%
fprintf('>>>>>>>>>> Loading basic runs data <<<<<<<<<<\n');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/basicRunSetData.mat');

%%%%%%%%%%% Calculating AP %%%%%%%%%%
fprintf('>>>>>>>>>> Measuring Average Precision <<<<<<<<<<<\n');
%measuredRunSet = averagePrecision(pool, basicRunSet(:,1));
%[basicRunSet, basicRunReport] = importRunsFromDirectoryTRECFormat('Path', runsDirectoryFilePath, 'Identifier', 'basicRuns', 'Delimiter', 'space');


%for i = 1 : 10
   %currentRun = basicRunSet(:,1);
   %currentRun.Properties.UserData.identifier = strcat('basicRun', num2str(i));
   %basicRunSet(:,i) = currentRun;
   %currentRun = importRunFromFileTRECFormat()    
%end
[assessedRunSet, poolStats, runSetStats, inputParams] = assess(pool, basicRunSet);