package com.stone.jory.audio.server;

import java.io.*;
import java.net.*;
import java.util.*;
import javax.sound.sampled.*;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004 Jory Stone</p>
 * <p>Company: </p>
 * @author jcsston
 * @version 1.0
 */

public class ULawAudioServer extends Thread {
  static protected int BUFFER_SIZE = 1024;
  protected ULawAudioServerListener listenerThread = null;
  protected AudioInputStream audio = null;
  protected boolean exit = false;

  public ULawAudioServer(int port) throws IOException {
    listenerThread = new ULawAudioServerListener(this, port);
  }

  public void setAudioStream(AudioInputStream audio) {
    this.audio = audio;
  }

  public void run() {
    listenerThread.start();

    byte [] buffer = new byte[BUFFER_SIZE];
    while (!exit) {
      try {
        audio.read(buffer, 0, BUFFER_SIZE);
        listenerThread.feedClients(buffer);
      } catch (IOException ex) {

      }
    }
  }

  public void exit() {
    listenerThread.exit();
    exit = true;
  }
}
