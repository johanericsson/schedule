#pragma once

//using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Schedule
{
	/// <summary> 
	/// Summary for MyForm
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class MyForm : public System::Windows::Forms::Form
	{
	public: 
		MyForm(void)
		{
			InitializeComponent();
		}
        
	protected: 
		void Dispose(bool disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
	private: System::Windows::Forms::Button *  button1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container* components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = new System::Windows::Forms::Button();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(184, 96);
			this->button1->Name = S"button1";
			this->button1->Size = System::Drawing::Size(80, 88);
			this->button1->TabIndex = 0;
			this->button1->Text = S"button1";
			this->button1->Click += new System::EventHandler(this, button1_Click);
			// 
			// MyForm
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(292, 266);
			this->Controls->Add(this->button1);
			this->Name = S"MyForm";
			this->Text = S"MyForm";
			this->ResumeLayout(false);

		}		
	private: System::Void button1_Click(System::Object *  sender, System::EventArgs *  e)
			 {

			 }

	};
}