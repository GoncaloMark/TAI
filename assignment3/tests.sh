./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_1.wav" -start 15 -dur 1 &&
./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_5.wav" -start 15 -dur 5 &&
./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_10.wav" -start 15 -dur 10 &&

./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_1_noisy_100.wav" -start 15 -dur 1 -n 100 &&
./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_5_noisy_100.wav" -start 15 -dur 5 -n 100 &&
./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_10_noisy_100.wav" -start 15 -dur 10 -n 100 &&

./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_1_noisy_1000.wav" -start 15 -dur 1 -n 1000 &&
./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_5_noisy_1000.wav" -start 15 -dur 5 -n 1000 &&
./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_10_noisy_1000.wav" -start 15 -dur 10 -n 1000 &&

./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_1_noisy_10000.wav" -start 15 -dur 1 -n 10000 &&
./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_5_noisy_10000.wav" -start 15 -dur 5 -n 10000 &&
./bin/SegmentExtractor -i "music/Belladona.wav" -o "data/query_segments/Belladona_15_10_noisy_10000.wav" -start 15 -dur 10 -n 10000 &&

########################
# Duration 10 no noise #
########################
echo " " &&
echo "Duration 10" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10.wav" -m zstd -d music &&

########################
# Duration 5 no noise  #
########################
echo " " &&
echo "Duration 5" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5.wav" -m zstd -d music &&

########################
# Duration 1 no noise  #
########################
echo " " &&
echo "Duration 1" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1.wav" -m zstd -d music &&

##########################
# Duration 10 with noise #
##########################
echo " " &&
echo "Duration 10 with noise 100" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_100.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_100.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_100.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_100.wav" -m zstd -d music &&

##########################
# Duration 5 with noise  #
##########################
echo " " &&
echo "Duration 5 with noise 100" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_100.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_100.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_100.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_100.wav" -m zstd -d music &&

##########################
# Duration 1 with noise  #
##########################
echo " " &&
echo "Duration 1 with noise 100" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_100.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_100.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_100.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_100.wav" -m zstd -d music &&

##########################
# Duration 10 with noise #
##########################
echo " " &&
echo "Duration 10 with noise 1000" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_1000.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_1000.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_1000.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_1000.wav" -m zstd -d music &&

##########################
# Duration 5 with noise  #
##########################
echo " " &&
echo "Duration 5 with noise 1000" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_1000.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_1000.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_1000.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_1000.wav" -m zstd -d music &&

##########################
# Duration 1 with noise  #
##########################
echo " " &&
echo "Duration 1 with noise 1000" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_1000.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_1000.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_1000.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_1000.wav" -m zstd -d music &&

##########################
# Duration 10 with noise #
##########################
echo " " &&
echo "Duration 10 with noise 10000" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_10000.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_10000.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_10000.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_10_noisy_10000.wav" -m zstd -d music &&

##########################
# Duration 5 with noise  #
##########################
echo " " &&
echo "Duration 5 with noise 10000" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_10000.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_10000.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_10000.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_5_noisy_10000.wav" -m zstd -d music &&

##########################
# Duration 1 with noise  #
##########################
echo " " &&
echo "Duration 1 with noise 10000" &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_10000.wav" -m gzip -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_10000.wav" -m bzip2 -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_10000.wav" -m lzma -d music &&
./bin/MusicIdentifier -q "data/query_segments/Belladona_15_1_noisy_10000.wav" -m zstd -d music