```markdown
# Audio Processing and Signature Generation

## Overview

This project provides a set of tools for processing audio files, adding noise, segmenting them, and generating FFT signatures. These signatures can be used for audio identification tasks. The project includes the following components:

- **SegmentsCreator**: Segments audio tracks into smaller segments and adds noise.
- **SignaturesCreator**: Generates FFT signatures for each audio segment.
- **NoiseAdder**: Adds noise to specific audio segments and generates their FFT signatures.
- **MusicIdentifier**: Identifies the best matching track from a database of audio segments based on a query segment.

## Project Structure

```
.
├── bin
│   ├── MusicIdentifier
│   ├── NoiseAdder
│   ├── SegmentsCreator
│   └── SignaturesCreator
├── build
├── cmake
│   └── toolchains
│       ├── linux.toolchain.cmake
│       └── windows.toolchain.cmake
├── CMakeLists.txt
├── commands
│   ├── MusicIdentifier.sh
│   ├── NoiseAdder.sh
│   ├── SegmentsCreator.sh
│   └── SignaturesCreator.sh
├── Makefile
├── README.md
└── src
    ├── Compressor.cpp
    ├── Helpers.cpp
    ├── include
    │   ├── Compressor.hpp
    │   ├── Constants.hpp
    │   └── Helpers.hpp
    ├── MusicIdentifier.cpp
    ├── NoiseAdder.cpp
    ├── SegmentsCreator.cpp
    └── SignaturesCreator.cpp
```

## Build Instructions

### Prerequisites

Ensure you have the following libraries installed:

- `sndfile`
- `fftw3`
- `sox`
- `zlib`
- `bzip2`
- `lzma`
- `zstd`

On Debian-based systems, you can install these using:

```bash
sudo apt-get install libsndfile1-dev libfftw3-dev sox zlib1g-dev libbz2-dev liblzma-dev libzstd-dev
```

If necessary to use the Python scripts ensure you have the following libraries installed:

- `soundfile`
- `librosa`
- `moviepy`
- `pytube`

### Building the Project

1. Clone the repository:

```bash
git clone https://github.com/your_username/your_repository.git
cd your_repository
```

2. Build the project:

```bash
make linux   # For Linux
# or
make windows # For Windows
```

## Usage

### SegmentsCreator

Segments audio tracks into smaller segments and adds noise.

```bash
SegmentsCreator -i <input_directory> -o <output_directory> -d <segment_duration> -n <noise_level>
```

- `-i`, `--input`: Input directory containing audio files.
- `-o`, `--output`: Output directory for the segmented audio files.
- `-d`, `--duration`: Duration of each segment in seconds (default is 10).
- `-n`, `--noise`: Noise level to add to each segment (default is 0.05).

### MusicIdentifier

Identifies the best matching track from a database of audio segments based on a query segment.

```bash
MusicIdentifier -q <query_file> -d <database_dir> -m <compression_method> [-ws <window_size> -sh <shift> -ds <downsampling> -nf <num_freqs>]
```

- `-q`, `--query`: Query file (FFT signature).
- `-d`, `--database`: Database directory containing FFT signatures of audio segments.
- `-m`, `--method`: Compression method to use (gzip, bzip2, lzma, zstd).
- `-ws`, `--window_size`: FFT window size (default is 1024).
- `-sh`, `--shift`: FFT window shift (default is 256).
- `-ds`, `--downsampling`: Downsampling factor (default is 4).
- `-nf`, `--num_freqs`: Number of significant frequencies to identify (default is 4).

## Acknowledgements

- [libsndfile](http://www.mega-nerd.com/libsndfile/)
- [FFTW](http://www.fftw.org/)
- [SOX](http://sox.sourceforge.net/)
- [zlib](https://zlib.net/)
- [bzip2](http://www.bzip.org/)
- [lzma](https://tukaani.org/xz/)
- [zstd](https://facebook.github.io/zstd/)
```
