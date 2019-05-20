

class dataHandler():
    def parse_song_info(self,song_info):
        try:
            song_id, name, artist, popularity = song_info.split(":::")
            # return ",".join([song_id, name, artist, popularity])
            return "\t".join([song_id, name, artist,popularity])
        except Exception as e:
            # print e
            # print song_info
            return ""


    def parse_playlist_line(self, in_line):
        try:
            contents = in_line.strip().split("\t")
            name, tags, playlist_id, subscribed_count = contents[0].split("##")
            taglist = tags.split(",")
            # play_info = str(playlist_id +'\t'+name+'\t'+tags+'\t'+subscribed_count)
            # play_info = map(lambda x: playlist_id + "," + name+","+x+"，"+subscribed_count, taglist[0:])
            songs=''
            for tag in taglist:
                songs_info = map(lambda x: playlist_id + "\t" +name+"\t"+tag+"\t"+subscribed_count+ self.parse_song_info(x), contents[1:])
                songs_info =  "\n".join(songs_info)
                songs = songs+songs_info
            return songs

        except Exception as e:
            print(e)
            return False

    def parse_file(self, in_file, out_file):
        out = open(out_file, 'w')
        for line in open(in_file):
            result = self.parse_playlist_line(line)
            if (result):
                out.write(result + "\n")
        out.close()
if __name__ == '__main__':
    dataHandler().parse_file("playlist-music.txt","text_format.txt")
