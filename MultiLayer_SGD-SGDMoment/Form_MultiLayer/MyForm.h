#pragma once
#include "ArtificalNeuralNetwork.h"
#include "Sample.h"
#include "Process.h"
#include <Windows.h>
#include <time.h>
#include <cmath>
#include <fstream>
#include <atlstr.h>	
#include <msclr/marshal_cppstd.h>
#include <iomanip>


#define NETWORKINPUTS 3			// X1 and X2 => total 2 dimensional space + BIAS
#define ERRORTH 0.001
#define LEARNINGRATE 0.02
#define MOMENTRATE 0.4

#define TOTALCOLORS 5
#define SAMPLESCALE 50
#define LINESCALE 50

namespace Form_Empty {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			pictureBox1->Width = 500;
			pictureBox1->Height = 500;


			comboClasses->Items->Add(2);
			comboClasses->Items->Add(3);
			comboClasses->Items->Add(4);
			comboClasses->Items->Add(5);
			comboClasses->SelectedIndex = 0;

			comboLayers->Items->Add(1);
			comboLayers->Items->Add(2);
			comboLayers->Items->Add(3);
			comboLayers->Items->Add(4);
			comboLayers->Items->Add(5);
			comboLayers->SelectedIndex = 0;

			comboNeurons->Items->Add(4);
			comboNeurons->Items->Add(6);
			comboNeurons->Items->Add(8);
			comboNeurons->Items->Add(10);
			comboNeurons->SelectedIndex = 0;


			comboActivation->Items->Add("Sigmoid");		
			comboActivation->Items->Add("TanH");
			comboActivation->SelectedIndex = 0;

			classColors = gcnew array<Color>(TOTALCOLORS) { Color::Red, Color::Green, Color::Blue, Color::Cyan, Color::Yellow };

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>

		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	protected:
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::ComboBox^ comboColors;

	private: System::Windows::Forms::Label^ label1;

	private: System::Windows::Forms::RichTextBox^ rTextBoxPoints;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		ANeuralNetwork* ANN;
		Classes* classes;
		Sample* means;
		Sample* standardD;
		int totalClass = 0;
		int totalLayer = 0;
		double* inputs;
		bool isSaved;
		array<Color>^ classColors;
		String^ ANNText;
		TrainType trainType;

	private: System::Windows::Forms::ToolStripMenuItem^ initializeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ randomlyToolStripMenuItem;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ labelError;
	private: System::Windows::Forms::ToolStripMenuItem^ trainingToolStripMenuItem;
	private: System::Windows::Forms::Button^ buttonTrain;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ labelCycles;
	private: System::Windows::Forms::Button^ buttonSave;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::ComboBox^ comboClasses;
	private: System::Windows::Forms::ToolStripMenuItem^ SGDToolStripMenuItem;


	private: System::Windows::Forms::ToolStripMenuItem^ manuallyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ getWeightsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ getDataToolStripMenuItem;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chartError;
	private: System::Windows::Forms::ToolStripMenuItem^ testToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ solutionAreaToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ showSamplesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ inputsSaveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ SGDMomentToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^ inputsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ weightsToolStripMenuItem;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::ComboBox^ comboLayers;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::ComboBox^ comboNeurons;

private: System::Windows::Forms::Label^ label8;
private: System::Windows::Forms::Label^ labelNetArc;
private: System::Windows::Forms::CheckBox^ checkB_isChart;
private: System::Windows::Forms::Label^ label7;
private: System::Windows::Forms::ComboBox^ comboActivation;


		   System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		   /// <summary>
		   /// Required method for Designer support - do not modify
		   /// the contents of this method with the code editor.
		   /// </summary>
		   /// 

		   void InitializeSets() {

			   rTextBoxPoints->Clear();
			   comboColors->Items->Clear();
			   pictureBox1->Refresh();

			   comboClasses->Enabled = true;
			   comboLayers->Enabled = true;
			   comboNeurons->Enabled = true;
			   buttonSave->Enabled = true;
			   comboActivation->Enabled = true;

			   comboColors->Enabled = false;
			   buttonTrain->Enabled = false;
			   pictureBox1->Enabled = false;
			   isSaved = false;

		   }

		   void Saved() {

			   comboClasses->Enabled = false;
			   comboLayers->Enabled = false;
			   comboNeurons->Enabled = false;
			   buttonSave->Enabled = false;
			   comboActivation->Enabled = false;

			   comboColors->Enabled = true;
			   buttonTrain->Enabled = true;

			   pictureBox1->Enabled = true;
			   isSaved = true;
		   }


		   void drawSamples(Classes* classes) {

			   Pen^ pen = gcnew Pen(Color::Red, 2);
			   int currentPosX, currentPosY;

			   for (int classIndex = 0; classIndex < totalClass; classIndex++) {

				   pen->Color = classColors[classIndex % TOTALCOLORS];

				   for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++)
				   {
					   currentPosX = classes[classIndex].sample[sampleIndex].x + (pictureBox1->Width / 2);
					   currentPosY = (pictureBox1->Height / 2) - classes[classIndex].sample[sampleIndex].y;

					   pictureBox1->CreateGraphics()->DrawLine(pen, currentPosX - 5, currentPosY, currentPosX + 5, currentPosY);
					   pictureBox1->CreateGraphics()->DrawLine(pen, currentPosX, currentPosY - 5, currentPosX, currentPosY + 5);
				   }
			   }
		   }

		   void drawLines(int scaleMult) {

			   int minX = pictureBox1->Width / -2;
			   int maxX = pictureBox1->Width / 2;
			   int minY, maxY;
			   int dimension = 2;
			   Pen^ pen = gcnew Pen(Color::Red, 3);

			   for (int neuronIndex = 0; neuronIndex < ANN->layers[0].GetNeurons(); neuronIndex++) {

				   inputs[0] = ANN->layers[0].GetWeight(neuronIndex, 0);
				   inputs[1] = ANN->layers[0].GetWeight(neuronIndex, 1);
				   inputs[2] = ANN->layers[0].GetWeight(neuronIndex, 2);

				   minY = YPoint(minX, inputs, scaleMult);
				   maxY = YPoint(maxX, inputs, scaleMult);

				   pen->Color = classColors[neuronIndex % TOTALCOLORS];

				   pictureBox1->CreateGraphics()->DrawLine(pen, (pictureBox1->Width / 2) + minX, (pictureBox1->Height / 2) - minY, (pictureBox1->Width / 2) + maxX, (pictureBox1->Height / 2) - maxY);
			   }
		   }

		   void InitializeComponent(void)
		   {
			   System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			   System::Windows::Forms::DataVisualization::Charting::Legend^ legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			   System::Windows::Forms::DataVisualization::Charting::Series^ series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			   this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			   this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			   this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			   this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->initializeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->randomlyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->manuallyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->getWeightsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->getDataToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->trainingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->SGDToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->SGDMomentToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->testToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->solutionAreaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->showSamplesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->inputsSaveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->inputsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->weightsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->comboColors = (gcnew System::Windows::Forms::ComboBox());
			   this->label1 = (gcnew System::Windows::Forms::Label());
			   this->rTextBoxPoints = (gcnew System::Windows::Forms::RichTextBox());
			   this->label4 = (gcnew System::Windows::Forms::Label());
			   this->labelError = (gcnew System::Windows::Forms::Label());
			   this->buttonTrain = (gcnew System::Windows::Forms::Button());
			   this->label5 = (gcnew System::Windows::Forms::Label());
			   this->labelCycles = (gcnew System::Windows::Forms::Label());
			   this->buttonSave = (gcnew System::Windows::Forms::Button());
			   this->label6 = (gcnew System::Windows::Forms::Label());
			   this->comboClasses = (gcnew System::Windows::Forms::ComboBox());
			   this->chartError = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			   this->label2 = (gcnew System::Windows::Forms::Label());
			   this->comboLayers = (gcnew System::Windows::Forms::ComboBox());
			   this->label3 = (gcnew System::Windows::Forms::Label());
			   this->comboNeurons = (gcnew System::Windows::Forms::ComboBox());
			   this->label8 = (gcnew System::Windows::Forms::Label());
			   this->labelNetArc = (gcnew System::Windows::Forms::Label());
			   this->checkB_isChart = (gcnew System::Windows::Forms::CheckBox());
			   this->label7 = (gcnew System::Windows::Forms::Label());
			   this->comboActivation = (gcnew System::Windows::Forms::ComboBox());
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			   this->menuStrip1->SuspendLayout();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartError))->BeginInit();
			   this->SuspendLayout();
			   // 
			   // pictureBox1
			   // 
			   this->pictureBox1->BackColor = System::Drawing::SystemColors::ControlLightLight;
			   this->pictureBox1->Enabled = false;
			   this->pictureBox1->Location = System::Drawing::Point(12, 52);
			   this->pictureBox1->Name = L"pictureBox1";
			   this->pictureBox1->Size = System::Drawing::Size(500, 500);
			   this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			   this->pictureBox1->TabIndex = 0;
			   this->pictureBox1->TabStop = false;
			   this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::pictureBox1_Paint);
			   this->pictureBox1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::pictureBox1_MouseClick);
			   // 
			   // openFileDialog1
			   // 
			   this->openFileDialog1->FileName = L"openFileDialog1";
			   // 
			   // menuStrip1
			   // 
			   this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			   this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
			   this->menuStrip1->Location = System::Drawing::Point(0, 0);
			   this->menuStrip1->Name = L"menuStrip1";
			   this->menuStrip1->Size = System::Drawing::Size(1469, 28);
			   this->menuStrip1->TabIndex = 1;
			   this->menuStrip1->Text = L"menuStrip1";
			   // 
			   // openToolStripMenuItem
			   // 
			   this->openToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				   this->initializeToolStripMenuItem,
					   this->trainingToolStripMenuItem, this->testToolStripMenuItem, this->inputsSaveToolStripMenuItem
			   });
			   this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			   this->openToolStripMenuItem->Size = System::Drawing::Size(59, 24);
			   this->openToolStripMenuItem->Text = L"Open";
			   // 
			   // initializeToolStripMenuItem
			   // 
			   this->initializeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				   this->randomlyToolStripMenuItem,
					   this->manuallyToolStripMenuItem
			   });
			   this->initializeToolStripMenuItem->Name = L"initializeToolStripMenuItem";
			   this->initializeToolStripMenuItem->Size = System::Drawing::Size(148, 26);
			   this->initializeToolStripMenuItem->Text = L"Initialize";
			   // 
			   // randomlyToolStripMenuItem
			   // 
			   this->randomlyToolStripMenuItem->Name = L"randomlyToolStripMenuItem";
			   this->randomlyToolStripMenuItem->Size = System::Drawing::Size(159, 26);
			   this->randomlyToolStripMenuItem->Text = L"Randomly";
			   this->randomlyToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::randomlyToolStripMenuItem_Click);
			   // 
			   // manuallyToolStripMenuItem
			   // 
			   this->manuallyToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				   this->getWeightsToolStripMenuItem,
					   this->getDataToolStripMenuItem
			   });
			   this->manuallyToolStripMenuItem->Name = L"manuallyToolStripMenuItem";
			   this->manuallyToolStripMenuItem->Size = System::Drawing::Size(159, 26);
			   this->manuallyToolStripMenuItem->Text = L"Manually";
			   // 
			   // getWeightsToolStripMenuItem
			   // 
			   this->getWeightsToolStripMenuItem->Name = L"getWeightsToolStripMenuItem";
			   this->getWeightsToolStripMenuItem->Size = System::Drawing::Size(168, 26);
			   this->getWeightsToolStripMenuItem->Text = L"GetWeights";
			   this->getWeightsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::getWeightsToolStripMenuItem_Click);
			   // 
			   // getDataToolStripMenuItem
			   // 
			   this->getDataToolStripMenuItem->Name = L"getDataToolStripMenuItem";
			   this->getDataToolStripMenuItem->Size = System::Drawing::Size(168, 26);
			   this->getDataToolStripMenuItem->Text = L"GetData";
			   this->getDataToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::getDataToolStripMenuItem_Click);
			   // 
			   // trainingToolStripMenuItem
			   // 
			   this->trainingToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				   this->SGDToolStripMenuItem,
					   this->SGDMomentToolStripMenuItem
			   });
			   this->trainingToolStripMenuItem->Name = L"trainingToolStripMenuItem";
			   this->trainingToolStripMenuItem->Size = System::Drawing::Size(148, 26);
			   this->trainingToolStripMenuItem->Text = L"Training";
			   // 
			   // SGDToolStripMenuItem
			   // 
			   this->SGDToolStripMenuItem->Name = L"SGDToolStripMenuItem";
			   this->SGDToolStripMenuItem->Size = System::Drawing::Size(183, 26);
			   this->SGDToolStripMenuItem->Text = L"SGD";
			   this->SGDToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::SGDToolStripMenuItem_Click);
			   // 
			   // SGDMomentToolStripMenuItem
			   // 
			   this->SGDMomentToolStripMenuItem->Name = L"SGDMomentToolStripMenuItem";
			   this->SGDMomentToolStripMenuItem->Size = System::Drawing::Size(183, 26);
			   this->SGDMomentToolStripMenuItem->Text = L"SGD_Moment";
			   this->SGDMomentToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::SGDMomentToolStripMenuItem_Click);
			   // 
			   // testToolStripMenuItem
			   // 
			   this->testToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				   this->solutionAreaToolStripMenuItem,
					   this->showSamplesToolStripMenuItem
			   });
			   this->testToolStripMenuItem->Name = L"testToolStripMenuItem";
			   this->testToolStripMenuItem->Size = System::Drawing::Size(148, 26);
			   this->testToolStripMenuItem->Text = L"Test";
			   // 
			   // solutionAreaToolStripMenuItem
			   // 
			   this->solutionAreaToolStripMenuItem->Name = L"solutionAreaToolStripMenuItem";
			   this->solutionAreaToolStripMenuItem->Size = System::Drawing::Size(184, 26);
			   this->solutionAreaToolStripMenuItem->Text = L"SolutionArea";
			   this->solutionAreaToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::solutionAreaToolStripMenuItem_Click);
			   // 
			   // showSamplesToolStripMenuItem
			   // 
			   this->showSamplesToolStripMenuItem->Name = L"showSamplesToolStripMenuItem";
			   this->showSamplesToolStripMenuItem->Size = System::Drawing::Size(184, 26);
			   this->showSamplesToolStripMenuItem->Text = L"ShowSamples";
			   this->showSamplesToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::showSamplesToolStripMenuItem_Click);
			   // 
			   // inputsSaveToolStripMenuItem
			   // 
			   this->inputsSaveToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				   this->inputsToolStripMenuItem,
					   this->weightsToolStripMenuItem
			   });
			   this->inputsSaveToolStripMenuItem->Name = L"inputsSaveToolStripMenuItem";
			   this->inputsSaveToolStripMenuItem->Size = System::Drawing::Size(148, 26);
			   this->inputsSaveToolStripMenuItem->Text = L"Save";
			   // 
			   // inputsToolStripMenuItem
			   // 
			   this->inputsToolStripMenuItem->Name = L"inputsToolStripMenuItem";
			   this->inputsToolStripMenuItem->Size = System::Drawing::Size(145, 26);
			   this->inputsToolStripMenuItem->Text = L"Inputs";
			   this->inputsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::inputsToolStripMenuItem_Click);
			   // 
			   // weightsToolStripMenuItem
			   // 
			   this->weightsToolStripMenuItem->Name = L"weightsToolStripMenuItem";
			   this->weightsToolStripMenuItem->Size = System::Drawing::Size(145, 26);
			   this->weightsToolStripMenuItem->Text = L"Weights";
			   this->weightsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::weightsToolStripMenuItem_Click);
			   // 
			   // comboColors
			   // 
			   this->comboColors->Enabled = false;
			   this->comboColors->FormattingEnabled = true;
			   this->comboColors->Location = System::Drawing::Point(1010, 126);
			   this->comboColors->Name = L"comboColors";
			   this->comboColors->Size = System::Drawing::Size(121, 24);
			   this->comboColors->TabIndex = 2;
			   // 
			   // label1
			   // 
			   this->label1->AutoSize = true;
			   this->label1->Location = System::Drawing::Point(1010, 107);
			   this->label1->Name = L"label1";
			   this->label1->Size = System::Drawing::Size(48, 17);
			   this->label1->TabIndex = 4;
			   this->label1->Text = L"Colors";
			   // 
			   // rTextBoxPoints
			   // 
			   this->rTextBoxPoints->Location = System::Drawing::Point(772, 117);
			   this->rTextBoxPoints->Name = L"rTextBoxPoints";
			   this->rTextBoxPoints->Size = System::Drawing::Size(208, 154);
			   this->rTextBoxPoints->TabIndex = 6;
			   this->rTextBoxPoints->Text = L"";
			   // 
			   // label4
			   // 
			   this->label4->AutoSize = true;
			   this->label4->Location = System::Drawing::Point(1157, 215);
			   this->label4->Name = L"label4";
			   this->label4->Size = System::Drawing::Size(44, 17);
			   this->label4->TabIndex = 11;
			   this->label4->Text = L"Error:";
			   // 
			   // labelError
			   // 
			   this->labelError->AutoSize = true;
			   this->labelError->Location = System::Drawing::Point(1220, 215);
			   this->labelError->Name = L"labelError";
			   this->labelError->Size = System::Drawing::Size(38, 17);
			   this->labelError->TabIndex = 12;
			   this->labelError->Text = L"label";
			   // 
			   // buttonTrain
			   // 
			   this->buttonTrain->Enabled = false;
			   this->buttonTrain->Location = System::Drawing::Point(879, 52);
			   this->buttonTrain->Name = L"buttonTrain";
			   this->buttonTrain->Size = System::Drawing::Size(101, 43);
			   this->buttonTrain->TabIndex = 17;
			   this->buttonTrain->Text = L"Train";
			   this->buttonTrain->UseVisualStyleBackColor = true;
			   this->buttonTrain->Click += gcnew System::EventHandler(this, &MyForm::buttonTrain_Click);
			   // 
			   // label5
			   // 
			   this->label5->AutoSize = true;
			   this->label5->Location = System::Drawing::Point(1157, 251);
			   this->label5->Name = L"label5";
			   this->label5->Size = System::Drawing::Size(57, 17);
			   this->label5->TabIndex = 18;
			   this->label5->Text = L"Cycles: ";
			   // 
			   // labelCycles
			   // 
			   this->labelCycles->AutoSize = true;
			   this->labelCycles->Location = System::Drawing::Point(1220, 251);
			   this->labelCycles->Name = L"labelCycles";
			   this->labelCycles->Size = System::Drawing::Size(38, 17);
			   this->labelCycles->TabIndex = 19;
			   this->labelCycles->Text = L"label";
			   // 
			   // buttonSave
			   // 
			   this->buttonSave->Enabled = false;
			   this->buttonSave->Location = System::Drawing::Point(772, 52);
			   this->buttonSave->Name = L"buttonSave";
			   this->buttonSave->Size = System::Drawing::Size(101, 43);
			   this->buttonSave->TabIndex = 20;
			   this->buttonSave->Text = L"Save";
			   this->buttonSave->UseVisualStyleBackColor = true;
			   this->buttonSave->Click += gcnew System::EventHandler(this, &MyForm::buttonSave_Click);
			   // 
			   // label6
			   // 
			   this->label6->AutoSize = true;
			   this->label6->Location = System::Drawing::Point(1010, 52);
			   this->label6->Name = L"label6";
			   this->label6->Size = System::Drawing::Size(42, 17);
			   this->label6->TabIndex = 22;
			   this->label6->Text = L"Class";
			   // 
			   // comboClasses
			   // 
			   this->comboClasses->Enabled = false;
			   this->comboClasses->FormattingEnabled = true;
			   this->comboClasses->Location = System::Drawing::Point(1010, 71);
			   this->comboClasses->Name = L"comboClasses";
			   this->comboClasses->Size = System::Drawing::Size(121, 24);
			   this->comboClasses->TabIndex = 21;
			   // 
			   // chartError
			   // 
			   chartArea1->Name = L"ChartArea1";
			   this->chartError->ChartAreas->Add(chartArea1);
			   legend1->Name = L"Legend1";
			   this->chartError->Legends->Add(legend1);
			   this->chartError->Location = System::Drawing::Point(772, 293);
			   this->chartError->Name = L"chartError";
			   series1->ChartArea = L"ChartArea1";
			   series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			   series1->Legend = L"Legend1";
			   series1->Name = L"Error";
			   this->chartError->Series->Add(series1);
			   this->chartError->Size = System::Drawing::Size(672, 355);
			   this->chartError->TabIndex = 23;
			   this->chartError->Text = L"chart1";
			   // 
			   // label2
			   // 
			   this->label2->AutoSize = true;
			   this->label2->Location = System::Drawing::Point(1010, 167);
			   this->label2->Name = L"label2";
			   this->label2->Size = System::Drawing::Size(100, 17);
			   this->label2->TabIndex = 25;
			   this->label2->Text = L"Hidden Layers";
			   // 
			   // comboLayers
			   // 
			   this->comboLayers->Enabled = false;
			   this->comboLayers->FormattingEnabled = true;
			   this->comboLayers->Location = System::Drawing::Point(1010, 186);
			   this->comboLayers->Name = L"comboLayers";
			   this->comboLayers->Size = System::Drawing::Size(121, 24);
			   this->comboLayers->TabIndex = 24;
			   // 
			   // label3
			   // 
			   this->label3->AutoSize = true;
			   this->label3->Location = System::Drawing::Point(1010, 228);
			   this->label3->Name = L"label3";
			   this->label3->Size = System::Drawing::Size(111, 17);
			   this->label3->TabIndex = 27;
			   this->label3->Text = L"Hidden Neurons";
			   // 
			   // comboNeurons
			   // 
			   this->comboNeurons->Enabled = false;
			   this->comboNeurons->FormattingEnabled = true;
			   this->comboNeurons->Location = System::Drawing::Point(1010, 247);
			   this->comboNeurons->Name = L"comboNeurons";
			   this->comboNeurons->Size = System::Drawing::Size(121, 24);
			   this->comboNeurons->TabIndex = 26;
			   // 
			   // label8
			   // 
			   this->label8->AutoSize = true;
			   this->label8->Location = System::Drawing::Point(1157, 126);
			   this->label8->Name = L"label8";
			   this->label8->Size = System::Drawing::Size(139, 17);
			   this->label8->TabIndex = 29;
			   this->label8->Text = L"Network Architecture";
			   // 
			   // labelNetArc
			   // 
			   this->labelNetArc->AutoSize = true;
			   this->labelNetArc->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->labelNetArc->Location = System::Drawing::Point(1157, 165);
			   this->labelNetArc->Name = L"labelNetArc";
			   this->labelNetArc->Size = System::Drawing::Size(20, 18);
			   this->labelNetArc->TabIndex = 30;
			   this->labelNetArc->Text = L"...";
			   // 
			   // checkB_isChart
			   // 
			   this->checkB_isChart->AutoSize = true;
			   this->checkB_isChart->Location = System::Drawing::Point(1346, 247);
			   this->checkB_isChart->Name = L"checkB_isChart";
			   this->checkB_isChart->Size = System::Drawing::Size(98, 21);
			   this->checkB_isChart->TabIndex = 32;
			   this->checkB_isChart->Text = L"Error Draw";
			   this->checkB_isChart->UseVisualStyleBackColor = true;
			   // 
			   // label7
			   // 
			   this->label7->AutoSize = true;
			   this->label7->Location = System::Drawing::Point(1157, 52);
			   this->label7->Name = L"label7";
			   this->label7->Size = System::Drawing::Size(69, 17);
			   this->label7->TabIndex = 34;
			   this->label7->Text = L"Activation";
			   // 
			   // comboActivation
			   // 
			   this->comboActivation->Enabled = false;
			   this->comboActivation->FormattingEnabled = true;
			   this->comboActivation->Location = System::Drawing::Point(1160, 72);
			   this->comboActivation->Name = L"comboActivation";
			   this->comboActivation->Size = System::Drawing::Size(121, 24);
			   this->comboActivation->TabIndex = 33;
			   // 
			   // MyForm
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(1469, 669);
			   this->Controls->Add(this->label7);
			   this->Controls->Add(this->comboActivation);
			   this->Controls->Add(this->checkB_isChart);
			   this->Controls->Add(this->labelNetArc);
			   this->Controls->Add(this->label8);
			   this->Controls->Add(this->label3);
			   this->Controls->Add(this->comboNeurons);
			   this->Controls->Add(this->label2);
			   this->Controls->Add(this->comboLayers);
			   this->Controls->Add(this->chartError);
			   this->Controls->Add(this->label6);
			   this->Controls->Add(this->comboClasses);
			   this->Controls->Add(this->buttonSave);
			   this->Controls->Add(this->labelCycles);
			   this->Controls->Add(this->label5);
			   this->Controls->Add(this->buttonTrain);
			   this->Controls->Add(this->labelError);
			   this->Controls->Add(this->label4);
			   this->Controls->Add(this->rTextBoxPoints);
			   this->Controls->Add(this->label1);
			   this->Controls->Add(this->comboColors);
			   this->Controls->Add(this->pictureBox1);
			   this->Controls->Add(this->menuStrip1);
			   this->MainMenuStrip = this->menuStrip1;
			   this->Margin = System::Windows::Forms::Padding(4);
			   this->Name = L"MyForm";
			   this->Text = L"Multilayer Classification";
			   this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			   this->menuStrip1->ResumeLayout(false);
			   this->menuStrip1->PerformLayout();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chartError))->EndInit();
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion


	private: System::Void pictureBox1_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {

		Pen^ pen = gcnew Pen((Color)comboColors->SelectedItem, 2);

		int mousePosX = Convert::ToInt32(e->X);
		int mousePosY = Convert::ToInt32(e->Y);

		double clickX = mousePosX - (pictureBox1->Width / 2);
		double clickY = (pictureBox1->Height / 2) - mousePosY;

		int classIndex = comboColors->SelectedIndex;

		if (classes[classIndex].totalSample == 0) {

			classes[classIndex].sample = new Sample;
			classes[classIndex].sample->x = clickX;
			classes[classIndex].sample->y = clickY;
			classes[classIndex].totalSample++;
		}
		else {

			classes[classIndex].AddSample(clickX, clickY);
		}

		pictureBox1->CreateGraphics()->DrawLine(pen, mousePosX - 5, mousePosY, mousePosX + 5, mousePosY);
		pictureBox1->CreateGraphics()->DrawLine(pen, mousePosX, mousePosY - 5, mousePosX, mousePosY + 5);

		rTextBoxPoints->AppendText("X: " + clickX.ToString() + " Y: " + clickY.ToString() + " C: " + comboColors->GetItemText(comboColors->SelectedItem) + "\n");
	}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {


	}

	private: System::Void pictureBox1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {

		Pen^ pen = gcnew Pen(Color::Black, 2);
		int centerWidth = pictureBox1->Width / 2;
		int centerHeight = pictureBox1->Height / 2;

		e->Graphics->DrawLine(pen, centerWidth, 0, centerWidth, pictureBox1->Height);
		e->Graphics->DrawLine(pen, 0, centerHeight, pictureBox1->Width, centerHeight);
	}

	private: System::Void randomlyToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

		Pen^ pen = gcnew Pen(Color::Blue, 3.0);

		srand(time(0));

		for (int layerIndex = 0; layerIndex < totalLayer; layerIndex++)
		{			
			for (int i = 0; i < ANN->layers[layerIndex].GetWeightSize(); i++)
			{
				ANN->layers[layerIndex].SetWeight(i, (double)((double)rand() / RAND_MAX));
			}
		}

		// initialize a line is draw such as: w1*x1 + w2*x2 + w3 = 0;
		int minX, minY, maxX, maxY;

		minX = pictureBox1->Width / -2;
		maxX = pictureBox1->Width / 2;

		for (int neuronIndex = 0; neuronIndex < ANN->layers[0].GetNeurons(); neuronIndex++) {

			inputs[0] = ANN->layers[0].GetWeight(neuronIndex, 0);
			inputs[1] = ANN->layers[0].GetWeight(neuronIndex, 1);
			inputs[2] = ANN->layers[0].GetWeight(neuronIndex, 2);

			minY = YPoint(minX, inputs);
			maxY = YPoint(maxX, inputs);

			pen->Color = classColors[neuronIndex % TOTALCOLORS];

			pictureBox1->CreateGraphics()->DrawLine(pen, (pictureBox1->Width / 2) + minX, (pictureBox1->Height / 2) - minY, (pictureBox1->Width / 2) + maxX, (pictureBox1->Height / 2) - maxY);
		}

	}

	private: System::Void buttonTrain_Click(System::Object^ sender, System::EventArgs^ e) {

		chartError->Series["Error"]->Points->Clear();

		double totalError = 0.0;
		double normError = 0.0;
		int totalSample = 0;
		long int cycleCounter = 0;

		for (int classIndex = 0; classIndex < totalClass; classIndex++)
		{
			totalSample += classes[classIndex].totalSample;
		}

		Classes* batchNormalized = BatchNormalization(classes, totalClass, means, standardD);

		do {
			totalError = 0.0;

			for (int classIndex = 0; classIndex < totalClass; classIndex++)
			{
				for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++)
				{
					inputs[0] = batchNormalized[classIndex].sample[sampleIndex].x;
					inputs[1] = batchNormalized[classIndex].sample[sampleIndex].y;
					inputs[2] = batchNormalized[classIndex].sample[sampleIndex].b;

					// TRAIN ///////////////////////////////////////////////

					totalError += ANN->Train(inputs, classes[classIndex].d);
				}
			}

			normError = ANN->RMSE(totalError, totalSample);
			labelError->Text = normError.ToString();
			labelError->Refresh();

			if(checkB_isChart->Checked && cycleCounter % 10 == 0)
				chartError->Series["Error"]->Points->AddXY(cycleCounter, normError);

			cycleCounter++;

		} while (normError > ERRORTH);


		for (int classIndex = 0; classIndex < totalClass; classIndex++)
		{
			for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++) {
				batchNormalized[classIndex].sample[sampleIndex].x *= SAMPLESCALE;
				batchNormalized[classIndex].sample[sampleIndex].y *= SAMPLESCALE;
			}
		}

		pictureBox1->Refresh();
		drawLines(LINESCALE);
		drawSamples(batchNormalized);

		labelCycles->Text = cycleCounter.ToString();

	}

	private: System::Void SGDToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

		InitializeSets();
		trainType = TrainType::SGD;										// Continuous

	}

	private: System::Void buttonSave_Click(System::Object^ sender, System::EventArgs^ e) {

		totalClass = (int)comboClasses->Items[comboClasses->SelectedIndex];
		
		int hiddenLayers = (int)comboLayers->Items[comboLayers->SelectedIndex];
		totalLayer = hiddenLayers + 1;									// total layer = hidden layers + output layer
		
		int totalNeuron = (int)comboNeurons->Items[comboNeurons->SelectedIndex];

		int activationIndex = (int)comboActivation->SelectedIndex;
		Activations activationType;

		switch (activationIndex)
		{
		case 0:
			activationType = Activations::Sigmoid;
			break;

		case 1:
			activationType = Activations::TanH;
			break;

		default:
			break;
		}

		classes = new Classes[totalClass];
		means = new Sample;
		standardD = new Sample;
		inputs = new double[NETWORKINPUTS];
		
		ANN = new ANeuralNetwork(NETWORKINPUTS, hiddenLayers, totalClass);

		ANN->SetTrainParameters(LEARNINGRATE, MOMENTRATE, trainType, activationType);

		for (int i = 0; i < totalLayer - 1; i++)
		{
			ANN->SetLayerNeurons(i, totalNeuron);
		}

		ANN->SetLayerNeurons(totalLayer - 1, totalClass);		// Son katmanin neuron sayisi (yani agin cikis sayisi)

		ANN->Create();

		for (int classIndex = 0; classIndex < totalClass; classIndex++)
			comboColors->Items->Add(classColors[classIndex]);

		comboColors->SelectedIndex = 0;


		switch (totalClass)
		{
		case 2:

			classes[0].d = new int[2]{  1,-1 };
			classes[1].d = new int[2]{ -1, 1 };
			break;

		case 3:

			classes[0].d = new int[3]{  1,-1,-1 };
			classes[1].d = new int[3]{ -1,1, -1 };
			classes[2].d = new int[3]{ -1,-1, 1 };
			break;

		case 4:

			classes[0].d = new int[4]{  1,-1,-1,-1 };
			classes[1].d = new int[4]{ -1,1,-1, -1 };
			classes[2].d = new int[4]{ -1,-1, 1,-1 };
			classes[3].d = new int[4]{ -1,-1,-1, 1 };
			break;

		case 5:

			classes[0].d = new int[5]{ 1,-1,-1,-1,-1 };
			classes[1].d = new int[5]{ -1,1,-1,-1,-1 };
			classes[2].d = new int[5]{ -1,-1,1,-1,-1 };
			classes[3].d = new int[5]{ -1,-1,-1,1,-1 };
			classes[4].d = new int[5]{ -1,-1,-1,-1,1 };
			break;
		}


		ANNText = NETWORKINPUTS.ToString();

		for (int i = 0; i < totalLayer; i++)
		{
			ANNText += (" " + ANN->layers[i].GetNeurons().ToString());
		}

		labelNetArc->Text = ANNText;
		labelNetArc->Refresh();

		Saved();
	}

	private: System::Void getWeightsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

		if (isSaved) {

			std::string mssg1 = "Select layer weights | Weights_";

			String^ msg = msclr::interop::marshal_as<System::String^>(mssg1);
			MessageBox::Show(msg + ANNText + "_" + comboActivation->SelectedItem, "Select Weight");

			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {

				CString filePath = openFileDialog1->FileName;
				
				std::ifstream file(filePath);
				double temp;

				if (file.is_open()) {
					
					for (int layerIndex = 0; layerIndex < totalLayer; layerIndex++)
					{
						for (int i = 0; i < ANN->layers[layerIndex].GetWeightSize(); i++) {
							file >> temp;
							ANN->layers[layerIndex].SetWeight(i, temp);
						}
					}
					
					file.close();
					drawLines(LINESCALE);
				}
			}
		}
		else {
			std::string mssg1 = "Select the number of classes and SAVE";

			String^ msg = msclr::interop::marshal_as<System::String^>(mssg1);
			MessageBox::Show(msg, "Select Train Type");
		}
	}

	private: System::Void getDataToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

		if (isSaved) {

			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				CString filePath = openFileDialog1->FileName;

				std::ifstream file(filePath);

				int classIndex = 0;
				double tempClass;
				double tempX;
				double tempY;

				if (file.is_open()) {

					while (file >> tempClass, file >> tempX, file >> tempY) {

						classIndex = (int)tempClass;

						if (classes[classIndex].totalSample == 0) {

							classes[classIndex].sample = new Sample;
							classes[classIndex].sample->x = tempX;
							classes[classIndex].sample->y = tempY;
							classes[classIndex].totalSample++;

						}
						else {
							classes[classIndex].AddSample(tempX, tempY);
						}

						rTextBoxPoints->AppendText("X: " + tempX.ToString() + " Y: " + tempY.ToString() + " C: " + comboColors->GetItemText(classColors[classIndex]) + "\n");
					}

					Mean(classes, totalClass, means);
					StandardD(classes, totalClass, means, standardD);
					
					drawSamples(classes);
				}
				file.close();
			}
		}
		else
		{
			std::string mssg1 = "Select the number of classes and SAVE";

			String^ msg = msclr::interop::marshal_as<System::String^>(mssg1);
			MessageBox::Show(msg, "Select Train Type");
		}

	}
	private: System::Void solutionAreaToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {


		double coordX = 0.0;
		double coordY = 0.0;

		Bitmap^ bgDrawingArea = gcnew Bitmap(pictureBox1->Width, pictureBox1->Height);
		pictureBox1->Image = bgDrawingArea;

		// Girislere batch normalization uygulamak icin tum ornekler olusturulur
		Classes* batchNormalized = new Classes;
		batchNormalized->totalSample = 250 * 250;
		batchNormalized->sample = new Sample[batchNormalized->totalSample];

		int counter = 0;
		for (int row =0; row < pictureBox1->Height; row += 2) 
		{
			coordY = (pictureBox1->Height / 2) - row;

			for (int col = 0; col < pictureBox1->Width; col += 2) {

				coordX = col - (pictureBox1->Width / 2);

				// Batch normalize islemi train adiminda hesaplanan mean ve standart sapma degerleri ile yapilir
				batchNormalized->sample[counter].x = (coordX - means->x) / standardD->x;
				batchNormalized->sample[counter].y = (coordY - means->y) / standardD->y;
				counter++;
			}
		}

		int maxIndex = 0;
		counter = 0;
		double* predict;

		for (int row = 0; row < pictureBox1->Height ; row += 2)
			for (int col = 0; col < pictureBox1->Width ; col += 2) {

				inputs[0] = batchNormalized->sample[counter].x;
				inputs[1] = batchNormalized->sample[counter].y;
				inputs[2] = batchNormalized->sample[counter].b;


				predict = ANN->Predict(inputs);

				maxIndex = MaxIndex(predict, ANN->GetNetworkOutputs());

				bgDrawingArea->SetPixel(col, row, classColors[maxIndex]);

				counter++;
			}

	}

	private: System::Void showSamplesToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

		drawSamples(classes);
	}

	private: System::Void SGDMomentToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

		InitializeSets();
		trainType = TrainType::SGD_Moment;				// Continuous with Momentum
	}

	private: System::Void inputsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {


		String^ fileText = "Inputs_Samples_" + ANNText + ".txt";
		std::string  filename = msclr::interop::marshal_as<std::string>(fileText);
		std::ofstream file(filename);

		for (int classIndex = 0; classIndex < totalClass; classIndex++)
			for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++)
				file << classIndex << " " << classes[classIndex].sample[sampleIndex].x << " " << classes[classIndex].sample[sampleIndex].y << std::endl;

		file.close();

	}

	private: System::Void weightsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

		String^ fileText = "Weights_" + ANNText + "_" + comboActivation->SelectedItem + ".txt";
		std::string  filename = msclr::interop::marshal_as<std::string>(fileText);
		std::ofstream file(filename);

		for (int layerIndex = 0; layerIndex < totalLayer; layerIndex++)
		{
			for (int i = 0; i < ANN->layers[layerIndex].GetWeightSize(); i++)
			{
				file << std::fixed << std::setprecision(10) << ANN->layers[layerIndex].GetWeight(i) << " ";
			}
		}
		
		file.close();
	}
	};
}
