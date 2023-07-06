function [label, energy, energy_after] = GCMexPar(label_init, f, Q, label_cost, expansion, n_block, iterations)
%parallel GCMex using OpenMP, there are two labels 0 and 1
%f: the first-order cofficient of x*Q*x'+x*f'
%n_block: number of blocks
%iterations: number of iterations

%check inputs
if(~exist('n_block','var'))
    n_block = 10;
end

if(~exist('iterations','var'))
    iterations = 1;
end

if(~isa(label_cost,'single'))
    fprintf('label_cost must be single\n');
end

if(~isvector(label_init))
    fprintf('initial label must be a vector\n');
end
if(min(label_init)~=0||max(label_init)~=1)
    fprintf('initial labels must be 0 or 1\n');
end

if(~ismatrix(Q)||~issparse(Q))
    fprintf('pairwise term must be a sparse matrix\n');
end

label_init = label_init(:)';
label = label_init;
for iter = 1:iterations
    
    %generate block information
    [blocks,g_index] = gen_block(label_init, f, Q, label_cost, expansion, n_block);
    
    %call GCMex_parallel
    [label_cell,energy1,energy_after1] = GCMex_parallel(blocks);
    for tt = 1:length(label_cell)
        label(g_index{tt}) = label_cell{tt}(:)';
    end
    if(iter==1)
        energy = sum(energy1);
    end
    energy_after = sum(energy_after1);
    
    %update initial labels for subsequent updation
    label_init = label;
end

label = label';%convert to column vector

end

function [blocks,g_index] = gen_block(label_init, f, Q, label_cost, expansion, n_block)
%generate blocks
%
%blocks: cell structure, 5*n_block
%g_index: 1*n_block cell with each cell containing the corresponding index

num_pixels = length(label_init);
blocks = cell(5,n_block);
g_index = cell(1,n_block);

ind_zero = find(label_init==0);
num_zero = length(ind_zero);
perm = randperm(num_zero);
ind_zero = ind_zero(perm);
ind_one = find(label_init==1);
num_one = length(ind_one);
perm = randperm(num_one);
ind_one = ind_one(perm);
bs_zero = ceil(num_zero/n_block);
bs_one = ceil(num_one/n_block);
for i = 1:n_block
    id1 = ind_zero((i-1)*bs_zero+1:min(num_zero,i*bs_zero));
    id2 = ind_one((i-1)*bs_one+1:min(num_one,i*bs_one));
    ind = [id1,id2];
    ind_else = setdiff(1:num_pixels,ind);
    blocks{1,i} = label_init(ind);
    tmp = (f(:,ind)+2*label_init(ind_else)*Q(ind_else,ind)+diag(Q(ind,ind))')/2;
    blocks{2,i} = single([zeros(1,length(ind));tmp]);
    blocks{3,i} = Q(ind,ind);
    blocks{4,i} = label_cost;
    blocks{5,i} = expansion;
    g_index{i} = ind;
end

end