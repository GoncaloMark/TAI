./bin/SegmentExtractor -i "music/Balada.wav" -o "data/query_segments/Balada_15_1.wav" -start 15 -dur 1 &&
./bin/SegmentExtractor -i "music/Balada.wav" -o "data/query_segments/Balada_15_5.wav" -start 15 -dur 5 &&
./bin/SegmentExtractor -i "music/Balada.wav" -o "data/query_segments/Balada_15_10.wav" -start 15 -dur 10 &&

./bin/SegmentExtractor -i "music/Balada.wav" -o "data/query_segments/Balada_15_1_noisy_025.wav" -start 15 -dur 1 -n 0.25 &&
./bin/SegmentExtractor -i "music/Balada.wav" -o "data/query_segments/Balada_15_5_noisy_025.wav" -start 15 -dur 5 -n 0.25 &&
./bin/SegmentExtractor -i "music/Balada.wav" -o "data/query_segments/Balada_15_10_noisy_025.wav" -start 15 -dur 10 -n 0.25 &&

./bin/SegmentExtractor -i "music/Balada.wav" -o "data/query_segments/Balada_15_1_noisy_050.wav" -start 15 -dur 1 -n 0.50 &&
./bin/SegmentExtractor -i "music/Balada.wav" -o "data/query_segments/Balada_15_5_noisy_050.wav" -start 15 -dur 5 -n 0.50 &&
./bin/SegmentExtractor -i "music/Balada.wav" -o "data/query_segments/Balada_15_10_noisy_050.wav" -start 15 -dur 10 -n 0.50 &&

########################
# Duration 10 no noise #
########################
echo " " &&
echo "Duration 10" &&
./bin/MusicIdentifier -q "data/query_segments/Balada_15_10.wav" -m zstd -d music &&

########################
# Duration 5 no noise  #
########################
echo " " &&
echo "Duration 5" &&
./bin/MusicIdentifier -q "data/query_segments/Balada_15_5.wav" -m zstd -d music &&

########################
# Duration 1 no noise  #
########################
echo " " &&
echo "Duration 1" &&
./bin/MusicIdentifier -q "data/query_segments/Balada_15_1.wav" -m zstd -d music &&

##########################
# Duration 10 with noise #
##########################
echo " " &&
echo "Duration 10 with noise 25" &&
./bin/MusicIdentifier -q "data/query_segments/Balada_15_10_noisy_025.wav" -m zstd -d music &&

##########################
# Duration 5 with noise  #
##########################
echo " " &&
echo "Duration 5 with noise 25" &&
./bin/MusicIdentifier -q "data/query_segments/Balada_15_5_noisy_025.wav" -m zstd -d music &&

##########################
# Duration 1 with noise  #
##########################
echo " " &&
echo "Duration 1 with noise 25" &&
./bin/MusicIdentifier -q "data/query_segments/Balada_15_1_noisy_025.wav" -m zstd -d music &&

##########################
# Duration 10 with noise #
##########################
echo " " &&
echo "Duration 10 with noise 50" &&
./bin/MusicIdentifier -q "data/query_segments/Balada_15_10_noisy_050.wav" -m zstd -d music &&

##########################
# Duration 5 with noise  #
##########################
echo " " &&
echo "Duration 5 with noise 50" &&
./bin/MusicIdentifier -q "data/query_segments/Balada_15_5_noisy_050.wav" -m zstd -d music &&

##########################
# Duration 1 with noise  #
##########################
echo " " &&
echo "Duration 1 with noise 50" &&
./bin/MusicIdentifier -q "data/query_segments/Balada_15_1_noisy_050.wav" -m zstd -d music





