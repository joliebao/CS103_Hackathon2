#include <iostream>
#include <string>
using namespace std;

class Song {
   public:
        Song();
        Song(string id, string n, string s, string a, string g, string t);
        void setID(string identification);
        void setName(string name);
        void setArtist(string artist);
        void setAlbum(string album);
        void setGenre(string genre);
        void setDuration(string time);
        void print();

   private:
        string id;
        string name;
        string artist;
        string album;
        string genre;
        string duration;
};

Song::Song(){
    id = "unknown";
    name = "unknown";
    artist = "unknown";
    album = "unknown";
    genre = "unknown";
    duration = "0:00";
}

Song::Song(string identification, string songName, string singer, string albumName, string g, string time){
    id = identification;
    name = songName;
    artist = singer;
    album = albumName;
    genre = g;
    duration = time;
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

void Song::setDuration(string time){
    duration = time;
}

void Song::print(){
    cout << "ID: " << id << endl;
    cout << "Song: " << name << endl;
    cout << "Artist: " << artist << endl;
    cout << "Album: " << album << endl;
    cout << "Genre: " << genre << endl;
    cout << "Duration: " << duration << endl;
    cout << endl;
}