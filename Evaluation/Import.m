addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/io'); %Library for IO functions

%%%%%%%%%%% Importing pool and run %%%%%%%%%%
poolFileName = '/Users/Marco/Documents/GitHub/IR_Project/qrels.trec7.txt'; %pool file path
relevanceGrades = 0:1;  %Pool relevance grades vector
relevanceDegree = {'NotRelevant', 'Relevant'};  %Pool relevance degree vector
poolId = 'TREC_Pool';
fprintf('>>>>>>>> Importing pool <<<<<<<<<< \n');
[pool, report] = importPoolFromFileTRECFormat('FileName', poolFileName, 'Identifier', poolId, 'RelevanceGrades', relevanceGrades, 'RelevanceDegree', relevanceDegree); %importing pool

fprintf('>>>>>>>> Importing runs <<<<<<<<<< \n');
runsDirectoryFilePath = '/Users/Marco/Documents/GitHub/IR_Project/Runs';
[runSet, report] = importRunsFromDirectoryTRECFormat('Path', runsDirectoryFilePath, 'Identifier', 'basicRuns', 'Delimiter', 'space');
fprintf('>>>>>>>> End of importing process <<<<<<<<<<');