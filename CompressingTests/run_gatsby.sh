### 25%
echo "Compressing mutated1_gatsby.txt with gzip" && 
gzip -c -v mutated1_gatsby.txt > ./gzip/mutated1_gatsby.gz && 
echo "Compressing mutated1_gatsby.txt p7zip" && 
7z a ./7zip/mutated1_gatsby.7z mutated1_gatsby.txt && 
echo "Compressing mutated1_gatsby.txt with bzip2" && 
bzip2 -k -v mutated1_gatsby.txt && 
mv mutated1_gatsby.txt.bz2 ./bzip2 && 
echo "Compressing mutated1_gatsby.txt with lz4" && 
lz4 -v mutated1_gatsby.txt && 
mv mutated1_gatsby.txt.lz4 ./lz4 && 
echo "Compressing mutated1_gatsby.txt with tar" && 
tar cvf ./tar/mutated1_gatsby.tar mutated1_gatsby.txt && 
echo "Compressing mutated1_gatsby.txt zstd" && 
zstd -v mutated1_gatsby.txt && 
mv mutated1_gatsby.txt.zst ./zstd &&

### 50%
echo "Compressing mutated2_gatsby.txt with gzip" && 
gzip -c -v mutated2_gatsby.txt > ./gzip/mutated2_gatsby.gz && 
echo "Compressing mutated2_gatsby.txt p7zip" && 
7z a ./7zip/mutated2_gatsby.7z mutated2_gatsby.txt && 
echo "Compressing mutated1_gatsby.txt with bzip2" && 
bzip2 -k -v mutated2_gatsby.txt && 
mv mutated2_gatsby.txt.bz2 ./bzip2 && 
echo "Compressing mutated2_gatsby.txt with lz4" && 
lz4 -v mutated2_gatsby.txt && 
mv mutated2_gatsby.txt.lz4 ./lz4 && 
echo "Compressing mutated2_gatsby.txt with tar" && 
tar cvf ./tar/mutated2_gatsby.tar mutated2_gatsby.txt && 
echo "Compressing mutated2_gatsby.txt zstd" && 
zstd -v mutated2_gatsby.txt && 
mv mutated2_gatsby.txt.zst ./zstd &&

### 75%
echo "Compressing mutated3_gatsby.txt with gzip" && 
gzip -c -v mutated3_gatsby.txt > ./gzip/mutated3_gatsby.gz && 
echo "Compressing mutated3_gatsby.txt p7zip" && 
7z a ./7zip/mutated3_gatsby.7z mutated3_gatsby.txt && 
echo "Compressing mutated3_gatsby.txt with bzip2" && 
bzip2 -k -v mutated3_gatsby.txt && 
mv mutated3_gatsby.txt.bz2 ./bzip2 && 
echo "Compressing mutated3_gatsby.txt with lz4" && 
lz4 -v mutated3_gatsby.txt && 
mv mutated3_gatsby.txt.lz4 ./lz4 && 
echo "Compressing mutated3_gatsby.txt with tar" && 
tar cvf ./tar/mutated3_gatsby.tar mutated3_gatsby.txt && 
echo "Compressing mutated3_gatsby.txt zstd" && 
zstd -v mutated3_gatsby.txt && 
mv mutated3_gatsby.txt.zst ./zstd &&


### 100%
echo "Compressing mutated4_gatsby.txt with gzip" && 
gzip -c -v mutated4_gatsby.txt > ./gzip/mutated4_gatsby.gz && 
echo "Compressing mutated4_gatsby.txt p7zip" && 
7z a ./7zip/mutated4_gatsby.7z mutated4_gatsby.txt && 
echo "Compressing mutated4_gatsby.txt with bzip2" && 
bzip2 -k -v mutated4_gatsby.txt && 
mv mutated4_gatsby.txt.bz2 ./bzip2 && 
echo "Compressing mutated4_gatsby.txt with lz4" && 
lz4 -v mutated4_gatsby.txt && 
mv mutated4_gatsby.txt.lz4 ./lz4 && 
echo "Compressing mutated4_gatsby.txt with tar" && 
tar cvf ./tar/mutated4_gatsby.tar mutated4_gatsby.txt && 
echo "Compressing mutated4_gatsby.txt zstd" && 
zstd -v mutated4_gatsby.txt && 
mv mutated4_gatsby.txt.zst ./zstd