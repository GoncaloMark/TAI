import os
import soundfile as sf
import librosa

def convert_mp3_to_wav(folder_path):
    for filename in os.listdir(folder_path):
        if filename.endswith('.mp3'):
            mp3_path = os.path.join(folder_path, filename)
            wav_path = os.path.join(folder_path, filename[:-4] + '.wav')
            
            # Load the mp3 file
            data, samplerate = librosa.load(mp3_path, sr=None)
            
            # Save as wav file
            sf.write(wav_path, data, samplerate)
            
            # Delete the original mp3 file
            os.remove(mp3_path)
            print(f"Converted and deleted: {filename}")

if __name__ == "__main__":
    folder_path = input("Enter the path to the folder containing MP3 files: ")
    convert_mp3_to_wav(folder_path)
