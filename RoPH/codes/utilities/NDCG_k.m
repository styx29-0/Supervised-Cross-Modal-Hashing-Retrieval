function ndcg = NDCG_k(traingnd, testgnd, IX, k)
%compute the NDCG metric
%
%inputs:
% traingnd,testgnd: each column is a label vector
% IX: ranked list, ntrain*ntest matrix
% k: ndcg@k
%
%outputs:
% ndcg:

if(islogical(traingnd))
    traingnd = sparse(double(traingnd));
end

if(islogical(testgnd))
    testgnd = sparse(double(testgnd));
end

IX = IX(1:k,:);
idx = (1:k)';
ndcg = 0;
for i = 1:size(testgnd,2)
    r = (testgnd(:,i)'*traingnd)';
    r1 = r(IX(:,i));
    a = sum((2.^(r1)-1)./log(1+idx));
    
    [~,ind] = sort(r,'descend');
    r2 = r(ind(1:k));
    b = sum((2.^(r2)-1)./log(1+idx));
    
    ndcg = ndcg + a/b;
end
ndcg = ndcg/size(testgnd,2);

end