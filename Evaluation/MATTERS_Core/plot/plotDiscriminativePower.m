%% plotDiscriminativePower
% 
% It plot the discriminative power of a set of runs.
%
%% Synopsis
%
%   plotDiscriminativePower(asl, varargin)
%
% *Parameters*
%
% * *|asl|* - a table containing the achieved significance level for each
% measures. It is a table in the same format returned by the output |asl| 
% in <../analysis/discriminativePower.html discriminativePower>.
%
% *Name-Value Pair Arguments*
%
% Specify comma-separated pairs of |Name|, |Value| arguments. |Name| is the 
% argument name and |Value| is the corresponding value. |Name| must appear 
% inside single quotes (' '). You can specify several name and value pair 
% arguments in any order as |Name1, Value1, ..., NameN, ValueN|.
%
% * *|Title|* (mandatory) - a string specifying the title of the plot.
% * *|OutputPath|* (optional) - a string specifying the path to the output 
% directory where the PDF of the plot will be saved. If not specified, the
% plot will not be saved to a PDF.
% * *|Observed|* (mandatory) - a cell vector of strings containing the list 
% of measure short names in |asl| to be considered as observations.
% * *|Reference|* (optional) -  a cell vector of strings containing the list 
% of measure short names in |asl| to be considered as references.
% Reference performances will be plot with a different color and line style
% with respect to observed performances.
% * *|Dp|* (optional) -  a table containing the discriminative power 
% for each measure. It is a table in the same format 
% returned by the output |dp| in <../analysis/discriminativePower.html discriminativePower>. 
% * *|Delta|* (optional) -  a table containing the needed delta for each 
% measure. It is a table in the same format returned by the output
% |delta| in <../analysis/discriminativePower.html discriminativePower>. 
%
%% Example of use
%  
%   plotDiscriminativePower(asl, 'Title', 'Campaign 2014, Ad Hoc', ...
%                  'Observed', {'P_10', 'P_100', 'Rprec'}, ...
%                  'Reference', {'AP', 'bpref', 'RBP_080'}, ....
%                  'Dp', dp, ...
%                  'Delta', delta, ...
%                  'OutputPath', '/output')
%
% It produces the following plot and saves it to a PDF file in the provided
% output directory.
%
% 
% <<discriminativePower.png>>
% 
% You can note as |Observed| performances are plotted in blu with a
% continuous line while |Reference| performance are plotted in black with a
% dashed line.
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
function [] = plotDiscriminativePower(asl, varargin)

    persistent MARKERS ...
        REF_LINESTYLE REF_LINEWIDTH REF_MARKERSIZE ...
        OBS_LINESTYLE OBS_LINEWIDTH OBS_MARKERSIZE ...
        X_LIM_STEP;
    
    if isempty(MARKERS)
        
        MARKERS = {'o', '+', '*', 'x', 's', 'd', '^', 'v', '>', '<', 'p', 'h'};

        REF_LINESTYLE = '--';
        REF_LINEWIDTH = 1.0;
        REF_MARKERSIZE = 6;
        
        OBS_LINESTYLE = '-';
        OBS_LINEWIDTH = 1.2;
        OBS_MARKERSIZE = 7; 
        
        X_LIM_STEP = 50;
    end;
       
    % check that we have the correct number of input arguments. 
    narginchk(1, inf);
    
     % parse the variable inputs
    pnames = {'Title' 'OutputPath' 'Reference', 'Observed', 'Dp', 'Delta'};
    dflts =  {[]      []           []           []          []          []};
    [plotTitle, outputPath, reference, observed, dp, delta, supplied] = matlab.internal.table.parseArgs(pnames, dflts, varargin{:});
       
    if supplied.OutputPath
        % check that path is a non-empty string
        validateattributes(outputPath, {'char', 'cell'}, {'nonempty', 'vector'}, '', 'OutputPath');
        
        if iscell(outputPath)
            % check that path is a cell array of strings with one element
            assert(iscellstr(outputPath) && numel(outputPathpath) == 1, ...
                'MATTERS:IllegalArgument', 'Expected OutputPath to be a cell array of strings containing just one string.');
        end

        % remove useless white spaces, if any, and ensure it is a char row
        outputPath = char(strtrim(outputPath));
        outputPath = outputPath(:).';

        % check if the path is a directory and if it exists
        if ~(isdir(outputPath))
            error('MATTERS:IllegalArgument', 'Expected OutputPath to be a directory.');
        end;

        % check if the given directory path has the correct separator at the
        % end.
        if outputPath(end) ~= filesep;
           outputPath(end + 1) = filesep;
        end; 
    end;
     
    if supplied.Title
        % check that title is a non-empty string
        validateattributes(plotTitle, {'char', 'cell'}, {'nonempty', 'vector'}, '', 'Title');
    
         if iscell(plotTitle)
            % check that title is a cell array of strings with one element
            assert(iscellstr(plotTitle) && numel(plotTitle) == 1, ...
                'MATTERS:IllegalArgument', 'Expected Title to be a cell array of strings containing just one string.');
        end
        
        % remove useless white spaces, if any, and ensure it is a char row
        plotTitle = char(strtrim(plotTitle));
        plotTitle = plotTitle(:).'; 
        plotTitle = strrep(plotTitle, '_', '\_');
    else
        error('MATTERS:MissingArgument', 'Parameter ''Title'' not provided: the title of the plot is mandatory.');
    end;
    
    if supplied.Dp
        % check that each element is a non-empty table
        validateattributes(dp, {'table'}, {'nonempty'}, '', 'ASLRatio', 1);    
    end;
    
     if supplied.Delta
        % check that each element is a non-empty table
        validateattributes(delta, {'table'}, {'nonempty'}, '', 'Delta', 1);    
    end;
                  
    if supplied.Observed
        % check that observed is a cell array
        validateattributes(observed, {'cell'}, {'nonempty', 'vector'}, '', 'Observed');

        % check that observed is a cell array of strings with one element
        assert(iscellstr(observed), ...
        'MATTERS:IllegalArgument', 'Expected Observed to be a cell array of strings.');

        % remove useless white spaces, if any, and ensure it is a char row
        observed = strtrim(observed);
        observed = observed(:).';
        
        % check that all the requested observed measures are part of the ASL table
        tmp = setdiff(observed, asl.Properties.RowNames);
        if ~isempty(tmp)        
            error('MATTERS:IllegalArgument', 'The following requested observed measures %s are not part of the ASL table for run set %s%s.', ...
                strjoin(tmp, ', '), asl.Properties.UserData.identifier); 
        end;       
    else
        error('MATTERS:MissingArgument', 'Parameter ''Observed'' not provided: the observed performances are mandatory.');
    end;
    
    % the labels for the plot legend
    legendLabels = observed;
    
    % markers for the observed performances
    obsMarkers = MARKERS;
    
    if supplied.Reference
        % check that reference is a cell array
        validateattributes(reference, {'cell'}, {'nonempty', 'vector'}, '', 'Reference');

        % check that observed is a cell array of strings with one element
        assert(iscellstr(reference), ...
        'MATTERS:IllegalArgument', 'Expected Reference to be a cell array of strings.');

        % remove useless white spaces, if any, and ensure it is a char row
        reference = strtrim(reference);
        reference = reference(:).';
        
        % check that all the requested reference measures are part of the ASL table
        tmp = setdiff(reference, asl.Properties.RowNames);
        if ~isempty(tmp)        
            error('MATTERS:IllegalArgument', 'The following requested reference measures %s are not part of the ASL table for run set %s%s.', ...
                strjoin(tmp, ', '), asl.Properties.UserData.identifier); 
        end;
                    
        % add the short name of the measure as label for the plot
        legendLabels = [legendLabels reference];
        
        % do not use more than half of the markers for the reference
        % performances
        refMarkers = MARKERS(1:min(length(reference), length(MARKERS)/2));
        
        % assign all the remaining markers for the observed performances
        obsMarkers = MARKERS(length(refMarkers)+1:end);
        
    end;
    
    % adjust the legend, if needed
    if supplied.Dp || supplied.Delta
        
        for k = 1:length(legendLabels)
            
            tmp = [legendLabels{k} ' ['];
            
            if supplied.Dp                
                tmp = [tmp 'DP = ' num2str(dp{legendLabels{k}, 1}*100, '%4.2f%%')];
            end;
            
            if supplied.Delta
                if supplied.Dp                
                    tmp = [tmp '; '];
                end;
                
                tmp = [tmp '\Delta = ' num2str(delta{legendLabels{k}, 1}, '%4.2f')];
            end;
            
            tmp = [tmp ']'];
            
            legendLabels{k} = tmp;
            
        end;
        
    end
    
    legendLabels = strrep(legendLabels, '_', '\_');
            
    switch lower(asl.Properties.UserData.shortMethod)
        case {'pbt'}
            plotTitle = {plotTitle; 'ASL using Paired Bootstrap Test'};
        case 'rthsdt'
            plotTitle = {plotTitle; 'ASL using Randomised Tukey HSD Test'};
        otherwise
            error('MATTERS:IllegalArgument', 'Unrecognized discriminative power method %s. Only Paired Bootstrap Test and Randomised Tukey HSD Test are allowed', ...
                asl.Properties.UserData.method);
    end;
    
    % adjust the title, if needed
    if supplied.Dp || supplied.Delta
        plotTitle{2} = [plotTitle{2} ', \alpha = ' num2str(asl.Properties.UserData.alpha, '%3.2f')];
    end;
    
    obsColors = hsv(length(observed));
    refColors = copper(length(reference));

        
    % if output path is supplied, hide the figure to avoid render it on
    % screen when it is actually saved to a file
    if supplied.OutputPath
        h = figure('Visible', 'off');
    else
        h = figure;
    end;
    hold on
        
        % plot the observed performances
        for k = 1:length(observed)
            tmp = asl{observed{k}, 1}{1, 1}{:, :};
            tmp = sort(tmp(~isnan(tmp)), 1, 'descend');
            
            plot(tmp, 'Color', obsColors(k, :), 'LineStyle', OBS_LINESTYLE, ...
                'Marker', obsMarkers{mod(k, length(obsMarkers)) + 1}, ...
                'LineWidth', OBS_LINEWIDTH, 'MarkerSize', OBS_MARKERSIZE, ...
                'MarkerFaceColor', obsColors(k, :));
        end
    
        % plot the reference performances
        for k = 1:length(reference)
            tmp = asl{reference{k}, 1}{1, 1}{:, :};
            tmp = sort(tmp(~isnan(tmp)), 1, 'descend');
                        
            plot(tmp, 'Color', refColors(k, :), 'LineStyle', REF_LINESTYLE, ...
                'Marker', refMarkers{mod(k, length(refMarkers)) + 1}, ...
                'LineWidth', REF_LINEWIDTH, 'MarkerSize', REF_MARKERSIZE, ...
                'MarkerFaceColor', refColors(k, :));
        end
        
        set(gca, 'FontSize', 14);

        set(gca, 'XLim', [0 (fix(length(tmp)/X_LIM_STEP) + 1)*X_LIM_STEP])
        
        set(gca, 'YLim', [0 0.15], ...
            'YTick', [0:0.01:0.15], ...
            'YTickLabel', cellstr(num2str([0:0.01:0.15].', '%3.2f')).');
        
        grid on;

        hl = legend(legendLabels{:}, 'Location', 'SouthWest');
        set(hl, 'FontSize', 12);

        xlabel('System Pair (sorted by decreasing ASL)')
        ylabel('Achieved Significance Level (ASL)')
        title(plotTitle);

        if ~isempty(outputPath)
            set(h,'PaperPositionMode','manual'); 
            set(h,'PaperUnits','normalized');
            set(h,'PaperPosition',[0.05 0.05 0.9 0.9]);
            set(h,'PaperType','A4');
            set(h,'PaperOrientation','landscape');
            print(h, '-dpdf', [outputPath 'discriminativePower_' uuid() '.pdf']);
            close(h);
        end;
   
    
            
end



