#include <iostream>;
#include <string>;
using namespace std;

class Song {
   public:
        void setName(string name);
        void setArtist(string artist);
        void setAlbum(string album);
        void setGenre(string genre);
        void setDuration(int time);
        void setID(string identification);
        void print();

   private:
        string name;
        string artist;
        string album;
        string genre;
        string id;
        int duration;
};

Song::Song(){
    name = "unknown";
    artist = "unknown";
    album = "unknown";
    genre = "unknown";
    duration = -1;
}

void Song::setName(string songName){
    name = songName;
}

void Song::setArtist(string singer){
    artist = singer;
}

void Song::setAlbum(string albumName){
    album = albumName;
}

void Song::setGenre(string g){
    genre = g;
}

void Song::setID(string identification){
    id = identification;
}

void Song::setDuration(int time){
    duration = time;
}

void Song::print(){
    cout << "Song: " << name << endl;
    cout << "Artist: " << artist << endl;
    cout << "Album: " << album << endl;
    cout << "Genre: " << genre << endl;
    cout << "Duration: " << duration << endl;
}