%%%%%%%%%%% Loading runs %%%%%%%%%
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/basicRunSetData.mat');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/measuredAPBasicRunSetData.mat');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MAPBasicRunSetData.mat');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/NFRunSetData.mat');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/measuredAPNFRunSetData.mat');
load('/Users/Marco/Documents/GitHub/IR_Project/Evaluation/MAPNFRunSetData.mat');

%%%%%%%%%% Plotting %%%%%%%%%%
plot(351:400, measuredAPBasicRunSet{:,:});
title('Precision for the different retrieval methods for basic TREC set');
ylabel('Precision')
xlabel('Topics')
legend(basicRunSet.Properties.VariableNames);

plot(351:400, measuredAPNFRunSet{:,:});
title('Precision for the different retrieval methods for normalized and fused TREC set');
ylabel('Precision')
xlabel('Topics')
legend(NFRunSet.Properties.VariableNames);