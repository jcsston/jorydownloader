using System;
using System.Net;
using System.IO;
using System.Text;
using System.Threading;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace WindowsApplication1
{
	/// <summary>
	/// Summary description for Progress.
	/// </summary>
	public class Progress : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button buttonPause;
		private System.Windows.Forms.Button buttonCancel;
		private System.Windows.Forms.Label labelFile;
		private System.Windows.Forms.ProgressBar progressBarDownload;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public static ManualResetEvent allDone = new ManualResetEvent(false);
		private System.Windows.Forms.LinkLabel labelURL;
		const int BUFFER_SIZE = 1024;

		public Progress()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.progressBarDownload = new System.Windows.Forms.ProgressBar();
			this.buttonPause = new System.Windows.Forms.Button();
			this.buttonCancel = new System.Windows.Forms.Button();
			this.labelFile = new System.Windows.Forms.Label();
			this.labelURL = new System.Windows.Forms.LinkLabel();
			this.SuspendLayout();
			// 
			// progressBarDownload
			// 
			this.progressBarDownload.Location = new System.Drawing.Point(0, 40);
			this.progressBarDownload.Name = "progressBarDownload";
			this.progressBarDownload.Size = new System.Drawing.Size(352, 32);
			this.progressBarDownload.TabIndex = 0;
			// 
			// buttonPause
			// 
			this.buttonPause.Location = new System.Drawing.Point(200, 80);
			this.buttonPause.Name = "buttonPause";
			this.buttonPause.TabIndex = 1;
			this.buttonPause.Text = "Pause";
			this.buttonPause.Click += new System.EventHandler(this.buttonPause_Click);
			// 
			// buttonCancel
			// 
			this.buttonCancel.Location = new System.Drawing.Point(280, 80);
			this.buttonCancel.Name = "buttonCancel";
			this.buttonCancel.TabIndex = 2;
			this.buttonCancel.Text = "Cancel";
			this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
			// 
			// labelFile
			// 
			this.labelFile.Name = "labelFile";
			this.labelFile.Size = new System.Drawing.Size(304, 16);
			this.labelFile.TabIndex = 3;
			this.labelFile.Text = "Downloading File:";
			// 
			// labelURL
			// 
			this.labelURL.Location = new System.Drawing.Point(0, 16);
			this.labelURL.Name = "labelURL";
			this.labelURL.Size = new System.Drawing.Size(352, 16);
			this.labelURL.TabIndex = 4;
			// 
			// Progress
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(360, 109);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																																	this.labelURL,
																																	this.labelFile,
																																	this.buttonCancel,
																																	this.buttonPause,
																																	this.progressBarDownload});
			this.Name = "Progress";
			this.Text = "Progress";
			this.Load += new System.EventHandler(this.Progress_Load);
			this.ResumeLayout(false);

		}
		#endregion

		public void StartDownload(aDownloadItem download_item){
			labelURL.Text = download_item.RemoteURL;
			
			HttpWebRequest new_url = (HttpWebRequest)WebRequest.Create(download_item.RemoteURL);
			new_url.AllowAutoRedirect = true;
			HttpWebResponse url_response = (HttpWebResponse)new_url.GetResponse();
			
			//Create a RequestState object to store all the data needed by the Aync Callback
			RequestState rs = new RequestState();
			
			//Store the File's UID so we can find it again in the main download list
			rs.DownloadFileUID = download_item.FileUID;
			
			//Create and store a FileWriter object
			rs.FileWriter = new FileStream(download_item.LocalFilename, FileMode.Append);
			
			//Cut off the last 1K for resuming
			download_item.doneFileSize = rs.FileWriter.Length;
			if ((download_item.doneFileSize > 1000) && (url_response.ContentLength > 1000)) {
				rs.FileWriter.Seek(download_item.doneFileSize-1000, SeekOrigin.Begin);
			}else {
				rs.FileWriter.Seek(download_item.doneFileSize, SeekOrigin.Begin);
			}
									
			//Resuming?
			new_url.AddRange((int)rs.FileWriter.Position, (int)url_response.ContentLength);			

			// Put the HttpWebRequest into the state object so it can be passed around.
			rs.Request = new_url;
									
			//Setup the progress bar
			progressBarDownload.Minimum = (int)0;
			progressBarDownload.Maximum = (int)100;
			progressBarDownload.Tag = (long)url_response.ContentLength;
			progressBarDownload.Value = (int)((rs.FileWriter.Position+1) / url_response.ContentLength);

			// Issue the async request.
			IAsyncResult r = (IAsyncResult) new_url.BeginGetResponse(new AsyncCallback(RespCallback), rs);

			this.Tag = download_item;
		}

		private static void RespCallback(IAsyncResult ar) {
			// Get the RequestState object from the async result.
			RequestState rs = (RequestState) ar.AsyncState;

			// Get the HttpWebResponse from RequestState.
			HttpWebRequest req = rs.Request;

			// Call EndGetResponse, which produces the HttpWebResponse object
			//  that came from the request issued above.
			HttpWebResponse resp = (HttpWebResponse)req.EndGetResponse(ar);         

			//  Start reading data from the response stream.
			Stream ResponseStream = resp.GetResponseStream();

			// Store the response stream in RequestState to read 
			// the stream asynchronously.
			rs.ResponseStream = ResponseStream;

			//  Pass rs.BufferRead to BeginRead. Read data into rs.BufferRead
			IAsyncResult iarRead = ResponseStream.BeginRead(rs.BufferRead, 0, BUFFER_SIZE, new AsyncCallback(ReadCallBack), rs);
		}


		private static void ReadCallBack(IAsyncResult asyncResult) {
			// Get the RequestState object from AsyncResult.
			RequestState rs = (RequestState)asyncResult.AsyncState;

			// Retrieve the ResponseStream that was set in RespCallback. 
			Stream responseStream = rs.ResponseStream;

			// Read rs.BufferRead to verify that it contains data. 
			int read = responseStream.EndRead(asyncResult);
			if (read > 0) {
				// Prepare a Char array buffer for converting to Unicode.
				byte[] byteBuffer = new byte[BUFFER_SIZE];
         
				rs.FileWriter.Write(rs.BufferRead, 0, rs.BufferRead.Length);
				if ((int)((long)MainForm.file_download.progressBarDownload.Tag / rs.FileWriter.Position) != MainForm.file_download.progressBarDownload.Value) {
					MainForm.file_download.progressBarDownload.Value = (int)((long)MainForm.file_download.progressBarDownload.Tag / rs.FileWriter.Position);
				}
				
				int file_index = MainForm.OpenDownloadList.FindUID(rs.DownloadFileUID);
				if (file_index != -1) {
					aDownloadItem current_item = (aDownloadItem)MainForm.OpenDownloadList.master_file_list[file_index];
					current_item.doneFileSize = rs.FileWriter.Position;
					MainForm.OpenDownloadList.master_file_list[file_index] = current_item;
				}
				
				IAsyncResult ar = responseStream.BeginRead(rs.BufferRead, 0, BUFFER_SIZE, new AsyncCallback(ReadCallBack), rs);
			}
			else {
				// Close down the response stream.
				responseStream.Close();         
				rs.FileWriter.Close();
				// Set the ManualResetEvent so the main thread can exit.
				allDone.Set();                           
			}
		}

		private void Progress_Load(object sender, System.EventArgs e) {
		
		}

		private void buttonCancel_Click(object sender, System.EventArgs e) {
		
		}

		private void buttonPause_Click(object sender, System.EventArgs e) {
		
		}
	}
}

// The RequestState class passes data across async calls.
public class RequestState {
	const int BufferSize = 1024;
	public byte[] BufferRead;
	public Int32 DownloadFileUID;
	public HttpWebRequest Request;
	public Stream ResponseStream;
	public FileStream FileWriter;
      
	public RequestState() {
		BufferRead = new byte[BufferSize];
		Request = null;
		ResponseStream = null;
		FileWriter = null;
	}     
}