using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Windows.Forms;

namespace WindowsApplication1
{
	/// <summary>
	/// Summary description for NeatProgressBar.
	/// </summary>
	public class NeatProgressBar : System.Windows.Forms.UserControl
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		
		//Current Percent Value, Range 0 to 100
		private int myValue = 0;

		public int Value {
			get {				
				return myValue;
			} 
			set {
				if ((value >= 0) && (value <= 100))
					myValue = value;
				this.Refresh();
			}
		}
		
		public NeatProgressBar()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// TODO: Add any initialization after the InitForm call

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

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			// 
			// NeatProgressBar
			// 
			this.Name = "NeatProgressBar";
			this.Size = new System.Drawing.Size(252, 40);
			this.Load += new System.EventHandler(this.NeatProgressBar_Load);
			this.Paint += new System.Windows.Forms.PaintEventHandler(this.NeatProgressBar_Paint);

		}
		#endregion

		private void NeatProgressBar_Load(object sender, System.EventArgs e) {
			this.BackColor = SystemColors.Control;
		}

		private void NeatProgressBar_Paint(object sender, System.Windows.Forms.PaintEventArgs e) {
			Graphics drawing_object = this.CreateGraphics();
			//Draw Top Line
			drawing_object.DrawLine(new Pen(SystemColors.ControlDarkDark, 3), new Point(0, 0), new Point(this.Width, 0));
			//Draw Left Line
			drawing_object.DrawLine(new Pen(SystemColors.ControlDarkDark, 3), new Point(0, 0), new Point(0, this.Height));
			//Draw Right Line
			drawing_object.DrawLine(new Pen(SystemColors.ControlLightLight, 2), new Point(this.Width-1, 0), new Point(this.Width-1, this.Height));			
			//Draw Bottom
			drawing_object.DrawLine(new Pen(SystemColors.ControlLightLight, 2), new Point(0, this.Height-1), new Point(this.Width, this.Height-1));
			LinearGradientBrush bar_brush = new LinearGradientBrush(new Rectangle(0, 0, this.Width-3, this.Height-5), Color.Red, Color.Blue, LinearGradientMode.Horizontal);
			drawing_object.FillRectangle(bar_brush, 2, 2, ((this.Width-5) / 100 * myValue), this.Height-5);
			//Display the Percent Text
			StringFormat display_stringformat = new StringFormat();
			display_stringformat.Alignment =	StringAlignment.Center;
			display_stringformat.LineAlignment =	StringAlignment.Center;
			Font display_font = new Font(DefaultFont.FontFamily, DefaultFont.Size, FontStyle.Bold);
			drawing_object.DrawString((myValue.ToString() + "%"), display_font, new SolidBrush(SystemColors.ActiveCaptionText), new RectangleF(0, 0, this.Width, this.Height), display_stringformat);
		}
	}
}
