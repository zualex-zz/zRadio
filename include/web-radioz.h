#ifndef WEBRADIOZ_H
#define WEBRADIOZ_H

#include <Arduino.h>
#include <Preferences.h>
#include <Audio.h> //see "https://github.com/schreibfaul1/ESP32-audioI2S"
#include <vector>

static void (*callback)(const char *);

// String stations[] = {
std::vector< const char* > stations = { 
        /*- 0 N - 70s on Radio*/ "0n-80s.radionetz.de:8000/0n-70s.mp3",
        // 320k slow /*Hi On Line Gold Radio*/ "mediaserv30.live-streams.nl:8000/stream",
        // FAIL /*dashitradio*/"www.surfmusic.de/m3u/100-5-das-hitradio,4529.m3u",
        /*saw-deutsch*/ "stream.1a-webradio.de/deutsch/mp3-128/vtuner-1a",
        /*HIT RADIO FFH*/ "mp3.ffh.de/radioffh/hqlivestream.aac", //  128k aac
        /*ANTENNE BAYERN*/ "www.antenne.de/webradio/antenne.m3u",
        /*RUSSIAN SONGS*/ "listen.rusongs.ru/ru-mp3-128",
        /*Hitradio SKW*/ "edge.audio.3qsdn.com/senderkw-mp3",
        /* "MacSlon's Irish Pub Radio" */ "macslons-irish-pub-radio.com/media.asx",

        // /* IROCK 247.com */ "https://streamer.radio.co/s493283a7e/listen"
        // [E][WiFiGeneric.cpp:654] hostByName(): DNS Failed for streamer.radio.co/s493283a7e/listenhttps
        //[E][WiFiClientSecure.cpp:132] connect(): start_ssl_client: -1
        //info        Request streamer.radio.co/s493283a7e/listenhttps://stream-mz.planetradio.co.uk/planetrock.aachttps://streamingv2.shoutcast.com/radiofrecciahttp://wma08.fluidstream.net:4610/ failed!
        // /* Planet rock */ "https://stream-mz.planetradio.co.uk/planetrock.aac"
        // /* Radio Swiss Jazz */ "https://stream.srg-ssr.ch/m/rsj/mp3_128",   
        // not enough memory to allocate mp3decoder buffers /* Jazz Radio - Blues */ "https://jazzblues.ice.infomaniak.ch/jazzblues-high",

        // FAIL /* RTL: */ "http://shoutcast.rtl.it:3010/",
        // FAIL /* RTL Best: */ "http://shoutcast.rtl.it:3020/",
        // FAIL /* RTL ItalianStyle: */ "http://shoutcast.rtl.it:3030/",
        // FAIL /* RTL Lounge: */ "http://shoutcast.rtl.it:3070/",
        // /* RTL Radiofreccia: */ "https://streamingv2.shoutcast.com/radiofreccia", // FAIL "http://shoutcast.rtl.it:3060/",
        // FAIL /* Radio Zeta L’Italiana: */ "http://shoutcast.rtl.it:3030/stream/1/",
        /* Radio Kiss Kiss: */ "http://wma08.fluidstream.net:4610/",
                                "http://ice07.fluidstream.net:8080/KissKiss.mp3",
                                // "https://ice07.fluidstream.net/KissKiss.aac?FLID=6",
        /* Radio Kiss Kiss Italia: */ "http://wma07.fluidstream.net:3616/;stream.nsv",
        /* RDS: */  "https://icstream.rds.radio/rds" // "https://stream.rds.radio/audio/rds.stream_aac/chunklist.m3u8", // FAIL "http://www.rds.it:8000/stream",
        /* Radio Deejay: */ //  DNS Failed for radiodeejay-lh.akamaihd.net/i/RadioDeejay_Live_1@189857/master.m3u8http "https://radiodeejay-lh.akamaihd.net/i/RadioDeejay_Live_1@189857/master.m3u8"
        // /* R101 Diretta: */ "http://icecast.unitedradio.it/r101" // FAIL  "http://wradio.msf.ticdn.it/r101_mp3",
        // /* Virgin Radio: */ "http://icecast.unitedradio.it/Virgin.mp3" // FAIL "http://icecast.unitedradio.it/virgin.mp3",
        // /* Radio Capital */ "https://radiocapital-lh.akamaihd.net/i/RadioCapital_Live_1@196312/index_96_a-b.m3u8"
        /* Radio Capital */  // FAIL End of webstream "http://radiocapital-lh.akamaihd.net/i/RadioCapital_Live_1@196312/master.m3u8",
        /* Rai Radio 1: */ "http://icestreaming.rai.it/1.mp3",
        /* Rai Radio 2: */ "http://icestreaming.rai.it/2.mp3",
        /* Rai Radio 3: */ "http://icestreaming.rai.it/3.mp3",
        /* Rai IsoRadio: */ "http://icestreaming.rai.it/6.mp3",
        /* Rai GrParlamento: */ "http://icestreaming.rai.it/7.mp3",
        // FAIL /* Radio 105 */ "http://icecast.105.net/105.mp3",
        /* Radio Monte Carlo */ "http://edge.radiomontecarlo.net/RMC.mp3",// FAIL  "http://icecast.unitedradio.it/rmc.mp3",
        // /* Radio Monte Carlo 2: */ "http://edge.radiomontecarlo.net/MC2.mp3" // FAIL "http://shoutcast.unitedradio.it:1107",
        /* Radio m2o */ // FAIL "http://radiom2o-lh.akamaihd.net/i/RadioM2o_Live_1@42518/master.m3u8",
        /* Radio 24 */ "http://shoutcast.radio24.it:8000/listen.pls",
        // FAIL /* Radio LatteMiele: */ "http://onair18.xdevel.com:8014/",
        // FAIL /* Discoradio: */ "http://46.37.20.206:1935/live/1discoradio.stream/playlist.m3u8",
        /* Radio Norba: */ "http://onair20.xdevel.com:8218/",
        /* Radio Subasio: */ "http://onair18.xdevel.com:8152",
        // FAIL /* Radio Subasio +: */ "http://onair11.xdevel.com:8050",
        // FAIL /* Radio Cuore */ "http://nr9.newradio.it:9029",
        // FAIL /* RIN Radio Italia Network */ "http://sr9.inmystream.info:8006",
        /* Radio VivaFm */ "http://stream2.vivafm.it:8002/listen.pls",
        /* Radio Sempreviva */ "http://stream2.vivafm.it:8000/listen.pls",
        /* Radio Ibiza */ "http://wma02.fluidstream.net:5010/",
        /* Radio italia smi */ // [E][WiFiGeneric.cpp:654] hostByName(): DNS Failed for radioitaliasmi-lh.akamaihd.net/i/radioitaliasmi_1@329643/master.m3u8https "https://radioitaliasmi-lh.akamaihd.net/i/radioitaliasmi_1@329643/master.m3u8"
 
        /* Radio Company Rock */ // [E][mp3_decoder.cpp:1526] MP3Decoder_AllocateBuffers(): not enough memory to allocate mp3decoder buffers "https://sphera.fluidstream.eu/companyrock.mp3?FLID=1",
        /* Radio Company Fitness */ // [E][mp3_decoder.cpp:1526] MP3Decoder_AllocateBuffers(): not enough memory to allocate mp3decoder buffers "https://sphera.fluidstream.eu/companyfitness.mp3?FLID=1",

        // /* Radio Aktual*/ "https://live.radio.si/Aktual",
        // /* Hitradio center */ "https://stream.nextmedia.si/proxy/center2?mp=/center",
        // /* Radio 1 */ "https://live.radio.si/Radio1",
        // /* Val 202 */ "https://mp3.rtvslo.si/val202",
        // /* Radio City (Maribor)*/ "https://stream1.radiocity.si/CityMp3128.mp3",
        // /* Radio Ptuj */ "https://live.radio.si/Ptuj",
        // /* Radio Veseljak (Ljubljana) */ "https://live.radio.si/Veseljak",
        // /* Radio Maxi */ "https://live.radio.si/Maxi",
        // /* Radio Salomon */ "https://live.radio.si/Salomon",
        // /* Radio Ekspres (Ljubljana) */ "https://stream.nextmedia.si/proxy/ekspres1?mp=/stream",
        // /* Radio Antena (Ljubljana)*/ "https://live.radio.si/Antena",
        // /* Radio Ognjisce */ "https://live.radio.si/ognjisce.mp3",
        // /* Radio Aktual Kum*/ "https://live.radio.si/AktualK",
        // /* Radio Capris (Koper) */ "https://stream.exit.si/live",
        // /* Radio Robin */ "https://live.radio.si/Robin"
        // see: https://siradio.si/

    };


    /* static */ Preferences pref;

    /* const */ uint8_t max_volume   = 21;
    // static uint8_t max_stations = 0;   //will be set later
    /* static */ uint8_t cur_station; //  = 0;   //current station(nr), will be set later
    /* static */ uint8_t cur_volume; //   = 0;   //will be set from stored preferences
    // int8_t  cur_btn      =-1;   //current button (, -1 means idle)



class WebRadioz {
    static Audio* audio;
    static int8_t newVolume;
    public:
    static uint8_t cur_station; //  = 0;   //current station(nr), will be set later
    static uint8_t cur_volume; //   = 0;   //will be set from stored preferences
    static char* station;
    static char* streamtitle;
    static boolean isPlaying;

    
    // static void audioTask(void * parameter) {
    //     for(;;) {
    //         if (audio) {
    //             audio->loop();
    //         }
    //     }
    // }

    static void begin(const i2s_pin_config_t pins, void (*callbackz)(const char *)) {

        audio = new Audio(/* pins.bck_io_num, pins.ws_io_num, pins.data_out_num */);

        Serial.print("WebRadioz constructor");
        audio->setPinout(pins.bck_io_num, pins.ws_io_num, pins.data_out_num);
        callback = callbackz;

        pref.begin("WebRadio", false);  // instance of preferences for defaults (station, volume ...)
        if (pref.getShort("volume", 1000) == 1000){ // if that: pref was never been initialized
            pref.putShort("volume", 10);
            pref.putShort("station", 0);
        }
        // else{ // get the stored values
        cur_station = pref.getShort("station");
        cur_volume = pref.getShort("volume");
        // }
        if (cur_volume <= 0) {
            cur_volume = 10;
        }
        audio->setVolume(cur_volume); // 0...21

        if (cur_station >= stations.size()) {
            cur_station = 0;
        }

        // https://translate.google.com.vn/translate_tts?ie=UTF-8&q=buongiorno&tl=it&client=tw-ob
        // audio->connecttospeech("Buongiorno!", "it");

        audio->connecttohost(stations.at(cur_station));
        isPlaying = true;

        // xTaskCreatePinnedToCore(
        //     audioTask,          // Function that should be called
        //     "Audio task",     // Name of the task (for debugging)
        //     10000,              // Stack size (bytes)
        //     NULL,               // Parameter to pass
        //     1,                  // Task priority
        //     NULL,               // Task handle
        //     1                   // Core you want to run the task on (0 or 1)
        // );
    }

    static void stop() {
        if (audio) {
            audio->~Audio();
            audio = NULL;
        }
        isPlaying = false;
    }

    static String setVolume(String volStr)  {
        uint8_t volume = volStr.toInt();
        cur_volume = volume;
        Serial.print("volume: "); Serial.println(String(cur_volume));
        audio->setVolume(cur_volume);
        pref.putShort("volume", cur_volume); // store the current volume in nvs
        return String(cur_volume);
    }

    static String volumeUp() {
        if (cur_volume+1 < max_volume) {
            newVolume = cur_volume + 1;
            // audio->setVolume(++cur_volume);
            // pref.putShort("volume", cur_volume);
        }
        return "";
    }

    static String volumeDown() {
        if (cur_volume-1 > 0) {
            newVolume = cur_volume - 1;
            // audio->setVolume(--cur_volume);
            // pref.putShort("volume", cur_volume);
        }
        return "";
    }

    static String next(String val) {
        if (cur_station + 1 < stations.size()) {
            cur_station++;
            Serial.print("stationNr: "); Serial.println(String(cur_station));
            audio->connecttohost(stations.at(cur_station));
            pref.putShort("station", cur_station); // store the current station in nvs
        }
        return String(cur_station);
    } 

    static String prev(String volStr) {
        if (cur_station - 1 > 0) {
            cur_station--;
            Serial.print("stationNr: "); Serial.println(String(cur_station));
            audio->connecttohost(stations.at(cur_station));
            pref.putShort("station", cur_station); // store the current station in nvs
        }
        return String(cur_station);
    }

    static String addStation(String station) {
        // cur_station--;
        Serial.print("new station: "); Serial.println(station);
        stations.push_back(station.c_str());
        cur_station = stations.size() - 1;
        audio->connecttohost(stations.at(cur_station));
        pref.putShort("station", cur_station); // store the current station in nvs
        return String(cur_station);
    } 

    static void loop() {
        if (audio) {
            audio->loop();
            // if (newVolume >= 0) {
            //     cur_volume = newVolume;
            //     audio->setVolume(cur_volume);
            //     pref.putShort("volume", cur_volume);
            //     newVolume = -1;
            // }
        }
    }

};

Audio* WebRadioz::audio = NULL;
int8_t WebRadioz::newVolume = -1;
uint8_t WebRadioz::cur_station;
uint8_t WebRadioz::cur_volume;
char* WebRadioz::station; // = "";
char* WebRadioz::streamtitle; // = "";
boolean WebRadioz::isPlaying = false;

// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
    WebRadioz::station = (char *)info;
    // callback("{\"station\", \"" + info + "\"");
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
    
    WebRadioz::streamtitle = new char[strlen(info)];
    strcpy(WebRadioz::streamtitle, info);
    
    // WebRadioz::streamtitle = (char *)info;
    //callback("{\"streamtitle\", \"" + info + "\"");
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}

#endif