/*
    The following file allows you to get the metadata from the API
    It then puts the metadata into readable format into the song object
    It does this 10 times to add to the 10 song vector

    I looked at the following code file with documentation as an example to help me:
    https://github.com/metabrainz/libmusicbrainz/blob/master/examples/cdlookup.cc

    I also used gemini on 11/23 at 7 am to help me fix bugs with how I accessed my 
    metadata
*/ 

// IF TESTING RUN THE FOLLOWING CODE IN TERMINAL:
// g++ songs.cpp -o a -lmusicbrainz5cc

#include <iostream>
#include <string>
#include <vector>
#include "musicbrainz5/Query.h"
#include "musicbrainz5/Medium.h"
#include "musicbrainz5/MediumList.h"
#include "musicbrainz5/ReleaseGroup.h"
#include "musicbrainz5/Track.h"
#include "musicbrainz5/TrackList.h"
#include "musicbrainz5/Recording.h"
#include "musicbrainz5/Disc.h"
#include "musicbrainz5/HTTPFetch.h"
#include "musicbrainz5/Release.h"
#include "musicbrainz5/Collection.h"
#include "musicbrainz5/CollectionList.h"
#include <musicbrainz5/ArtistCredit.h>
#include <musicbrainz5/NameCreditList.h>
#include <musicbrainz5/NameCredit.h>
#include <musicbrainz5/Artist.h>
#include <musicbrainz5/Tag.h>
#include <unistd.h> //time library for mac/os
#include "song.cpp"
using namespace std;
using namespace MusicBrainz5; 

vector<Song> getSongs(int limit, int offset) {
    vector<Song> songs;
    MusicBrainz5::CQuery Query("MyMusicApp/1.0 (contact@example.com)");

    try{
        MusicBrainz5::CQuery::tParamMap Params;
        Params["query"] = "country:US AND tag:*";
        Params["limit"] = to_string(limit);
        Params["offset"] = to_string(offset);

        MusicBrainz5::CMetadata Metadata = Query.Query("recording", "", "", Params);

        if (Metadata.RecordingList()) {
            MusicBrainz5::CRecordingList *RecordingList = Metadata.RecordingList();
            cout << "Found " << RecordingList->NumItems() << " recordings(s)\n" << endl;

            for (int i = 0; i < RecordingList->NumItems(); i++) {
                MusicBrainz5::CRecording *Recording = RecordingList->Item(i);

                string id = Recording->ID(); // Gets song ID
                string title = Recording->Title(); // Gets song name

                // 11/23/26 11:45 Gemini generated the following artist accessor because 
                // it kept returning errors that it couldn't find the following.
                // This returns the first listed artist if the artist credit list isn't empty.
                string artist = "Unknown";
                if (Recording->ArtistCredit() && 
                    Recording->ArtistCredit()->NameCreditList() && 
                    Recording->ArtistCredit()->NameCreditList()->NumItems() > 0) {
                    
                    auto *NC = Recording->ArtistCredit()->NameCreditList()->Item(0);
                    if (!NC->Name().empty()) {
                        artist = NC->Name();
                    } else if (NC->Artist()) {
                        artist = NC->Artist()->Name();
                    }
                }

                string album = "Unknown";
                if (Recording->ReleaseList() && Recording->ReleaseList()->NumItems() > 0) {
                    album = Recording->ReleaseList()->Item(0)->Title();
                }

                // the genre is inside the taglist in this database
                string genre = "Unknown";
                if (Recording->TagList() && Recording->TagList()->NumItems() > 0) {
                    genre = Recording->TagList()->Item(0)->Name();
                }

                int durationS = Recording->Length()/1000; // Gives duration in seconds
                int durationM = durationS / 60; // Gets the amount of time in minutes
                durationS %= 60; // Gets remainder of time in seconds
                string duration = "";
                if (durationS < 10){ // Converts duration to a string
                    duration = to_string(durationM) + ":0" + to_string(durationS);
                } else {
                    duration = to_string(durationM) + ":" + to_string(durationS);
                }

                Song s = Song(id, title, artist, album, genre, duration);
                s.print();
                songs.push_back(s);
            }
            return songs;
        }
    }
    catch (MusicBrainz5::CConnectionError& Error)
    {
        cout << "Connection Exception: '" << Error.what() << "'" << endl;
        cout << "LastResult: " << Query.LastResult() << endl;
        cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
        cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
    }
    catch (MusicBrainz5::CTimeoutError& Error)
    {
        cout << "Timeout Exception: '" << Error.what() << "'" << endl;
        cout << "LastResult: " << Query.LastResult() << endl;
        cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
        cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
    }
    catch (MusicBrainz5::CAuthenticationError& Error)
    {
        cout << "Authentication Exception: '" << Error.what() << "'" << endl;
        cout << "LastResult: " << Query.LastResult() << endl;
        cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
        cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
    }
    catch (MusicBrainz5::CFetchError& Error) {
        cout << "Fetch Exception: '" << Error.what() << "'" << endl;
        cout << "LastResult: " << Query.LastResult() << endl;
        cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
        cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
    }
    catch (MusicBrainz5::CRequestError& Error) {
        cout << "Request Exception: '" << Error.what() << "'" << endl;
        cout << "LastResult: " << Query.LastResult() << endl;
        cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
        cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
    }
    catch (MusicBrainz5::CResourceNotFoundError& Error) {
        cout << "ResourceNotFound Exception: '" << Error.what() << "'" << endl;
        cout << "LastResult: " << Query.LastResult() << endl;
        cout << "LastHTTPCode: " << Query.LastHTTPCode() << endl;
        cout << "LastErrorMessage: " << Query.LastErrorMessage() << endl;
    }
    return songs;
}

int main() {
    int limit = 10;
    vector<Song> songs;

    for (int offset = 0; offset < 100; offset += limit) {
        vector<Song> page = getSongs(limit, offset);
        songs.insert(songs.end(), page.begin(), page.end());

        // wait one second to match MusicBrainz's 1 request/second rate limit
        sleep(1);
    }

    return 0;
}