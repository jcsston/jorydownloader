package com.stone.jory.audio.server;

import java.io.*;
import java.net.*;
import java.util.*;

/**
 * <p>Title: This manages the client threads</p>
 * <p>Description: </p>
 * <p>Copyright: Copyright (c) 2004 Jory Stone</p>
 * <p>Company: </p>
 * @author jcsston
 * @version 1.0
 */

public class ULawAudioServerListener extends Thread {
  protected ULawAudioServer parent = null;
  protected ServerSocket socket = null;
  protected LinkedList clients = new LinkedList();
  protected boolean exit = false;

  public ULawAudioServerListener(ULawAudioServer parent, int port) throws IOException {
    this.parent = parent;
    socket = new ServerSocket(port);
  }

  public void feedClients(byte [] data) {
    synchronized (clients) {
      ListIterator listIter = clients.listIterator();
      while (listIter.hasNext()) {
        ULawAudioServerClient client = (ULawAudioServerClient)listIter.next();
        client.addData(data);
      }
    }
  }

  public boolean removeClient(ULawAudioServerClient client) {
    synchronized (clients) {
      return clients.remove(client);
    }
  }

  public void run() {
    Socket client = null;
    ULawAudioServerClient clientThread = null;

    try {
      socket.setSoTimeout(1000);
    } catch (SocketException ex1) {
      ex1.printStackTrace();
    }

    while (!exit) {
      try {
        client = socket.accept();
        clientThread = new ULawAudioServerClient(this, client);
        clientThread.start();
        synchronized (clients) {
          clients.add(clientThread);
        }
      } catch (InterruptedIOException ex) {

      } catch (IOException ex) {

      }
    }
  }

  public void exit() {
    synchronized (clients) {
      ListIterator listIter = clients.listIterator();
      while (listIter.hasNext()) {
        ULawAudioServerClient client = (ULawAudioServerClient)listIter.next();
        client.exit();
      }
    }
    exit = true;
  }
}
