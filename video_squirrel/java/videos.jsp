<html>
<body>
<jsp:directive.page import="java.lang.*"/>
<jsp:directive.page import="java.io.*"/>
<jsp:directive.page import="java.text.*"/>
<jsp:directive.page import="java.util.*"/>
<jsp:directive.page import="java.util.zip.*"/>
<jsp:directive.page import="javax.xml.parsers.*"/>
<jsp:directive.page import="org.xml.sax.*"/>
<jsp:directive.page import="org.w3c.dom.*"/>

<%
// Web Interface for Video Squirrel Database
/*

          if (1)
          {
            for (int v = 0; v < currentItem.videoTracks.size(); v++) {
                VideoSquirrelListItemVideoTrack currentTrack = (VideoSquirrelListItemVideoTrack)currentItem.videoTracks.get(v);

                index = currentTrack.compressor.toLowerCase().indexOf(searchStr);
                if (index != -1) {
                  searchResult += currentItem.GetFormattedString();
                  continue;
                }
            }

            for (int v = 0; v < currentItem.audioTracks.size(); v++) {
                VideoSquirrelListItemAudioTrack currentTrack = (VideoSquirrelListItemAudioTrack)currentItem.audioTracks.get(v);
                index = currentTrack.compressor.toLowerCase().indexOf(searchStr);
                if (index != -1) {
                  searchResult += currentItem.GetFormattedString();
                  continue;
                }
            }
          }

          
class BoyerMooreTextSearch
{
    // Member variables for storing precomputed pattern data
    private int   patLen;
    private int[] patElem;
    private int[] shifts;

    // Return a mask for the part of the order we're interested in
    public int getMask(int weight) {
       switch (weight) {
             case Collator.PRIMARY:
                     return 0xFFFF0000;
             case Collator.SECONDARY:
                     return 0xFFFFFF00;
             default:
                     return 0xFFFFFFFF;
       }
    }
    
    // Map a collation element to an array index
    int hash(int order) {
         return CollationElementIterator.primaryOrder(order) % 256;
    }

    // Initialize the Boyer-Moore shift tables
    void initialize(RuleBasedCollator c, String pat)
    {
     // First find out how many elements we're dealing with
     patLen = 0;
     CollationElementIterator iter = c.getCollationElementIterator(pat);
     while (iter.next() != CollationElementIterator.NULLORDER)
            patLen++;

     // Allocate space to store the pattern elements and the shift tables
     patElem = new int[patLen];
     shifts = new int[256];

     // Elements not found in the pattern get the maximum shift distance
     for (int i = 0; i < 256; i++) {
           shifts[i] = patLen;
     }

     // Now compute the shift distances for the elements in the pattern.
     // While we're at it, save the elements themselves for quick access.
     // The "-1" is in the calculation because Java indices are 0-based.
     iter.reset();
     for (int i = 0; i < patLen; i++) {
           patElem[i] = iter.next();
           int index = hash(patElem[i]);
           shifts[index] = Math.min(shifts[index], patLen - i - 1);
     }
    }
    public int find(String text, String pattern)
    {
        RuleBasedCollator coll = (RuleBasedCollator)Collator.getInstance();
        CollationElementIterator targIter =
                                        coll.getCollationElementIterator(text);

       // build the shift table and the constants we need
        initialize(coll, pattern);
        int mask = getMask(weight);
        int done = CollationElementIterator.NULLORDER & mask;

      // Start at the text position corresponding to the end of the pattern
        int textIndex = pattern.length();

        while (textIndex <= text.length()) {
            boolean getPattern = true, getTarget = true;
            int targetElement=0, patternElement=0;

            iter.setOffset(textIndex);
            int patIndex = pattern.length();

            // Iterate backward until we hit the beginning of the pattern
            while (patIndex > 0)
            {
                if (getTarget)  targetElement  = targIter.previous() & mask;
                if (getPattern) patternElement = patElem[--patIndex] & mask;
                getTarget = getPattern = true;

                if (targetElement == 0) {
                    getPattern = false;            // skip targetElement
                } else if (patternElement == 0) {
                    getTarget = false;             // skip patternElement
                } else if (targetElement != patternElement) {
                    // There is a mismatch at this position.  Decide how far
                    // over to shift the pattern, then try again.
                    textIndex = iter.getOffset() +
                                shifts[hash(targetElement)];
                    break;
                }
            }
            if (patIndex == 0) {
                // We made it back to the beginning of the pattern,
                // which means we matched it all.  Return the location.
                return targIter.getOffset();
            }
            // Otherwise, we're here because of a mismatch, so keep going....
        }
        return -1;            // No match.
    }
}
*/
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

class VideoSquirrelDatabaseReader
{
    public VideoSquirrelDatabaseReader()
    {
      videoItems = new ArrayList();
    }

    public boolean LoadDatabase(String filename)
    {
        try {
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
      String listing = new String("");
      
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
    
    public ArrayList videoItems;
}

%>

<%
  
out.print("<h1>Video Squirrel Database</h1>");
//out.print("<p align=\"right\">JSP interface</p>");
out.print("<hr>");

String filename;
filename = request.getParameter("database");
if (filename == null) {
  filename = "I:\\videos\\";
  filename += "videos.xml";
}
VideoSquirrelDatabaseReader videoDatabase = new VideoSquirrelDatabaseReader();
videoDatabase.LoadDatabase(filename);

if (request.getParameter("list") != null) {
  out.print("<b>Listing file: " + filename + "</b>");
  out.print(videoDatabase.GetFullListing());

} else if (request.getParameter("UID") != null) {
  out.print("<b>Looking up record with UID: </b>" + request.getParameter("UID") + "<br>");
  String lookupStr = videoDatabase.GetByUID(Long.parseLong(request.getParameter("UID")));
  if (lookupStr.length() == 0)
    out.print("<p>Record not found!</p>");
  else
    out.print(lookupStr);

} else if (request.getParameter("search") != null) {
  String searchStr = request.getParameter("search");
  out.print("<h2>Search Results for <i>" + searchStr + "</i></h2>");
  
  String titleOnlyStr = request.getParameter("titleOnly");
  boolean titleOnly = false;
  if ((titleOnlyStr != null) && (Long.parseLong(titleOnlyStr) == 1)) {
    out.print("<h3>Titles Only</h3>");
    titleOnly = true;
  }
  

  out.print(videoDatabase.SearchFor(searchStr, titleOnly));
  
} else {
  // Display the default main page
  out.print("<form action=\"" + request.getContextPath() + "videos.jsp\">");
  out.print("<p>Search: <input type=\"text\" size=\"20\" name=\"search\">");
  out.print("<input type=\"submit\" value=\"Search\">");
  out.print("<br><input type=\"checkbox\" name=\"titleOnly\" value=\"1\">Titles only</p>");
  out.print("</form>");
  out.print("<p>Total record count: " + String.valueOf(videoDatabase.videoItems.size()) + "</p>");
}

%>

<%
  out.print("<hr><p align=\"right\">Served by ");
  out.print(application.getServerInfo());
  out.print("</p>");
%>
</body>
</html>
