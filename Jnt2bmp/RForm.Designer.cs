namespace Jnt2bmp {
    partial class RForm {
        /// <summary>
        /// 必要なデザイナ変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナで生成されたコード

        /// <summary>
        /// デザイナ サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディタで変更しないでください。
        /// </summary>
        private void InitializeComponent() {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RForm));
            this.J = new AxNbDocViewerLib.AxNbDocView();
            ((System.ComponentModel.ISupportInitialize)(this.J)).BeginInit();
            this.SuspendLayout();
            // 
            // J
            // 
            this.J.Enabled = true;
            this.J.Location = new System.Drawing.Point(0, 0);
            this.J.Name = "J";
            this.J.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("J.OcxState")));
            this.J.Size = new System.Drawing.Size(256, 256);
            this.J.TabIndex = 0;
            // 
            // RForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.J);
            this.Name = "RForm";
            this.Text = "Jnt2bmp";
            this.Load += new System.EventHandler(this.RForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.J)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private AxNbDocViewerLib.AxNbDocView J;

    }
}

