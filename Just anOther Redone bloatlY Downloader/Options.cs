using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace WindowsApplication1
{
	/// <summary>
	/// Summary description for Options.
	/// </summary>
	public class formOptions : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPageDownloadFolder;
		private System.Windows.Forms.TabPage tabPageQueue;
		private System.Windows.Forms.Button buttonOK;
		private System.Windows.Forms.Button buttonCancel;
		private System.Windows.Forms.TabPage tabPageSpeed;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ComboBox comboBoxBufferSize;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.NumericUpDown numericUpDownBufferSize;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public formOptions()
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
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPageQueue = new System.Windows.Forms.TabPage();
			this.tabPageDownloadFolder = new System.Windows.Forms.TabPage();
			this.tabPageSpeed = new System.Windows.Forms.TabPage();
			this.label2 = new System.Windows.Forms.Label();
			this.numericUpDownBufferSize = new System.Windows.Forms.NumericUpDown();
			this.comboBoxBufferSize = new System.Windows.Forms.ComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.buttonOK = new System.Windows.Forms.Button();
			this.buttonCancel = new System.Windows.Forms.Button();
			this.tabControl1.SuspendLayout();
			this.tabPageSpeed.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownBufferSize)).BeginInit();
			this.SuspendLayout();
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.AddRange(new System.Windows.Forms.Control[] {
																																							this.tabPageSpeed,
																																							this.tabPageQueue,
																																							this.tabPageDownloadFolder});
			this.tabControl1.Location = new System.Drawing.Point(4, 4);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(284, 216);
			this.tabControl1.TabIndex = 0;
			// 
			// tabPageQueue
			// 
			this.tabPageQueue.Location = new System.Drawing.Point(4, 22);
			this.tabPageQueue.Name = "tabPageQueue";
			this.tabPageQueue.Size = new System.Drawing.Size(276, 190);
			this.tabPageQueue.TabIndex = 1;
			this.tabPageQueue.Text = "Queue";
			// 
			// tabPageDownloadFolder
			// 
			this.tabPageDownloadFolder.Location = new System.Drawing.Point(4, 22);
			this.tabPageDownloadFolder.Name = "tabPageDownloadFolder";
			this.tabPageDownloadFolder.Size = new System.Drawing.Size(276, 190);
			this.tabPageDownloadFolder.TabIndex = 0;
			this.tabPageDownloadFolder.Text = "Download Folder";
			// 
			// tabPageSpeed
			// 
			this.tabPageSpeed.Controls.AddRange(new System.Windows.Forms.Control[] {
																																							 this.label2,
																																							 this.numericUpDownBufferSize,
																																							 this.comboBoxBufferSize,
																																							 this.label1});
			this.tabPageSpeed.Location = new System.Drawing.Point(4, 22);
			this.tabPageSpeed.Name = "tabPageSpeed";
			this.tabPageSpeed.Size = new System.Drawing.Size(276, 190);
			this.tabPageSpeed.TabIndex = 2;
			this.tabPageSpeed.Text = "Speed";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(200, 36);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(36, 16);
			this.label2.TabIndex = 3;
			this.label2.Text = "bytes";
			// 
			// numericUpDownBufferSize
			// 
			this.numericUpDownBufferSize.Increment = new System.Decimal(new int[] {
																																							64,
																																							0,
																																							0,
																																							0});
			this.numericUpDownBufferSize.Location = new System.Drawing.Point(112, 32);
			this.numericUpDownBufferSize.Maximum = new System.Decimal(new int[] {
																																						2097152,
																																						0,
																																						0,
																																						0});
			this.numericUpDownBufferSize.Minimum = new System.Decimal(new int[] {
																																						1,
																																						0,
																																						0,
																																						0});
			this.numericUpDownBufferSize.Name = "numericUpDownBufferSize";
			this.numericUpDownBufferSize.Size = new System.Drawing.Size(84, 20);
			this.numericUpDownBufferSize.TabIndex = 2;
			this.numericUpDownBufferSize.ThousandsSeparator = true;
			this.numericUpDownBufferSize.Value = new System.Decimal(new int[] {
																																					1,
																																					0,
																																					0,
																																					0});
			this.numericUpDownBufferSize.ValueChanged += new System.EventHandler(this.numericUpDownBufferSize_ValueChanged);
			// 
			// comboBoxBufferSize
			// 
			this.comboBoxBufferSize.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBoxBufferSize.Items.AddRange(new object[] {
																														"Modem",
																														"Broadband",
																														"LAN",
																														"Custom"});
			this.comboBoxBufferSize.Location = new System.Drawing.Point(8, 32);
			this.comboBoxBufferSize.Name = "comboBoxBufferSize";
			this.comboBoxBufferSize.Size = new System.Drawing.Size(100, 21);
			this.comboBoxBufferSize.TabIndex = 1;
			this.comboBoxBufferSize.SelectedIndexChanged += new System.EventHandler(this.comboBoxBufferSize_SelectedIndexChanged);
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 12);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(252, 16);
			this.label1.TabIndex = 0;
			this.label1.Text = "Download Buffer Size";
			// 
			// buttonOK
			// 
			this.buttonOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.buttonOK.Location = new System.Drawing.Point(148, 224);
			this.buttonOK.Name = "buttonOK";
			this.buttonOK.Size = new System.Drawing.Size(64, 23);
			this.buttonOK.TabIndex = 1;
			this.buttonOK.Text = "&OK";
			// 
			// buttonCancel
			// 
			this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.buttonCancel.Location = new System.Drawing.Point(216, 224);
			this.buttonCancel.Name = "buttonCancel";
			this.buttonCancel.Size = new System.Drawing.Size(72, 23);
			this.buttonCancel.TabIndex = 2;
			this.buttonCancel.Text = "&Cancel";
			// 
			// formOptions
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 250);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																																	this.buttonCancel,
																																	this.buttonOK,
																																	this.tabControl1});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "formOptions";
			this.ShowInTaskbar = false;
			this.Text = "Options";
			this.Load += new System.EventHandler(this.formOptions_Load);
			this.tabControl1.ResumeLayout(false);
			this.tabPageSpeed.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownBufferSize)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		private void comboBoxBufferSize_SelectedIndexChanged(object sender, System.EventArgs e) {
			switch (comboBoxBufferSize.SelectedIndex) {
				case 0: //Modem
					numericUpDownBufferSize.Value = 1024; //1K Best value for a modem?
					break;
				case 1: //Broadband
					numericUpDownBufferSize.Value = 8192; //8K Best value for a Broadband?
					break;
				case 2: //LAN
					numericUpDownBufferSize.Value = 65536; //64K Best value for a LAN?
					break;
				case 3: //Custom
					break;
			}
		}

		private void numericUpDownBufferSize_ValueChanged(object sender, System.EventArgs e) {
			MainForm.OpenDownloadList.OpenDownloadOptions.download_buffer_size = (UInt32)numericUpDownBufferSize.Value;
		}

		private void formOptions_Load(object sender, System.EventArgs e) {
			try {
				comboBoxBufferSize.SelectedIndex = 3;
				numericUpDownBufferSize.Value = (int)MainForm.OpenDownloadList.OpenDownloadOptions.download_buffer_size;
			} catch(Exception exp) {
				//An Error happened when loading the default values
			}
		}

	}
}
