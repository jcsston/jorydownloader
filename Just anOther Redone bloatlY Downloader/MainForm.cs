using System;
using System.Net;
using System.IO;
using System.Xml;
using System.Threading;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace WindowsApplication1 {
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form {
		private System.Windows.Forms.ListView listViewDownloadList;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.StatusBar statusBarMain;
		private System.Windows.Forms.StatusBarPanel statusBarPanelCount;
		private System.Windows.Forms.StatusBarPanel statusBarPanelTotalSize;
		private System.Windows.Forms.StatusBarPanel statusBarPanelTotalTime;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem menuItemFile;
		private System.Windows.Forms.MenuItem menuItemFileAddFile;
		private System.Windows.Forms.MenuItem menuItemmenuItemFileExit;
		private System.Windows.Forms.TreeView treeViewCategories;
		private System.Windows.Forms.MenuItem menuItemFilePaste;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.Button buttonDownload;
		
		static public theDownloadList OpenDownloadList;
		static public Progress file_download;
		private Thread XMLParserThread;
		private Thread theMonitorListUpdateThread;
		private Thread theURLListMonitorThread;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public MainForm() {
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
		protected override void Dispose( bool disposing ) {
			if( disposing ) {
				if (components != null) {
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
		private void InitializeComponent() {
			this.listViewDownloadList = new System.Windows.Forms.ListView();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
			this.statusBarMain = new System.Windows.Forms.StatusBar();
			this.statusBarPanelCount = new System.Windows.Forms.StatusBarPanel();
			this.statusBarPanelTotalSize = new System.Windows.Forms.StatusBarPanel();
			this.statusBarPanelTotalTime = new System.Windows.Forms.StatusBarPanel();
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.menuItemFile = new System.Windows.Forms.MenuItem();
			this.menuItemFileAddFile = new System.Windows.Forms.MenuItem();
			this.menuItemFilePaste = new System.Windows.Forms.MenuItem();
			this.menuItem2 = new System.Windows.Forms.MenuItem();
			this.menuItemmenuItemFileExit = new System.Windows.Forms.MenuItem();
			this.treeViewCategories = new System.Windows.Forms.TreeView();
			this.buttonDownload = new System.Windows.Forms.Button();
			this.columnHeader6 = new System.Windows.Forms.ColumnHeader();
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanelCount)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanelTotalSize)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanelTotalTime)).BeginInit();
			this.SuspendLayout();
			// 
			// listViewDownloadList
			// 
			this.listViewDownloadList.AllowColumnReorder = true;
			this.listViewDownloadList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																																													 this.columnHeader1,
																																													 this.columnHeader2,
																																													 this.columnHeader3,
																																													 this.columnHeader4,
																																													 this.columnHeader5,
																																													 this.columnHeader6});
			this.listViewDownloadList.Cursor = System.Windows.Forms.Cursors.Default;
			this.listViewDownloadList.FullRowSelect = true;
			this.listViewDownloadList.GridLines = true;
			this.listViewDownloadList.Location = new System.Drawing.Point(136, 48);
			this.listViewDownloadList.Name = "listViewDownloadList";
			this.listViewDownloadList.Size = new System.Drawing.Size(384, 208);
			this.listViewDownloadList.TabIndex = 0;
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Filename";
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Done";
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Size";
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "Time Left";
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "URL";
			// 
			// statusBarMain
			// 
			this.statusBarMain.Location = new System.Drawing.Point(0, 259);
			this.statusBarMain.Name = "statusBarMain";
			this.statusBarMain.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
																																										 this.statusBarPanelCount,
																																										 this.statusBarPanelTotalSize,
																																										 this.statusBarPanelTotalTime});
			this.statusBarMain.ShowPanels = true;
			this.statusBarMain.Size = new System.Drawing.Size(520, 22);
			this.statusBarMain.TabIndex = 1;
			// 
			// statusBarPanelCount
			// 
			this.statusBarPanelCount.AutoSize = System.Windows.Forms.StatusBarPanelAutoSize.Spring;
			this.statusBarPanelCount.BorderStyle = System.Windows.Forms.StatusBarPanelBorderStyle.Raised;
			this.statusBarPanelCount.Text = "statusBarPanelCount";
			this.statusBarPanelCount.Width = 304;
			// 
			// statusBarPanelTotalSize
			// 
			this.statusBarPanelTotalSize.Alignment = System.Windows.Forms.HorizontalAlignment.Right;
			this.statusBarPanelTotalSize.Text = "statusBarPanelTotalSize";
			// 
			// statusBarPanelTotalTime
			// 
			this.statusBarPanelTotalTime.Alignment = System.Windows.Forms.HorizontalAlignment.Right;
			this.statusBarPanelTotalTime.Text = "statusBarPanel1";
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																																							this.menuItemFile});
			// 
			// menuItemFile
			// 
			this.menuItemFile.Index = 0;
			this.menuItemFile.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																																								 this.menuItemFileAddFile,
																																								 this.menuItemFilePaste,
																																								 this.menuItem2,
																																								 this.menuItemmenuItemFileExit});
			this.menuItemFile.Text = "&File";
			this.menuItemFile.Click += new System.EventHandler(this.menuItemFile_Click);
			// 
			// menuItemFileAddFile
			// 
			this.menuItemFileAddFile.Index = 0;
			this.menuItemFileAddFile.Text = "Add File";
			this.menuItemFileAddFile.Click += new System.EventHandler(this.menuItemFileAddFile_Click);
			// 
			// menuItemFilePaste
			// 
			this.menuItemFilePaste.Index = 1;
			this.menuItemFilePaste.Text = "Paste";
			this.menuItemFilePaste.Click += new System.EventHandler(this.menuItemFilePaste_Click);
			// 
			// menuItem2
			// 
			this.menuItem2.Index = 2;
			this.menuItem2.Text = "-";
			// 
			// menuItemmenuItemFileExit
			// 
			this.menuItemmenuItemFileExit.Index = 3;
			this.menuItemmenuItemFileExit.Text = "E&xit";
			this.menuItemmenuItemFileExit.Click += new System.EventHandler(this.menuItemmenuItemFileExit_Click);
			// 
			// treeViewCategories
			// 
			this.treeViewCategories.ImageIndex = -1;
			this.treeViewCategories.Location = new System.Drawing.Point(0, 48);
			this.treeViewCategories.Name = "treeViewCategories";
			this.treeViewCategories.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
																																									 new System.Windows.Forms.TreeNode("All Categories"),
																																									 new System.Windows.Forms.TreeNode("Download Queue")});
			this.treeViewCategories.SelectedImageIndex = -1;
			this.treeViewCategories.Size = new System.Drawing.Size(128, 208);
			this.treeViewCategories.TabIndex = 2;
			this.treeViewCategories.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.treeViewCategories_AfterSelect);
			// 
			// buttonDownload
			// 
			this.buttonDownload.Location = new System.Drawing.Point(136, 16);
			this.buttonDownload.Name = "buttonDownload";
			this.buttonDownload.TabIndex = 3;
			this.buttonDownload.Text = "Download";
			this.buttonDownload.Click += new System.EventHandler(this.buttonDownload_Click);
			// 
			// columnHeader6
			// 
			this.columnHeader6.Text = "UID";
			// 
			// MainForm
			// 
			this.AllowDrop = true;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(520, 281);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																																	this.buttonDownload,
																																	this.treeViewCategories,
																																	this.statusBarMain,
																																	this.listViewDownloadList});
			this.Menu = this.mainMenu1;
			this.Name = "MainForm";
			this.Text = "Just anOther Redone bloatlY Downloader";
			this.Closing += new System.ComponentModel.CancelEventHandler(this.Form1_Closing);
			this.Load += new System.EventHandler(this.Form1_Load);
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanelCount)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanelTotalSize)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.statusBarPanelTotalTime)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() {
			try {
				Application.Run(new MainForm());
			}catch(Exception exp) {				
				MessageBox.Show("Error " + exp.Message + "\n" + exp.StackTrace);
			}
		}

		public void LoadDownloadList() {
			if (!File.Exists("OpenDownload.xml"))
				return;
			try {
				XmlDocument doc = new XmlDocument();				
				doc.Load("OpenDownload.xml");
				XmlNodeList nList0;
				XmlNodeList nList1;
				XmlNodeList nList2;
				XmlNodeList nList3;
				XmlNodeList nList4;
				nList0 = doc.GetElementsByTagName("OpenDownload");
				for(int level1 = 0; level1 < nList0.Count; level1++) {
					XmlElement element_level1 = (XmlElement)nList0.Item(level1);
					nList1 = element_level1.ChildNodes ;					
					for(int level2 = 0; level2 < nList1.Count; level2++) {
						XmlElement element_level2 = (XmlElement)nList1.Item(level2);
						nList2 = element_level2.ChildNodes;
						
						if (element_level2.Name == "DownloadList") {							
							//Read the Download File List
							if (element_level2.HasAttribute("Count")) {
								OpenDownloadList.master_file_list.Capacity = XmlConvert.ToInt16(element_level2.GetAttribute("Count"));
							}
							for(int level3 = 0; level3 < nList2.Count; level3++) {
								XmlElement element_level3 = (XmlElement)nList2.Item(level3);
								nList3 = element_level3.ChildNodes;
								
								if (element_level3.Name == "DownloadItem") {
									aDownloadItem read_item = new aDownloadItem();
									for(int level4 = 0; level4 < nList3.Count; level4++) {
										XmlElement element_level4 = (XmlElement)nList3.Item(level4);
										nList4 = element_level4.ChildNodes;
										
										if (element_level4.Name == "RemoteURL") {
											read_item.RemoteURL = element_level4.InnerText;
										}
										else if (element_level4.Name == "LocalFilename") {
											read_item.LocalFilename = element_level4.InnerText;
										}
										else if (element_level4.Name == "DoneFileSize") {
											read_item.doneFileSize = XmlConvert.ToInt32(element_level4.InnerText);
										}
										else if (element_level4.Name == "TotalFileSize") {
											read_item.totalFileSize = XmlConvert.ToInt32(element_level4.InnerText);
										}
										else if (element_level4.Name == "FileUID") {
											read_item.FileUID = XmlConvert.ToInt32(element_level4.InnerText);
										}										
									}
									if (read_item.FileUID == 0) {
										Random uid_creator = new Random();
										read_item.FileUID = uid_creator.Next(Int32.MaxValue);
									}
									OpenDownloadList.master_file_list.Add(read_item);
									OpenDownloadList.changed = true;
								}else {
									//We don't parse this element
								}
							}													
						} else if(element_level2.Name == "Options") {
							//Read our Options saved as XML
							for(int level3 = 0; level3 < nList2.Count; level3++) {
								XmlElement element_level3 = (XmlElement)nList2.Item(level3);
								nList3 = element_level3.ChildNodes;
								
								if (element_level3.Name == "DownloadListColumn") {																		
									int currentColumn = 0;										
									for(int level4 = 0; level4 < nList3.Count; level4++) {
										XmlElement element_level4 = (XmlElement)nList3.Item(level4);
										nList4 = element_level4.ChildNodes;
											
										if (element_level4.Name == "Width") {
											listViewDownloadList.Columns[currentColumn].Width = XmlConvert.ToInt16(element_level4.InnerText);
											currentColumn++;
										}
										//We don't what to try and set widths for colums that don't exist
										if (currentColumn > listViewDownloadList.Columns.Count)
											break;
									}
								}				
							}																				
						}
					}
				}
			}catch(Exception exp) {				
				//if (exp.
				MessageBox.Show("Error while loading XML Filelist " + exp.Message);
			}
			OpenDownloadList.changed = true;
		}

		public void SaveDownloadList() {
			try {
				XmlTextWriter data_save = new XmlTextWriter("OpenDownload.xml", System.Text.Encoding.UTF8);
				data_save.Formatting = Formatting.Indented;
				data_save.WriteStartDocument();
				data_save.WriteComment("Open Download " + Application.ProductVersion + " Download List");			
				data_save.WriteComment("Written on " + DateTime.Now.ToLongDateString() + " at " + DateTime.Now.ToLongTimeString());
				data_save.WriteComment("Some infomation on the layout of this file");
				data_save.WriteComment("The Count of items isn't required, but");
				data_save.WriteComment("it allows faster loading (PreAllocating the ArrayList)");

				data_save.WriteStartElement("OpenDownload");
				data_save.WriteStartElement("DownloadList");
				data_save.WriteAttributeString("Count", OpenDownloadList.master_file_list.Count.ToString());			
				for(int i = 0; i < OpenDownloadList.master_file_list.Count; i++) {
					aDownloadItem current_item = (aDownloadItem)OpenDownloadList.master_file_list[i];
					data_save.WriteStartElement("DownloadItem");
					data_save.WriteElementString("FileUID", "", current_item.FileUID.ToString());
					data_save.WriteElementString("RemoteURL", "", current_item.RemoteURL);
					data_save.WriteElementString("LocalFilename", "", current_item.LocalFilename);
					data_save.WriteElementString("DoneFileSize", "", current_item.doneFileSize.ToString());
					data_save.WriteElementString("TotalFileSize", "", current_item.totalFileSize.ToString());
					data_save.WriteEndElement();
				}
				data_save.WriteEndElement();
				//Save the current User settings
				data_save.WriteStartElement("Options");
				data_save.WriteStartElement("DownloadListColumn");
				data_save.WriteAttributeString("Count", listViewDownloadList.Columns.Count.ToString());
				for(int i = 0; i < listViewDownloadList.Columns.Count; i++) {
					data_save.WriteElementString("Width", listViewDownloadList.Columns[i].Width.ToString());
				}
				data_save.WriteEndElement();
				data_save.WriteEndElement();
				data_save.WriteEndElement();
				data_save.Close();
			}catch(Exception exp) {
				MessageBox.Show("Error while saving the XML Filelist :" + exp.Message);
			}
		}

		public void UpdateDownloadList() {
			//listViewDownloadList.Items.Clear();			
			for(int i = 0; i < OpenDownloadList.master_file_list.Count; i++) {
				aDownloadItem current_item = (aDownloadItem)OpenDownloadList.master_file_list[i];
								
				ListViewItem all_files_item = new ListViewItem(new Random().Next().ToString());
				//Fill out the sub-items with blanks
				for (int b = 0; b < listViewDownloadList.Columns.Count; b++)
					all_files_item.SubItems.Add("");

				//Search the list to find the columns
				int filename_column = 0;
				int percent_column = 0;
				int totalsize_column = 0;
				int timeleft_column = 0;
				int remoteURL_column = 0;
				int UID_column = 0;
				for(int c = 0; c < listViewDownloadList.Columns.Count; c++) {
					if (listViewDownloadList.Columns[c].Text == "Filename") {
						filename_column = c;
					} else if (listViewDownloadList.Columns[c].Text == "Done") {
						percent_column = c;
					} else if (listViewDownloadList.Columns[c].Text == "Size") {
						totalsize_column = c;
					} else if (listViewDownloadList.Columns[c].Text == "Time Left") {
						timeleft_column = c;
					} else if (listViewDownloadList.Columns[c].Text == "URL") {
						remoteURL_column = c;
					} else if (listViewDownloadList.Columns[c].Text == "UID") {
						//yep, we found the UID column
						UID_column = c;
					}
				}
				//Now search the column rows for a matching UID
				bool found_existing_item = false;
				int item_row = 0;
				for(item_row = 0; item_row < listViewDownloadList.Items.Count; item_row++) {
					if (listViewDownloadList.Items[item_row].SubItems[UID_column].Text == current_item.FileUID.ToString()) {
						//We found it :)
						found_existing_item = true;
						break;
					}					
				}
				//If we didn't find the item already in the list
				if (!found_existing_item) {
					//Add it
					listViewDownloadList.Items.Add(all_files_item);
					item_row = listViewDownloadList.Items.IndexOf(all_files_item);
				}				
				//Empty this to prevent usage
				all_files_item = null;
				
				//Now we set all the values of the Item/Row
				listViewDownloadList.Items[item_row].Text = current_item.LocalFilename;
				//Add the Percent done
				float percent_done;
				percent_done = (float)(100 / (float)current_item.totalFileSize * (float)current_item.doneFileSize);
				if (listViewDownloadList.Items[item_row].SubItems[percent_column].Text != percent_done.ToString("#0.##'%'"))
					listViewDownloadList.Items[item_row].SubItems[percent_column].Text = percent_done.ToString("#0.##'%'");
				
				if (listViewDownloadList.Items[item_row].SubItems[totalsize_column].Text != current_item.totalFileSize.ToString())
					listViewDownloadList.Items[item_row].SubItems[totalsize_column].Text = current_item.totalFileSize.ToString();
				
				if (listViewDownloadList.Items[item_row].SubItems[timeleft_column].Text != "")
					listViewDownloadList.Items[item_row].SubItems[timeleft_column].Text = "";
				
				if (listViewDownloadList.Items[item_row].SubItems[remoteURL_column].Text != current_item.RemoteURL)
					listViewDownloadList.Items[item_row].SubItems[remoteURL_column].Text = current_item.RemoteURL;
				
				if (listViewDownloadList.Items[item_row].SubItems[UID_column].Text != current_item.FileUID.ToString())
					listViewDownloadList.Items[item_row].SubItems[UID_column].Text = current_item.FileUID.ToString();
				
				listViewDownloadList.Items[item_row].Tag = (object)current_item.FileUID;
			}
		}

		public void MonitorListUpdateThread() {
			while (true) {
				if (OpenDownloadList.changed) {
					UpdateDownloadList();
					OpenDownloadList.changed = false;
				}
				Thread.Sleep(500);
			}
		}
		private void Form1_Load(object sender, System.EventArgs e)
		{
			listViewDownloadList.View = View.Details;
			OpenDownloadList = new theDownloadList();
			
			XMLParserThread = new Thread(new ThreadStart(LoadDownloadList));
			XMLParserThread.Name = "XML Filelist Parser";
			XMLParserThread.Priority = ThreadPriority.Lowest;
			XMLParserThread.Start();
			
			theMonitorListUpdateThread = new Thread(new ThreadStart(MonitorListUpdateThread));			
			theMonitorListUpdateThread.Name = "List Update";
			theMonitorListUpdateThread.IsBackground = true;
			theMonitorListUpdateThread.Start();			
			
			theURLListMonitorThread = new Thread(new ThreadStart(OpenDownloadList.URLListMonitor));
			theURLListMonitorThread.Name = "Background Add URL Monitor";
			theURLListMonitorThread.Priority = ThreadPriority.BelowNormal;
			theURLListMonitorThread.IsBackground = true;
			theURLListMonitorThread.Start();
		}

		private void menuItemFile_Click(object sender, System.EventArgs e)
		{
		
		}

		private void menuItemFileAddFile_Click(object sender, System.EventArgs e)
		{
			Form new_file = new formAddFile();
			new_file.ShowDialog(formAddFile.ActiveForm);
			if (new_file.DialogResult == DialogResult.OK)
			{
				file_list file_list = (file_list)new_file.Tag;
				OpenDownloadList.AddURL(file_list.GetData());
			}
			UpdateDownloadList();
		}

		private void menuItemmenuItemFileExit_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		private void menuItemFilePaste_Click(object sender, System.EventArgs e)
		{
			//IDataObject clipboard_data = Clipboard.GetDataObject();
			//String clipboard_url = (String)clipboard_data.GetData(DataFormats.Text);
			String[] url_list = {"http://192.168.0.1/images/xvid_vorbis_mkv.jpg", "http://192.168.0.1/images/xvid_interlaced.jpg"};
			
			for(int i = 0; i < 10; i++) {
				OpenDownloadList.AddURL(url_list);
			}
		}

		private void treeViewCategories_AfterSelect(object sender, System.Windows.Forms.TreeViewEventArgs e)
		{
			if (e.Node.Text == "All Categories")
			{
				//Display All Categories
				UpdateDownloadList();
			}
		}

		private void Form1_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			SaveDownloadList();
		}

		private void buttonDownload_Click(object sender, System.EventArgs e) {
			file_download = new Progress();						
			file_download.Show();
			int selected_master_index = OpenDownloadList.FindUID((Int32)listViewDownloadList.SelectedItems[0].Tag);
			file_download.DownloadFile(OpenDownloadList.GetDownloadListItem(selected_master_index));
		}
	}
}

public class file_list : Object
{
	public file_list()
	{
		data = new String[1];
	}
	public file_list(int item_count)
	{
		data = new String[item_count];
	}
	public void SetData(String[] file_list)
	{
		data = file_list;
	}

	public void SetData(String single_file)
	{
		data[0] = single_file;
	}

	public String[] GetData()
	{
		return data;
	}

	String[] data;
};	

public class aDownloadItem : Object
{
	public aDownloadItem()
	{
		//
	}

	public aDownloadItem(Int32 new_UID) {
		this.FileUID = new_UID;
	}

	public Int32 FileUID; //Used to keep files named the same apart
	public String RemoteURL;
	public String LocalFilename;
	public long totalFileSize;
	public long doneFileSize;
};

public class theDownloadList
{
	public theDownloadList()
	{
		master_file_list = new ArrayList(0);
		waiting_url_list = new ArrayList(0);
	}

	public void AddURL(String one_url)
	{		
		this.waiting_url_list.Add(one_url);
	}	

	public void AddURL(String[] url_list)
	{
		//Pre-Alloc the URL List Array
		this.waiting_url_list.Capacity = this.waiting_url_list.Count + (url_list.GetUpperBound(0) - url_list.GetLowerBound(0));
		for (int i = url_list.GetLowerBound(0); i <= url_list.GetUpperBound(0); i++)
		{
			this.waiting_url_list.Add((String)url_list[i]);
		}
	}

	private void RealAddURL(String one_url) {
		aDownloadItem another_file_to_download = new aDownloadItem();		
		another_file_to_download.FileUID = CreateUID();
		
		HttpWebRequest new_url = (HttpWebRequest)WebRequest.Create(one_url);						
		new_url.AllowAutoRedirect = true;
		
		//HttpWebResponse resp = (HttpWebResponse)req.EndGetResponse(ar);         
		
		HttpWebResponse url_response = (HttpWebResponse)new_url.GetResponse();
		another_file_to_download.RemoteURL = url_response.ResponseUri.AbsoluteUri;
		another_file_to_download.LocalFilename = "" + url_response.ResponseUri.Segments[url_response.ResponseUri.Segments.GetUpperBound(0)];
		another_file_to_download.totalFileSize = url_response.ContentLength;
		
				
		//Add this one to the list
		this.master_file_list.Add(another_file_to_download);
		this.changed = true;
		
		url_response.Close();
	}

	public void URLListMonitor() {
		while (true) {
			if (waiting_url_list.Count > 0) {
				String url_to_process = (String)waiting_url_list[0];
				RealAddURL(url_to_process);
				waiting_url_list.RemoveAt(0);
			}
			Thread.Sleep(500);
		}
	}
	public Int32 CreateUID() {
		Random uid_creator = new Random();		
		Int32 FileUID = uid_creator.Next(Int32.MaxValue); 
		//Make sure the UID isn't already in the list
		while (FindUID(FileUID) != -1)
			FileUID = uid_creator.Next(Int32.MaxValue); 
		//Return the UID
		return FileUID;
	}

	public int FindUID(Int32 UID_to_find) {
		int i = 0;
		aDownloadItem compare = null;
		while (i < master_file_list.Count) {
			compare = (aDownloadItem)master_file_list[i];
			if (compare.FileUID == UID_to_find)
				return i;
			i++;
		}
		return -1;
	}

	public aDownloadItem GetDownloadListItem(int index) {
		return (aDownloadItem)this.master_file_list[index];
	}

	public aDownloadItem GetDownloadListItemByUID(Int32 UID_number) {
		int index = FindUID(UID_number);
		if (index != -1) {
			return (aDownloadItem)this.master_file_list[index];
		}else {
			return new aDownloadItem(UID_number);
		}
	}

	public int SetDownloadListItem(int index, aDownloadItem replace_item) {
		try {
			this.master_file_list[index] = replace_item;
		}catch(Exception exp) {
			MessageBox.Show("Error trying to replace one of the Download List Items\n" + "Item Index: " + index.ToString() + "\n" + "Error Message: " + exp.Message);
		}
		return 0;
	}

	public bool changed = false;
	public ArrayList waiting_url_list;
	public ArrayList master_file_list;
};