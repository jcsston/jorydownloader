
//import VideoSquirrel.*;

import javax.servlet.*;
import javax.servlet.http.*;
import java.lang.*;
import java.io.*;
import java.util.*;

public class VideoSquirrelDatabaseServlet extends HttpServlet {
  protected static String databaseFilename = new String("I:\\videos\\videos.xml");
  protected static VideoSquirrelDatabaseReader videoDatabase = new VideoSquirrelDatabaseReader();

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
      throws ServletException, IOException {

    PrintWriter out = response.getWriter();
    out.print("<html><body>");
    out.print("<h1>Video Squirrel Database</h1>");
    //out.print("<p align=\"right\">JSP interface</p>");
    out.print("<hr>");
          
    synchronized(videoDatabase) {
      if (videoDatabase.isLoaded == false)
        videoDatabase.LoadDatabase(databaseFilename);
    };

    if (request.getParameter("list") != null) {
      out.print("<b>Listing</b>");// file: " + filename + "</b>");
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

    } else if (request.getParameter("reload") != null) {
        synchronized(videoDatabase) {
            videoDatabase = new VideoSquirrelDatabaseReader();
            videoDatabase.LoadDatabase(databaseFilename);
        };
    } else {
      // Display the default main page
      out.print("<form action=\"" + request.getContextPath() + "videos.jsp\">");
      out.print("<p>Search: <input type=\"text\" size=\"20\" name=\"search\">");
      out.print("<input type=\"submit\" value=\"Search\">");
      out.print("<br><input type=\"checkbox\" name=\"titleOnly\" value=\"1\">Titles only</p>");
      out.print("</form>");
      out.print("<p>Total record count: " + String.valueOf(videoDatabase.videoItems.size()) + "</p>");
    }

    out.print("<hr>");//<p align=\"right\">Served by ");
    //out.print(application.getServerInfo());
    //out.print("</p>");
    out.print("</body></html>");
  }

  public void doPost(HttpServletRequest request,
                     HttpServletResponse response)
      throws ServletException, IOException {
    doGet(request, response);
  }

}

