addpath('/Users/Marco/Documents/GitHub/ProjectIR/core/io'); %Library for IO functions

%%%%%%%%%%% Importing pool and run %%%%%%%%%%
poolFileName = '/Users/Marco/Documents/GitHub/ProjectIR/results/qrels.trec7.txt'; %pool file path
relevanceGrades = 0:1;  %Pool relevance grades vector
relevanceDegree = {'NotRelevant', 'Relevant'};  %Pool relevance degree vector
poolId = 'Pool_TREC';
fprintf('>>>>>>>> Importing pool <<<<<<<<<<');
[pool, report] = importPoolFromFileTRECFormat('FileName', poolFileName, 'Identifier', poolId, 'RelevanceGrades', relevanceGrades, 'RelevanceDegree', relevanceDegree); %importing pool

runFileName = '/Users/Marco/Documents/GitHub/ProjectIR/results/BB2c1.0_0.res';  %run file path
documentOrdering = 'TrecEvalLexDesc';
fprintf('>>>>>>>> Importing run <<<<<<<<<<');
%[run, report] = importRunFromFileTRECFormat('FileName', runFileName, 'DocumentOrdering', documentOrdering, 'Delimiter', 'space');

runsDirectoryFilePath = '/Users/Marco/Documents/GitHub/ProjectIR/results/';
[runSet, report] = importRunsFromDirectoryTRECFormat('Path', runsDirectoryFilePath, 'Identifier', poolId, 'Delimiter', 'tab');
fprintf('>>>>>>>> End <<<<<<<<<<');