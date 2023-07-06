function cb = compactbit(b, wordsize)
%each column is a sample
% cb = compacted string of bits (using words of 'wordsize' bits)
% default wordsize is 8

if (nargin<2)
  wordsize = 8;
end

[nbits, nSamples] = size(b);
nwords = ceil(nbits/wordsize);

if (wordsize == 8)
  cb = zeros([nwords nSamples], 'uint8');
elseif (wordsize == 16)
  cb = zeros([nwords nSamples], 'uint16');
elseif (wordsize == 32)
  cb = zeros([nwords nSamples], 'uint32');
elseif (wordsize == 64)
  cb = zeros([nwords nSamples], 'uint64');
else
  error('unrecognized wordsize');
end

for j = 1:nbits
  w = ceil(j/wordsize);
  cb(w,:) = bitset(cb(w,:), mod(j-1,wordsize)+1, b(j,:));
end