function Results_Visualize_Lotka_Volterra(file_data, file_conf, textTime, inputPath, outputPath)

%defines
show_figure = 1;	%if 1: display graph and wait

save_fig = 1;		%save figure in matlab format
save_eps = 0;		%save figure in eps format
save_pdf = 0;		%save figure in pdf format
save_png = 1;		%save figure in png format

plot_lines_width = 0.7;	%default is 0.5
conf_text_size = 8;	%size of text on left-side of figures

plot_state_axis_limit = 1; %limit Y axis on state and error plot; ALSO does not plot actions instead plots another state plot without axis limit)
plot_error_limit_value = 1.5; %set error plot Y axis limits

%CStr = dataread('file', 'file_conf', '%s', 'delimiter', '\n')
c_config = fileread([inputPath file_conf]);

%init script
scriptName = 'EEG';
current_datetime = now;

%INIT PLOT
if(show_figure)
	hFig = figure(1);
else
	hFig = figure('Visible','off');
end
clf(hFig);
set(hFig, 'Position', [200 100 1200 800])

%LOAD FILE
results = load([inputPath file_data]);

steps = results(:,1);
actions = results(:,2);
prey = results(:,3);
predator = results(:,4);
target = results(:,5);
error = results(:,6);
gamma = results(:,7);
param_tauXY = results(:,8);

%plot prey and predator
sp1 = axes('position',[0.2 0.55 0.3 0.35]);
plot(predator,'b','LineWidth',plot_lines_width);
hold on
plot(target,'g','LineWidth',plot_lines_width);
hold off
xlabel('Time steps');
legend('output','EEG sample','Location','NorthOutside','Orientation','horizontal');

%calculate and plot error
avg_sq_error = sqrt(sum(error.^2)/size(error,1));  %root of average sum squared errors

sprintf('%3.5f',avg_sq_error);
sp2 = axes('position',[0.61 0.55 0.3 0.35]);
%semilogy(abs(error),'LineWidth',plot_lines_width);
plot(abs(error),'LineWidth',plot_lines_width);
legend( ['avg sq error: ' num2str(avg_sq_error ,5)],'Location','NorthOutside','Orientation','horizontal' );
% if(plot_state_axis_limit)
%     ylim([max(-plot_error_limit_value,min(error)) min(plot_error_limit_value,max(error))])
% end

%plot control parameters values
sp3 = axes('position',[0.2 0.08 0.3 0.35]);
plot(gamma,'r','LineWidth',plot_lines_width);
hold on
plot(param_tauXY*1000.0,'g','LineWidth',plot_lines_width);
hold off
%legend('signal P','change factor','Location','NorthOutside','Orientation','horizontal' );
legend('signal P','tauXY x 1000','Location','NorthOutside','Orientation','horizontal' );
%legend('signal P','Location','NorthOutside','Orientation','horizontal' );

%plot actions (or state without axis-limit)
sp4 = axes('position',[0.61 0.08 0.3 0.35]);
plot(prey,'r','LineWidth',plot_lines_width/2);
hold on
plot(predator,'b','LineWidth',plot_lines_width);
plot(target,'g','LineWidth',plot_lines_width);
hold off
xlabel('Time steps');
legend('x','output','EEG sample','Location','NorthOutside','Orientation','horizontal');

%---- show text about configuration info ----

% Generate text
text_conf = sprintf('MlabScript: %s\n\n%s\n%s\n\nMErr: %3.6f\n\n\nC++config:\n\n%s\n\n',...
                scriptName,...
                datestr(current_datetime,'yyyy-mm-dd'),...
                datestr(current_datetime,'HH:MM:SS'),...				
				(sum((predator-target).^2,1)/size(predator,1))^0.5,...
                c_config...
            );
        
axes('position',[0.01 0.01 0.15 0.98],'visible','off')
text(0,1,text_conf,'HorizontalAlignment','left','VerticalAlignment','top','Interpreter', 'none','FontSize',conf_text_size)
   
       
% 
% %---- print to file ---- tukaj je vsega in svašta, èe bom rabil poravnavat save to file
% 
% %fp = fillPage(gcf, 'margins', [0 0 0 0], 'papersize', [11 8.5]);
% %daspect([1 1 1]);
% %set(hFig,'PaperPositionMode','auto'); 
% %set(hFig,'PaperOrientation','landscape');
% %set(hFig,'PaperPosition', [1 1 28 19]);
% %set(hFig,'PaperUnits','normalized');
% %set(hFig,'PaperPosition', [0 0 1 1]);
% %set(gcf,'PaperUnits','centimeters')
% %This sets the units of the current figure (gcf = get current figure) on paper to centimeters.
% %xSize = 8; ySize = 12;
% %These are my size variables, width of 8 and a height of 12, will be used a lot later.
% %xLeft = (21-xSize)/2; yTop = (30-ySize)/2;
% %Additional coordinates to center the figure on A4-paper
% %set(gcf,'PaperPosition',[xLeft yTop xSize ySize])
% %This command sets the position and size of the figure on the paper to the desired values.
% %set(gcf,'Position',[0 0 xSize*50 ySize*50])
%set(gcf,'PaperUnits','centimeters')
%set(gcf,'PaperPosition',[0 0 20 10])     

config_lines = strread(c_config,'%s','delimiter','\n');
problemName = config_lines{1};
problemSolver = config_lines{6};
filename = [outputPath problemName '__' problemSolver textTime];
if(save_fig)
    saveas(hFig, [filename '.fig']);
end
if(save_eps)
    print(hFig, '-depsc2', [filename '.eps']);
end
if(save_pdf)
    print(hFig, '-dpdf', [filename '.pdf']);
end
if(save_png)
    print(hFig, '-dpng','-r400', [filename '.png']);
end

if(show_figure)
 	pause();
end