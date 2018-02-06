addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/measure'); %Library for measure functions
addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/io'); %Library for io functions
addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/util'); %Library for util functions

%%%%%%%%%%% Importing runs %%%%%%%%%%
fprintf('>>>>>>>>>> Loading basic runs data <<<<<<<<<<\n');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/basicRunSetData.mat');

%%%%%%%%%%% Calculating AP %%%%%%%%%%
fprintf('>>>>>>>>>> Measuring Average Precision on basic runs <<<<<<<<<<<\n');
%[assessedBasicRunSet, poolStats, runSetStats, inputParams] = assess(pool, basicRunSet);
[measuredAPBasicRunSet, poolStats, runSetStats, inputParams] = averagePrecision(pool, basicRunSet);
save('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/measuredAPBasicRunSetData.mat', 'poolStats', 'runSetStats', 'measuredAPBasicRunSet', 'inputParams');
clear assess;

%%%%%%%%%%% Calculating MAP %%%%%%%%%%
fprintf('>>>>>>>>>> Measuring Mean Average Precision on basic runs <<<<<<<<<<<\n');
MAPBasicRuns = struct();
MAPBasicRuns = mean(measuredAPBasicRunSet{:, 1:end});
MAPBasicRuns = array2table(MAPBasicRuns');
MAPBasicRuns.Properties.RowNames = measuredAPBasicRunSet.Properties.VariableNames;
save('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MAPBasicRunSetData.mat', 'MAPBasicRuns');

%%%%%%%%%%% Calculating Discounted Cumulated Gain %%%%%%%%%%
fprintf('>>>>>>>>>> Measuring DCG on basic runs <<<<<<<<<<<\n');
measuredDCGBasicRunSet = discountedCumulatedGain(pool, basicRunSet);
save('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/measuredDCGBasicRunSetData.mat', 'measuredDCGBasicRunSet');
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

%%%%%%%%%%% Calculating MAP %%%%%%%%%%
fprintf('>>>>>>>>>> Measuring Mean Average Precision on norm and fused runs<<<<<<<<<<<\n');
MAPNFRuns = struct();
MAPNFRuns = mean(measuredAPNFRunSet{:, 1:end});
MAPNFRuns = array2table(MAPNFRuns');
MAPNFRuns.Properties.RowNames = measuredAPNFRunSet.Properties.VariableNames;

save('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MAPNFRunSetData.mat', 'MAPNFRuns');

%%%%%%%%%%% Calculating Discounted Cumulated Gain %%%%%%%%%%
fprintf('>>>>>>>>>> Measuring DCG on norm and fused runs <<<<<<<<<<<\n');
measuredDCGNFRunSet = discountedCumulatedGain(pool, NFRunSet);
save('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/measuredDCGNFRunSetData.mat', 'measuredDCGNFRunSet');
clear assess;
clear all;

fprintf('>>>>>>>>>> DONE! <<<<<<<<<<<<<\n');