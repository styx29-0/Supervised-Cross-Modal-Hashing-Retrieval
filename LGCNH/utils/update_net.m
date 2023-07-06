function gpu_net = update_net(gpu_net,res_back,lr,N,n_layers,batch_size)
    weight_decay = 5*10^-4;
    for ii = 1:n_layers
        if ~isempty(gpu_net.layers{ii}.weights)
            gpu_net.layers{ii}.weights{1} = gpu_net.layers{ii}.weights{1} - ...
                lr*(res_back(ii).dzdw{1}/(batch_size*N) + weight_decay*gpu_net.layers{ii}.weights{1});
            gpu_net.layers{ii}.weights{2} = gpu_net.layers{ii}.weights{2} - ...
                lr*(res_back(ii).dzdw{2}/(batch_size*N) + weight_decay*gpu_net.layers{ii}.weights{2});
        end
    end
end