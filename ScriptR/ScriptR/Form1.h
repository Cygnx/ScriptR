#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
using namespace std;
void dbgOutput(string s);
void printScriptStatuses();
class script
{
public:
	boolean status; 
	int key;
	vector<string> commands;
	HANDLE scriptThread; 
	void printStatus()
	{
		cout << key << " :: ";
		switch(status)
		{
		case true : cout << "ON" << endl; break;
		case false: cout<< "OFF" << endl; break;
		}

	}
};
vector <script> myScripts;
namespace ScriptR {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		static Form1^ TheInstance;
		Form1(void)
		{
			InitializeComponent();
			TheInstance = this;
			//
			//TODO: Add the constructor code here
			//
		}
		Void outputText(string text) 
		{
			text += '\n';
			this->textBox1->AppendText(gcnew System::String(text.c_str()));
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected: 





	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  button2;

	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(16, 24);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(102, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"update script";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// textBox1
			// 
			this->textBox1->AcceptsReturn = true;
			this->textBox1->AcceptsTab = true;
			this->textBox1->Location = System::Drawing::Point(16, 53);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(211, 628);
			this->textBox1->TabIndex = 6;
			this->textBox1->WordWrap = false;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(124, 24);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 7;
			this->button2->Text = L"Edit Script";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(240, 750);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"ScriptR::Cygnx";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: 
		System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			dbgOutput("Loading Scripts");
			textBox1->Clear();
			myScripts.clear();
			ifstream file("scripts.txt");
			if (file.is_open())
			{
				for( std::string line; getline( file, line ); )
				{
					if(line.find("[") != string::npos && line.find("]") != string::npos)
					{
						int hotkey;
						script newScript;

						sscanf(line.c_str(), "[%x]", &hotkey);

						newScript.status = false;
						newScript.key = hotkey;
						for( std::string commands; getline( file, commands ); )
						{
							if(commands.empty()) {break;}
							newScript.commands.push_back(commands);
						}
						myScripts.push_back(newScript);
					}
				}
				outputText("*************");
				for(script current_script : myScripts)
				{
					outputText("Hotkey: " + std::to_string(current_script.key) + " will execute:");
					for(string current_command : current_script.commands)
						outputText(current_command);
					outputText("*************");
				}
				outputText("Monitoring Hotkeys: ");
				printScriptStatuses();
			}
			else
			{
				dbgOutput("Failed to open Script.txt");
				dbgOutput("Make sure this file exists in your maple story directory");
			}
		}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				 dbgOutput("Editing Script.txt");
				if((int)ShellExecute(0, "open", "scripts.txt", NULL, NULL, SW_SHOWNORMAL ) < 32)
					dbgOutput("Failed to open Script.txt");
			 }
};
}
