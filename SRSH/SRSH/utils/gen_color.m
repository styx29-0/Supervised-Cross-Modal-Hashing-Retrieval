function color=gen_color(curve_idx)

colors=[];

colors{end+1}='g';
colors{end+1}='b';
colors{end+1}='c';
colors{end+1}='m';
colors{end+1}=[0 0.7 0.7];
colors{end+1}='k';

colors{end+1}=[0.5 0.5 0.5];
colors{end+1}=[0.3 0.8 0.9];
colors{end+1}=[0.75 0.2 0];
colors{end+1}=[1 0.5 0];

colors{end+1}='r';

colors{end+1}=[0.7 0 0.7];



sel_idx=mod(curve_idx-1, length(colors))+1;
color=colors{sel_idx};

end