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
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.Label labelURL;
		private WindowsApplication1.NeatProgressBar neatProgressBarProgress;
		private System.Windows.Forms.Label labelSpeed;
		private System.Windows.Forms.Label labelSize;
		private System.Windows.Forms.TrackBar trackBarVisible;
		private System.Windows.Forms.CheckBox checkBoxOnTop;
		private System.Windows.Forms.Label labelTransparency;


		private int BUFFER_SIZE = 1024;
		private Thread theDownloadThread;
		private bool statPause = false;
		private bool statCancel = false;
		private avgSpeed theSpeed;

		private class avgSpeed : object {
			//
			public avgSpeed() {
				//
			}
			public UInt64 totalTime;
			public UInt32 totalSize;
		};

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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(Progress));
			this.buttonPause = new System.Windows.Forms.Button();
			this.buttonCancel = new System.Windows.Forms.Button();
			this.labelFile = new System.Windows.Forms.Label();
			this.neatProgressBarProgress = new WindowsApplication1.NeatProgressBar();
			this.labelURL = new System.Windows.Forms.Label();
			this.labelSpeed = new System.Windows.Forms.Label();
			this.labelSize = new System.Windows.Forms.Label();
			this.trackBarVisible = new System.Windows.Forms.TrackBar();
			this.labelTransparency = new System.Windows.Forms.Label();
			this.checkBoxOnTop = new System.Windows.Forms.CheckBox();
			((System.ComponentModel.ISupportInitialize)(this.trackBarVisible)).BeginInit();
			this.SuspendLayout();
			// 
			// buttonPause
			// 
			this.buttonPause.Location = new System.Drawing.Point(240, 100);
			this.buttonPause.Name = "buttonPause";
			this.buttonPause.Size = new System.Drawing.Size(64, 23);
			this.buttonPause.TabIndex = 1;
			this.buttonPause.Text = "Pause";
			this.buttonPause.Click += new System.EventHandler(this.buttonPause_Click);
			// 
			// buttonCancel
			// 
			this.buttonCancel.Location = new System.Drawing.Point(312, 100);
			this.buttonCancel.Name = "buttonCancel";
			this.buttonCancel.Size = new System.Drawing.Size(72, 23);
			this.buttonCancel.TabIndex = 2;
			this.buttonCancel.Text = "Cancel";
			this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
			// 
			// labelFile
			// 
			this.labelFile.Name = "labelFile";
			this.labelFile.Size = new System.Drawing.Size(352, 16);
			this.labelFile.TabIndex = 3;
			this.labelFile.Text = "Downloading File:";
			// 
			// neatProgressBarProgress
			// 
			this.neatProgressBarProgress.BackColor = System.Drawing.SystemColors.Control;
			this.neatProgressBarProgress.Location = new System.Drawing.Point(4, 68);
			this.neatProgressBarProgress.Name = "neatProgressBarProgress";
			this.neatProgressBarProgress.Size = new System.Drawing.Size(378, 28);
			this.neatProgressBarProgress.TabIndex = 5;
			this.neatProgressBarProgress.Value = 0;
			// 
			// labelURL
			// 
			this.labelURL.Location = new System.Drawing.Point(4, 32);
			this.labelURL.Name = "labelURL";
			this.labelURL.Size = new System.Drawing.Size(376, 16);
			this.labelURL.TabIndex = 6;
			this.labelURL.Text = "URL";
			// 
			// labelSpeed
			// 
			this.labelSpeed.Location = new System.Drawing.Point(296, 52);
			this.labelSpeed.Name = "labelSpeed";
			this.labelSpeed.Size = new System.Drawing.Size(88, 16);
			this.labelSpeed.TabIndex = 7;
			this.labelSpeed.Text = "200.0 KB/s";
			this.labelSpeed.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// labelSize
			// 
			this.labelSize.Location = new System.Drawing.Point(4, 52);
			this.labelSize.Name = "labelSize";
			this.labelSize.Size = new System.Drawing.Size(160, 16);
			this.labelSize.TabIndex = 8;
			this.labelSize.Text = "100KB of 960KB";
			// 
			// trackBarVisible
			// 
			this.trackBarVisible.Location = new System.Drawing.Point(72, 100);
			this.trackBarVisible.Maximum = 100;
			this.trackBarVisible.Minimum = 10;
			this.trackBarVisible.Name = "trackBarVisible";
			this.trackBarVisible.Size = new System.Drawing.Size(88, 34);
			this.trackBarVisible.TabIndex = 9;
			this.trackBarVisible.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBarVisible.Value = 100;
			this.trackBarVisible.ValueChanged += new System.EventHandler(this.trackBarVisible_ValueChanged);
			// 
			// labelTransparency
			// 
			this.labelTransparency.Location = new System.Drawing.Point(0, 100);
			this.labelTransparency.Name = "labelTransparency";
			this.labelTransparency.Size = new System.Drawing.Size(80, 23);
			this.labelTransparency.TabIndex = 10;
			this.labelTransparency.Text = "Transparency:";
			this.labelTransparency.TextAlign = System.Drawing.ContentAlignment.TopRight;
			// 
			// checkBoxOnTop
			// 
			this.checkBoxOnTop.Location = new System.Drawing.Point(168, 100);
			this.checkBoxOnTop.Name = "checkBoxOnTop";
			this.checkBoxOnTop.Size = new System.Drawing.Size(72, 24);
			this.checkBoxOnTop.TabIndex = 11;
			this.checkBoxOnTop.Text = "On Top";
			this.checkBoxOnTop.CheckedChanged += new System.EventHandler(this.checkBoxOnTop_CheckedChanged);
			// 
			// Progress
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(384, 125);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																																	this.checkBoxOnTop,
																																	this.labelTransparency,
																																	this.trackBarVisible,
																																	this.labelSize,
																																	this.labelSpeed,
																																	this.labelURL,
																																	this.neatProgressBarProgress,
																																	this.labelFile,
																																	this.buttonCancel,
																																	this.buttonPause});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "Progress";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Progress";
			this.Load += new System.EventHandler(this.Progress_Load);
			this.Closed += new System.EventHandler(this.Progress_Closed);
			((System.ComponentModel.ISupportInitialize)(this.trackBarVisible)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		public void DownloadFile(aDownloadItem download_item) {
			//Store the download item
			this.Tag = download_item;
			theSpeed = new avgSpeed();
			//Start the real downloading
			theDownloadThread = new Thread(new ThreadStart(DownloadThread));
			theDownloadThread.Name = "File Download";
			theDownloadThread.Start();
			//Add this download to the queue
			MainForm.OpenDownloadList.downloads_in_progress++;
		}

		public void DownloadThread(){
			aDownloadItem download_item = (aDownloadItem)this.Tag;
			labelURL.Text = download_item.RemoteURL;
			labelFile.Text = download_item.LocalFilename;

			HttpWebRequest new_url = (HttpWebRequest)WebRequest.Create(download_item.RemoteURL);
			new_url.AllowAutoRedirect = true;			
						
			//Create the FileStream object			
			FileStream FileWriter = new FileStream(download_item.LocalFilename, FileMode.OpenOrCreate);
			FileWriter.Seek(0, SeekOrigin.End);
			
			//Cut off the last 1K for resuming
			download_item.doneFileSize = FileWriter.Length;
			if ((download_item.doneFileSize > 1000) && (download_item.totalFileSize > 1000)) {
				FileWriter.Seek(download_item.doneFileSize - 1000, SeekOrigin.Begin);
			}else {
				FileWriter.Seek(download_item.doneFileSize, SeekOrigin.Begin);
			}
									
			//Resuming?
			new_url.AddRange((int)FileWriter.Position);			

			HttpWebResponse url_response = (HttpWebResponse)new_url.GetResponse();
			//Setup the progress bar
			neatProgressBarProgress.Value = (int)((FileWriter.Position+1) / (download_item.totalFileSize+1));

			Stream ResponseStream = url_response.GetResponseStream();
			byte[] ByteBuffer = new byte[BUFFER_SIZE];

			int read_amount = 1;
			int start_ticks = Environment.TickCount;
			//Read the BUFFER_SIZE
			read_amount = ResponseStream.Read(ByteBuffer, 0, BUFFER_SIZE);
			this.theSpeed.totalTime += (UInt64)(Environment.TickCount - start_ticks);
			this.theSpeed.totalSize += (UInt32)read_amount;
			while (read_amount > 0) {				
				//Write to the file
				FileWriter.Write(ByteBuffer, 0, read_amount);
				
				//Update Progress Bar
				if ((int)(100 / (float)download_item.totalFileSize * (float)FileWriter.Position) != this.neatProgressBarProgress.Value) {
					this.neatProgressBarProgress.Value = (int)(100 / (float)download_item.totalFileSize * (float)FileWriter.Position);
				}
				//Update the Size Label
				this.labelSize.Text = ((int)(FileWriter.Position/1024)).ToString() + "KB of " + ((int)(download_item.totalFileSize/1024)).ToString() + "KB";
				
				//Update the Download Item with the completed amount
				Int32 DownloadFileUID = ((aDownloadItem)this.Tag).FileUID;
				int file_index = MainForm.OpenDownloadList.FindUID(DownloadFileUID);
				if (file_index != -1) {
					MainForm.OpenDownloadList.GetDownloadListItem(file_index).doneFileSize = FileWriter.Position;;
					MainForm.OpenDownloadList.changed = true;
				}
				this.theSpeed.totalTime += (UInt64)(Environment.TickCount - start_ticks);
				this.theSpeed.totalSize += (UInt32)read_amount;
				//this.avgSpeed = (avgSpeed + (((float)BUFFER_SIZE / (float)((Environment.TickCount+1) - start_ticks)))) / 2;
				//this.labelSpeed.Text = avgSpeed.ToString() + "KB/s";
				//We check here if the user paused or canceled the download
				if (statPause) {
					while (statPause && !statCancel) {
						Thread.Sleep(500);
					}
				}
				if (statCancel)
					break;
				
				start_ticks = Environment.TickCount;
				//Read the data for the next loop				
				read_amount = ResponseStream.Read(ByteBuffer, 0, BUFFER_SIZE);
			}
			//We are done :)
			//Take this download out of the queue
			MainForm.OpenDownloadList.downloads_in_progress--;
			//Close down the streams
			ResponseStream.Close();         
			FileWriter.Close();
			this.Close();
		}

		private void FadeInProgressWindow() {
			for (float i = 0; i <= 1; i += 0.1F) {
				this.Opacity = i;
				Thread.Sleep(10);
			}
		}

		private void buttonCancel_Click(object sender, System.EventArgs e) {
			statCancel = true;
		}

		private void buttonPause_Click(object sender, System.EventArgs e) {
			if (buttonPause.Text == "Pause") {
				statPause = true;
				buttonPause.Text = "Resume";
			}else if (buttonPause.Text == "Resume") {
				statPause = false;
				buttonPause.Text = "Pause";
			}
		}

		private void Progress_Closed(object sender, System.EventArgs e) {
			statCancel = true;
		}

		private void Progress_Load(object sender, System.EventArgs e) {
			this.labelFile.Text = "";
			this.labelSize.Text = "";
			this.labelSpeed.Text = "";
			this.labelURL.Text = "";
			this.neatProgressBarProgress.Value = 0;
			/*if (Environment.OSVersion.Platform == Win32NT) {
				//Ok we show the
			}else {
				this.trackBarVisible.Visible = false;
				this.labelTransparency.Visible = false;
			}*/
			BUFFER_SIZE = (int)MainForm.OpenDownloadList.OpenDownloadOptions.download_buffer_size;
		}

		private void checkBoxOnTop_CheckedChanged(object sender, System.EventArgs e) {
			this.TopMost = checkBoxOnTop.Checked;
		}

		private void trackBarVisible_ValueChanged(object sender, System.EventArgs e) {
			this.Opacity = (float)trackBarVisible.Value / 100;
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