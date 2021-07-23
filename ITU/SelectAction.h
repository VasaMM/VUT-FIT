#pragma once
#include "MainWindow.h"

namespace ITU {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Souhrn pro SelectAction
	/// </summary>
	public ref class SelectAction : public System::Windows::Forms::Form
	{
	public:
		SelectAction( ITU::MainWindow^ parent_ ) {
			parent = parent_;
			Owner = parent_;
			InitializeComponent();
		}
		
		SelectAction()
		{
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Uvolnìte všechny používané prostøedky.
		/// </summary>
		~SelectAction()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Button^  logout;

	private: System::Windows::Forms::Button^  sleep;

	private: System::Windows::Forms::Button^  restart;

	private: System::Windows::Forms::Button^  shutdown;


	private:
		ITU::MainWindow^ parent;
		/// <summary>
		/// Vyžaduje se promìnná návrháøe.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda vyžadovaná pro podporu Návrháøe - neupravovat
		/// obsah této metody v editoru kódu.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = ( gcnew System::ComponentModel::ComponentResourceManager( Images::typeid ) );
			this->tableLayoutPanel1 = ( gcnew System::Windows::Forms::TableLayoutPanel() );
			this->logout = ( gcnew System::Windows::Forms::Button() );
			this->sleep = ( gcnew System::Windows::Forms::Button() );
			this->restart = ( gcnew System::Windows::Forms::Button() );
			this->shutdown = ( gcnew System::Windows::Forms::Button() );
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Percent,
				50 ) ) );
			this->tableLayoutPanel1->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Percent,
				50 ) ) );
			this->tableLayoutPanel1->Controls->Add( this->logout, 1, 1 );
			this->tableLayoutPanel1->Controls->Add( this->sleep, 0, 1 );
			this->tableLayoutPanel1->Controls->Add( this->restart, 1, 0 );
			this->tableLayoutPanel1->Controls->Add( this->shutdown, 0, 0 );
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point( 0, 0 );
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 2;
			this->tableLayoutPanel1->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle( System::Windows::Forms::SizeType::Percent, 50 ) ) );
			this->tableLayoutPanel1->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle( System::Windows::Forms::SizeType::Percent, 50 ) ) );
			this->tableLayoutPanel1->Size = System::Drawing::Size( 282, 253 );
			this->tableLayoutPanel1->TabIndex = 0;
			// 
			// logout
			// 
			this->logout->Dock = System::Windows::Forms::DockStyle::Fill;
			this->logout->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"logout.Image48" ) ) );
			this->logout->ImageAlign = System::Drawing::ContentAlignment::TopCenter;
			this->logout->Location = System::Drawing::Point( 144, 129 );
			this->logout->Name = L"logout";
			this->logout->Padding = System::Windows::Forms::Padding( 15 );
			this->logout->Size = System::Drawing::Size( 135, 121 );
			this->logout->TabIndex = 3;
			this->logout->Text = L"Odhlásit";
			this->logout->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->logout->UseVisualStyleBackColor = true;
			this->logout->Click += gcnew System::EventHandler( this, &SelectAction::logout_Click );
			// 
			// sleep
			// 
			this->sleep->Dock = System::Windows::Forms::DockStyle::Fill;
			this->sleep->Enabled = false;
			this->sleep->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"sleep.Image48" ) ) );
			this->sleep->ImageAlign = System::Drawing::ContentAlignment::TopCenter;
			this->sleep->Location = System::Drawing::Point( 3, 129 );
			this->sleep->Name = L"sleep";
			this->sleep->Padding = System::Windows::Forms::Padding( 15 );
			this->sleep->Size = System::Drawing::Size( 135, 121 );
			this->sleep->TabIndex = 2;
			this->sleep->Text = L"Uspat";
			this->sleep->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->sleep->UseVisualStyleBackColor = true;
			this->sleep->Click += gcnew System::EventHandler( this, &SelectAction::sleep_Click );
			// 
			// restart
			// 
			this->restart->Dock = System::Windows::Forms::DockStyle::Fill;
			this->restart->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"restart.Image48" ) ) );
			this->restart->ImageAlign = System::Drawing::ContentAlignment::TopCenter;
			this->restart->Location = System::Drawing::Point( 144, 3 );
			this->restart->Name = L"restart";
			this->restart->Padding = System::Windows::Forms::Padding( 15 );
			this->restart->Size = System::Drawing::Size( 135, 120 );
			this->restart->TabIndex = 1;
			this->restart->Text = L"Restartovat";
			this->restart->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->restart->UseVisualStyleBackColor = true;
			this->restart->Click += gcnew System::EventHandler( this, &SelectAction::restart_Click );
			// 
			// shutdown
			// 
			this->shutdown->Dock = System::Windows::Forms::DockStyle::Fill;
			this->shutdown->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"shutdown.Image48" ) ) );
			this->shutdown->ImageAlign = System::Drawing::ContentAlignment::TopCenter;
			this->shutdown->Location = System::Drawing::Point( 3, 3 );
			this->shutdown->Name = L"shutdown";
			this->shutdown->Padding = System::Windows::Forms::Padding( 15 );
			this->shutdown->Size = System::Drawing::Size( 135, 120 );
			this->shutdown->TabIndex = 0;
			this->shutdown->Text = L"Vypnout";
			this->shutdown->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->shutdown->UseVisualStyleBackColor = true;
			this->shutdown->Click += gcnew System::EventHandler( this, &SelectAction::shutdown_Click );
			// 
			// SelectAction
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF( 8, 16 );
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size( 282, 253 );
			this->ControlBox = false;
			this->Controls->Add( this->tableLayoutPanel1 );
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"SelectAction";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler( this, &SelectAction::SelectAction_FormClosed );
			this->Load += gcnew System::EventHandler( this, &SelectAction::SelectAction_Load );
			this->tableLayoutPanel1->ResumeLayout( false );
			this->ResumeLayout( false );

		}
#pragma endregion
	Void SelectAction_FormClosed ( Object^ sender, FormClosedEventArgs^ e ) { Owner->Enabled = true; }
	Void SelectAction_Load       ( Object^ sender, EventArgs^ e ) { Owner->Enabled = false; }

	Void shutdown_Click ( Object^ sender, EventArgs^ e ) { actionSelected( ITU::Shutdown::SHUTDOWN ); }
	Void restart_Click  ( Object^ sender, EventArgs^ e ) { actionSelected( ITU::Shutdown::RESTART  ); }
	Void sleep_Click    ( Object^ sender, EventArgs^ e ) { actionSelected( ITU::Shutdown::SLEEP    ); }
	Void logout_Click   ( Object^ sender, EventArgs^ e ) { actionSelected( ITU::Shutdown::LOGOUT   ); }

	void actionSelected( ITU::Shutdown::actionType action );
};
}
