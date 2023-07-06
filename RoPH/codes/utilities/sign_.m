function X = sign_(X, X1)

if(nargin<2)
    X = sign(X);
else
    pos = X~=0;
    X(pos) = sign(X(pos));
    pos = ~pos;
    X(pos) = X1(pos);
end

end