
#以后编写python代码都要尽量使用这个模式
class dataHandler():
    def parse_song_info(self,song_info):#解析歌曲信息，每一行都按:::切分，变为一个新的
        try:
            song_id, name, artist, popularity = song_info.split(":::")
            # return ",".join([song_id, name, artist, popularity])
            return "\t".join([song_id, name, artist,popularity])#之间这样组成一个字符串，这个是字符串
        except Exception as e:
            # print e
            # print song_info
            return ""


    def parse_playlist_line(self, in_line):#解析文件
        try:
            contents = in_line.strip().split("\t")#按行解析
            name, tags, playlist_id, subscribed_count = contents[0].split("##")
            taglist = tags.split(",")#再把这个给变为逗号
            # play_info = str(playlist_id +'\t'+name+'\t'+tags+'\t'+subscribed_count)
            # play_info = map(lambda x: playlist_id + "," + name+","+x+"，"+subscribed_count, taglist[0:])
            songs=''
            for tag in taglist:#lambda表达式，把整个文本文件大处理一下
                songs_info = map(lambda x: playlist_id + "\t" +name+"\t"+tag+"\t"+subscribed_count+ self.parse_song_info(x), contents[1:])
                songs_info =  "\n".join(songs_info)
                songs = songs+songs_info
            return songs

        except Exception as e:
            print(e)
            return False

    def parse_file(self, in_file, out_file):#
        out = open(out_file, 'w')
        for line in open(in_file):#打开输入文件，每行进行遍历
            result = self.parse_playlist_line(line)#解析每一行
            if (result):
                out.write(result + "\n")
        out.close()
if __name__ == '__main__':
    dataHandler().parse_file("playlist-music.txt","text_format.txt")
