using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace WindowsApplication1
{
	/// <summary>
	/// Summary description for Leech.
	/// </summary>
	public class formLeech : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button buttonLeech;
		private System.Windows.Forms.Button buttonCancel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public formLeech()
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
			this.buttonLeech = new System.Windows.Forms.Button();
			this.buttonCancel = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// buttonLeech
			// 
			this.buttonLeech.Location = new System.Drawing.Point(256, 292);
			this.buttonLeech.Name = "buttonLeech";
			this.buttonLeech.Size = new System.Drawing.Size(68, 20);
			this.buttonLeech.TabIndex = 0;
			this.buttonLeech.Text = "Leech";
			// 
			// buttonCancel
			// 
			this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.buttonCancel.Location = new System.Drawing.Point(328, 292);
			this.buttonCancel.Name = "buttonCancel";
			this.buttonCancel.Size = new System.Drawing.Size(68, 20);
			this.buttonCancel.TabIndex = 1;
			this.buttonCancel.Text = "Cancel";
			// 
			// formLeech
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(398, 318);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																																	this.buttonCancel,
																																	this.buttonLeech});
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "formLeech";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Leech";
			this.ResumeLayout(false);

		}
		#endregion
	}
}
