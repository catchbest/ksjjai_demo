using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Jai_FactoryDotNET;
using System.Threading;
using System.Runtime.InteropServices;
using System.Drawing.Imaging;
using System.IO;
using KSJ_Win;
using HalconDotNet;

namespace SimpleImageDisplaySample
{
    public partial class Form1 : Form
    {
       
        // Main factory object
        CFactory myFactory = new CFactory();

        // Opened camera object
        CCamera myCamera;

        // GenICam nodes
        CNode myWidthNode;
        CNode myHeightNode;
        CNode myExposureNode;
        CNode myGainNode;
        Jai_FactoryWrapper.ImageInfo m_ConvertedImageInfo = new Jai_FactoryWrapper.ImageInfo();
        private int _progressCharIndex = 0;
        int m_nWidth = 0;
        int m_nHeight = 0;
        byte[] imagedata;
        byte[] arrayR;//红色数组 
        byte[] arrayG;//绿色数组 
        byte[] arrayB;//蓝色数组 
        public Form1()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;       
            Jai_FactoryWrapper.EFactoryError error = Jai_FactoryWrapper.EFactoryError.Success;

            // Open the factory with the default Registry database
            error = myFactory.Open("");

            // Search for cameras and update all controls
            SearchButton_Click(null, null);

            //recordingModeComboBox.SelectedIndex = 0;
        }

        private void WidthNumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (myWidthNode != null)
            {
                myWidthNode.Value = int.Parse(WidthNumericUpDown.Value.ToString());
                SetFramegrabberValue("Width", (Int64)myWidthNode.Value);
            }
        }

        private void HeightNumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (myHeightNode != null)
            {
                myHeightNode.Value = int.Parse(HeightNumericUpDown.Value.ToString());
                SetFramegrabberValue("Height", (Int64)myHeightNode.Value);
            }
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            if (myCamera != null)
            {
                int size = (int)(m_nWidth * m_nHeight * 3);
                imagedata = new byte[size];
                arrayR = new byte[m_nWidth * m_nHeight];//红色数组 
                arrayG = new byte[m_nWidth * m_nHeight];//绿色数组 
                arrayB = new byte[m_nWidth * m_nHeight];//蓝色数组 
                myCamera.NewImageDelegate += new Jai_FactoryWrapper.ImageCallBack(HandleImage1);
                myCamera.StartImageAcquisition(true, 5);
            }
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            if (myCamera != null)
            {
                myCamera.StopImageAcquisition();
                myCamera.NewImageDelegate -= new Jai_FactoryWrapper.ImageCallBack(HandleImage1);
            }
        }

        unsafe void HandleImage1(ref Jai_FactoryWrapper.ImageInfo ImageInfo)
        {
            bool bCheck = CheckBox_SAVE.Checked;
            if (ImageInfo.PixelFormat == Jai_FactoryWrapper.EPixelFormatType.GVSP_PIX_MONO8)
            {
                long counterStart = 0;
                KSJWin.QueryPerformanceCounter(ref counterStart);
                HObject Hobj;
                HOperatorSet.GenImage1(out Hobj, "byte", ImageInfo.SizeX, ImageInfo.SizeY, ImageInfo.ImageBuffer);
                long counterEnd = 0;
                KSJWin.QueryPerformanceCounter(ref counterEnd);
                long nFreq = 0;
                KSJWin.QueryPerformanceFrequency(ref nFreq);
                float fInterval = (float)(counterEnd - counterStart);
                float fElapse = fInterval / (float)nFreq * 1000;    // MS
                TextBox_ELAPSE_TIME.Text = string.Format("Elapse: {0}ms", fElapse);
                if (bCheck) HOperatorSet.WriteImage(Hobj, "bmp", 0, "JAIToHalcon.bmp");
            }
            else
            {
                Jai_FactoryWrapper.J_Image_Malloc(ref ImageInfo, ref m_ConvertedImageInfo);
                Jai_FactoryWrapper.J_Image_FromRawToImage(ref ImageInfo, ref m_ConvertedImageInfo, 4096, 4096, 4096);
                if (bCheck)
                {
                    long counterStart = 0;
                    KSJWin.QueryPerformanceCounter(ref counterStart);
                    int size = (int)(ImageInfo.SizeX * ImageInfo.SizeY * 3);
                    Marshal.Copy((IntPtr)m_ConvertedImageInfo.ImageBuffer, imagedata, 0, size);
                    for (int i = 0; i < ImageInfo.SizeY; i++)
                    {
                        int nOffset = (int)(i * ImageInfo.SizeX);
                        int nRealOffset = (int)(i * ImageInfo.SizeX * 3);
                        for (int j = 0; j < ImageInfo.SizeX; j++)
                        {
                            int nPixelOffset = j * 3;
                            arrayR[nOffset + j] = imagedata[nRealOffset + nPixelOffset + 2];
                            arrayG[nOffset + j] = imagedata[nRealOffset + nPixelOffset + 1];
                            arrayB[nOffset + j] = imagedata[nRealOffset + nPixelOffset];
                        }
                    }

                    HObject Hobj;
                    fixed (byte* pR = arrayR, pG = arrayG, pB = arrayB)
                    { HOperatorSet.GenImage3(out Hobj, "byte", ImageInfo.SizeX, ImageInfo.SizeY, new IntPtr(pR), new IntPtr(pG), new IntPtr(pB)); }

                    long counterEnd = 0;
                    KSJWin.QueryPerformanceCounter(ref counterEnd);

                    long nFreq = 0;
                    KSJWin.QueryPerformanceFrequency(ref nFreq);
                    float fInterval = (float)(counterEnd - counterStart);
                    float fElapse = fInterval / (float)nFreq * 1000;    // MS
                    TextBox_ELAPSE_TIME.Text = string.Format("Elapse: {0}ms", fElapse);
                    HOperatorSet.WriteImage(Hobj, "bmp", 0, "JAIToHalcon.bmp");
                    bCheck = false;
                    CheckBox_SAVE.Checked = false;
                }
                Jai_FactoryWrapper.J_Image_Free(ref m_ConvertedImageInfo);
            }

            return;
        }

        private void SearchButton_Click(object sender, EventArgs e)
        {
            if (null != myCamera)
            {
                if (myCamera.IsOpen)
                {
                    myCamera.Close();
                }

                myCamera = null;
            }

            // Discover GigE and/or generic GenTL devices using myFactory.UpdateCameraList(in this case specifying Filter Driver for GigE cameras).
            myFactory.UpdateCameraList(Jai_FactoryDotNET.CFactory.EDriverType.FilterDriver);

            // Open the camera - first check for GigE devices
            for (int i = 0; i < myFactory.CameraList.Count; i++)
            {
                myCamera = myFactory.CameraList[i];
                if (Jai_FactoryWrapper.EFactoryError.Success == myCamera.Open())
                {
                    break;
                }
            }

            if (null != myCamera && myCamera.IsOpen)
            {
                CameraIDTextBox.Text = myCamera.CameraID;

                // Attach an event that will be called every time the Async Recording finishes
                myCamera.AsyncImageRecordingDoneEvent += new CCamera.AsyncImageRecordingDoneHandler(myCamera_AsyncImageRecordingDoneEvent);

                if (myCamera.NumOfDataStreams > 0)
                {
                    StartButton.Enabled = true;
                    StopButton.Enabled = true;
                }
                else
                {
                    StartButton.Enabled = false;
                    StopButton.Enabled = false;
                }

                int currentValue = 0;

                // Get the Width GenICam Node
                myWidthNode = myCamera.GetNode("Width");
                if (myWidthNode != null)
                {
                    currentValue = int.Parse(myWidthNode.Value.ToString());

                    // Update range for the Numeric Up/Down control
                    // Convert from integer to Decimal type
                    WidthNumericUpDown.Maximum = decimal.Parse(myWidthNode.Max.ToString());
                    WidthNumericUpDown.Minimum = decimal.Parse(myWidthNode.Min.ToString());
                    WidthNumericUpDown.Value = decimal.Parse(currentValue.ToString());

                    WidthNumericUpDown.Enabled = true;
                }
                else
                    WidthNumericUpDown.Enabled = false;

                SetFramegrabberValue("Width", (Int64)myWidthNode.Value);

                // Get the Height GenICam Node
                myHeightNode = myCamera.GetNode("Height");
                if (myHeightNode != null)
                {
                    currentValue = int.Parse(myHeightNode.Value.ToString());

                    // Update range for the Numeric Up/Down control
                    // Convert from integer to Decimal type
                    HeightNumericUpDown.Maximum = decimal.Parse(myHeightNode.Max.ToString());
                    HeightNumericUpDown.Minimum = decimal.Parse(myHeightNode.Min.ToString());
                    HeightNumericUpDown.Value = decimal.Parse(currentValue.ToString());

                    HeightNumericUpDown.Enabled = true;
                }
                else
                    HeightNumericUpDown.Enabled = false;

                SetFramegrabberValue("Height", (Int64)myHeightNode.Value);

                m_nWidth = int.Parse(WidthNumericUpDown.Value.ToString());
                m_nHeight = int.Parse(HeightNumericUpDown.Value.ToString());
                SetFramegrabberPixelFormat();


                myExposureNode = myCamera.GetNode("ExposureTime");
                if (myExposureNode != null)
                {
                    currentValue = int.Parse(myExposureNode.Value.ToString());

                    // Update range for the Numeric Up/Down control
                    // Convert from integer to Decimal type
                    ExposureNumericUpDown.Maximum = decimal.Parse(myExposureNode.Max.ToString());
                    ExposureNumericUpDown.Minimum = decimal.Parse(myExposureNode.Min.ToString());
                    ExposureNumericUpDown.Value = decimal.Parse(currentValue.ToString());

                    ExposureNumericUpDown.Enabled = true;
                }
                else
                    ExposureNumericUpDown.Enabled = false;

                // Get the GainRaw GenICam Node
                myGainNode = myCamera.GetNode("Gain");
                if (myGainNode != null)
                {
                    GaintextBox.Text = String.Format("{0:F}",myGainNode.Value);
                }
            }
            else
            {
                StartButton.Enabled = false;
                StopButton.Enabled = false;
                WidthNumericUpDown.Enabled = false;
                HeightNumericUpDown.Enabled = true;

                MessageBox.Show("No Cameras Found!");
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (myCamera != null)
            {                
                stopCaptureButton_Click(null, null);
                myCamera.Close();
            }
        }

        private void startCaptureButton_Click(object sender, EventArgs e)
        {
            if (myCamera != null)
            {
                if (myCamera.IsAsyncImageRecordingRunning || (myCamera.TotalAsyncImagesRecordedCount > 0))
                {
                    DialogResult res = MessageBox.Show(this, "The Asychynchronuous Image Recording is already active or the internal buffer is not empty! Do you want to restart the image recording and discard recorded images?", "Asynchronous Image Capture", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2);

                    if (res == DialogResult.Yes)
                    {
                        myCamera.StopAsyncImageRecording();
                        myCamera.FreeAsyncRecordedImages();
                    }
                }
            }
        }

        void myCamera_AsyncImageRecordingDoneEvent(int Count)
        {
            MessageBox.Show("Done capturing " + Count.ToString() + " images asynchronously!", "Asynch Image Capture");
        }

        private void stopCaptureButton_Click(object sender, EventArgs e)
        {
            if (myCamera != null)
            {
                myCamera.StopAsyncImageRecording();
            }
        }

        private void SetFramegrabberValue(String nodeName, Int64 int64Val)
        {
            if (null == myCamera)
            {
                return;
            }

            IntPtr hDevice = IntPtr.Zero;
            Jai_FactoryWrapper.EFactoryError error = Jai_FactoryWrapper.J_Camera_GetLocalDeviceHandle(myCamera.CameraHandle, ref hDevice);
            if (Jai_FactoryWrapper.EFactoryError.Success != error)
            {
                return;
            }

            if (IntPtr.Zero == hDevice)
            {
                return;
            }

            IntPtr hNode;
            error = Jai_FactoryWrapper.J_Camera_GetNodeByName(hDevice, nodeName, out hNode);
            if (Jai_FactoryWrapper.EFactoryError.Success != error)
            {
                return;
            }

            if (IntPtr.Zero == hNode)
            {
                return;
            }

            error = Jai_FactoryWrapper.J_Node_SetValueInt64(hNode, false, int64Val);
            if (Jai_FactoryWrapper.EFactoryError.Success != error)
            {
                return;
            }

            //Special handling for Active Silicon CXP boards, which also has nodes prefixed
            //with "Incoming":
            if ("Width" == nodeName || "Height" == nodeName)
            {
                string strIncoming = "Incoming" + nodeName;
                IntPtr hNodeIncoming;
                error = Jai_FactoryWrapper.J_Camera_GetNodeByName(hDevice, strIncoming, out hNodeIncoming);
                if (Jai_FactoryWrapper.EFactoryError.Success != error)
                {
                    return;
                }

                if (IntPtr.Zero == hNodeIncoming)
                {
                    return;
                }

                error = Jai_FactoryWrapper.J_Node_SetValueInt64(hNodeIncoming, false, int64Val);
            }
        }

        private void SetFramegrabberPixelFormat()
        {
            String nodeName = "PixelFormat";

            if (null == myCamera)
            {
                return;
            }

            IntPtr hDevice = IntPtr.Zero;
            Jai_FactoryWrapper.EFactoryError error = Jai_FactoryWrapper.J_Camera_GetLocalDeviceHandle(myCamera.CameraHandle, ref hDevice);
            if (Jai_FactoryWrapper.EFactoryError.Success != error)
            {
                return;
            }

            if (IntPtr.Zero == hDevice)
            {
                return;
            }

            long pf = 0;
            error = Jai_FactoryWrapper.J_Camera_GetValueInt64(myCamera.CameraHandle, nodeName, ref pf);
            if (Jai_FactoryWrapper.EFactoryError.Success != error)
            {
                return;
            }
            UInt64 pixelFormat = (UInt64)pf;

            UInt64 jaiPixelFormat = 0;
            error = Jai_FactoryWrapper.J_Image_Get_PixelFormat(myCamera.CameraHandle, pixelFormat, ref jaiPixelFormat);
            if (Jai_FactoryWrapper.EFactoryError.Success != error)
            {
                return;
            }

            StringBuilder sbJaiPixelFormatName = new StringBuilder(512);
            uint iSize = (uint)sbJaiPixelFormatName.Capacity;
            error = Jai_FactoryWrapper.J_Image_Get_PixelFormatName(myCamera.CameraHandle, jaiPixelFormat, sbJaiPixelFormatName, iSize);
            if (Jai_FactoryWrapper.EFactoryError.Success != error)
            {
                return;
            }

            IntPtr hNode;
            error = Jai_FactoryWrapper.J_Camera_GetNodeByName(hDevice, nodeName, out hNode);
            if (Jai_FactoryWrapper.EFactoryError.Success != error)
            {
                return;
            }

            if (IntPtr.Zero == hNode)
            {
                return;
            }

            error = Jai_FactoryWrapper.J_Node_SetValueString(hNode, false, sbJaiPixelFormatName.ToString());
            if (Jai_FactoryWrapper.EFactoryError.Success != error)
            {
                return;
            }

            //Special handling for Active Silicon CXP boards, which also has nodes prefixed
            //with "Incoming":
            string strIncoming = "Incoming" + nodeName;
            IntPtr hNodeIncoming;
            error = Jai_FactoryWrapper.J_Camera_GetNodeByName(hDevice, strIncoming, out hNodeIncoming);
            if (Jai_FactoryWrapper.EFactoryError.Success != error)
            {
                return;
            }

            if (IntPtr.Zero == hNodeIncoming)
            {
                return;
            }

            error = Jai_FactoryWrapper.J_Node_SetValueString(hNodeIncoming, false, sbJaiPixelFormatName.ToString());
        }

        private void ExposureNumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (myExposureNode != null)
            {
                myExposureNode.Value = int.Parse(ExposureNumericUpDown.Value.ToString());
                SetFramegrabberValue("ExposureTime", (Int64)myExposureNode.Value);
            }
        }

        private void GaintextBox_TextChanged(object sender, EventArgs e)
        {
            if (myGainNode != null)
            {
                double value = Convert.ToSingle(GaintextBox.Text);
                myGainNode.Value = value;
                if (null == myCamera)
                {
                    return;
                }

                IntPtr hDevice = IntPtr.Zero;
                Jai_FactoryWrapper.EFactoryError error = Jai_FactoryWrapper.J_Camera_GetLocalDeviceHandle(myCamera.CameraHandle, ref hDevice);
                if (Jai_FactoryWrapper.EFactoryError.Success != error)
                {
                    return;
                }

                if (IntPtr.Zero == hDevice)
                {
                    return;
                }

                IntPtr hNode;
                error = Jai_FactoryWrapper.J_Camera_GetNodeByName(hDevice, "Gain", out hNode);
                if (Jai_FactoryWrapper.EFactoryError.Success != error)
                {
                    return;
                }

                if (IntPtr.Zero == hNode)
                {
                    return;
                }

                Jai_FactoryWrapper.J_Node_SetValueDouble(hNode, false, value);
            }
        }
    }
}