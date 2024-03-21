#include "../include/cpm.hpp"
#include "../include/utils/utils.hpp"

namespace CPM {
    void CopyModel::start(){
        bool flag;
        Nh = Nf = 0;
        size_t count = 0;
        
        double HitProbability = 0.0;
        cbuffer::CircularBuffer<uint32_t> kmerBuf(kmerSize);

        while (true) {
            flag = decoder.readChunk(buffers[0], bufSize);
            if (!flag) {
                std::cerr << "Error reading chunk or end of data...\n";
                break;
            }

            if (!buffers[1].empty()) {
                processBufferTransition(kmerBuf, buffers[0], buffers[1], kmerSize);
            }

            processBuffer(kmerBuf, buffers[0], kmerSize);

            buffers[1] = buffers[0];
            count++;
        }
        HitProbability = calculateHitProbability(Nh, Nf, alpha);
        std::cout << "End: " << HitProbability/count << std::endl;
    }

    //TODO Pos is end position of kmer not start of kmer
    void CopyModel::processKmer(std::string& kmer, size_t pos, size_t bufIndex){
        //TODO gotta find a way to update every bufIndex in the map at the end of a buffer run, if anchors were at bufIndex 1 they get deleted, if anchors were at bufIndex 0 they get updated to 1.
        if (positions.find(kmer) != positions.end()){
            //TODO: This wont work at buffer transition
            if(buffers[std::get<0>(positions[kmer])][std::get<1>(positions[kmer]) + kmerSize] == buffers[bufIndex][pos + kmerSize]) {
                Nh++;
            } else {
                Nf++;
            }

            positions[kmer] = std::make_tuple(bufIndex, pos);
        } else {
            positions.insert({kmer, std::make_tuple(bufIndex, pos)});
        }

        if(!precision()) {
            resetModel();
        }
    }

    void CopyModel::processBuffer(cbuffer::CircularBuffer<uint32_t>& kmerBuf, const std::vector<uint32_t>& buffer, size_t kmerSize) {
        // Logic to process the current buffer
        for (size_t index = 0; index <= buffer.size() - kmerSize; ++index) {
            kmerBuf.clear();
            for (size_t i = 0; i < kmerSize; ++i) {
                kmerBuf.enqueue(buffer[index + i]);
            }
            std::string kmerString = convertKmerToString(kmerBuf);
            std::cout << kmerString << std::endl;

            processKmer(kmerString, index, 0);
        }
    }

    void CopyModel::processBufferTransition(cbuffer::CircularBuffer<uint32_t>& kmerBuf, const std::vector<uint32_t>& buffer1, const std::vector<uint32_t>& buffer2, size_t kmerSize) {
        if (buffer2.size() >= kmerSize - 1) {
            for (size_t i = buffer2.size() - kmerSize + 1; i < buffer2.size(); ++i) {
                kmerBuf.clear();

                // Start from the end of buffer2
                for (size_t j = i; j < buffer2.size(); ++j) {
                    kmerBuf.enqueue(buffer2[j]);
                }

                // Fill the rest from the start of buffer1
                for (size_t j = 0; kmerBuf.size() < kmerSize && j < buffer1.size(); ++j) {
                    kmerBuf.enqueue(buffer1[j]);
                }

                if (kmerBuf.size() == kmerSize) {
                    std::string kmerString = convertKmerToString(kmerBuf);
                    std::cout << kmerString << std::endl;
                }
            }
        }
    }
}

