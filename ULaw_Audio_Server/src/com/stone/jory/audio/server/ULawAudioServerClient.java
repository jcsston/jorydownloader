package com.stone.jory.audio.server;

import java.io.*;
import java.net.*;
import java.util.*;

/**
 * <p>Title: </p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004 Jory Stone</p>
 * <p>Company: </p>
 * @author jcsston
 * @version 1.0
 */

public class ULawAudioServerClient extends Thread {
  protected ULawAudioServerListener parent = null;
  protected Socket client = null;
  protected LinkedList queue = new LinkedList();
  protected boolean exit = false;

  public ULawAudioServerClient(ULawAudioServerListener parent, Socket client) {
    this.parent = parent;
    this.client = client;
  }

  public void addData(byte [] data) {
    synchronized (this.queue) {
      this.queue.add(data);
    }
  }

  public void run() {
    InputStream input = null;
    OutputStream output = null;
    try {
      input = client.getInputStream();
      output = client.getOutputStream();
    } catch (IOException ex1) {
      ex1.printStackTrace();
      exit = true;
    }

    while (!exit) {
      try {
        byte [] data = null;
        synchronized (queue) {
          if (!queue.isEmpty())
            data = (byte [])queue.removeFirst();
        }
        if (data != null) {
          output.write(data);
        }
      } catch (Exception ex) {
        ex.printStackTrace();
      }
    }
    parent.removeClient(this);
  }

  public void exit() {
    exit = true;
  }
}
