/*
 * VideoSquirrelDatabaseViewer.java
 *
 * Created on June 26, 2003, 12:20 AM
 */

/**
 *
 * @author  Jory
 */
import java.util.ArrayList;
import javax.xml.parsers.DocumentBuilder; 
import javax.xml.parsers.DocumentBuilderFactory; 
import javax.xml.parsers.FactoryConfigurationError; 
import javax.xml.parsers.ParserConfigurationException;  
import org.xml.sax.SAXException; 
import org.xml.sax.SAXParseException; 
//Add these lines to read the sample XML file and identify errors:
import java.io.File;
import java.io.IOException; 
//Finally, import the W3C definition for a DOM and DOM exceptions:
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.DOMException; 

public class VideoSquirrelDatabaseViewer extends javax.swing.JDialog {  
    protected String databaseFilename;    
    
    protected long recordCount;
    
    /** Creates new form VideoSquirrelDatabaseViewer */
    public VideoSquirrelDatabaseViewer(java.awt.Frame parent, boolean modal) {
        super(parent, modal);
        initComponents();
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    private void initComponents() {//GEN-BEGIN:initComponents
        jFileChooser1 = new javax.swing.JFileChooser();
        jSplitPane1 = new javax.swing.JSplitPane();
        jScrollPane1 = new javax.swing.JScrollPane();
        jList1Model = new javax.swing.DefaultListModel();
        jList1 = new javax.swing.JList();
        jList1 = new javax.swing.JList(jList1Model);
        jScrollPane2 = new javax.swing.JScrollPane();
        jTextArea1 = new javax.swing.JTextArea();
        jMenuBar1 = new javax.swing.JMenuBar();
        jMenu1 = new javax.swing.JMenu();
        jMenuItemOpen = new javax.swing.JMenuItem();
        jSeparator1 = new javax.swing.JSeparator();
        jMenuItemExit = new javax.swing.JMenuItem();

        getContentPane().setLayout(new java.awt.GridLayout(1, 1, 5, 0));

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Video Squirrel Database Java View");
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent evt) {
                closeDialog(evt);
            }
        });

        jSplitPane1.setDividerSize(9);
        jScrollPane1.setPreferredSize(new java.awt.Dimension(150, 131));
        jList1.setBorder(new javax.swing.border.CompoundBorder(new javax.swing.border.LineBorder(new java.awt.Color(0, 0, 0)), new javax.swing.border.EmptyBorder(new java.awt.Insets(1, 1, 1, 1))));
        jList1.setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);
        jList1.setMaximumSize(new java.awt.Dimension(2147483647, 2147483647));
        jList1.setMinimumSize(new java.awt.Dimension(50, 50));
        jList1.setPreferredSize(new java.awt.Dimension(100, 2000));
        jList1.addListSelectionListener(new javax.swing.event.ListSelectionListener() {
            public void valueChanged(javax.swing.event.ListSelectionEvent evt) {
                jList1ValueChanged(evt);
            }
        });
        jList1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                jList1MouseClicked(evt);
            }
        });

        jScrollPane1.setViewportView(jList1);

        jSplitPane1.setLeftComponent(jScrollPane1);

        jScrollPane2.setPreferredSize(new java.awt.Dimension(200, 67));
        jTextArea1.setEditable(false);
        jTextArea1.setLineWrap(true);
        jTextArea1.setText("Video Squirrel Database Java Viewer");
        jTextArea1.setWrapStyleWord(true);
        jTextArea1.setBorder(new javax.swing.border.CompoundBorder(new javax.swing.border.CompoundBorder(new javax.swing.border.EmptyBorder(new java.awt.Insets(1, 1, 1, 1)), new javax.swing.border.LineBorder(new java.awt.Color(0, 0, 0))), new javax.swing.border.EmptyBorder(new java.awt.Insets(2, 2, 2, 2))));
        jTextArea1.setMinimumSize(new java.awt.Dimension(80, 100));
        jTextArea1.setPreferredSize(new java.awt.Dimension(200, 64));
        jScrollPane2.setViewportView(jTextArea1);

        jSplitPane1.setRightComponent(jScrollPane2);

        getContentPane().add(jSplitPane1);

        jMenu1.setText("File");
        jMenu1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenu1ActionPerformed(evt);
            }
        });

        jMenuItemOpen.setText("Open");
        jMenuItemOpen.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItemOpenActionPerformed(evt);
            }
        });

        jMenu1.add(jMenuItemOpen);

        jMenu1.add(jSeparator1);

        jMenuItemExit.setText("Exit");
        jMenuItemExit.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jMenuItemExitActionPerformed(evt);
            }
        });

        jMenu1.add(jMenuItemExit);

        jMenuBar1.add(jMenu1);

        setJMenuBar(jMenuBar1);

        pack();
    }//GEN-END:initComponents

    private void jMenuItemExitActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItemExitActionPerformed
        // Add your handling code here:
        
    }//GEN-LAST:event_jMenuItemExitActionPerformed

    private void jList1MouseClicked(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jList1MouseClicked
        // Add your handling code here:

    }//GEN-LAST:event_jList1MouseClicked

    private void jMenuItemOpenActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenuItemOpenActionPerformed
        // Add your handling code here:
        //javax.swing.JLabel listText = new javax.swing.JLabel();
        //listText.setText("test");
        //jList1.add("Test", listText);
        jFileChooser1.showOpenDialog(this);
        File selectedFile = jFileChooser1.getSelectedFile();
        this.setTitle("Opening: " + selectedFile.getPath());
        ReadDatabase(selectedFile.getPath());
    }//GEN-LAST:event_jMenuItemOpenActionPerformed

    private void jMenu1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jMenu1ActionPerformed
        // Add your handling code here:
    }//GEN-LAST:event_jMenu1ActionPerformed

    private void jList1ValueChanged(javax.swing.event.ListSelectionEvent evt) {//GEN-FIRST:event_jList1ValueChanged
        // Add your handling code here:        
        try {
            Object objSelectedItem = jList1.getSelectedValue();
            VideoSquirrelListItem selectedItem = (VideoSquirrelListItem)objSelectedItem;
            int lastLinePos = jTextArea1.getLineEndOffset(jTextArea1.getLineCount()-1);
            jTextArea1.replaceRange(selectedItem.GetFormattedString(), 0, lastLinePos);        
        } catch (Exception prob){
            //Error
            prob.printStackTrace();
        }        
    }//GEN-LAST:event_jList1ValueChanged
    
    /** Closes the dialog */
    private void closeDialog(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_closeDialog
        setVisible(false);
        dispose();
    }//GEN-LAST:event_closeDialog
    
    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        new VideoSquirrelDatabaseViewer(new javax.swing.JFrame(), true).show();
    }
    
    boolean ReadDatabase(java.lang.String filename) {
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
                    int pos = jList1.getModel().getSize();
                    VideoSquirrelListItem test = new VideoSquirrelListItem(level1Node.getNodeName());
                    jList1Model.add(pos, test);                    
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
                                        
                                    } else if (level3Node.getNodeName() == "CD") {
                                        Node level3NodeData = level3Node.getFirstChild();
                                        
                                    } else if (level3Node.getNodeName() == "Title") {
                                        Node level3NodeData = level3Node.getFirstChild();
                                        itemFromXML.title = level3NodeData.getNodeValue();

                                    } else if (level3Node.getNodeName() == "Filename") {
                                        Node level3NodeData = level3Node.getFirstChild();

                                    } else if (level3Node.getNodeName() == "Filesize") {
                                        Node level3NodeData = level3Node.getFirstChild();
                                        
                                    } else if (level3Node.getNodeName() == "VideoData") {
                                        NodeList level4 = level3Node.getChildNodes();
                                        //Create a new video track data class
                                        VideoSquirrelListItemVideoTrack videoTrack = new VideoSquirrelListItemVideoTrack();
                                        for (int level4Num = 0; level4Num < level4.getLength(); level4Num++) {
                                            Node level4Node = level4.item(level4Num);
                                            if (level4Node.getNodeName() == "Bitrate") {
                                                Node level4NodeData = level4Node.getFirstChild();                                                
                                                
                                            } else if (level4Node.getNodeName() == "PixelWidth") {
                                                Node level4NodeData = level4Node.getFirstChild();                                                

                                            } else if (level4Node.getNodeName() == "PixelHeight") {
                                                Node level4NodeData = level4Node.getFirstChild();                                                
                                                
                                            } else if (level4Node.getNodeName() == "ColorDepth") {
                                                Node level4NodeData = level4Node.getFirstChild();                                                
                                                if (level4NodeData != null)
                                                    videoTrack.color_depth = Integer.parseInt(level4NodeData.getNodeValue());
                                                
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
                                                
                                            } else if (level4Node.getNodeName() == "VideoCompressor2") {
                                                Node level4NodeData = level4Node.getFirstChild();                                                
                                                if (level4NodeData != null)
                                                    videoTrack.compressor2 = level4NodeData.getNodeValue();
                                                
                                            }
                                        }// Level 4 scaning
                                        itemFromXML.videoTracks.add(videoTrack);
                                    } else if (level3Node.getNodeName() == "AudioTrackList") {
                                        Node level3NodeData = level3Node.getFirstChild();

                                    } else if (level3Node.getNodeName() == "Comment") {
                                        Node level3NodeData = level3Node.getFirstChild();
                                        if (level3NodeData != null)
                                            itemFromXML.comment_text = level3NodeData.getNodeValue();
                                    }
                                    //AudioTrackList

                                } //Level 3 for      
                                jList1Model.addElement(itemFromXML);
                            }
                        } //Level 2 for
                    }                    
                } //Level 1 for
            }
            
            return true;
        }catch (Exception prob){
            //Error
            prob.printStackTrace();
        }
        return false;
    }    
    private javax.swing.DefaultListModel jList1Model;
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JFileChooser jFileChooser1;
    private javax.swing.JList jList1;
    private javax.swing.JMenu jMenu1;
    private javax.swing.JMenuBar jMenuBar1;
    private javax.swing.JMenuItem jMenuItemExit;
    private javax.swing.JMenuItem jMenuItemOpen;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JSeparator jSeparator1;
    private javax.swing.JSplitPane jSplitPane1;
    private javax.swing.JTextArea jTextArea1;
    // End of variables declaration//GEN-END:variables
    
}

class VideoSquirrelListItemVideoTrack extends java.lang.Object {
    public String compressor;
    public String compressor2;
    public int x;
    public int y;
    public int display_x;
    public int display_y;
    public float frame_rate;
    public long duration;
    public int color_depth;
    public int avg_bitrate;
};

class VideoSquirrelListItemAudioTrack extends java.lang.Object {
    public String compression;
    public int channels;
    public short bit_depth;
    float sample_rate;
    public int avg_bitrate;
};

class VideoSquirrelListItem extends java.lang.Object {
    public String displayName;
    public String otherData;
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
    };
    public VideoSquirrelListItem(String initalName) {
        title = initalName;
        otherData = "COOL!!!";
    };
    
    public String toString() {
        return title;
    };
    
    public String GetFormattedString() {
      String formattedString;
      
      formattedString = "Title: " + title;
      formattedString += "\nFilename: " + filename;
      
      for (int v = 0; v < videoTracks.size(); v++) {
          VideoSquirrelListItemVideoTrack currentTrack = (VideoSquirrelListItemVideoTrack)videoTracks.get(v);
          formattedString += "\nVideo Track " + String.valueOf(v);
          formattedString += "\nPixel Width: " + String.valueOf(currentTrack.x);
          formattedString += "\nPixel Height: " + String.valueOf(currentTrack.y);
          formattedString += "\nFramerate: " + String.valueOf(currentTrack.frame_rate);
          formattedString += "\n Compressor: " + currentTrack.compressor;
          formattedString += "\n Compressor2: " + currentTrack.compressor2;
      }
      
      return formattedString;
    };
}
