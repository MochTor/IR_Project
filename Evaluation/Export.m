restoredefaultpath;

load measuredAPBasicRunSetData.mat
%T = table(BB2c1.0Stemmer,BB2c1.0NoStemmer);%, 'BM25b0.75 stemmer', 'BM25b0.75 no stemmer', 'HiemstraL_M0.15 stemmer', 'HiemstraL_M0.15 no stemmer', 'PL2c1.0 stemmer', 'PL2c1.0 no stemmer', 'TF_IDF stemmer', 'TF_IDF no stemmer'});
%T = table(measuredAPBasicRunSet);
% writetable(run_noSM_noSW, filename, 'WriteRowNames', true, 'Sheet', 'run_noSM_noSW');
writetable(measuredAPBasicRunSet, 'measuredAPBasicRunSet.txt','WriteRowNames', true);