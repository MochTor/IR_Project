addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/measure'); %Library for measure functions
addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/io'); %Library for io functions
addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/util'); %Library for util functions


%%%%%%%%%%% Importing runs %%%%%%%%%%
fprintf('>>>>>>>>>> Loading basic runs data <<<<<<<<<<\n');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/basicRunSetData.mat');

%%%%%%%%%%% Calculating AP %%%%%%%%%%
fprintf('>>>>>>>>>> Measuring Average Precision on basic runs<<<<<<<<<<<\n');
%[assessedBasicRunSet, poolStats, runSetStats, inputParams] = assess(pool, basicRunSet);
[measuredAPBasicRunSet, poolStats, runSetStats, inputParams] = averagePrecision(pool, basicRunSet);
save('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/measuredAPBasicRunSetData.mat', 'poolStats', 'runSetStats', 'measuredAPBasicRunSet', 'inputParams');
clear assess;
clear all;

%%%%%%%%%%% Importing runs %%%%%%%%%%
fprintf('>>>>>>>>>> Loading norm and fused runs data <<<<<<<<<<\n');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/NFRunSetData.mat');

%%%%%%%%%%% Calculating AP %%%%%%%%%%
fprintf('>>>>>>>>>> Measuring Average Precision on norm and fused runs<<<<<<<<<<<\n');
%[assessedBasicRunSet, poolStats, runSetStats, inputParams] = assess(pool, basicRunSet);
[measuredAPNFRunSet, poolStats, runSetStats, inputParams] = averagePrecision(pool, NFRunSet);
save('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/measuredAPNFRunSetData.mat', 'poolStats', 'runSetStats', 'measuredAPNFRunSet', 'inputParams');
fprintf('>>>>>>>>>> DONE! <<<<<<<<<<<<<');
clear all;
