package com.stone.jory.audio.server;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004 Jory Stone</p>
 * <p>Company: </p>
 * @author jcsston
 * @version 1.0
 */

public class ULawAudioServerGUIFrame extends JFrame {
  JPanel contentPane;
  JLabel statusBar = new JLabel();
  BorderLayout borderLayout1 = new BorderLayout();
  JPanel jPanel1 = new JPanel();
  GridLayout gridLayout1 = new GridLayout();
  JLabel jLabel1 = new JLabel();

  //Construct the frame
  public ULawAudioServerGUIFrame() {
    enableEvents(AWTEvent.WINDOW_EVENT_MASK);
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  //Component initialization
  private void jbInit() throws Exception  {
    contentPane = (JPanel) this.getContentPane();
    contentPane.setLayout(borderLayout1);
    this.setSize(new Dimension(400, 300));
    this.setTitle("ULaw Audio Server");
    statusBar.setText(" ");
    jPanel1.setLayout(gridLayout1);
    gridLayout1.setColumns(1);
    gridLayout1.setRows(4);
    jLabel1.setFont(new java.awt.Font("Dialog", 1, 11));
    jLabel1.setHorizontalAlignment(SwingConstants.CENTER);
    jLabel1.setHorizontalTextPosition(SwingConstants.CENTER);
    jLabel1.setText("ULaw Audio Server");
    contentPane.add(statusBar, BorderLayout.NORTH);
    contentPane.add(jPanel1, BorderLayout.CENTER);
    jPanel1.add(jLabel1, null);
  }

  //Overridden so we can exit when window is closed
  protected void processWindowEvent(WindowEvent e) {
    super.processWindowEvent(e);
    if (e.getID() == WindowEvent.WINDOW_CLOSING) {
      System.exit(0);
    }
  }
}
