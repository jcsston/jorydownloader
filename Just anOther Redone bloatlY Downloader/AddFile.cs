using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace WindowsApplication1
{
	/// <summary>
	/// Summary description for AddFile.
	/// </summary>
	public class formAddFile : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox textBoxURL;
		private System.Windows.Forms.Button buttonDownload;
		private System.Windows.Forms.Label label1;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public formAddFile()
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
			this.textBoxURL = new System.Windows.Forms.TextBox();
			this.buttonDownload = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// textBoxURL
			// 
			this.textBoxURL.Location = new System.Drawing.Point(8, 24);
			this.textBoxURL.Name = "textBoxURL";
			this.textBoxURL.Size = new System.Drawing.Size(304, 20);
			this.textBoxURL.TabIndex = 0;
			this.textBoxURL.Text = "";
			// 
			// buttonDownload
			// 
			this.buttonDownload.Location = new System.Drawing.Point(240, 48);
			this.buttonDownload.Name = "buttonDownload";
			this.buttonDownload.TabIndex = 1;
			this.buttonDownload.Text = "Download";
			this.buttonDownload.Click += new System.EventHandler(this.buttonDownload_Click);
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(304, 16);
			this.label1.TabIndex = 2;
			this.label1.Text = "Enter URL to Download:";
			// 
			// formAddFile
			// 
			this.AcceptButton = this.buttonDownload;
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(320, 77);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.label1,
																		  this.buttonDownload,
																		  this.textBoxURL});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "formAddFile";
			this.ShowInTaskbar = false;
			this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Add File";
			this.TopMost = true;
			this.Load += new System.EventHandler(this.formAddFile_Load);
			this.ResumeLayout(false);

		}
		#endregion

		private void buttonDownload_Click(object sender, System.EventArgs e)
		{
			file_list return_value = new file_list();
			return_value.SetData(textBoxURL.Text);
			this.Tag = return_value;
			this.DialogResult = DialogResult.OK;
		}

		private void formAddFile_Load(object sender, System.EventArgs e)
		{
			
		}
	}
}
