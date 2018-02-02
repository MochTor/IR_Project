addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/io'); %Library for IO functions

%%%%%%%%%%% Importing pool and run %%%%%%%%%%
poolFileName = '/Users/Marco/Documents/GitHub/IR_Project/qrels.trec7.txt'; %Pool file path
relevanceGrades = 0:1;  %Pool relevance grades vector
relevanceDegree = {'NotRelevant', 'Relevant'};  %Pool relevance degree vector
poolId = 'TREC_Pool';
fprintf('>>>>>>>> Importing pool <<<<<<<<<<\n');
[pool, report] = importPoolFromFileTRECFormat('FileName', poolFileName, 'Identifier', poolId, 'RelevanceGrades', relevanceGrades, 'RelevanceDegree', relevanceDegree); %importing pool

fprintf('>>>>>>>> Importing basic runs <<<<<<<<<<\n');
runsDirectoryFilePath = '/Users/Marco/Documents/GitHub/IR_Project/Runs';
[basicRunSet, basicRunReport] = importRunsFromDirectoryTRECFormat('Path', runsDirectoryFilePath, 'Identifier', 'basicRuns', 'Delimiter', 'space');   %importing base runs (no normalization no fusion)

fprintf('>>>>>>>> Importing normalized and fused runs <<<<<<<<<<\n');
nrunsDirectoryFilePath = '/Users/Marco/Documents/GitHub/IR_Project/Normalized_runs';
[NFRunSet, NFRunReport] = importRunsFromDirectoryTRECFormat('Path', nrunsDirectoryFilePath, 'Identifier', 'normAndFusedRuns', 'Delimiter', 'space');   %importing normalized and fused runs
fprintf('>>>>>>>> End of importing process <<<<<<<<<<\n');

fprintf('>>>>>>>> Saving data <<<<<<<<<<\n');
save('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/basicRunSetData.mat', 'pool', 'report', 'basicRunSet', 'basicRunReport');
save('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/NFRunSetData.mat', 'pool', 'report', 'NFRunSet', 'NFRunReport');
fprintf('>>>>>>>> Saving data done successfully <<<<<<<<<<\n');
fprintf('>>>>>>>> DONE! <<<<<<<<<<\n');
clear;