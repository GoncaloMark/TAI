### 25%
echo "Compressing mutated1_maias.txt with gzip" && 
gzip -c -v mutated1_maias.txt > ./gzip/mutated1_maias.gz && 
echo "Compressing mutated1_maias.txt p7zip" && 
7z a ./7zip/mutated1_maias.7z mutated1_maias.txt && 
echo "Compressing mutated1_maias.txt with bzip2" && 
bzip2 -k -v mutated1_maias.txt && 
mv mutated1_maias.txt.bz2 ./bzip2 && 
echo "Compressing mutated1_maias.txt with lz4" && 
lz4 -v mutated1_maias.txt && 
mv mutated1_maias.txt.lz4 ./lz4 && 
echo "Compressing mutated1_maias.txt with tar" && 
tar cvf ./tar/mutated1_maias.tar mutated1_maias.txt && 
echo "Compressing mutated1_maias.txt zstd" && 
zstd -v mutated1_maias.txt && 
mv mutated1_maias.txt.zst ./zstd &&

### 50%
echo "Compressing mutated2_maias.txt with gzip" && 
gzip -c -v mutated2_maias.txt > ./gzip/mutated2_maias.gz && 
echo "Compressing mutated2_maias.txt p7zip" && 
7z a ./7zip/mutated2_maias.7z mutated2_maias.txt && 
echo "Compressing mutated1_maias.txt with bzip2" && 
bzip2 -k -v mutated2_maias.txt && 
mv mutated2_maias.txt.bz2 ./bzip2 && 
echo "Compressing mutated2_maias.txt with lz4" && 
lz4 -v mutated2_maias.txt && 
mv mutated2_maias.txt.lz4 ./lz4 && 
echo "Compressing mutated2_maias.txt with tar" && 
tar cvf ./tar/mutated2_maias.tar mutated2_maias.txt && 
echo "Compressing mutated2_maias.txt zstd" && 
zstd -v mutated2_maias.txt && 
mv mutated2_maias.txt.zst ./zstd &&

### 75%
echo "Compressing mutated3_maias.txt with gzip" && 
gzip -c -v mutated3_maias.txt > ./gzip/mutated3_maias.gz && 
echo "Compressing mutated3_maias.txt p7zip" && 
7z a ./7zip/mutated3_maias.7z mutated3_maias.txt && 
echo "Compressing mutated3_maias.txt with bzip2" && 
bzip2 -k -v mutated3_maias.txt && 
mv mutated3_maias.txt.bz2 ./bzip2 && 
echo "Compressing mutated3_maias.txt with lz4" && 
lz4 -v mutated3_maias.txt && 
mv mutated3_maias.txt.lz4 ./lz4 && 
echo "Compressing mutated3_maias.txt with tar" && 
tar cvf ./tar/mutated3_maias.tar mutated3_maias.txt && 
echo "Compressing mutated3_maias.txt zstd" && 
zstd -v mutated3_maias.txt && 
mv mutated3_maias.txt.zst ./zstd &&


### 100%
echo "Compressing mutated4_maias.txt with gzip" && 
gzip -c -v mutated4_maias.txt > ./gzip/mutated4_maias.gz && 
echo "Compressing mutated4_maias.txt p7zip" && 
7z a ./7zip/mutated4_maias.7z mutated4_maias.txt && 
echo "Compressing mutated4_maias.txt with bzip2" && 
bzip2 -k -v mutated4_maias.txt && 
mv mutated4_maias.txt.bz2 ./bzip2 && 
echo "Compressing mutated4_maias.txt with lz4" && 
lz4 -v mutated4_maias.txt && 
mv mutated4_maias.txt.lz4 ./lz4 && 
echo "Compressing mutated4_maias.txt with tar" && 
tar cvf ./tar/mutated4_maias.tar mutated4_maias.txt && 
echo "Compressing mutated4_maias.txt zstd" && 
zstd -v mutated4_maias.txt && 
mv mutated4_maias.txt.zst ./zstd