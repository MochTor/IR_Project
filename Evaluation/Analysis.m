%addpath('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MATTERS_Core/measure'); %Library for measure functions

%%%%%%%%%%% Loading runs %%%%%%%%%
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/basicRunSetData.mat');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/measuredAPBAsicRunSetData.mat');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MAPBasicRunSetData.mat');

%%%%%%%%%% Plotting %%%%%%%%%%
plot(351:400, measuredAPBasicRunSet{:,:});
title('Precision for the different retrieval methods in different topics');
ylabel('precision')
xlabel('topics')
legend(basicRunSet.Properties.VariableNames);

%basicRunSet = horzcat(basicRunSet, MAPBasicRuns);
subplot(2,1,1);
scatter(1:10, MAPBasicRuns{:,1});
ylabel('MAP')
xlabel('Models')
legend(MAPBasicRuns.Properties.RowNames)