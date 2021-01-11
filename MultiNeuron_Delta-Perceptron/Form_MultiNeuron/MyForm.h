#pragma once
#include <Windows.h>
#include "Sample.h"
#include "Process.h"
#include <time.h>
#include <cmath>

#define NETWORKINPUTS 3			// X1 and X2 => total 2 dimensional space + BIAS

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

			classColors = gcnew array<Color>(5) { Color::Red, Color::Green, Color::Blue, Color::Cyan, Color::Yellow };
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


		Classes* classes;
		int totalClass = 0;
		int totalNeuron = 0;
		double* weights;
		char* trainType;		// Perceptron or Delta
		array<Color>^ classColors;

	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ labelX;
	private: System::Windows::Forms::Label^ labelY;
	private: System::Windows::Forms::ToolStripMenuItem^ initializeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ randomlyToolStripMenuItem;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ labelW0;
	private: System::Windows::Forms::Label^ labelW1;


	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ labelW2;

	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::ToolStripMenuItem^ trainingToolStripMenuItem;

	private: System::Windows::Forms::Button^ buttonTrain;

	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ labelCycles;


	private: System::Windows::Forms::Button^ buttonSave;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::ComboBox^ comboClasses;
	private: System::Windows::Forms::ToolStripMenuItem^ discreteToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^ continuousToolStripMenuItem;



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
			   buttonSave->Enabled = true;

			   comboColors->Enabled = false;
			   buttonTrain->Enabled = false;
			   pictureBox1->Enabled = false;

		   }

		   void Saved() {

			   comboClasses->Enabled = false;
			   buttonSave->Enabled = false;

			   comboColors->Enabled = true;
			   buttonTrain->Enabled = true;

			   pictureBox1->Enabled = true;
		   }


		   void draw(int mult) {

			   int minX = pictureBox1->Width / -2;
			   int maxX = pictureBox1->Width / 2;
			   int minY, maxY;
			   int dimension = 2;
			   Pen^ pen = gcnew Pen(Color::Red, 3);

			   double tempWeight[NETWORKINPUTS];

			   for (int neuronIndex = 0; neuronIndex < totalNeuron; neuronIndex++) {

				   tempWeight[0] = weights[neuronIndex * NETWORKINPUTS];
				   tempWeight[1] = weights[neuronIndex * NETWORKINPUTS + 1];
				   tempWeight[2] = weights[neuronIndex * NETWORKINPUTS + 2];

				   minY = YPoint(minX, tempWeight, mult);
				   maxY = YPoint(maxX, tempWeight, mult);

				   pen->Color = classColors[neuronIndex];

				   pictureBox1->CreateGraphics()->DrawLine(pen, (pictureBox1->Width / 2) + minX, (pictureBox1->Height / 2) - minY, (pictureBox1->Width / 2) + maxX, (pictureBox1->Height / 2) - maxY);
			   }

			   int currentPosX, currentPosY;

			   for (int classIndex = 0; classIndex < totalClass; classIndex++) {

				   pen->Color = classColors[classIndex];

				   for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++)
				   {
					   currentPosX = classes[classIndex].sample[sampleIndex].x + (pictureBox1->Width / 2);
					   currentPosY = (pictureBox1->Height / 2) - classes[classIndex].sample[sampleIndex].y;

					   pictureBox1->CreateGraphics()->DrawLine(pen, currentPosX - 5, currentPosY, currentPosX + 5, currentPosY);
					   pictureBox1->CreateGraphics()->DrawLine(pen, currentPosX, currentPosY - 5, currentPosX, currentPosY + 5);
				   }
			   }

		   }

		   void InitializeComponent(void)
		   {
			   this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			   this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			   this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			   this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->initializeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->randomlyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->trainingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->discreteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->continuousToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->comboColors = (gcnew System::Windows::Forms::ComboBox());
			   this->label1 = (gcnew System::Windows::Forms::Label());
			   this->rTextBoxPoints = (gcnew System::Windows::Forms::RichTextBox());
			   this->label2 = (gcnew System::Windows::Forms::Label());
			   this->label3 = (gcnew System::Windows::Forms::Label());
			   this->labelX = (gcnew System::Windows::Forms::Label());
			   this->labelY = (gcnew System::Windows::Forms::Label());
			   this->label4 = (gcnew System::Windows::Forms::Label());
			   this->labelW0 = (gcnew System::Windows::Forms::Label());
			   this->labelW1 = (gcnew System::Windows::Forms::Label());
			   this->label7 = (gcnew System::Windows::Forms::Label());
			   this->labelW2 = (gcnew System::Windows::Forms::Label());
			   this->label9 = (gcnew System::Windows::Forms::Label());
			   this->buttonTrain = (gcnew System::Windows::Forms::Button());
			   this->label5 = (gcnew System::Windows::Forms::Label());
			   this->labelCycles = (gcnew System::Windows::Forms::Label());
			   this->buttonSave = (gcnew System::Windows::Forms::Button());
			   this->label6 = (gcnew System::Windows::Forms::Label());
			   this->comboClasses = (gcnew System::Windows::Forms::ComboBox());
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			   this->menuStrip1->SuspendLayout();
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
			   this->menuStrip1->Size = System::Drawing::Size(1108, 28);
			   this->menuStrip1->TabIndex = 1;
			   this->menuStrip1->Text = L"menuStrip1";
			   // 
			   // openToolStripMenuItem
			   // 
			   this->openToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				   this->initializeToolStripMenuItem,
					   this->trainingToolStripMenuItem
			   });
			   this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			   this->openToolStripMenuItem->Size = System::Drawing::Size(59, 24);
			   this->openToolStripMenuItem->Text = L"Open";
			   // 
			   // initializeToolStripMenuItem
			   // 
			   this->initializeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->randomlyToolStripMenuItem });
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
			   // trainingToolStripMenuItem
			   // 
			   this->trainingToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				   this->discreteToolStripMenuItem,
					   this->continuousToolStripMenuItem
			   });
			   this->trainingToolStripMenuItem->Name = L"trainingToolStripMenuItem";
			   this->trainingToolStripMenuItem->Size = System::Drawing::Size(148, 26);
			   this->trainingToolStripMenuItem->Text = L"Training";
			   // 
			   // discreteToolStripMenuItem
			   // 
			   this->discreteToolStripMenuItem->Name = L"discreteToolStripMenuItem";
			   this->discreteToolStripMenuItem->Size = System::Drawing::Size(166, 26);
			   this->discreteToolStripMenuItem->Text = L"Discrete";
			   this->discreteToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::discreteToolStripMenuItem_Click);
			   // 
			   // continuousToolStripMenuItem
			   // 
			   this->continuousToolStripMenuItem->Name = L"continuousToolStripMenuItem";
			   this->continuousToolStripMenuItem->Size = System::Drawing::Size(166, 26);
			   this->continuousToolStripMenuItem->Text = L"Continuous";
			   this->continuousToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::continuousToolStripMenuItem_Click);
			   // 
			   // comboColors
			   // 
			   this->comboColors->Enabled = false;
			   this->comboColors->FormattingEnabled = true;
			   this->comboColors->Location = System::Drawing::Point(772, 126);
			   this->comboColors->Name = L"comboColors";
			   this->comboColors->Size = System::Drawing::Size(121, 24);
			   this->comboColors->TabIndex = 2;
			   // 
			   // label1
			   // 
			   this->label1->AutoSize = true;
			   this->label1->Location = System::Drawing::Point(772, 107);
			   this->label1->Name = L"label1";
			   this->label1->Size = System::Drawing::Size(48, 17);
			   this->label1->TabIndex = 4;
			   this->label1->Text = L"Colors";
			   // 
			   // rTextBoxPoints
			   // 
			   this->rTextBoxPoints->Location = System::Drawing::Point(772, 156);
			   this->rTextBoxPoints->Name = L"rTextBoxPoints";
			   this->rTextBoxPoints->Size = System::Drawing::Size(202, 178);
			   this->rTextBoxPoints->TabIndex = 6;
			   this->rTextBoxPoints->Text = L"";
			   // 
			   // label2
			   // 
			   this->label2->AutoSize = true;
			   this->label2->Location = System::Drawing::Point(772, 352);
			   this->label2->Name = L"label2";
			   this->label2->Size = System::Drawing::Size(25, 17);
			   this->label2->TabIndex = 7;
			   this->label2->Text = L"X :";
			   // 
			   // label3
			   // 
			   this->label3->AutoSize = true;
			   this->label3->Location = System::Drawing::Point(868, 352);
			   this->label3->Name = L"label3";
			   this->label3->Size = System::Drawing::Size(25, 17);
			   this->label3->TabIndex = 8;
			   this->label3->Text = L"Y :";
			   // 
			   // labelX
			   // 
			   this->labelX->AutoSize = true;
			   this->labelX->Location = System::Drawing::Point(803, 352);
			   this->labelX->Name = L"labelX";
			   this->labelX->Size = System::Drawing::Size(16, 17);
			   this->labelX->TabIndex = 9;
			   this->labelX->Text = L"..";
			   // 
			   // labelY
			   // 
			   this->labelY->AutoSize = true;
			   this->labelY->Location = System::Drawing::Point(899, 352);
			   this->labelY->Name = L"labelY";
			   this->labelY->Size = System::Drawing::Size(16, 17);
			   this->labelY->TabIndex = 10;
			   this->labelY->Text = L"..";
			   // 
			   // label4
			   // 
			   this->label4->AutoSize = true;
			   this->label4->Location = System::Drawing::Point(772, 398);
			   this->label4->Name = L"label4";
			   this->label4->Size = System::Drawing::Size(33, 17);
			   this->label4->TabIndex = 11;
			   this->label4->Text = L"w[0]";
			   // 
			   // labelW0
			   // 
			   this->labelW0->AutoSize = true;
			   this->labelW0->Location = System::Drawing::Point(824, 398);
			   this->labelW0->Name = L"labelW0";
			   this->labelW0->Size = System::Drawing::Size(20, 17);
			   this->labelW0->TabIndex = 12;
			   this->labelW0->Text = L"...";
			   // 
			   // labelW1
			   // 
			   this->labelW1->AutoSize = true;
			   this->labelW1->Location = System::Drawing::Point(824, 430);
			   this->labelW1->Name = L"labelW1";
			   this->labelW1->Size = System::Drawing::Size(20, 17);
			   this->labelW1->TabIndex = 14;
			   this->labelW1->Text = L"...";
			   // 
			   // label7
			   // 
			   this->label7->AutoSize = true;
			   this->label7->Location = System::Drawing::Point(772, 430);
			   this->label7->Name = L"label7";
			   this->label7->Size = System::Drawing::Size(33, 17);
			   this->label7->TabIndex = 13;
			   this->label7->Text = L"w[1]";
			   // 
			   // labelW2
			   // 
			   this->labelW2->AutoSize = true;
			   this->labelW2->Location = System::Drawing::Point(824, 465);
			   this->labelW2->Name = L"labelW2";
			   this->labelW2->Size = System::Drawing::Size(20, 17);
			   this->labelW2->TabIndex = 16;
			   this->labelW2->Text = L"...";
			   // 
			   // label9
			   // 
			   this->label9->AutoSize = true;
			   this->label9->Location = System::Drawing::Point(772, 465);
			   this->label9->Name = L"label9";
			   this->label9->Size = System::Drawing::Size(33, 17);
			   this->label9->TabIndex = 15;
			   this->label9->Text = L"w[2]";
			   // 
			   // buttonTrain
			   // 
			   this->buttonTrain->Enabled = false;
			   this->buttonTrain->Location = System::Drawing::Point(928, 107);
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
			   this->label5->Location = System::Drawing::Point(769, 517);
			   this->label5->Name = L"label5";
			   this->label5->Size = System::Drawing::Size(57, 17);
			   this->label5->TabIndex = 18;
			   this->label5->Text = L"Cycles: ";
			   // 
			   // labelCycles
			   // 
			   this->labelCycles->AutoSize = true;
			   this->labelCycles->Location = System::Drawing::Point(832, 517);
			   this->labelCycles->Name = L"labelCycles";
			   this->labelCycles->Size = System::Drawing::Size(20, 17);
			   this->labelCycles->TabIndex = 19;
			   this->labelCycles->Text = L"...";
			   // 
			   // buttonSave
			   // 
			   this->buttonSave->Enabled = false;
			   this->buttonSave->Location = System::Drawing::Point(928, 52);
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
			   this->label6->Location = System::Drawing::Point(772, 52);
			   this->label6->Name = L"label6";
			   this->label6->Size = System::Drawing::Size(42, 17);
			   this->label6->TabIndex = 22;
			   this->label6->Text = L"Class";
			   // 
			   // comboClasses
			   // 
			   this->comboClasses->Enabled = false;
			   this->comboClasses->FormattingEnabled = true;
			   this->comboClasses->Location = System::Drawing::Point(772, 71);
			   this->comboClasses->Name = L"comboClasses";
			   this->comboClasses->Size = System::Drawing::Size(121, 24);
			   this->comboClasses->TabIndex = 21;
			   // 
			   // MyForm
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(1108, 686);
			   this->Controls->Add(this->label6);
			   this->Controls->Add(this->comboClasses);
			   this->Controls->Add(this->buttonSave);
			   this->Controls->Add(this->labelCycles);
			   this->Controls->Add(this->label5);
			   this->Controls->Add(this->buttonTrain);
			   this->Controls->Add(this->labelW2);
			   this->Controls->Add(this->label9);
			   this->Controls->Add(this->labelW1);
			   this->Controls->Add(this->label7);
			   this->Controls->Add(this->labelW0);
			   this->Controls->Add(this->label4);
			   this->Controls->Add(this->labelY);
			   this->Controls->Add(this->labelX);
			   this->Controls->Add(this->label3);
			   this->Controls->Add(this->label2);
			   this->Controls->Add(this->rTextBoxPoints);
			   this->Controls->Add(this->label1);
			   this->Controls->Add(this->comboColors);
			   this->Controls->Add(this->pictureBox1);
			   this->Controls->Add(this->menuStrip1);
			   this->MainMenuStrip = this->menuStrip1;
			   this->Margin = System::Windows::Forms::Padding(4);
			   this->Name = L"MyForm";
			   this->Text = L"MultiNeuron - Perceptron & Delta Learning";
			   this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			   this->menuStrip1->ResumeLayout(false);
			   this->menuStrip1->PerformLayout();
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
		labelX->Text = clickX.ToString();
		labelY->Text = clickY.ToString();
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

		// initialize a line is draw such as: w1*x1 + w2*x2 + w3 = 0;
		int minX, minY, maxX, maxY;

		// totalNeuron * NETWORKINPUTS => Weights matrix size
		weights = new double[totalNeuron * NETWORKINPUTS];
		srand(time(0));

		for (size_t classIndex = 0; classIndex < totalNeuron; classIndex++)
			for (size_t i = 0; i < NETWORKINPUTS; i++)
				weights[classIndex * NETWORKINPUTS + i] = ((double)rand() / RAND_MAX);

		labelW0->Text = Convert::ToString(weights[0]);
		labelW1->Text = Convert::ToString(weights[1]);
		labelW2->Text = Convert::ToString(weights[2]);

		double tempWeight[3];
		minX = pictureBox1->Width / -2;
		maxX = pictureBox1->Width / 2;

		for (int neuronIndex = 0; neuronIndex < totalNeuron; neuronIndex++) {

			tempWeight[0] = weights[neuronIndex * NETWORKINPUTS];
			tempWeight[1] = weights[neuronIndex * NETWORKINPUTS + 1];
			tempWeight[2] = weights[neuronIndex * NETWORKINPUTS + 2];

			minY = YPoint(minX, tempWeight);
			maxY = YPoint(maxX, tempWeight);

			pen->Color = classColors[neuronIndex];

			pictureBox1->CreateGraphics()->DrawLine(pen, (pictureBox1->Width / 2) + minX, (pictureBox1->Height / 2) - minY, (pictureBox1->Width / 2) + maxX, (pictureBox1->Height / 2) - maxY);
		}

	}


	private: System::Void buttonTrain_Click(System::Object^ sender, System::EventArgs^ e) {

		double sampleMatrix[NETWORKINPUTS];
		double* output = new double[totalNeuron];
		
		double c = 0.5;
		double multiple = 0.0;
		long int weightState = 0;
		long int cycleCounter = 0;

		// Perceptron Learning  
		if (!strcmp(trainType, "Discrete")) {

			do {
				weightState = 0;

				for (int classIndex = 0; classIndex < totalClass; classIndex++)
				{
					for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++) {

						sampleMatrix[0] = classes[classIndex].sample[sampleIndex].x;
						sampleMatrix[1] = classes[classIndex].sample[sampleIndex].y;
						sampleMatrix[2] = classes[classIndex].sample[sampleIndex].b;

						MatrixMultiplication(weights, sampleMatrix, output, totalNeuron, NETWORKINPUTS, 1);

						for (int outputIndex = 0; outputIndex < totalNeuron; outputIndex++)
						{
							output[outputIndex] = Sgn(output[outputIndex]);

							if ((int)output[outputIndex] != classes[classIndex].d[outputIndex]) {

								multiple = c * (double)(classes[classIndex].d[outputIndex] - (int)output[outputIndex]);
								WeightsUpdate(weights, outputIndex * NETWORKINPUTS, multiple, sampleMatrix, NETWORKINPUTS);
								weightState++;
							}
						}
					}
				}
				cycleCounter++;

			} while (weightState != 0);

			pictureBox1->Refresh();
			draw(1);				// girisler normalize edilmediginden dolayi olcekleme mult = 1
		}

		//// Delta Learning 
		else if (!strcmp(trainType, "Continuous")) {

			double lamda = 7.0;
			double sigmoidO = 0.0;
			double gradientSO = 0.0;
			double errorTH = 0.1;
			double totalError = 0.0;

			BatchNormalization(classes, totalClass);

			do {
				totalError = 0.0;

				for (int classIndex = 0; classIndex < totalClass; classIndex++)
				{
					for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++) {

						sampleMatrix[0] = classes[classIndex].sample[sampleIndex].x;
						sampleMatrix[1] = classes[classIndex].sample[sampleIndex].y;
						sampleMatrix[2] = classes[classIndex].sample[sampleIndex].b;

						MatrixMultiplication(weights, sampleMatrix, output, totalNeuron, NETWORKINPUTS, 1);

						for (int outputIndex = 0; outputIndex < totalNeuron; outputIndex++) {

							sigmoidO = Sigmoid(lamda, output[outputIndex]);		// f(net)
							totalError += pow(((double)classes[classIndex].d[outputIndex] - sigmoidO), 2);

							gradientSO = GradientSigmoid(sigmoidO);				// f'(net)
							multiple = c * ((double)classes[classIndex].d[outputIndex] - sigmoidO) * gradientSO;
							WeightsUpdate(weights, outputIndex * NETWORKINPUTS, multiple, sampleMatrix, NETWORKINPUTS);

						}
					}
				}
				cycleCounter++;

			} while ((double)( 0.5 * totalError / totalNeuron) > errorTH);


			for (int classIndex = 0; classIndex < totalClass; classIndex++)
			{
				for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++) {

					classes[classIndex].sample[sampleIndex].x *= 50.0;
					classes[classIndex].sample[sampleIndex].y *= 50.0;
				}
			}

			pictureBox1->Refresh();
			draw(50);				// girisler normalize edildiginden dolayi olcekleme mult = 50 		
		}

		this->labelW0->Text = weights[0].ToString();
		this->labelW1->Text = weights[1].ToString();
		this->labelW2->Text = weights[2].ToString();

		labelCycles->Text = cycleCounter.ToString();

		delete[] output;
	}

	private: System::Void discreteToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

		InitializeSets();
		trainType = "Discrete";

	}

	private: System::Void continuousToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {

		InitializeSets();
		trainType = "Continuous";

	}

	private: System::Void buttonSave_Click(System::Object^ sender, System::EventArgs^ e) {

		totalClass = (int)comboClasses->Items[comboClasses->SelectedIndex];
		classes = new Classes[totalClass];

		for (int classIndex = 0; classIndex < totalClass; classIndex++)
			comboColors->Items->Add(classColors[classIndex]);

		comboColors->SelectedIndex = 0;

		switch (totalClass)
		{
		case 2:
			classes[0].d = new int{ 1 };
			classes[1].d = new int{ -1 };
			totalNeuron = 1;
			break;

		case 3:
			classes[0].d = new int[3]{  1,-1,-1 };
			classes[1].d = new int[3]{ -1, 1,-1 };
			classes[2].d = new int[3]{ -1,-1, 1 };
			totalNeuron = 3;
			break;

		case 4:
			classes[0].d = new int[4]{  1,-1,-1,-1 };
			classes[1].d = new int[4]{ -1, 1,-1,-1 };
			classes[2].d = new int[4]{ -1,-1, 1,-1 };
			classes[3].d = new int[4]{ -1,-1,-1, 1 };
			totalNeuron = 4;
			break;

		case 5:
			classes[0].d = new int[5]{  1,-1,-1,-1,-1 };
			classes[1].d = new int[5]{ -1, 1,-1,-1,-1 };
			classes[2].d = new int[5]{ -1,-1, 1,-1,-1 };
			classes[3].d = new int[5]{ -1,-1,-1, 1,-1 };
			classes[4].d = new int[5]{ -1,-1,-1,-1, 1 };
			totalNeuron = 5;
			break;

		}

		Saved();
	}

	};
}
