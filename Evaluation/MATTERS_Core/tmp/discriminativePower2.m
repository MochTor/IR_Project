%% discriminativePower
% 
% Computes the discriminative power of a measure.

%% Synopsis
%
%   [asl, aslRatio, delta] = discriminativePower(measuredRunSet, varargin)
%
% 
% *Parameters*
%
% * *|measuredRunSet|* - the measures of a given run set whose discriminative
% power has to be computed. It is a table in the same format returned by, 
% for example, <../measure/precision.html precision> or by 
% <../measure/averagePrecision.html averagePrecision>. It must
% be scalar-valued measure, like average precision or precision at 10
% retrieved documents.
%
%
% *Name-Value Pair Arguments*
%
% Specify comma-separated pairs of |Name|, |Value| arguments. |Name| is the 
% argument name and |Value| is the corresponding value. |Name| must appear 
% inside single quotes (' '). You can specify several name and value pair 
% arguments in any order as |Name1, Value1, ..., NameN, ValueN|.
%
% * *|Method|* (optional) - a string specifying the method to be used for
% computing discriminative power. It can be either |PairedBootstrapTest|
% (see Sakai, 2006) or |RandomisedTukeyHSDTest| (see Sakai, 2012).
% The default is |PairedBootstrapTest|.
% * *|IgnoreNaN|* (optional) - a boolean specifying whether |NaN| values 
% have to be ignored when computing mean and standard deviation. The
% default is |false|.
% * *|BootstrapSamples|* (optional) - a scalar integer value greater than
% zero which indicates how many samples have to be used in the bootstrap.
% It can be used only when method is |PairedBootstrapTest|. The default is 
% |1000| (see Sakai, 2006 and Sakai, 2012).
% * *|TukeySamples|* (optional) - a scalar integer value greater than
% zero which indicates how many samples have to be used in the randomised
% Tukey HSD test. It can be used only when method is |RandomisedTukeyHSDTest|. 
% The default is |5000| (see Sakai, 2012).
% * *|UseParalle|* (optional) - a boolean specifying whether parallel 
% computation of bootstrap has to be used or not. The default is |false|.
% * *|Verbose|* (optional) - a boolean specifying whether additional
% information has to be displayed or not. If not specified, then |false| is 
% used as default.
%
% *Returns*
%
% * |asl| - a table containing the pairwise Achieved Significance Level
% (ASL)  among the provided systems
% The |UserData| property contains a struct with the following
% fields: |identifier| of the analysed run set; |pool| with the
% identifier of the pool with respect to which the measure has been computed;
% |name| and |shortName| of the computed measure; |method| with the method
% used for computing the discriminative power; |alpha| with the requested
% significance level alpha; |samples| with the number of samples used for
% computing the discriminative power.
% * |aslRatio|  - the ratio of the number of times the requested ASL has
% been achieved
% * |delta| - the estimated difference needed to expect two system being
% significantly different.

%% Example of use
%  
%   [asl, aslRatio, delta] = discriminativePower(measuredRunSet, 'Method', 'PairedBootstrapTest')
%
% It produces the following results.
%
%   asl = 
%   
%                   System_1    System_2    System_3    System_4    System_5
%                   ________    ________    ________    ________    ________
%   
%       System_1    NaN         0.955       0.048        0.32       0.007   
%       System_2    NaN           NaN       0.703       0.511       0.133   
%       System_3    NaN           NaN         NaN       0.776       0.024   
%       System_4    NaN           NaN         NaN         NaN       0.007   
%       System_5    NaN           NaN         NaN         NaN         NaN   
%
% Note that all the ASL outside the upper triangle are NaN since it does
% not make sense to compute them.
%
%   aslRatio =
%   
%       0.4000
%   
%   
%   delta =
%   
%       0.1273
%
%
%% References
% 
% Please refer to:
%
% * Carterette, B. A. (2012). Multiple Testing in Statistical Analysis of 
% Systems-Based Information Retrieval Experiments. _ACM Transactions on 
% Information Systems (TOIS)_, 30(1):4:1-4:34.
% * Sakai, T. (2006). Evaluating Evaluation Metrics based on the Bootstrap. 
% In Efthimiadis, E. N., Dumais, S., Hawking, D., and Järvelin, K., editors, 
% _Proc. 29th Annual International ACM SIGIR Conference on Research and 
% Development in Information Retrieval (SIGIR 2006)_, pages 525-532. ACM 
% Press, New York, USA.
% * Sakai, T. (2012). Evaluation with Informational and Navigational Intents. 
% In Mille, A., Gandon, F. L., Misselis, J., Rabinovich, M., and Staab, S.,
% editors, _Proc. 21st International Conference on World Wide Web (WWW 2012)_, 
% pages 499-508. ACM Press, New York, USA.
% * Sakai, T. (2014). Metrics, Statistics, Tests. In Ferro, N., editor,
% _Bridging Between Information Retrieval and Databases - PROMISE Winter School 2013, Revised Tutorial Lectures_, 
% pages 116-163. Lecture Notes in Computer Science (LNCS) 8173, Springer,
% Heidelberg, Germany.
% * Smucker, M. D., Allan, J., and Carterette, B. A. (2007). A Comparison
% of Statistical Significance Tests for Information Retrieval Evaluation. 
% In Silva, M. J., Laender, A. A. F., Baeza-Yates, R., McGuinness, D. L., 
% Olstad, B., Olsen, Ø. H., and Falcão, A. a., editors, 
% _Proc. 16th International Conference on Information and Knowledge Management (CIKM 2007)_,
% pages 623-632. ACM Press, New York, USA.
%
%% Acknowledgements
% 
% We would like to warmly thank Tetsuya Sakai, Waseda University, Japan for
% providing us his original C/shell source code for computing the Paired 
% Bootstrap Test and the Randomised Tukey HSD Test and for his great 
% willingness in discussing with us the details of the implementation.

%% Information
% 
% * *Author*: <mailto:ferro@dei.unipd.it Nicola Ferro>,
% <mailto:silvello@dei.unipd.it Gianmaria Silvello>
% * *Version*: 1.00
% * *Since*: 1.00
% * *Requirements*: Matlab 2013b or higher
% * *Copyright:* (C) 2013-2014 <http://ims.dei.unipd.it/ Information 
% Management Systems> (IMS) research group, <http://www.dei.unipd.it/ 
% Department of Information Engineering> (DEI), <http://www.unipd.it/ 
% University of Padua>, Italy
% * *License:* <http://www.apache.org/licenses/LICENSE-2.0 Apache License, 
% Version 2.0>

%%
%{
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
      http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
%}

%%
function [asl, aslRatio, delta] = discriminativePower(measuredRunSet, varargin)
   
    % check that we have the correct number of input arguments. 
    narginchk(1, inf);
    
    % ensure we have a table of measures as first input
    validateattributes(measuredRunSet, {'table'}, {'nonempty'}, '', 'measuredRunSet', 1);
    
    % only numeric and scalar values can be processed
    if ~isnumeric(measuredRunSet{1, 1}) || ~isscalar(measuredRunSet{1, 1})
         error('MATTERS:IllegalArgument', 'Run set %s for measure %s does not contain numeric scalar values.', ...
            measuredRunSet.Properties.UserData.identifier, measuredRunSet.Properties.UserData.shortName);       
    end;
    
    % the systems to be compared
    dataset = measuredRunSet{:, :};

    % number of topics
    t = size(dataset, 1);
    
    % number of systems to be compared
    s = size(dataset, 2);
    
    % total number of pair to be examined
    pairs = nchoosek(s, 2);

    
    % parse the variable inputs
    pnames = {'Method',              'IgnoreNaN', 'Alpha', 'BootstrapSamples', 'TukeySamples', 'UseParallel', 'Verbose'};
    dflts =  {'PairedBootstrapTest', false,       0.05,    1000,               5000,           false,         false};
    [method, ignoreNaN, alpha, bootstrapSamples, tukeySamples, useParallel, verbose, supplied] ...
         = matlab.internal.table.parseArgs(pnames, dflts, varargin{:});
    
     if supplied.Method        
        % check that method is a non-empty string
        validateattributes(method, ...
            {'char', 'cell'}, {'nonempty', 'vector'}, '', 'Method');
        
        if iscell(method)
            % check that method is a cell array of strings with one element
            assert(iscellstr(method) && numel(method) == 1, ...
                'MATTERS:IllegalArgument', 'Expected Method to be a cell array of strings containing just one string.');
        end
        
        % remove useless white spaces, if any, and ensure it is a char row
        method = char(strtrim(method));
        method = method(:).';
        
        % check that method assumes a valid value
        validatestring(method, ...
            {'PairedBootstrapTest', 'RandomisedTukeyHSDTest'}, '', 'Method');             
    end;  
              
    if supplied.IgnoreNaN
        % check that ignoreNaN is a non-empty scalar logical value
        validateattributes(ignoreNaN, {'logical'}, {'nonempty','scalar'}, '', 'IgnoreNaN');
    end;   
    
    if supplied.Alpha
        % check that Alpha is a nonempty scalar "real| value
        % greater than 0 and less than or equal to 1
        validateattributes(alpha, {'numeric'}, ...
            {'nonempty', 'scalar', 'real', '>', 0, '<=', 1}, '', 'Alpha');
    end;
            
    if supplied.BootstrapSamples        
        if supplied.Method && ~strcmpi(method, 'PairedBootstrapTest') 
            error('MATTERS:IllegalArgument', 'Bootstrap samples can be used only when the requested method is PairedBootstrapTest.'); 
        end;
        
        % check that BootstrapSamples is a nonempty scalar integer value
        % greater than 0
        validateattributes(bootstrapSamples, {'numeric'}, ...
            {'nonempty', 'scalar', 'integer', '>', 0}, '', 'BootstrapSamples');
    end;
    
    if supplied.TukeySamples  
        if supplied.Method && ~strcmpi(method, 'RandomisedTukeyHSDTest') 
            error('MATTERS:IllegalArgument', 'Tukey samples can be used only when the requested method is RandomisedTukeyHSD.'); 
        end;
        
        % check that TukeySamples is a nonempty scalar integer value
        % greater than 0
        validateattributes(tukeySamples, {'numeric'}, ...
            {'nonempty', 'scalar', 'integer', '>', 0}, '', 'TukeySamples');               
    end;
        
    if supplied.UseParallel
        if ~supplied.Bootstrap
             error('MATTERS:IllegalArgument', 'Cannot ask for parallel computation when no bootstrap has been asked.');
        end;
        
        % check that parallel is a non-empty scalar logical value
        validateattributes(useParallel, {'logical'}, {'nonempty','scalar'}, '', 'UseParallel');
    end;  
        
    if supplied.Verbose
        % check that verbose is a non-empty scalar logical value
        validateattributes(verbose, {'logical'}, {'nonempty','scalar'}, '', 'Verbose');
    end;  
                  
    if verbose
        fprintf('\n\n----------\n');
        
        fprintf('Computing discriminative power for run set %s and measure %s with respect to pool %s: %d runs and % pairs to be processed.\n\n', ...
            varargin{1}.Properties.UserData.identifier, ....
            varargin{1}.Properties.UserData.shortName, ...
            varargin{1}.Properties.UserData.pool, s, pairs);

        fprintf('Settings:\n');
        
        fprintf('  - method %s;\n', method);
        
        fprintf('  - confidence level alpha %d;\n', alpha);
        
        if ignoreNaN
           fprintf('  - NaN values will be ignored;\n');
        else
           fprintf('  - NaN values will not be ignored;\n');
        end
        
        switch lower(method)
            case 'pairedbootstraptest'
                fprintf('  - bootstrap data samples: %d;\n', bootstrapSamples);
            case 'randomisedtukeyhsdtest'
                fprintf('  - Tukey data samples: %d;\n', tukeySamples);
        end;

        if useParallel
            fprintf('  - parallel computing will be used;\n');
        else
            fprintf('  - parallel computing will not be used;\n');
        end;
                
        fprintf('\n');
    end;
       
    
    switch lower(method)
        case 'pairedbootstraptest'            
            % compute the Paired Bootstrap Test on each pair of systems
            [asl, aslRatio, delta] = computePairedBootstrapTest();
            
        case 'randomisedtukeyhsdtest'
            % compute the Randomise Tukey HSD Test on each pair of systems
            [asl, aslRatio, delta] = computeRandomisedTukeyHSDTest();
    end;

               
    % transform asl into a table
    asl = array2table(asl);
    asl.Properties.RowNames = measuredRunSet.Properties.VariableNames;
    asl.Properties.VariableNames = measuredRunSet.Properties.VariableNames;
    asl.Properties.UserData.identifier = measuredRunSet.Properties.UserData.identifier;
    asl.Properties.UserData.pool = measuredRunSet.Properties.UserData.pool;
    asl.Properties.UserData.name = measuredRunSet.Properties.UserData.name;
    asl.Properties.UserData.shortName = measuredRunSet.Properties.UserData.shortName;
    asl.Properties.UserData.method = method;           
    asl.Properties.UserData.alpha = alpha;
    switch lower(method)
        case 'pairedbootstraptest'
            asl.Properties.UserData.samples = bootstrapSamples;
            asl.Properties.UserData.shortMethod = 'PBT';  
        case 'randomisedtukeyhsdtest'
           asl.Properties.UserData.samples = tukeySamples;
           asl.Properties.UserData.shortMethod = 'RTHSDT';
    end;   
         
    %%
    
    % computes the Paired Bootstrap Test
    %
    % See Figures 1, 2 and 5 of Sakai, 2006
    % See Figures 2 and 3 of Sakai, 2012    
    % See Figures 11 and 12 of Sakai, 2014
    function [asl, aslRatio, delta] = computePairedBootstrapTest()

        % the achieved significance level (ASL) resulting from the test
        asl = NaN(s);
        
        % the ratio of the  number of pairs that are significantly 
        % different with respect to the total number of compared pairs
        aslRatio = NaN;
        
        % estimated performance difference required for achieving a given
        % ASL
        delta = NaN(s);
        
        % fraction of the samples to pick up 
        ba = ceil(alpha * bootstrapSamples);
        
        % generate the bootstrap replicates
        replicates = randi(t, t, bootstrapSamples);
        %replicates = loadReplicates();

        % compute only the upper triangle
        for r = 1:s
            for c = r+1:s 

                % per topic performance differences - H0 z comes from a
                % distribution with zero mean
                z = dataset(:, r) - dataset(:, c);
                
                if ignoreNaN
                    zMean = nanmean(z);
                    zStd = nanstd(z);

                    % if we have NaN values, the sample size is less than 
                    % the number of rows in the dataset 
                    sampleSize = sum(~isnan(z));
                else
                    zMean = mean(z);
                    zStd = std(z);  

                   sampleSize = t;
                end;

                % compute the (absolute value of the) observed t statistics
                zT = abs(zMean ./ (zStd ./ sqrt(sampleSize)));
                
                % empirical distribution under H0 to be sampled with the
                % bootstrap
                w = z - zMean;
                
                % observed t statistic for each bootstrap sample
                wbT = NaN(1, bootstrapSamples);
                
                % observed mean for each bootstrap sample
                wbMean = NaN(1, bootstrapSamples);
                
                for b = 1:bootstrapSamples
                    
                    % get the b-th replicate
                    wb = w(replicates(:, b));
                                        
                    if ignoreNaN
                        wbMean(b) = nanmean(wb);
                        wbStd = nanstd(wb);

                        % if we have NaN values, the sample size is less 
                        % than  the number of rows in the dataset 
                        sampleSize = sum(~isnan(wb));
                    else
                        wbMean(b) = mean(wb);
                        wbStd = std(wb);  

                       sampleSize = t;
                    end;

                    % compute the (absolute value of the) observed t 
                    % statistics
                    wbT(b) = abs(wbMean(b) ./ (wbStd ./ sqrt(sampleSize)));
                end;
                
                % count how many times the observed t statistic of the
                % empirical distribution is above the one of the per-topic
                % performance differences
                asl(r, c) = length(find(wbT >= zT));
                                                                
                % sort the observed t statistics of the empirical 
                % distribution in descending order (NaN first)
                tmp = sort(wbT, 'descend');
                
                % get the positions of the observed t statistic of the
                % empirical distribution which are equal to the 
                % bootstrapSamples*alpha-th largest value. It can be:
                % - one single value
                % - more values (but equal) 
                % - empty, if the two systems are equal
                pos = find(abs(wbT) == tmp(ba));
                
                % if the systems are different, % get the difference, 
                % considering only the first position if multiple are 
                % possible
                if ~isempty(pos)                                                              
                    delta(r, c) = abs(wbMean(pos(1)));
                end;
            end;
        end; 

        % compute the Achieved Significance Level (ASL) for each system
        % pair
        asl = asl / bootstrapSamples;
                
        % compute the ASL ratio
        aslRatio = length(find(asl < alpha)) / pairs;
                
        % get the maximum delta to obtain significant differences among
        % systems
        delta = max(max(delta));
        
    end % computePairedBootstrapTest

  
    %%
    
    % computes the Randomised Tukey HSD Test
    %
    % See Figures 4 and 5 of Sakai, 2012
    % See Figure 15 of Sakai, 2014
    function [asl, aslRatio, delta] = computeRandomisedTukeyHSDTest()

        % the achieved significance level (ASL) resulting from the test
        asl = NaN(s);
        
        % the ratio of the  number of pairs that are significantly 
        % different with respect to the total number of compared pairs
        aslRatio = NaN;
        
        % estimated performance difference required for achieving a given
        % ASL
        delta = NaN(s);
        
        % largest observed difference of mean performances for each 
        % randomised sample - honestly significant difference (HSD)
        hsd = NaN(1, tukeySamples);
        
        % compute the largest observed difference of mean performances for
        % each randomised sample 
        for b = 1:tukeySamples
            
            % the current randomised data set
            randDataset = NaN(size(dataset));
        
            % for each topic, generate a random permutation of the topic 
            % row
            for k = 1:t            
                randDataset(k, :) = dataset(k, randperm(s));            
            end;  
            
            % compute the mean performances for each system in the
            % randomised dataset across all the topics
            if ignoreNaN
                rdMean = nanmean(randDataset);
            else
                rdMean = mean(randDataset); 
            end;

            % largest observed difference of the mean performances in the
            % randomised dataset 
            hsd(b) = max(rdMean) - min(rdMean);
        end;    
        
         % compute only the upper triangle
        for r = 1:s
            for c = r+1:s 

                % compute the observed difference between the mean
                % performances of the two compared systems
                if ignoreNaN                    
                    delta(r, c) = abs(nanmean(dataset(:, r)) - nanmean(dataset(:, c)));                    
                else
                    delta(r, c) = abs(nanmean(dataset(:, r)) - nanmean(dataset(:, c)));                    
                end;
                
                
                % compute the Achieved Significance Level (ASL) for each 
                % system pair: number of times the observed difference is
                % less than the honestly significant difference (HSD)
                asl(r, c) = length(find(hsd >= delta(r, c))) / tukeySamples;
         
                %fprintf('%s %s %3.2f %3.2f\n', ...
                %    measuredRunSet.Properties.VariableNames{r}, ...
                %measuredRunSet.Properties.VariableNames{c},        ...        
                %delta(r, c), asl(r, c));
                    
                % if the difference is not significant, then remove it
                if (asl(r, c) >= alpha)
                    delta(r, c) = NaN;
                end;
            end;
        end; 

        % compute the ASL ratio
        aslRatio = length(find(asl < alpha)) / pairs;
                
        % get the maximum delta to obtain significant differences among
        % systems
        delta = max(max(delta));            
    end % computeRandomisedTukeyHSDTest

    %%

    % diagnostic function to load a test dataset
    %{
    function [replicate] = loadReplicates()
        
        % return the list of text files in the directory. It ignores non-text
        % files.
        path = '/Users/ferro/Documents/progetti/software/matters/output/bootsamples/';
        files = dir([path 'test*']);
        
        bootstrapSamples = length(files);

        % extract an horizontal cell array of file names
        [bootSamples{1:bootstrapSamples}] = deal(files.name);

        % concatenate the file names with the directory
        bootSamples = strcat(repmat(path, bootstrapSamples, 1), bootSamples.');
        
        replicate = NaN(t, bootstrapSamples);
        
        for f = 1:bootstrapSamples
            fid = fopen(bootSamples{f});
            topics = textscan(fid, '%s');
            fclose(fid);
            topics = cellstr(topics{:});

            [~, loc] = ismember(topics, measuredRunSet.Properties.RowNames);

            replicate(:, f) = loc;

        end;        
    end % loadReplicates
    %}
end % discriminativePower