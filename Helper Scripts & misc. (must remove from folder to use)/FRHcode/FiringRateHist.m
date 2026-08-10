%% Firing Rate Histograms for SDH Data:
% Code written by L. Medlock (Mar2021)

% load in the data
clearvars
filename='200mN-Control';
load(filename)

% Setting and Parameters
totCells = max(simData.spkid)+1;
indx = 0:totCells;

EndTime = 5;  % 5s long recording
CellTypes = struct2cell(net.params.popParams);
numCellTypes = length(CellTypes);
time = simData.t;
timeMs = 1:length(time)/1000*(1000*time(2))+1;

binSize = 10;                                % bin size in ms
bins = 0:1:length(time)/1000*(1000*time(2)); % bins for histogram
binsNew = bins*binSize;

Cellcmap = load('SDH-colormap.mat');
Cellcmap = Cellcmap.Cellcmap;
clrs = zeros(totCells,3);

PopRates = struct2cell(simData.popRates);

for t = 1:numCellTypes
     
    numAff(t,1) = CellTypes{t,1}.numCells;
    numIndx{t,1} = indx(1):indx(numAff(t,1));
    indx(:,[1:numAff(t,1)]) = [];
    Name = erase(CellTypes{t,1}.pop,"_");
    affName{t,1} = Name;
    NewNumIndx = numIndx{t}+1;

    for v = 1:length(NewNumIndx)

        this = NewNumIndx(v);
        clrs(this,:) = Cellcmap(t,:); % assign a colour to that spike train

    end

end

%% Reorder Neuron Populations
% Reorder cell numbers
% Combine SA1 and SA2 for Abeta population:
ABeta = {numIndx{1};numIndx{2}};
ABeta = horzcat(ABeta{:});

ReCellNum = {ABeta;numIndx{3};numIndx{4};numIndx{5};...  % primary afferents
              numIndx{6};numIndx{7};numIndx{9};numIndx{10};numIndx{12};numIndx{13};... % excitatory int
              numIndx{8};numIndx{11};numIndx{14};...  % inhibitory neurons
              numIndx{15}};  % projection neurons
          
NewCellNum = horzcat(ReCellNum{:})';  % reorder the cell numbers for plotting

numCellTypesNew = length(ReCellNum);
    
% Reorder afferent names
NewAffNames = {'AB';affName{3};'C,TRPV1';'C,IB4';...  % primary afferents
              affName{6};affName{7};affName{9};affName{10};affName{12};affName{13};... % excitatory int
              affName{8};affName{11};affName{14};...  % inhibitory neurons
              affName{15}};  % projection neurons

% Reorder colors per population
NewClr = [];              
for h = 1:numCellTypesNew  % Combined SA1 and SA2 so Num of Aff is now 14 not 15
          
ClrVect = clrs(min(ReCellNum{h}+1):max(ReCellNum{h}+1),:);
NewClr = [NewClr ; ClrVect];  %  for plotting rasters

end

% for FRH plotting:
unqNewClr = [Cellcmap(2,:);Cellcmap(3,:);Cellcmap(4,:);Cellcmap(5,:);...  % primary afferents
              Cellcmap(6,:);Cellcmap(7,:);Cellcmap(9,:);Cellcmap(10,:);Cellcmap(12,:);Cellcmap(13,:);... % excitatory int
              Cellcmap(8,:);Cellcmap(11,:);Cellcmap(14,:);...  % inhibitory neurons
              Cellcmap(15,:)];  % projection neurons

          
% Inverting order of neurons:
ReCellNum = flip(ReCellNum);
NewCellNum = flip(NewCellNum);
NewAffNames = flip(NewAffNames);
NewClr = flip(NewClr);
unqNewClr = flip(unqNewClr);

%% Plotting FRH (for Figure 3)
   
kWid = 100;  % width of kernel 

%fig=figure; 
hold on
for k = 1:numCellTypesNew

    neurIdx = ReCellNum(k);
    neurIdx = neurIdx{:};

    for h = 1:length(neurIdx)

       neur = neurIdx(h);
       neurLog = simData.spkid == neur;
       neurSpk = simData.spkt(neurLog);
       spikeArrG{h,1} = neurSpk;

    end

    spikeArr = spikeArrG(1:length(neurIdx),1); % only analyze afferents from this fibre type
    spiketimes{k,1} = horzcat(spikeArr{:});
    spiketimesSort = sort(spiketimes{k,1});

    F_binary = histcounts(spiketimesSort,time)';
    psth_total = PSTH_(F_binary, 0.025, 0.025);
    KSpikes = KernelPSTH(psth_total, kWid, 0.025);

    NoSpikes = isnan(KSpikes);
    KSpikes(NoSpikes)=0;    % if there is no spiking KSpikes =0

    Trial_num = length(neurIdx); % number of neurons for this type
    fr_neuron = sum(psth_total)/EndTime/Trial_num; % Avg firing rate (# spikes / s / neuron)
    scale_ = (fr_neuron/mean(KSpikes));           % average firing rate / average KSpikes (for scaling factor)
    scale_(isnan(scale_))=0;   % if there is no spiking scale = 0

    FRSave{k,1} = KSpikes*scale_;


% plotting by type of afferent:
    if k==11 || k==12 || k==13 || k==14   % afferent fibres
    %subplot(4,2,8)
    subplot(4,1,1)
    plot(time(1:length(KSpikes)),KSpikes*scale_,'Color',unqNewClr(k,:),'LineWidth',1.5) %
    hold on
    xlim([0,5000])
    ylim([-5,50]); yticks([0 25 50])
    box off
    set(gca,'FontSize',14,'TickDir', 'out','xticklabels',[],'TickLength',[0.01,0.025]) 
    title1 = erase(filename,".mat");
    title1 = append(title1,' Stimulus');
    title(title1)
        
    elseif k==5 || k==6 || k==7 || k==8 || k==9 || k==10  % excitatory neurons
    subplot(4,1,2)
    plot(time(1:length(KSpikes)),KSpikes*scale_,'Color',unqNewClr(k,:),'LineWidth',1.5)
    hold on
    xlim([0,5000])
    ylim([-5,150]); yticks([0 50 100 150])
    box off
    set(gca,'FontSize',14,'TickDir', 'out','xticklabels',[],'TickLength',[0.01,0.025]) 
  
    elseif k==2 || k==3 || k==4  % inhibitory neurons
    subplot(4,1,3)
    plot(time(1:length(KSpikes)),KSpikes*scale_,'Color',unqNewClr(k,:),'LineWidth',1.5)
    hold on
    xlim([0,5000])
    ylim([-5,75]); yticks([0 25 50 75])
    box off
    set(gca,'FontSize',14,'TickDir', 'out','xticklabels',[],'TickLength',[0.01,0.025]) 

    elseif k==1    % projection neuron
    subplot(4,1,4)
    plot(time(1:length(KSpikes)),KSpikes*scale_,'Color',unqNewClr(k,:),'LineWidth',1.5)  
    xlim([0,5000]); xticks([0 1000 2000 3000 4000 5000]); xticklabels([0 1 2 3 4 5]);
    ylim([-5,50]); yticks([0 25 50]);
    xlabel('Time (s)')
    box off
    set(gca,'FontSize',14,'TickDir', 'out','TickLength',[0.01,0.025]) 
    end

end


