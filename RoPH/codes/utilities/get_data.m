function data = get_data(data, split)

ind_train = split.ind_train;
ind_test = split.ind_test;

%view 1
data.X1_train = data.X1(:,ind_train);
data.X1_test = data.X1(:,ind_test);

%view2
data.X2_train = data.X2(:,ind_train);
data.X2_test = data.X2(:,ind_test);

%label
data.Y_train = data.Y(:,ind_train);
data.Y_test = data.Y(:,ind_test);

end