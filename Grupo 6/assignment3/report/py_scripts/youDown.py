from pytube import YouTube
from moviepy.editor import *
import os

def download_audio(url, output_path):
    try:
        yt = YouTube(url)
        # Get the highest resolution audio stream
        audio_stream = yt.streams.filter(only_audio=True).order_by('abr').desc().first()
        # Download the audio
        audio_file = audio_stream.download(output_path=output_path)
        # Convert the audio to mp3
        audio_clip = AudioFileClip(audio_file)
        mp3_file = os.path.join(output_path, yt.title + ".mp3")
        audio_clip.write_audiofile(mp3_file)
        audio_clip.close()
        print("Audio downloaded successfully as", yt.title + ".mp3")
        delete_original(audio_file)
        
    except Exception as e:
        delete_original(audio_file)
        print("Error:", str(e))

def delete_original(audio_file):
    # Delete the original audio file
        os.remove(audio_file)
        print("Original audio file deleted successfully")

if __name__ == "__main__":
    url = input("Enter the YouTube video URL: ")
    output_path = "C:\\Users\\jprw3\\Downloads\\Music"
    download_audio(url, output_path)
    
    
    
    
    
    