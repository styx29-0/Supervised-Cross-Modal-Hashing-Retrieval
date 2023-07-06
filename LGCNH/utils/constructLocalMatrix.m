function [ W ] = constructLocalMatrix( X, kneighbors)
    options = [];
    options.NeighborMode = 'KNN';
    options.k = kneighbors;
    options.WeightMode = 'Binary';
    W = constructW(X,options);
end