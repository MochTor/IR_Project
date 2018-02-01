%% sensitivity
% 
% Computes the sensitivity of a measure.

%% Synopsis
%
% [tauB, meanTauB, stdTauB, ciTauB] = kendall(varargin)
%
% For further details about the computation of Kendall's tau B correlation
% please see at the MATLAB <http://www.mathworks.it/it/help/stats/corr.html 
% corr> and <http://www.mathworks.it/it/help/stats/tiedrank.html tiedrank>
% functions on which |kendall| relies on.
%
% *Parameters*
%
% * *|measuredRunSet1|*, *|measuredRunSet2|*, ..., *|measuredRunSetN|* - 
% two or more variables corresponding to different measures of a given run 
% set. It is a table in the same format returned by, for example, 
% <../measure/precision.html precision> or by 
% <../measure/averagePrecision.html averagePrecision>. All the measured run
% sets must refer to the same pool, topics, and runs. Moreover, they must
% be scalar-valued measures, like average precision or precision at 10
% retrieved documents.
%
% *Name-Value Pair Arguments*
%
% Specify comma-separated pairs of |Name|, |Value| arguments. |Name| is the 
% argument name and |Value| is the corresponding value. |Name| must appear 
% inside single quotes (' '). You can specify several name and value pair 
% arguments in any order as |Name1, Value1, ..., NameN, ValueN|.
%
% * *|IgnoreNaN|* (optional) - a boolean specifying whether |NaN| values 
% have to be ignored when computing mean and standard deviation. The
% default is |false|.
% * *|Statistic|* (optional) - a string specifying the statistic to be used
% to aggregate the measures, either |Mean| or |Median|. The default is
% |Mean|.
% * *|Bootstrap|* (optional) - a boolean specifying whether bootstrapping
% has to be performed. The output parameters depending on bootstrap
% (meanTauB, stdTauB, ciTauB) can be used only if it is |true|. The default
% is |false|.
% * *|BootstrapSample|* (optional) - a scalar integer value greater than
% zero which indicates how many samples have to be used in the bootstrap.
% The default is |1000|.
% * *|UseParalle|* (optional) - a boolean specifying whether parallel 
% computation of bootstrap has to be used or not. The default is |false|.
% * *|Verbose|* (optional) - a boolean specifying whether additional
% information has to be displayed or not. If not specified, then |false| is 
% used as default.
%
% *Returns*
%
% * |tauB| - a table containing the pairwise Kendall's tau-B correlation
% among the provided measures according to the specified aggregation
% statitic. Row and column names are the short names of the analysed
% measures. The |UserData| property contains a struct with the following
% fields: |identifier| of the analysed run set and |pool| with the
% identifier of the pool with respect to which measures have been computed.
% * |meanTauB| (only if |Bootstrap| is |true| - a table containing the 
%  mean of the pairwise bootstrapped Kendall's tau-B correlation
% among the provided measures according to the specified aggregation
% statitic. Row and column names are the short names of the analysed
% measures. The |UserData| property contains a struct with the following
% fields: |identifier| of the analysed run set and |pool| with the
% identifier of the pool with respect to which measures have been computed.
% * |stdTauB| (only if |Bootstrap| is |true| - a table containing the 
% standad deviation of the pairwise bootstrapped Kendall's tau-B correlation
% among the provided measures according to the specified aggregation
% statitic. Row and column names are the short names of the analysed
% measures. The |UserData| property contains a struct with the following
% fields: |identifier| of the analysed run set and |pool| with the
% identifier of the pool with respect to which measures have been computed.
% * |ciTauB| (only if |Bootstrap| is |true| - a table containing the 
% confidence intervals of the pairwise bootstrapped Kendall's tau-B 
% correlation among the provided measures according to the specified 
% aggregation statitic. Row and column names are the short names of the 
% analysed measures, values are bidimensional vector where the first and
% secondo element are, respectively, the lower and upper bound of the
% confidence interval for the correlation of that measure pair.The 
% |UserData| property contains a struct with the following
% fields: |identifier| of the analysed run set and |pool| with the
% identifier of the pool with respect to which measures have been computed.

%% Example of use
%  
%   ap = averagePrecision(pool, runSet);
%   p10 = precision(pool, runSet, 'CutOffs', 10);
%   p20 = precision(pool, runSet, 'CutOffs', 20);
%   p30 = precision(pool, runSet, 'CutOffs', 30);
%   Rprec = precision(pool, runSet, 'Rprec', true);
%   [tauB, meanTauB, stdTauB, ciTauB] = kendall(ap, p10, p20, p30, Rprec, 'Bootstrap', true)
%
% It computes the correlation between AP, P@10, P@20, P@30, and Rprec, also
% using the bootstrap
%
% It returns the |tauB| table.
%
%   tauB = 
%
%               AP        P_10       P_20       P_30       Rprec 
%             _______    _______    _______    _______    _______
%
%    AP             1    0.74885    0.75949    0.71954    0.94483
%    P_10     0.74885          1    0.91119     0.8318    0.73042
%    P_20     0.75949    0.91119          1    0.88378    0.74108
%    P_30     0.71954     0.8318    0.88378          1    0.72874
%    Rprec    0.94483    0.73042    0.74108    0.72874          1
%
%
% The |meanTauB| and |stdTauB| tables have the same structure as the |tauB|
% table. 
%
% It returns the |ciTauB| table.
%
%   ciTauB = 
%                     AP                    P_10                   P_20        
%           ___________________    ___________________    ___________________
%
%    AP             1           1    0.57866     0.84471    0.59559     0.85289
%    P_10     0.57866     0.84471          1           1    0.72922     0.96674
%    P_20     0.59559     0.85289    0.72922     0.96674          1           1
%    P_30     0.47958     0.84234    0.66284     0.90703    0.71535     0.94313
%    Rprec    0.87039     0.98095    0.53826     0.82679    0.53358      0.8335
%
%
%                    P_30                   Rprec       
%             ___________________    ___________________
%
%    AP       0.47958     0.84234    0.87039     0.98095
%    P_10     0.66284     0.90703    0.53826     0.82679
%    P_20     0.71535     0.94313    0.53358      0.8335
%    P_30           1           1    0.52307     0.84435
%    Rprec    0.52307     0.84435          1           1
%
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
function [asl, aslRatio, delta, t, tRatio] = sensitivity(varargin)
   
    % check that we have the correct number of input arguments. 
    narginchk(1, inf);
    
    % ensure we have a table of measures as first input
    validateattributes(varargin{1}, {'table'}, {'nonempty'}, '', 'measuredRunSet', 1);
    
    % only numeric and scalar values can be processed
    if ~isnumeric(varargin{1}{1, 1}) || ~isscalar(varargin{1}{1, 1})
         error('MATTERS:IllegalArgument', 'Run set %s for measure %s does not contain numeric scalar values.', ...
            varargin{1}.Properties.UserData.identifier, varargin{1}.Properties.UserData.shortName);       
    end;
    
    % the systems to be compared
    dataset = varargin{1}{:, :};
    
    % number of systems to be compared
    s = size(dataset, 2);

    % total number of pair to be examined
    pairs = nchoosek(s, 2);
        
    nvp = {};
    if nargin > 1
        nvp = varargin(2:end);
    end;
          
    % parse the variable inputs
    pnames = {'IgnoreNaN', 'Alpha', 'BootstrapSamples', 'UseParallel', 'Verbose'};
    dflts =  {false,       0.05,    1000,               false,         false};
    [ignoreNaN, alpha, bootstrapSamples, useParallel, verbose, supplied] ...
         = matlab.internal.table.parseArgs(pnames, dflts, nvp{:});
    
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
        % check that BootstrapSamples is a nonempty scalar integer value
        % greater than 0
        validateattributes(bootstrapSamples, {'numeric'}, ...
            {'nonempty', 'scalar', 'integer', '>', 0}, '', 'BootstrapSamples');
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
        
        fprintf('Computing Paired Bootstrap Test for run set %s and measure %s with respect to pool %s: %d runs and % pairs to be processed.\n\n', ...
            varargin{1}.Properties.UserData.identifier, ....
            varargin{1}.Properties.UserData.shortName, ...
            varargin{1}.Properties.UserData.pool, s, pairs);

        fprintf('Settings:\n');
        
        fprintf('  - confidence level alpha %d;\n', alpha);
        
        if ignoreNaN
           fprintf('  - NaN values will be ignored;\n');
        else
           fprintf('  - NaN values will not be ignored;\n');
        end
        
        fprintf('  - bootstrap data samples: %d;\n', bootstrapSamples);

        if useParallel
            fprintf('  - parallel computing will be used;\n');
        else
            fprintf('  - parallel computing will not be used;\n');
        end;
                
        fprintf('\n');
    end;
       

    % compute the Student's t test on each pair of systems
    [t, tRatio] = computeTTest();
    
    % return the list of text files in the directory. It ignores non-text
    % files.
    path = '/Users/ferro/Documents/progetti/software/matters/output/bootsamples/';
    files = dir([path 'test*']);
    n = length(files);
    
    bootstrapSamples = n;

    % extract an horizontal cell array of file names
    [bootSamples{1:n}] = deal(files.name);

    % concatenate the file names with the directory
    bootSamples = strcat(repmat(path, n, 1), bootSamples.')
    
    % compute the Paired Bootstrap test on each pair of systems
    [asl, aslRatio, delta] = bootstrap();
        
    % transform t into a table
    t = array2table(t);
    t.Properties.RowNames = varargin{1}.Properties.VariableNames;
    t.Properties.VariableNames = varargin{1}.Properties.VariableNames;
    
    % transform asl into a table
    asl = array2table(asl);
    asl.Properties.RowNames = varargin{1}.Properties.VariableNames;
    asl.Properties.VariableNames = varargin{1}.Properties.VariableNames;
    
  
    %%
    
    % Computes the two tails paired Student's t test for each pair of systems.
    function [t, tRatio] = computeTTest()

        % when we do not check for NaN, this is the number of compared samples
        samplesize = size(dataset, 1);

        % the t statistic resulting from the Student's t test for each pair
        % of systems
        t = NaN(s);

        % the ratio of the  number of pairs that are significantly 
        % different with respect to the total number of compared pairs
        tRatio = 0;

        % compute only the upper triangle
        for r = 1:s
            for c = r+1:s

                % compute the difference between two systems
                z = dataset(:, r) - dataset(:, c);

                if ignoreNaN
                    zMean = nanmean(z);
                    zStd = nanstd(z);

                    % if we have NaN values, the sample size is less than 
                    % the number of rows of the dataset 
                    samplesize = sum(~isnan(z));
                else
                    zMean = mean(z);
                    zStd = std(z);  

                    % the sample size is always equale to the number of 
                    % rows of the dataset, when we do not check for NaN. 
                    % This has been already computed outside the for
                end;

                % compute the t statitics
                t(r, c) = zMean ./ (zStd ./ sqrt(samplesize));

                % check if they are significantly different, ensuring that the
                % degrees of freedom are zero also in the case that all samples
                % are NaN
                if abs(t(r, c)) > tinv(1 - alpha/2, max(samplesize - 1, 0))
                    tRatio = tRatio + 1;
                end;

            end;
        end;   

        % compute the final ratio
        tRatio = tRatio ./ pairs;
    end  % computeTTest 
    
    %%
    
    %    
    function [asl, aslRatio, delta] = bootstrap()

        % the achieved significance level resulting from the test
        asl = NaN(s);
        
        % the ratio of the  number of pairs that are significantly 
        % different with respect to the total number of compared pairs
        aslRatio = 0;
        
        % estimated performance difference required for achieving a given
        % asl
        delta = NaN(1, s);
        
        % fraction of the samples to pick up 
        ba = ceil(alpha * bootstrapSamples);

        % compute only the upper triangle
        for r = 1:s
            for c = r+1:s 

                % compute the difference between two systems
                z = dataset(:, r) - dataset(:, c);

                if ignoreNaN
                    zMean = nanmean(z);
                else
                    zMean = mean(z);
                end;

                % test the difference again the hypotesis of zero mean
                %ptb = bootstrp(bootstrapSamples, @computePairedBootstrapTest, z, zMean);
                ptb = NaN(n, 2);
                                                
                fprintf('\n%s vs %s \n', varargin{1}.Properties.VariableNames{r}, varargin{1}.Properties.VariableNames{c});
                fprintf('#NR = %d mean = %5.4f std %5.4f tobs = %5.4f\n', size(z, 1), zMean, std(z), t(r, c));
                
                for f = 1:n
                    
                    name = bootSamples{f};
                    name = name(find(name == filesep, 1, 'last')+1:end);

                    
                    fprintf('%4d (%s) ', f, name);
                    
                    fid = fopen(bootSamples{f});
                    topics = textscan(fid, '%s');
                    fclose(fid);
                    topics = cellstr(topics{:});
                    
                    [~, loc] = ismember(topics, varargin{1}.Properties.RowNames);
                    
                    ptb(f, :) = computePairedBootstrapTest(z(loc), zMean);

                    
                    
                end;
                                                
                asl(r, c) = length(find(abs(ptb(:, 1)) >= abs(t(r, c)))) / bootstrapSamples;
                
                if asl(r, c) < alpha
                    aslRatio = aslRatio + 1;
                end;
                
                % sort the obtained t statistics in descending order (NaN
                % first)
                tmp = sort(abs(ptb(:, 1)), 'descend');
                
                % get the difference for the values of the t statistics
                % which corresponds to the bootstrapSamples*alpha largest
                % value
                delta(r) = abs(ptb(abs(ptb(:, 1)) == tmp(ba), 2));
            end;
        end; 
                
        % compute the final ratio
        aslRatio = aslRatio ./ pairs;
        
        % get the maximum difference
        delta = max(delta);
        
    end % bootstrap

    %%

    % computes the Paired Bootstrap Test and the estimated difference for one
    % replicate of the bootstrap according to Figures 2 and 5 of 
    % Sakai, SIGIR 2006
     function [pbt] = computePairedBootstrapTest(zBootstrap, zMean)

        w = zBootstrap - zMean;
    
        if ignoreNaN
            wMean = nanmean(w);
            wStd = nanstd(w);

            % if we have NaN values, the sample size is less than the
            % number of rows of x
            samplesize = sum(~isnan(w));
        else
            wMean = mean(w);
            wStd = std(w);  

            samplesize = size(w, 1);
        end;

        tStat = wMean ./ (wStd ./ sqrt(samplesize));
        
        fprintf('NR = %d mean = %5.4f std = %5.4f absT = %5.4f\n', samplesize, wMean, wStd, abs(tStat));

        pbt = [tStat wMean];

     end % computePairedBootstrapTest

end % sensitivity