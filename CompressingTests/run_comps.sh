### 25%
echo "Compressing mutated1_chry.txt with gzip" && 
gzip -c -v mutated1_chry.txt > ./gzip/mutated1_chry.gz && 
echo "Compressing mutated1_chry.txt p7zip" && 
7z a ./7zip/mutated1_chry.7z mutated1_chry.txt && 
echo "Compressing mutated1_chry.txt with bzip2" && 
bzip2 -k -v mutated1_chry.txt && 
mv mutated1_chry.txt.bz2 ./bzip2 && 
echo "Compressing mutated1_chry.txt with lz4" && 
lz4 -v mutated1_chry.txt && 
mv mutated1_chry.txt.lz4 ./lz4 && 
echo "Compressing mutated1_chry.txt with tar" && 
tar cvf ./tar/mutated1_chry.tar mutated1_chry.txt && 
echo "Compressing mutated1_chry.txt zstd" && 
zstd -v mutated1_chry.txt && 
mv mutated1_chry.txt.zst ./zstd &&

### 50%
echo "Compressing mutated2_chry.txt with gzip" && 
gzip -c -v mutated2_chry.txt > ./gzip/mutated2_chry.gz && 
echo "Compressing mutated2_chry.txt p7zip" && 
7z a ./7zip/mutated2_chry.7z mutated2_chry.txt && 
echo "Compressing mutated1_chry.txt with bzip2" && 
bzip2 -k -v mutated2_chry.txt && 
mv mutated2_chry.txt.bz2 ./bzip2 && 
echo "Compressing mutated2_chry.txt with lz4" && 
lz4 -v mutated2_chry.txt && 
mv mutated2_chry.txt.lz4 ./lz4 && 
echo "Compressing mutated2_chry.txt with tar" && 
tar cvf ./tar/mutated2_chry.tar mutated2_chry.txt && 
echo "Compressing mutated2_chry.txt zstd" && 
zstd -v mutated2_chry.txt && 
mv mutated2_chry.txt.zst ./zstd &&

### 75%
echo "Compressing mutated3_chry.txt with gzip" && 
gzip -c -v mutated3_chry.txt > ./gzip/mutated3_chry.gz && 
echo "Compressing mutated3_chry.txt p7zip" && 
7z a ./7zip/mutated3_chry.7z mutated3_chry.txt && 
echo "Compressing mutated3_chry.txt with bzip2" && 
bzip2 -k -v mutated3_chry.txt && 
mv mutated3_chry.txt.bz2 ./bzip2 && 
echo "Compressing mutated3_chry.txt with lz4" && 
lz4 -v mutated3_chry.txt && 
mv mutated3_chry.txt.lz4 ./lz4 && 
echo "Compressing mutated3_chry.txt with tar" && 
tar cvf ./tar/mutated3_chry.tar mutated3_chry.txt && 
echo "Compressing mutated3_chry.txt zstd" && 
zstd -v mutated3_chry.txt && 
mv mutated3_chry.txt.zst ./zstd &&


### 100%
echo "Compressing mutated4_chry.txt with gzip" && 
gzip -c -v mutated4_chry.txt > ./gzip/mutated4_chry.gz && 
echo "Compressing mutated4_chry.txt p7zip" && 
7z a ./7zip/mutated4_chry.7z mutated4_chry.txt && 
echo "Compressing mutated4_chry.txt with bzip2" && 
bzip2 -k -v mutated4_chry.txt && 
mv mutated4_chry.txt.bz2 ./bzip2 && 
echo "Compressing mutated4_chry.txt with lz4" && 
lz4 -v mutated4_chry.txt && 
mv mutated4_chry.txt.lz4 ./lz4 && 
echo "Compressing mutated4_chry.txt with tar" && 
tar cvf ./tar/mutated4_chry.tar mutated4_chry.txt && 
echo "Compressing mutated4_chry.txt zstd" && 
zstd -v mutated4_chry.txt && 
mv mutated4_chry.txt.zst ./zstd