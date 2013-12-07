%dynamics parameters
a = 0.4;
b = 0.06;
c = 0.2;
d = 0.02;

%simulation parameters
timestep = 0.001;
t_per_a = 1000;
duration_a = 100;


% -- calcuclate dynamics --

rab = zeros(duration_a,1);
fox = zeros(duration_a,1);
rew = zeros(duration_a,1);  %how many foxes are "exported"
rab(1) = 20;
fox(1) = 5;
%simulation duration
for i = 2:duration_a
    
    rab(i) = rab(i-1);
    fox(i) = fox(i-1);
    %simulation steps
    for j = 1:t_per_a
        drab =  rab(i) * (a - b*fox(i));
        dfox = -fox(i) * (c - d*rab(i));
        rab(i) = rab(i) + drab * timestep;
        fox(i) = fox(i) + dfox * timestep;
    end
    
    if( (fox(i) - fox(i-1)) / fox(i) > 0.05 )
        %rew(i) = fox(i) * 0.75;
        %fox(i) = fox(i) - rew(i);
    end

end

plot(rab);
hold on
plot(fox,'r');
plot(rew,'g');
hold off
sum(rew)