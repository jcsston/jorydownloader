using System;
using System.Threading;
using System.IO;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace WindowsApplication1
{
	/// <summary>
	/// Summary description for LoadURLList.
	/// </summary>
	public class formLoadURLList : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListBox listBoxURLList;
		private System.Windows.Forms.Label labelInputURLList;
		private System.Windows.Forms.Button buttonLoad;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label labelExample;
		private System.Windows.Forms.Button buttonAdd;
		private System.Windows.Forms.Button buttonCancel;
		private System.Windows.Forms.TextBox textBoxPrefix;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox textBoxPostfix;
		private System.Windows.Forms.ContextMenu contextMenuList;
		private System.Windows.Forms.MenuItem menuItemDeleteItem;
		private System.Windows.Forms.MenuItem menuItemClearList;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public formLoadURLList()
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
			this.listBoxURLList = new System.Windows.Forms.ListBox();
			this.labelInputURLList = new System.Windows.Forms.Label();
			this.buttonLoad = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.textBoxPrefix = new System.Windows.Forms.TextBox();
			this.labelExample = new System.Windows.Forms.Label();
			this.buttonAdd = new System.Windows.Forms.Button();
			this.buttonCancel = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.textBoxPostfix = new System.Windows.Forms.TextBox();
			this.contextMenuList = new System.Windows.Forms.ContextMenu();
			this.menuItemDeleteItem = new System.Windows.Forms.MenuItem();
			this.menuItemClearList = new System.Windows.Forms.MenuItem();
			this.SuspendLayout();
			// 
			// listBoxURLList
			// 
			this.listBoxURLList.ContextMenu = this.contextMenuList;
			this.listBoxURLList.HorizontalScrollbar = true;
			this.listBoxURLList.Location = new System.Drawing.Point(4, 56);
			this.listBoxURLList.Name = "listBoxURLList";
			this.listBoxURLList.Size = new System.Drawing.Size(292, 121);
			this.listBoxURLList.TabIndex = 5;
			this.listBoxURLList.SelectedIndexChanged += new System.EventHandler(this.listBoxURLList_SelectedIndexChanged);
			// 
			// labelInputURLList
			// 
			this.labelInputURLList.Location = new System.Drawing.Point(4, 4);
			this.labelInputURLList.Name = "labelInputURLList";
			this.labelInputURLList.Size = new System.Drawing.Size(292, 28);
			this.labelInputURLList.TabIndex = 1;
			this.labelInputURLList.Text = "Input File:";
			// 
			// buttonLoad
			// 
			this.buttonLoad.Location = new System.Drawing.Point(252, 28);
			this.buttonLoad.Name = "buttonLoad";
			this.buttonLoad.Size = new System.Drawing.Size(44, 23);
			this.buttonLoad.TabIndex = 1;
			this.buttonLoad.Text = "Load";
			this.buttonLoad.Click += new System.EventHandler(this.buttonLoad_Click);
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(4, 36);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(44, 16);
			this.label2.TabIndex = 3;
			this.label2.Text = "Prefix:";
			this.label2.TextAlign = System.Drawing.ContentAlignment.TopRight;
			// 
			// textBoxPrefix
			// 
			this.textBoxPrefix.Location = new System.Drawing.Point(48, 32);
			this.textBoxPrefix.Name = "textBoxPrefix";
			this.textBoxPrefix.Size = new System.Drawing.Size(76, 20);
			this.textBoxPrefix.TabIndex = 4;
			this.textBoxPrefix.Text = "";
			// 
			// labelExample
			// 
			this.labelExample.Location = new System.Drawing.Point(4, 180);
			this.labelExample.Name = "labelExample";
			this.labelExample.Size = new System.Drawing.Size(292, 16);
			this.labelExample.TabIndex = 5;
			this.labelExample.Text = "Example:";
			// 
			// buttonAdd
			// 
			this.buttonAdd.Location = new System.Drawing.Point(136, 200);
			this.buttonAdd.Name = "buttonAdd";
			this.buttonAdd.TabIndex = 6;
			this.buttonAdd.Text = "Add";
			this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
			// 
			// buttonCancel
			// 
			this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.buttonCancel.Location = new System.Drawing.Point(220, 200);
			this.buttonCancel.Name = "buttonCancel";
			this.buttonCancel.TabIndex = 7;
			this.buttonCancel.Text = "Cancel";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(128, 36);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(44, 16);
			this.label1.TabIndex = 8;
			this.label1.Text = "Postfix:";
			this.label1.TextAlign = System.Drawing.ContentAlignment.TopRight;
			// 
			// textBoxPostfix
			// 
			this.textBoxPostfix.Location = new System.Drawing.Point(172, 32);
			this.textBoxPostfix.Name = "textBoxPostfix";
			this.textBoxPostfix.Size = new System.Drawing.Size(76, 20);
			this.textBoxPostfix.TabIndex = 9;
			this.textBoxPostfix.Text = "";
			// 
			// contextMenuList
			// 
			this.contextMenuList.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																																										this.menuItemDeleteItem,
																																										this.menuItemClearList});
			// 
			// menuItemDeleteItem
			// 
			this.menuItemDeleteItem.Index = 0;
			this.menuItemDeleteItem.Text = "Delete Selected Item";
			this.menuItemDeleteItem.Click += new System.EventHandler(this.menuItemDeleteItem_Click);
			// 
			// menuItemClearList
			// 
			this.menuItemClearList.Index = 1;
			this.menuItemClearList.Text = "Clear List";
			this.menuItemClearList.Click += new System.EventHandler(this.menuItemClearList_Click);
			// 
			// formLoadURLList
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(300, 226);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																																	this.textBoxPostfix,
																																	this.label1,
																																	this.buttonCancel,
																																	this.buttonAdd,
																																	this.labelExample,
																																	this.textBoxPrefix,
																																	this.label2,
																																	this.buttonLoad,
																																	this.labelInputURLList,
																																	this.listBoxURLList});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "formLoadURLList";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Load URL List";
			this.ResumeLayout(false);

		}
		#endregion

		private void URLListParser () {
			String input_file = (String)buttonLoad.Tag;			
			StreamReader input_stream = File.OpenText(input_file);
			try {
				String URL_List;
				while (true) {
					URL_List = input_stream.ReadLine().Trim();
					if (URL_List.Length > 0)
						listBoxURLList.Items.Add(URL_List);
				}
			} catch(Exception exp) {
				//Do nothing as we likely hit the EOF
			}
			input_stream.Close();
			buttonLoad.Enabled = true;
		}

		private void buttonLoad_Click(object sender, System.EventArgs e) {
			OpenFileDialog new_url_list_file = new OpenFileDialog();
			new_url_list_file.Filter = "All Files (*.*)|*.*";
			if (new_url_list_file.ShowDialog(this) == DialogResult.OK) {
				buttonLoad.Tag = new_url_list_file.FileName;
				labelInputURLList.Text = "Input File: " + new_url_list_file.FileName;
				buttonLoad.Enabled = false;
				Thread listParser = new Thread(new ThreadStart(URLListParser));
				listParser.Start();
			}
		}

		private void listBoxURLList_SelectedIndexChanged(object sender, System.EventArgs e) {
			labelExample.Text = "Example: " + textBoxPrefix.Text.Trim() + (String)listBoxURLList.SelectedItem + textBoxPostfix.Text.Trim();
		}

		private void menuItemClearList_Click(object sender, System.EventArgs e) {
			listBoxURLList.Items.Clear();
		}

		private void menuItemDeleteItem_Click(object sender, System.EventArgs e) {
			listBoxURLList.Items.Remove(listBoxURLList.SelectedItem);
		}

		private void buttonAdd_Click(object sender, System.EventArgs e) {
			for(int i = 0; i < listBoxURLList.Items.Count; i++) {
				MainForm.OpenDownloadList.AddURL(textBoxPrefix.Text.Trim() + (String)listBoxURLList.Items[i] + textBoxPostfix.Text.Trim());
			}
			this.DialogResult = DialogResult.OK;
			this.Close();
		}
	}
}
