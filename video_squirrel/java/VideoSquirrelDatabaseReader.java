
//package VideoSquirrel;

import java.lang.*;
import javax.xml.parsers.*;
import org.xml.sax.*;
import org.w3c.dom.*;
import java.io.*;
import java.util.*;

public class VideoSquirrelDatabaseReader
{
    public VideoSquirrelDatabaseReader()
    {
      videoItems = new ArrayList();
      //isLoaded = new Boolean();
      isLoaded = false;
    }

    public boolean LoadDatabase(String filename)
    {
        try {
        isLoaded = true;
            // Read in a XML file (which should be a Video Squirrel Database file)
            //Raw text input file object
            //java.io.FileReader inputXML = new java.io.FileReader(filename);
            //XML reading configuration object?
            DocumentBuilderFactory videoSquirrelConfig = DocumentBuilderFactory.newInstance();
            DocumentBuilder videoSquirrelParser = videoSquirrelConfig.newDocumentBuilder();
            Document parsedXML = videoSquirrelParser.parse( new File(filename) );

            Element level0 = parsedXML.getDocumentElement();
            if (level0.getTagName() == "VideoSquirrelDatabase") {
              NodeList level1 = level0.getChildNodes();
              for (int level1Num = 0; level1Num < level1.getLength(); level1Num++) {
                  Node level1Node = level1.item(level1Num);
                  //int pos = jList1.getModel().getSize();
                  //VideoSquirrelListItem test = new VideoSquirrelListItem(level1Node.getNodeName());
                  //jList1Model.add(pos, test);
                  if (level1Node.getNodeName() == "VideoList") {
                      NodeList level2 = level1Node.getChildNodes();
                      for (int level2Num = 0; level2Num < level2.getLength(); level2Num++) {
                          Node level2Node = level2.item(level2Num);
                          if (level2Node.getNodeName() == "VideoItem") {
                              //We found a real Video Item, so create a new object
                              VideoSquirrelListItem itemFromXML = new VideoSquirrelListItem();

                              NodeList level3 = level2Node.getChildNodes();
                              for (int level3Num = 0; level3Num < level3.getLength(); level3Num++) {
                                  Node level3Node = level3.item(level3Num);
                                  if (level3Node.getNodeName() == "UID") {
                                      Node level3NodeData = level3Node.getFirstChild();
                                      if (level3NodeData != null)
                                          itemFromXML.UID = Long.parseLong(level3NodeData.getNodeValue());

                                  } else if (level3Node.getNodeName() == "CD") {
                                      Node level3NodeData = level3Node.getFirstChild();
                                      if (level3NodeData != null)
                                          itemFromXML.cd = level3NodeData.getNodeValue();

                                  } else if (level3Node.getNodeName() == "Title") {
                                      Node level3NodeData = level3Node.getFirstChild();
                                      if (level3NodeData != null)
                                          itemFromXML.title = level3NodeData.getNodeValue();


                                  } else if (level3Node.getNodeName() == "Filename") {
                                      Node level3NodeData = level3Node.getFirstChild();
                                      if (level3NodeData != null)
                                          itemFromXML.filename = level3NodeData.getNodeValue();

                                  } else if (level3Node.getNodeName() == "FileSize") {
                                      Node level3NodeData = level3Node.getFirstChild();
                                      if (level3NodeData != null)
                                          itemFromXML.file_size = Long.parseLong(level3NodeData.getNodeValue());

                                  } else if (level3Node.getNodeName() == "VideoData") {
                                      NodeList level4 = level3Node.getChildNodes();
                                      //Create a new video track data class
                                      VideoSquirrelListItemVideoTrack videoTrack = new VideoSquirrelListItemVideoTrack();
                                      for (int level4Num = 0; level4Num < level4.getLength(); level4Num++) {
                                          Node level4Node = level4.item(level4Num);
                                          if (level4Node.getNodeName() == "VideoBitrate") {
                                              Node level4NodeData = level4Node.getFirstChild();
                                              if (level4NodeData != null)
                                                  videoTrack.avg_bitrate = Long.parseLong(level4NodeData.getNodeValue());

                                          } else if (level4Node.getNodeName() == "PixelWidth") {
                                              Node level4NodeData = level4Node.getFirstChild();
                                              if (level4NodeData != null)
                                                  videoTrack.x = Long.parseLong(level4NodeData.getNodeValue());

                                          } else if (level4Node.getNodeName() == "PixelHeight") {
                                              Node level4NodeData = level4Node.getFirstChild();
                                              if (level4NodeData != null)
                                                  videoTrack.y = Long.parseLong(level4NodeData.getNodeValue());

                                          } else if (level4Node.getNodeName() == "VideoFrameRate") {
                                              Node level4NodeData = level4Node.getFirstChild();
                                              if (level4NodeData != null)
                                                  videoTrack.frame_rate = Float.parseFloat(level4NodeData.getNodeValue());

                                          } else if (level4Node.getNodeName() == "VideoDuration") {
                                              Node level4NodeData = level4Node.getFirstChild();
                                              if (level4NodeData != null)
                                                  videoTrack.duration = Long.parseLong(level4NodeData.getNodeValue());

                                          } else if (level4Node.getNodeName() == "VideoCompressor") {
                                              Node level4NodeData = level4Node.getFirstChild();
                                              if (level4NodeData != null)
                                                  videoTrack.compressor = level4NodeData.getNodeValue();

                                          }
                                      }// Level 4 scaning
                                      itemFromXML.videoTracks.add(videoTrack);
                                  } else if (level3Node.getNodeName() == "AudioTrack") {
                                      NodeList level4 = level3Node.getChildNodes();
                                      //Create a new video track data class
                                      VideoSquirrelListItemAudioTrack audioTrack = new VideoSquirrelListItemAudioTrack();
                                      for (int level4Num = 0; level4Num < level4.getLength(); level4Num++) {
                                          Node level4Node = level4.item(level4Num);
                                          if (level4Node.getNodeName() == "AudioBitrate") {
                                              Node level4NodeData = level4Node.getFirstChild();
                                              if (level4NodeData != null)
                                                  audioTrack.avg_bitrate = Long.parseLong(level4NodeData.getNodeValue());

                                          } else if (level4Node.getNodeName() == "AudioChannelCount") {
                                              Node level4NodeData = level4Node.getFirstChild();
                                              if (level4NodeData != null)
                                                  audioTrack.channels = Long.parseLong(level4NodeData.getNodeValue());

                                          } else if (level4Node.getNodeName() == "AudioSampleRate") {
                                              Node level4NodeData = level4Node.getFirstChild();
                                              if (level4NodeData != null)
                                                  audioTrack.sample_rate = Long.parseLong(level4NodeData.getNodeValue());

                                          } else if (level4Node.getNodeName() == "AudioCompressor") {
                                              Node level4NodeData = level4Node.getFirstChild();
                                              if (level4NodeData != null)
                                                  audioTrack.compressor = level4NodeData.getNodeValue();

                                          }
                                      }// Level 4 scaning
                                      itemFromXML.audioTracks.add(audioTrack);

                                  } else if (level3Node.getNodeName() == "Comment") {
                                      Node level3NodeData = level3Node.getFirstChild();
                                      if (level3NodeData != null)
                                          itemFromXML.comment_text = level3NodeData.getNodeValue();
                                  }
                                  //AudioTrackList

                              } //Level 3 for
                              videoItems.add(itemFromXML);
                          }
                      } //Level 2 for
                  }
              } //Level 1 for
            }
            return true;
        } catch (Exception prob){
            //Error
            prob.printStackTrace();
        }
        return false;
    }

    public String GetFullListing()
    {
      String listing = new String();

      for (int v = 0; v < videoItems.size(); v++) {
        VideoSquirrelListItem currentItem = (VideoSquirrelListItem)videoItems.get(v);

        listing += currentItem.GetFormattedString();
      }

      return listing;
    }

    public String GetByUID(long uid)
    {
      for (int v = 0; v < videoItems.size(); v++) {
        VideoSquirrelListItem currentItem = (VideoSquirrelListItem)videoItems.get(v);

        if (currentItem.UID == uid)
          return currentItem.GetFormattedString();
      }

      return "";
    }

    /// Returns an ArrayList of the Long objects with the UID's
    public ArrayList GetUIDList()
    {
      ArrayList uidList = new ArrayList(videoItems.size());
      
      for (int v = 0; v < videoItems.size(); v++) {
        VideoSquirrelListItem currentItem = (VideoSquirrelListItem)videoItems.get(v);

        uidList.add(new NamedUID(currentItem));
      }

      return uidList;
    }
        
    public String SearchFor(String searchStr, boolean bTitleOnly)
    {
      String searchResult = new String();
      int index = 0;
      searchStr = searchStr.toLowerCase();

      for (int v = 0; v < videoItems.size(); v++) {
        VideoSquirrelListItem currentItem = (VideoSquirrelListItem)videoItems.get(v);

        index = currentItem.title.toLowerCase().indexOf(searchStr);
        if (index != -1) {
          searchResult += currentItem.GetFormattedString();
          continue;
        }

        if (!bTitleOnly) {
          if (currentItem.cd != null) {
            index = currentItem.cd.toLowerCase().indexOf(searchStr);
            if (index != -1) {
              searchResult += currentItem.GetFormattedString();
              continue;
            }
          }
          if (currentItem.comment_text != null) {
            index = currentItem.comment_text.toLowerCase().indexOf(searchStr);
            if (index != -1) {
              searchResult += currentItem.GetFormattedString();
              continue;
            }
          }
          if (currentItem.filename != null) {
            index = currentItem.filename.toLowerCase().indexOf(searchStr);
            if (index != -1) {
              searchResult += currentItem.GetFormattedString();
              continue;
            }
          }
        }
      }

      if (searchResult.length() == 0)
        return "No results";

      return searchResult;
    }

    class FancyFormater
    {
        public String FormatFileSize(long file_size)
        {
            String formated_file_size = "N/A";
            if (file_size < 1024) {
                formated_file_size = Long.toString(file_size) + " bytes";
            } else if (file_size < 1024 * 1024) {
                formated_file_size = Float.toString((float)file_size / 1024) + " KB";
            } else if (file_size < 1024 * 1024 * 1024) {
                formated_file_size = Float.toString((float)file_size / 1024 / 1024) + " MB";
            } else {
                formated_file_size = Float.toString((float)file_size / 1024 / 1024 / 1024) + " GB";
            }
            return formated_file_size;
        }

        public String FormatDuration(long duration)
        {
            if (duration < 60)
            {
                    return Long.toString(duration) + "%i seconds";
            }
            else //if (length < 60 * 60)
            {
                    return Long.toString(duration / 60) + " minutes " + Long.toString(duration - (duration / 60) * 60) + " seconds";
            }
        }
    }

    class VideoSquirrelListItemVideoTrack extends java.lang.Object {
        public String compressor;
        public long x;
        public long y;
        public long display_x;
        public long display_y;
        public float frame_rate;
        public long duration;
        public long avg_bitrate;
    };

    class VideoSquirrelListItemAudioTrack extends java.lang.Object {
        public String compressor;
        public long channels;
        public long sample_rate;
        public long avg_bitrate;
    };

    class VideoSquirrelListItem extends java.lang.Object {
        public long UID;
        public String title;
        public String filename;
        public String cd;
        public long file_size;
        public ArrayList videoTracks;
        public ArrayList audioTracks;
        public String comment_text;

        public VideoSquirrelListItem() {
            videoTracks = new ArrayList();
            audioTracks = new ArrayList();
            title = "";
        };
        public VideoSquirrelListItem(String initalName) {
            title = initalName;
        };

        public String toString() {
            return title;
        };

        public String GetFormattedString() {
          String formattedString;

          formattedString = "<p><b><font size=+1>" + title + "</font></b><hr>";
          formattedString += "<p><b>CD:</b>" + cd + "<br>";
          formattedString += "<b>Filename:</b> " + filename + "<br>";
          formattedString += "<b>UID:</b> " + String.valueOf(UID) + "<br>";
          FancyFormater formatter = new FancyFormater();

          formattedString += "<b>File Size:</b> " + formatter.FormatFileSize(file_size) + "<br>";

          for (int v = 0; v < videoTracks.size(); v++) {
              VideoSquirrelListItemVideoTrack currentTrack = (VideoSquirrelListItemVideoTrack)videoTracks.get(v);
              formattedString += "<b>Video Track:</b> " + String.valueOf(v+1) + "<br>";
              formattedString += "<b>Duration:</b> " + formatter.FormatDuration(currentTrack.duration) + "<br>";
              formattedString += "<b>Video Bitrate:</b> " + String.valueOf(currentTrack.avg_bitrate) + "<br>";
              formattedString += "<b>Video Pixel Size:</b> " + String.valueOf(currentTrack.x) + " x " + String.valueOf(currentTrack.y) + "<br>";
              formattedString += "<b>Framerate:</b> " + String.valueOf(currentTrack.frame_rate) + "<br>";
              formattedString += "<b>Video Compression:</b> " + currentTrack.compressor;
          }

                      formattedString += "<table><tr><td width=20px></td><td>";

          for (int v = 0; v < audioTracks.size(); v++) {
              VideoSquirrelListItemAudioTrack currentTrack = (VideoSquirrelListItemAudioTrack)audioTracks.get(v);
              formattedString += "<b>Audio Track:</b> " + String.valueOf(v+1) + "<br>";
              formattedString += "<b>Compressor:</b> " + currentTrack.compressor + "<br>";
              formattedString += "<b>Bitrate:</b> " + String.valueOf(currentTrack.avg_bitrate) + "<br>";
              formattedString += "<b>Channels:</b> " + String.valueOf(currentTrack.channels) + "<br>";
              formattedString += "<b>Sample Rate:</b> " + String.valueOf(currentTrack.sample_rate) + "<br>";
          }
          formattedString += "</td></tr></table></p><hr>";

          return formattedString;
        };
    }
    
    class NamedUID  extends java.lang.Object {
        protected String title;
        protected long uid;
        
        public NamedUID(VideoSquirrelListItem item) {
            title = item.title;
            uid = item.UID;
        }
        public String toString() {
            return title;
        };
        
        public int hashCode() {
            return (int)uid;
        };
    };
    
    public boolean isLoaded;
    public ArrayList videoItems;
}

